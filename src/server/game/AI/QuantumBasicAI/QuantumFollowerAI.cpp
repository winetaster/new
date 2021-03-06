/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2015 QuantumCore <http://vk.com/quantumcore>
 *
 * Copyright (C) 2010-2012 MaNGOS project <http://getmangos.com>
 *
 */

#include "ScriptPCH.h"
#include "QuantumFollowerAI.h"
#include "Group.h"

const float MAX_PLAYER_DISTANCE = 100.0f;

enum ePoints
{
    POINT_COMBAT_START  = 0xFFFFFF
};

FollowerAI::FollowerAI(Creature* creature) : QuantumBasicAI(creature), m_uiLeaderGUID(0), m_uiUpdateFollowTimer(2500), m_uiFollowState(STATE_FOLLOW_NONE), m_pQuestForFollow(NULL){}

void FollowerAI::AttackStart(Unit* who)
{
    if (!who)
        return;

    if (me->Attack(who, true))
    {
        me->AddThreat(who, 0.0f);
        me->SetInCombatWith(who);
        who->SetInCombatWith(me);

        if (me->HasUnitState(UNIT_STATE_FOLLOW))
            me->ClearUnitState(UNIT_STATE_FOLLOW);

        if (IsCombatMovementAllowed())
            me->GetMotionMaster()->MoveChase(who);
    }
}

//This part provides assistance to a player that are attacked by who, even if out of normal aggro range
//It will cause me to attack who that are attacking _any_ player (which has been confirmed may happen also on offi)
//The flag (type_flag) is unconfirmed, but used here for further research and is a good candidate.
bool FollowerAI::AssistPlayerInCombat(Unit* who)
{
    if (!who || !who->GetVictim())
        return false;

    //experimental (unknown) flag not present
    if (!(me->GetCreatureTemplate()->type_flags & CREATURE_TYPEFLAGS_AID_PLAYERS))
        return false;

    //not a player
    if (!who->GetVictim()->GetCharmerOrOwnerPlayerOrPlayerItself())
        return false;

    //never attack friendly
    if (me->IsFriendlyTo(who))
        return false;

    //too far away and no free sight?
    if (me->IsWithinDistInMap(who, MAX_PLAYER_DISTANCE) && me->IsWithinLOSInMap(who))
    {
        //already fighting someone?
        if (!me->GetVictim())
        {
            AttackStart(who);
            return true;
        }
        else
        {
            who->SetInCombatWith(me);
            me->AddThreat(who, 0.0f);
            return true;
        }
    }

    return false;
}

void FollowerAI::MoveInLineOfSight(Unit* who)
{
    if (!me->HasUnitState(UNIT_STATE_STUNNED) && who->IsTargetableForAttack() && who->IsInAccessiblePlaceFor(me))
    {
        if (HasFollowState(STATE_FOLLOW_INPROGRESS) && AssistPlayerInCombat(who))
            return;

        if (!me->CanFly() && me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
            return;

        if (me->IsHostileTo(who))
        {
            float fAttackRadius = me->GetAttackDistance(who);
            if (me->IsWithinDistInMap(who, fAttackRadius) && me->IsWithinLOSInMap(who))
            {
                if (!me->GetVictim())
                {
                    who->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
                    AttackStart(who);
                }
                else if (me->GetMap()->IsDungeon())
                {
                    who->SetInCombatWith(me);
                    me->AddThreat(who, 0.0f);
                }
            }
        }
    }
}

void FollowerAI::JustDied(Unit* /*killer*/)
{
    if (!HasFollowState(STATE_FOLLOW_INPROGRESS) || !m_uiLeaderGUID || !m_pQuestForFollow)
        return;

    //TODO: need a better check for quests with time limit.
    if (Player* player = GetLeaderForFollower())
    {
        if (Group* group = player->GetGroup())
        {
            for (GroupReference* groupRef = group->GetFirstMember(); groupRef != NULL; groupRef = groupRef->next())
            {
                if (Player* member = groupRef->getSource())
                {
                    if (member->GetQuestStatus(m_pQuestForFollow->GetQuestId()) == QUEST_STATUS_INCOMPLETE)
                        member->FailQuest(m_pQuestForFollow->GetQuestId());
                }
            }
        }
        else
        {
            if (player->GetQuestStatus(m_pQuestForFollow->GetQuestId()) == QUEST_STATUS_INCOMPLETE)
                player->FailQuest(m_pQuestForFollow->GetQuestId());
        }
    }
}

void FollowerAI::JustRespawned()
{
    m_uiFollowState = STATE_FOLLOW_NONE;

    if (!IsCombatMovementAllowed())
        SetCombatMovement(true);

    if (me->GetFaction() != me->GetCreatureTemplate()->faction_A)
        me->SetCurrentFaction(me->GetCreatureTemplate()->faction_A);

    Reset();
}

void FollowerAI::EnterEvadeMode()
{
    me->RemoveAllAuras();
    me->DeleteThreatList();
    me->CombatStop(true);
    me->SetLootRecipient(NULL);

    if (HasFollowState(STATE_FOLLOW_INPROGRESS))
    {
        sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI left combat, returning to CombatStartPosition.");

        if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
        {
            float fPosX, fPosY, fPosZ;
            me->GetPosition(fPosX, fPosY, fPosZ);
            me->GetMotionMaster()->MovePoint(POINT_COMBAT_START, fPosX, fPosY, fPosZ);
        }
    }
    else
    {
        if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
            me->GetMotionMaster()->MoveTargetedHome();
    }

    Reset();
}

void FollowerAI::UpdateAI(const uint32 diff)
{
    if (HasFollowState(STATE_FOLLOW_INPROGRESS) && !me->GetVictim())
    {
        if (m_uiUpdateFollowTimer <= diff)
        {
            if (HasFollowState(STATE_FOLLOW_COMPLETE) && !HasFollowState(STATE_FOLLOW_POSTEVENT))
            {
                sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI is set completed, despawns.");
                me->DespawnAfterAction();
                return;
            }

            bool bIsMaxRangeExceeded = true;

            if (Player* player = GetLeaderForFollower())
            {
                if (HasFollowState(STATE_FOLLOW_RETURNING))
                {
                    sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI is returning to leader.");

                    RemoveFollowState(STATE_FOLLOW_RETURNING);
                    me->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
                    return;
                }

                if (Group* group = player->GetGroup())
                {
                    for (GroupReference* groupRef = group->GetFirstMember(); groupRef != NULL; groupRef = groupRef->next())
                    {
                        Player* member = groupRef->getSource();

                        if (member && me->IsWithinDistInMap(member, MAX_PLAYER_DISTANCE))
                        {
                            bIsMaxRangeExceeded = false;
                            break;
                        }
                    }
                }
                else
                {
                    if (me->IsWithinDistInMap(player, MAX_PLAYER_DISTANCE))
                        bIsMaxRangeExceeded = false;
                }
            }

            if (bIsMaxRangeExceeded)
            {
                sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI failed because player/group was to far away or not found");
                me->DespawnAfterAction();
                return;
            }

            m_uiUpdateFollowTimer = 1000;
        }
        else
            m_uiUpdateFollowTimer -= diff;
    }

    UpdateFollowerAI(diff);
}

void FollowerAI::UpdateFollowerAI(const uint32 /*diff*/)
{
    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();
}

void FollowerAI::MovementInform(uint32 motionType, uint32 pointId)
{
    if (motionType != POINT_MOTION_TYPE || !HasFollowState(STATE_FOLLOW_INPROGRESS))
        return;

    if (pointId == POINT_COMBAT_START)
    {
        if (GetLeaderForFollower())
        {
            if (!HasFollowState(STATE_FOLLOW_PAUSED))
                AddFollowState(STATE_FOLLOW_RETURNING);
        }
        else
            me->DespawnAfterAction();
    }
}

void FollowerAI::StartFollow(Player* player, uint32 factionForFollower, const Quest* quest)
{
    if (me->GetVictim())
    {
        sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI attempt to StartFollow while in combat.");
        return;
    }

    if (HasFollowState(STATE_FOLLOW_INPROGRESS))
    {
        sLog->OutErrorConsole("QUANTUM CORE SCRIPTS: FollowerAI attempt to StartFollow while already following.");
        return;
    }

    //set variables
    m_uiLeaderGUID = player->GetGUID();

    if (factionForFollower)
        me->SetCurrentFaction(factionForFollower);

    m_pQuestForFollow = quest;

    if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
    {
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
        sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI start with WAYPOINT_MOTION_TYPE, set to MoveIdle.");
    }

    me->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

    AddFollowState(STATE_FOLLOW_INPROGRESS);

    me->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);

    sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI start follow %s (GUID " UI64FMTD ")", player->GetName(), m_uiLeaderGUID);
}

Player* FollowerAI::GetLeaderForFollower()
{
    if (Player* player = Unit::GetPlayer(*me, m_uiLeaderGUID))
    {
        if (player->IsAlive())
            return player;
        else
        {
            if (Group* group = player->GetGroup())
            {
                for (GroupReference* groupRef = group->GetFirstMember(); groupRef != NULL; groupRef = groupRef->next())
                {
                    Player* member = groupRef->getSource();

                    if (member && member->IsAlive() && me->IsWithinDistInMap(member, MAX_PLAYER_DISTANCE))
                    {
                        sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI GetLeader changed and returned new leader.");
                        m_uiLeaderGUID = member->GetGUID();
                        return member;
                    }
                }
            }
        }
    }

    sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUM CORE SCRIPTS: FollowerAI GetLeader can not find suitable leader.");
    return NULL;
}

void FollowerAI::SetFollowComplete(bool bWithEndEvent)
{
    if (me->HasUnitState(UNIT_STATE_FOLLOW))
    {
        me->ClearUnitState(UNIT_STATE_FOLLOW);

        me->StopMoving();
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
    }

    if (bWithEndEvent)
        AddFollowState(STATE_FOLLOW_POSTEVENT);
    else
    {
        if (HasFollowState(STATE_FOLLOW_POSTEVENT))
            RemoveFollowState(STATE_FOLLOW_POSTEVENT);
    }

    AddFollowState(STATE_FOLLOW_COMPLETE);
}

void FollowerAI::SetFollowPaused(bool paused)
{
    if (!HasFollowState(STATE_FOLLOW_INPROGRESS) || HasFollowState(STATE_FOLLOW_COMPLETE))
        return;

    if (paused)
    {
        AddFollowState(STATE_FOLLOW_PAUSED);

        if (me->HasUnitState(UNIT_STATE_FOLLOW))
        {
            me->ClearUnitState(UNIT_STATE_FOLLOW);

            me->StopMoving();
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();
        }
    }
    else
    {
        RemoveFollowState(STATE_FOLLOW_PAUSED);

        if (Player* leader = GetLeaderForFollower())
            me->GetMotionMaster()->MoveFollow(leader, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }
}
