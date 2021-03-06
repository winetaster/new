/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2015 QuantumCore <http://vk.com/quantumcore>
 *
 * Copyright (C) 2010-2012 MaNGOS project <http://getmangos.com>
 *
 */

#include "ScriptMgr.h"
#include "QuantumCreature.h"
#include "QuantumGossip.h"
#include "QuantumEscortAI.h"

enum ManaforgeConsoleData
{
    EMOTE_START                 = -1000211,
    EMOTE_60                    = -1000212,
    EMOTE_30                    = -1000213,
    EMOTE_10                    = -1000214,
    EMOTE_COMPLETE              = -1000215,
    EMOTE_ABORT                 = -1000216,

    ENTRY_BNAAR_C_CONSOLE       = 20209,
    ENTRY_CORUU_C_CONSOLE       = 20417,
    ENTRY_DURO_C_CONSOLE        = 20418,
    ENTRY_ARA_C_CONSOLE         = 20440,

    ENTRY_SUNFURY_TECH          = 20218,
    ENTRY_SUNFURY_PROT          = 20436,

    ENTRY_ARA_TECH              = 20438,
    ENTRY_ARA_ENGI              = 20439,
    ENTRY_ARA_GORKLONN          = 20460,

    SPELL_DISABLE_VISUAL        = 35031,
    SPELL_INTERRUPT_1           = 35016,
    SPELL_INTERRUPT_2           = 35176,
};

class npc_manaforge_control_console : public CreatureScript
{
public:
    npc_manaforge_control_console() : CreatureScript("npc_manaforge_control_console") { }

    struct npc_manaforge_control_consoleAI : public QuantumBasicAI
    {
        npc_manaforge_control_consoleAI(Creature* creature) : QuantumBasicAI(creature) {}

        uint32 EventTimer;
        uint32 WaveTimer;
        uint32 Phase;
        bool Wave;
        uint64 someplayer;
        uint64 goConsole;
        Creature* add;

        void Reset()
        {
            EventTimer = 3000;
            WaveTimer = 0;
            Phase = 1;
            Wave = false;
            someplayer = 0;
            goConsole = 0;
            add = NULL;
        }

        void EnterToBattle(Unit* /*who*/){}

        /*void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_INTERRUPT_1)
                DoSay("Silence! I kill you!", LANG_UNIVERSAL, NULL);
        }*/

        void JustDied(Unit* /*killer*/)
        {
            DoSendQuantumText(EMOTE_ABORT, me);

            if (someplayer)
            {
                Unit* player = Unit::GetUnit(*me, someplayer);
                if (player && player->GetTypeId() == TYPE_ID_PLAYER)
                {
                    switch (me->GetEntry())
                    {
                        case ENTRY_BNAAR_C_CONSOLE:
                            CAST_PLR(player)->FailQuest(10299);
                            CAST_PLR(player)->FailQuest(10329);
                            break;
                        case ENTRY_CORUU_C_CONSOLE:
                            CAST_PLR(player)->FailQuest(10321);
                            CAST_PLR(player)->FailQuest(10330);
                            break;
                        case ENTRY_DURO_C_CONSOLE:
                            CAST_PLR(player)->FailQuest(10322);
                            CAST_PLR(player)->FailQuest(10338);
                            break;
                        case ENTRY_ARA_C_CONSOLE:
                            CAST_PLR(player)->FailQuest(10323);
                            CAST_PLR(player)->FailQuest(10365);
                            break;
                    }
                }
            }

            if (goConsole)
            {
                if (GameObject* go = GameObject::GetGameObject(*me, goConsole))
                    go->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
            }
        }

        void DoWaveSpawnForCreature(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case ENTRY_BNAAR_C_CONSOLE:
                    if (rand()%2)
                    {
                        add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2933.68f, 4162.55f, 164.00f, 1.60f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 2927.36f, 4212.97f, 164.00f);
                    }
                    else
                    {
                        add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2927.36f, 4212.97f, 164.00f, 4.94f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 2933.68f, 4162.55f, 164.00f);
                    }
                    WaveTimer = 30000;
                    break;
                case ENTRY_CORUU_C_CONSOLE:
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2445.21f, 2765.26f, 134.49f, 3.93f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2424.21f, 2740.15f, 133.81f);
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2429.86f, 2731.85f, 134.53f, 1.31f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2435.37f, 2766.04f, 133.81f);
                    WaveTimer = 20000;
                    break;
                case ENTRY_DURO_C_CONSOLE:
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2986.80f, 2205.36f, 165.37f, 3.74f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2985.15f, 2197.32f, 164.79f);
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2952.91f, 2191.20f, 165.32f, 0.22f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2060.01f, 2185.27f, 164.67f);
                    WaveTimer = 15000;
                    break;
                case ENTRY_ARA_C_CONSOLE:
                    if (rand()%2)
                    {
                        add = me->SummonCreature(ENTRY_ARA_TECH, 4035.11f, 4038.97f, 194.27f, 2.57f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 4003.42f, 4040.19f, 193.49f);
                        add = me->SummonCreature(ENTRY_ARA_TECH, 4033.66f, 4036.79f, 194.28f, 2.57f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 4003.42f, 4040.19f, 193.49f);
                        add = me->SummonCreature(ENTRY_ARA_TECH, 4037.13f, 4037.30f, 194.23f, 2.57f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 4003.42f, 4040.19f, 193.49f);
                    }
                    else
                    {
                        add = me->SummonCreature(ENTRY_ARA_TECH, 3099.59f, 4049.30f, 194.22f, 0.05f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 4028.01f, 4035.17f, 193.59f);
                        add = me->SummonCreature(ENTRY_ARA_TECH, 3999.72f, 4046.75f, 194.22f, 0.05f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 4028.01f, 4035.17f, 193.59f);
                        add = me->SummonCreature(ENTRY_ARA_TECH, 3996.81f, 4048.26f, 194.22f, 0.05f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        if (add) add->GetMotionMaster()->MovePoint(0, 4028.01f, 4035.17f, 193.59f);
                    }
                    WaveTimer = 15000;
                    break;
            }
        }
        void DoFinalSpawnForCreature(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case ENTRY_BNAAR_C_CONSOLE:
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2946.52f, 4201.42f, 163.47f, 3.54f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2927.49f, 4192.81f, 163.00f);
                    break;
                case ENTRY_CORUU_C_CONSOLE:
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2453.88f, 2737.85f, 133.27f, 2.59f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2433.96f, 2751.53f, 133.85f);
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2441.62f, 2735.32f, 134.49f, 1.97f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2433.96f, 2751.53f, 133.85f);
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2450.73f, 2754.50f, 134.49f, 3.29f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2433.96f, 2751.53f, 133.85f);
                    break;
                case ENTRY_DURO_C_CONSOLE:
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2956.18f, 2202.85f, 165.32f, 5.45f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2972.27f, 2193.22f, 164.48f);
                    add = me->SummonCreature(ENTRY_SUNFURY_TECH, 2975.30f, 2211.50f, 165.32f, 4.55f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2972.27f, 2193.22f, 164.48f);
                    add = me->SummonCreature(ENTRY_SUNFURY_PROT, 2965.02f, 2217.45f, 164.16f, 4.96f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 2972.27f, 2193.22f, 164.48f);
                    break;
                case ENTRY_ARA_C_CONSOLE:
                    add = me->SummonCreature(ENTRY_ARA_ENGI, 3994.51f, 4020.46f, 192.18f, 0.91f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 4008.35f, 4035.04f, 192.70f);
                    add = me->SummonCreature(ENTRY_ARA_GORKLONN, 4021.56f, 4059.35f, 193.59f, 4.44f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0, 4016.62f, 4039.89f, 193.46f);
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (EventTimer <= diff)
            {
                switch (Phase)
                {
                    case 1:
                        if (someplayer)
                        {
                            Unit* u = Unit::GetUnit(*me, someplayer);
                            if (u && u->GetTypeId() == TYPE_ID_PLAYER) DoSendQuantumText(EMOTE_START, me, u);
                        }
                        EventTimer = 60000;
                        Wave = true;
                        ++Phase;
                        break;
                    case 2:
                        DoSendQuantumText(EMOTE_60, me);
                        EventTimer = 30000;
                        ++Phase;
                        break;
                    case 3:
                        DoSendQuantumText(EMOTE_30, me);
                        EventTimer = 20000;
                        DoFinalSpawnForCreature(me);
                        ++Phase;
                        break;
                    case 4:
                        DoSendQuantumText(EMOTE_10, me);
                        EventTimer = 10000;
                        Wave = false;
                        ++Phase;
                        break;
                    case 5:
                        DoSendQuantumText(EMOTE_COMPLETE, me);
                        if (someplayer)
                        {
                            Unit* u = Unit::GetUnit(*me, someplayer);
                            if (u && u->GetTypeId() == TYPE_ID_PLAYER)
                                CAST_PLR(u)->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
                            DoCast(me, SPELL_DISABLE_VISUAL);
                        }
                        if (goConsole)
                        {
                            if (GameObject* go = GameObject::GetGameObject(*me, goConsole))
                                go->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                        }
                        ++Phase;
                        break;
                }
            }
			else EventTimer -= diff;

            if (Wave)
            {
                if (WaveTimer <= diff)
                {
                    DoWaveSpawnForCreature(me);
                }
				else WaveTimer -= diff;
            }
        }
    };

	CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_manaforge_control_consoleAI(creature);
    }
};

class go_manaforge_control_console : public GameObjectScript
{
public:
    go_manaforge_control_console() : GameObjectScript("go_manaforge_control_console") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (go->GetGoType() == GAMEOBJECT_TYPE_QUEST_GIVER)
        {
            player->PrepareQuestMenu(go->GetGUID());
            player->SendPreparedQuest(go->GetGUID());
        }

        Creature* manaforge = NULL;

        switch (go->GetAreaId())
        {
            case 3726:                                          //b'naar
                if ((player->GetQuestStatus(10299) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10329) == QUEST_STATUS_INCOMPLETE) &&
                    player->HasItemCount(29366, 1))
                    manaforge = player->SummonCreature(ENTRY_BNAAR_C_CONSOLE, 2918.95f, 4189.98f, 161.88f, 0.34f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
                break;
            case 3730:                                          //coruu
                if ((player->GetQuestStatus(10321) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10330) == QUEST_STATUS_INCOMPLETE) &&
                    player->HasItemCount(29396, 1))
                    manaforge = player->SummonCreature(ENTRY_CORUU_C_CONSOLE, 2426.77f, 2750.38f, 133.24f, 2.14f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
                break;
            case 3734:                                          //duro
                if ((player->GetQuestStatus(10322) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10338) == QUEST_STATUS_INCOMPLETE) &&
                    player->HasItemCount(29397, 1))
                    manaforge = player->SummonCreature(ENTRY_DURO_C_CONSOLE, 2976.48f, 2183.29f, 163.20f, 1.85f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
                break;
            case 3722:                                          //ara
                if ((player->GetQuestStatus(10323) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(10365) == QUEST_STATUS_INCOMPLETE) &&
                    player->HasItemCount(29411, 1))
                    manaforge = player->SummonCreature(ENTRY_ARA_C_CONSOLE, 4013.71f, 4028.76f, 192.10f, 1.25f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 125000);
                break;
        }

        if (manaforge)
        {
            CAST_AI(npc_manaforge_control_console::npc_manaforge_control_consoleAI, manaforge->AI())->someplayer = player->GetGUID();
            CAST_AI(npc_manaforge_control_console::npc_manaforge_control_consoleAI, manaforge->AI())->goConsole = go->GetGUID();
            go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
        }
        return true;
    }
};

/*######
## npc_commander_dawnforge
######*/

// The Speech of Dawnforge, Ardonis & Pathaleon
enum eCommanderDawnforgeData
{
    SAY_COMMANDER_DAWNFORGE_1       = -1000128,
    SAY_ARCANIST_ARDONIS_1          = -1000129,
    SAY_COMMANDER_DAWNFORGE_2       = -1000130,
    SAY_PATHALEON_CULATOR_IMAGE_1   = -1000131,
    SAY_COMMANDER_DAWNFORGE_3       = -1000132,
    SAY_PATHALEON_CULATOR_IMAGE_2   = -1000133,
    SAY_PATHALEON_CULATOR_IMAGE_2_1 = -1000134,
    SAY_PATHALEON_CULATOR_IMAGE_2_2 = -1000135,
    SAY_COMMANDER_DAWNFORGE_4       = -1000136,
    SAY_ARCANIST_ARDONIS_2          = -1000136,
    SAY_COMMANDER_DAWNFORGE_5       = -1000137,

    QUEST_INFO_GATHERING            = 10198,
    SPELL_SUNFURY_DISGUISE          = 34603,
};

const uint32 CreatureEntry[3] =
{
	19830,                                                // Ardonis
    19831,                                                // Dawnforge
	21504                                                 // Pathaleon
};

class npc_commander_dawnforge : public CreatureScript
{
public:
    npc_commander_dawnforge() : CreatureScript("npc_commander_dawnforge") { }

    struct npc_commander_dawnforgeAI : public QuantumBasicAI
    {
        npc_commander_dawnforgeAI(Creature* creature) : QuantumBasicAI(creature)
		{
			Reset();
		}

        uint64 PlayerGUID;
        uint64 ardonisGUID;
        uint64 pathaleonGUID;

        uint32 Phase;
        uint32 PhaseSubphase;
        uint32 PhaseTimer;
        bool isEvent;

        float AngleDawnforge;
        float AngleArdonis;

        void Reset()
        {
            PlayerGUID = 0;
            ardonisGUID = 0;
            pathaleonGUID = 0;

            Phase = 1;
            PhaseSubphase = 0;
            PhaseTimer = 4000;
            isEvent = false;
        }

        void EnterToBattle(Unit* /*who*/){}

        void JustSummoned(Creature* summoned)
        {
            pathaleonGUID = summoned->GetGUID();
        }

        // Emote Ardonis and Pathaleon
        void Turn_to_Pathaleons_Image()
        {
            Creature* ardonis = Unit::GetCreature(*me, ardonisGUID);
            Creature* pathaleon = Unit::GetCreature(*me, pathaleonGUID);
            Player* player = Unit::GetPlayer(*me, PlayerGUID);

            if (!ardonis || !pathaleon || !player)
                return;

            //Calculate the angle to Pathaleon
            AngleDawnforge = me->GetAngle(pathaleon->GetPositionX(), pathaleon->GetPositionY());
            AngleArdonis = ardonis->GetAngle(pathaleon->GetPositionX(), pathaleon->GetPositionY());

            //Turn Dawnforge and update
            me->SetOrientation(AngleDawnforge);
            me->SendUpdateToPlayer(player);
            //Turn Ardonis and update
            ardonis->SetOrientation(AngleArdonis);
            ardonis->SendUpdateToPlayer(player);

            //Set them to kneel
            me->SetStandState(UNIT_STAND_STATE_KNEEL);
            ardonis->SetStandState(UNIT_STAND_STATE_KNEEL);
        }

        //Set them back to each other
        void Turn_to_eachother()
        {
            if (Unit* ardonis = Unit::GetUnit(*me, ardonisGUID))
            {
                Player* player = Unit::GetPlayer(*me, PlayerGUID);

                if (!player)
                    return;

                AngleDawnforge = me->GetAngle(ardonis->GetPositionX(), ardonis->GetPositionY());
                AngleArdonis = ardonis->GetAngle(me->GetPositionX(), me->GetPositionY());

                //Turn Dawnforge and update
                me->SetOrientation(AngleDawnforge);
                me->SendUpdateToPlayer(player);
                //Turn Ardonis and update
                ardonis->SetOrientation(AngleArdonis);
                ardonis->SendUpdateToPlayer(player);

                //Set state
                me->SetStandState(UNIT_STAND_STATE_STAND);
                ardonis->SetStandState(UNIT_STAND_STATE_STAND);
            }
        }

        bool CanStartEvent(Player* player)
        {
            if (!isEvent)
            {
                Creature* ardonis = me->FindCreatureWithDistance(CreatureEntry[0], 10.0f);
                if (!ardonis)
                    return false;

                ardonisGUID = ardonis->GetGUID();
                PlayerGUID = player->GetGUID();

                isEvent = true;

                Turn_to_eachother();
                return true;
            }

            sLog->outDebug(LOG_FILTER_QCSCR, "QUANTUMCORE SCRIPTS: npc_commander_dawnforge event already in progress, need to wait.");
            return false;
        }

        void UpdateAI(const uint32 diff)
        {
            //Is event even running?
            if (!isEvent)
                return;

            //Phase timing
            if (PhaseTimer >= diff)
            {
                PhaseTimer -= diff;
                return;
            }

            Unit* ardonis = Unit::GetUnit(*me, ardonisGUID);
            Unit* pathaleon = Unit::GetUnit(*me, pathaleonGUID);
            Player* player = Unit::GetPlayer(*me, PlayerGUID);

            if (!ardonis || !player)
            {
                Reset();
                return;
            }

            if (Phase > 4 && !pathaleon)
            {
                Reset();
                return;
            }

            //Phase 1 Dawnforge say
            switch (Phase)
            {
            case 1:
                DoSendQuantumText(SAY_COMMANDER_DAWNFORGE_1, me);
                ++Phase;
                PhaseTimer = 16000;
                break;
                //Phase 2 Ardonis say
            case 2:
                DoSendQuantumText(SAY_ARCANIST_ARDONIS_1, ardonis);
                ++Phase;
                PhaseTimer = 16000;
                break;
                //Phase 3 Dawnforge say
            case 3:
                DoSendQuantumText(SAY_COMMANDER_DAWNFORGE_2, me);
                ++Phase;
                PhaseTimer = 16000;
                break;
                //Phase 4 Pathaleon spawns up to phase 9
            case 4:
                //spawn pathaleon's image
                me->SummonCreature(CreatureEntry[2], 2325.851563f, 2799.534668f, 133.084229f, 6.038996f, TEMPSUMMON_TIMED_DESPAWN, 90000);
                ++Phase;
                PhaseTimer = 500;
                break;
                //Phase 5 Pathaleon say
            case 5:
                DoSendQuantumText(SAY_PATHALEON_CULATOR_IMAGE_1, pathaleon);
                ++Phase;
                PhaseTimer = 6000;
                break;
                //Phase 6
            case 6:
                switch (PhaseSubphase)
                {
                case 0:
                    Turn_to_Pathaleons_Image();
                    ++PhaseSubphase;
                    PhaseTimer = 8000;
                    break;
                    //Subphase 2 Dawnforge say
                case 1:
                    DoSendQuantumText(SAY_COMMANDER_DAWNFORGE_3, me);
                    PhaseSubphase = 0;
                    ++Phase;
                    PhaseTimer = 8000;
                    break;
                }
                break;
                //Phase 7 Pathaleons say 3 Sentence, every sentence need a subphase
            case 7:
                switch (PhaseSubphase)
                {
                    //Subphase 1
                case 0:
                    DoSendQuantumText(SAY_PATHALEON_CULATOR_IMAGE_2, pathaleon);
                    ++PhaseSubphase;
                    PhaseTimer = 12000;
                    break;
                    //Subphase 2
                case 1:
                    DoSendQuantumText(SAY_PATHALEON_CULATOR_IMAGE_2_1, pathaleon);
                    ++PhaseSubphase;
                    PhaseTimer = 16000;
                    break;
                    //Subphase 3
                case 2:
                    DoSendQuantumText(SAY_PATHALEON_CULATOR_IMAGE_2_2, pathaleon);
                    PhaseSubphase = 0;
                    ++Phase;
                    PhaseTimer = 10000;
                    break;
                }
                break;
                //Phase 8 Dawnforge & Ardonis say
            case 8:
                DoSendQuantumText(SAY_COMMANDER_DAWNFORGE_4, me);
                DoSendQuantumText(SAY_ARCANIST_ARDONIS_2, ardonis);
                ++Phase;
                PhaseTimer = 4000;
                break;
                //Phase 9 Pathaleons Despawn, Reset Dawnforge & Ardonis angle
            case 9:
                Turn_to_eachother();
                //hide pathaleon, unit will despawn shortly
                pathaleon->SetVisible(false);
                PhaseSubphase = 0;
                ++Phase;
                PhaseTimer = 3000;
                break;
                //Phase 10 Dawnforge say
            case 10:
                DoSendQuantumText(SAY_COMMANDER_DAWNFORGE_5, me);
                player->AreaExploredOrEventHappens(QUEST_INFO_GATHERING);
                Reset();
                break;
            }
         }
    };

	CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_commander_dawnforgeAI(creature);
    }
};

class at_commander_dawnforge : public AreaTriggerScript
{
public:
    at_commander_dawnforge() : AreaTriggerScript("at_commander_dawnforge") { }

    bool OnTrigger(Player* player, const AreaTriggerEntry* /*at*/)
    {
        //if player lost aura or not have at all, we should not try start event.
        if (!player->HasAura(SPELL_SUNFURY_DISGUISE))
            return false;

        if (player->IsAlive() && player->GetQuestStatus(QUEST_INFO_GATHERING) == QUEST_STATUS_INCOMPLETE)
        {
            Creature* Dawnforge = player->FindCreatureWithDistance(CreatureEntry[1], 30.0f);

            if (!Dawnforge)
                return false;

            if (CAST_AI(npc_commander_dawnforge::npc_commander_dawnforgeAI, Dawnforge->AI())->CanStartEvent(player))
                return true;
        }
        return false;
    }
};

enum ProfessorDabiriData
{
    SPELL_PHASE_DISTRUPTOR  = 35780,
    WHISPER_DABIRI          = -1000522,
    QUEST_DIMENSIUS         = 10439,
    QUEST_ON_NETHERY_WINGS  = 10438,
};

#define GOSSIP_ITEM "I need a new phase distruptor, Professor"

class npc_professor_dabiri : public CreatureScript
{
public:
    npc_professor_dabiri() : CreatureScript("npc_professor_dabiri") { }

	bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(QUEST_ON_NETHERY_WINGS) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(29778, 1))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            creature->CastSpell(player, SPELL_PHASE_DISTRUPTOR, false);
            player->CLOSE_GOSSIP_MENU();
        }
        return true;
    }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_DIMENSIUS)
            DoSendQuantumText(WHISPER_DABIRI, creature, player);

        return true;
    }
};

enum PhaseHunterData
{
    QUEST_RECHARGING_THE_BATTERIES  = 10190,
    NPC_PHASE_HUNTER_ENTRY          = 18879,
    NPC_DRAINED_PHASE_HUNTER_ENTRY  = 19595,
    EMOTE_WEAK                      = -1000303,
    SPELL_RECHARGING_BATTERY        = 34219,
    SPELL_PHASE_SLIP                = 36574,
    SPELL_MANA_BURN                 = 13321,
    SPELL_MATERIALIZE               = 34804,
    SPELL_DE_MATERIALIZE            = 34814,
};

class mob_phase_hunter : public CreatureScript
{
public:
    mob_phase_hunter() : CreatureScript("mob_phase_hunter") { }

    struct mob_phase_hunterAI : public QuantumBasicAI
    {
        mob_phase_hunterAI(Creature* creature) : QuantumBasicAI(creature) {}

        bool Weak;
        bool Materialize;
        bool Drained;

        uint8 WeakPercent;
		uint32 ManaBurnTimer;
		uint64 PlayerGUID;
        Player* player;

        void Reset()
        {
            Weak = false;
            Materialize = false;
            Drained = false;
            WeakPercent = 25 + (rand() % 16); // 25-40

            PlayerGUID = 0;

            ManaBurnTimer = 5000 + (rand() % 3 * 1000); // 5-8 sec cd

            if (me->GetEntry() == NPC_DRAINED_PHASE_HUNTER_ENTRY)
                me->UpdateEntry(NPC_PHASE_HUNTER_ENTRY);
        }

        void EnterToBattle(Unit* who)
        {
            if (who->GetTypeId() == TYPE_ID_PLAYER)
                PlayerGUID = who->GetGUID();
        }

        void UpdateAI(const uint32 diff)
        {
            if (!Materialize)
            {
                DoCast(me, SPELL_MATERIALIZE);
                Materialize = true;
            }

            if (me->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED) || me->HasUnitState(UNIT_STATE_ROOT)) // if the mob is rooted/slowed by spells eg.: Entangling Roots, Frost Nova, Hamstring, Crippling Poison, etc. => remove it
                DoCast(me, SPELL_PHASE_SLIP);

            if (!UpdateVictim())
                return;

            // some code to cast spell Mana Burn on random target which has mana
            if (ManaBurnTimer <= diff)
            {
                std::list<HostileReference*> AggroList = me->getThreatManager().getThreatList();
                std::list<Unit*> UnitsWithMana;

                for (std::list<HostileReference*>::const_iterator itr = AggroList.begin(); itr != AggroList.end(); ++itr)
                {
                    if (Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                    {
                        if (unit->GetCreateMana() > 0)
                            UnitsWithMana.push_back(unit);
                    }
                }
                if (!UnitsWithMana.empty())
                {
                    DoCast(Quantum::DataPackets::SelectRandomContainerElement(UnitsWithMana), SPELL_MANA_BURN);
                    ManaBurnTimer = 8000 + (rand() % 10 * 1000); // 8-18 sec cd
                }
                else ManaBurnTimer = 3500;
            }
			else ManaBurnTimer -= diff;

            if (Player* player = Unit::GetPlayer(*me, PlayerGUID)) // start: support for quest 10190
            {
                if (!Weak && HealthBelowPct(WeakPercent) && player->GetQuestStatus(QUEST_RECHARGING_THE_BATTERIES) == QUEST_STATUS_INCOMPLETE)
                {
                    DoSendQuantumText(EMOTE_WEAK, me);
                    Weak = true;
                }

                if (Weak && !Drained && me->HasAura(SPELL_RECHARGING_BATTERY))
                {
                    Drained = true;
                    int32 uHpPct = int32(me->GetHealthPct());

                    me->UpdateEntry(NPC_DRAINED_PHASE_HUNTER_ENTRY);

                    me->SetHealth(me->CountPctFromMaxHealth(uHpPct));
                    me->LowerPlayerDamageReq(me->GetMaxHealth() - me->GetHealth());
                    me->SetInCombatWith(player);
                }
            }

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_phase_hunterAI(creature);
    }
};

enum BessyData
{
    Q_ALMABTRIEB    = 10337,
    N_THADELL       = 20464,
    SPAWN_FIRST     = 20512,
    SPAWN_SECOND    = 19881,
    SAY_THADELL_1   = -1000524,
    SAY_THADELL_2   = -1000525,
};

class npc_bessy : public CreatureScript
{
public:
    npc_bessy() : CreatureScript("npc_bessy") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == Q_ALMABTRIEB)
        {
            creature->SetCurrentFaction(113);
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            CAST_AI(npc_escortAI, (creature->AI()))->Start(true, false, player->GetGUID());
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_bessyAI(creature);
    }

    struct npc_bessyAI : public npc_escortAI
    {
        npc_bessyAI(Creature* creature) : npc_escortAI(creature) {}

        void JustDied(Unit* /*killer*/)
        {
            if (Player* player = GetPlayerForEscort())
                player->FailQuest(Q_ALMABTRIEB);
        }

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();

            if (!player)
                return;

            switch (i)
            {
                case 3: //first spawn
                    me->SummonCreature(SPAWN_FIRST, 2449.67f, 2183.11f, 96.85f, 6.20f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(SPAWN_FIRST, 2449.53f, 2184.43f, 96.36f, 6.27f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(SPAWN_FIRST, 2449.85f, 2186.34f, 97.57f, 6.08f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    break;
                case 7:
                    me->SummonCreature(SPAWN_SECOND, 2309.64f, 2186.24f, 92.25f, 6.06f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(SPAWN_SECOND, 2309.25f, 2183.46f, 91.75f, 6.22f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    break;
                case 12:
                    if (player)
                        player->GroupEventHappens(Q_ALMABTRIEB, me);
                    if (me->FindCreatureWithDistance(N_THADELL, 30.0f))
                        DoSendQuantumText(SAY_THADELL_1, me);
					break;
                case 13:
                    if (me->FindCreatureWithDistance(N_THADELL, 30.0f))
                        DoSendQuantumText(SAY_THADELL_2, me, player);
					break;
            }
        }

        void JustSummoned(Creature* summoned)
        {
            summoned->AI()->AttackStart(me);
        }

        void Reset()
        {
            me->RestoreFaction();
        }
    };
};

/*######
## npc_maxx_a_million
######*/

enum MillionEscort
{
    QUEST_MARK_V_IS_ALIVE  = 10191,
    GO_DRAENEI_MACHINE     = 183771,
};

class npc_maxx_a_million_escort : public CreatureScript
{
public:
    npc_maxx_a_million_escort() : CreatureScript("npc_maxx_a_million_escort") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_maxx_a_million_escortAI(creature);
    }

    struct npc_maxx_a_million_escortAI : public npc_escortAI
    {
        npc_maxx_a_million_escortAI(Creature* creature) : npc_escortAI(creature) {}

        bool bTake;
        uint32 TakeTimer;

        void Reset()
        {
            bTake=false;
            TakeTimer=3000;
        }

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (i)
            {
                case 7:
                case 17:
                case 29:
                    //Find Object and "work"
                    if (GetClosestGameObjectWithEntry(me, GO_DRAENEI_MACHINE, INTERACTION_DISTANCE))
                    {
                        // take the GO -> animation
                        me->HandleEmoteCommand(EMOTE_STATE_LOOT);
                        SetEscortPaused(true);
                        bTake = true;
                    }
                    break;
                case 36: //return and quest_complete
                    if (player)
                        player->CompleteQuest(QUEST_MARK_V_IS_ALIVE);
                    break;
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            Player* player = GetPlayerForEscort();
            if (player)
                player->FailQuest(QUEST_MARK_V_IS_ALIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if (bTake)
            {
                if (TakeTimer < diff)
                {
                    me->HandleEmoteCommand(EMOTE_STATE_NONE);
                    if (GameObject* go = GetClosestGameObjectWithEntry(me, GO_DRAENEI_MACHINE, INTERACTION_DISTANCE))
                    {
                        SetEscortPaused(false);
                        bTake=false;
                        TakeTimer = 3000;
                        go->Delete();
                    }
                }
                else
                    TakeTimer -= diff;
            }
            DoMeleeAttackIfReady();
        }
    };

    bool OnQuestAccept(Player* player, Creature* creature, const Quest* quest)
    {
        if (quest->GetQuestId() == QUEST_MARK_V_IS_ALIVE)
        {
            if (npc_maxx_a_million_escortAI* EscortAI = CAST_AI(npc_maxx_a_million_escort::npc_maxx_a_million_escortAI, creature->AI()))
            {
                creature->SetCurrentFaction(113);
                EscortAI->Start(false, false, player->GetGUID());
            }
        }
        return true;
    }
};

class spell_belmara_luminrath_frostweaver_dathric : public SpellScriptLoader
{
public:
    spell_belmara_luminrath_frostweaver_dathric() : SpellScriptLoader("spell_belmara_luminrath_frostweaver_dathric") { }

    class spell_belmara_luminrath_frostweaver_dathric_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_belmara_luminrath_frostweaver_dathric_SpellScript);

        void AfterHitEffect()
        {
            if (Player* target = GetHitPlayer())
            {
				switch(GetSpellInfo()->Id)
				{
				case 34140:
					if (target->GetQuestStatus(10305) == QUEST_STATUS_INCOMPLETE)
					{
						target->CompleteQuest(10305);
					}
					break;
				case 34142:
					if (target->GetQuestStatus(10306) == QUEST_STATUS_INCOMPLETE)
					{
						target->CompleteQuest(10306);
					}
					break;
				case 34144:
					if (target->GetQuestStatus(10307) == QUEST_STATUS_INCOMPLETE)
					{
						target->CompleteQuest(10307);
					}
					break;
				case 34141:
					if (target->GetQuestStatus(10182) == QUEST_STATUS_INCOMPLETE)
					{
						target->CompleteQuest(10182);
					}
					break;
				}	
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_belmara_luminrath_frostweaver_dathric_SpellScript::AfterHitEffect);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_belmara_luminrath_frostweaver_dathric_SpellScript();
    }
};

class item_belmara_luminrath_frostweaver_dathric : public ItemScript
{
public:
	item_belmara_luminrath_frostweaver_dathric() : ItemScript("item_belmara_luminrath_frostweaver_dathric") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
	{
		switch(item->GetEntry())
		{
		case 28336:
			player->CastSpell(player, 34140, false);
			break;
		case 28352:
			player->CastSpell(player, 34142, false);
			break;
		case 28353:
			player->CastSpell(player, 34144, false);
			break;
		case 34144:
			player->CastSpell(player, 34141, false);
			break;
		}
		return true;
	}
};

enum CaptainTyralius
{
    NPC_CAPTAIN_TYRALIUS = 20787,
    SAY_FREE             = 0,
};

class go_captain_tyralius_prison : public GameObjectScript
{
public:
	go_captain_tyralius_prison() : GameObjectScript("go_captain_tyralius_prison") { }

	bool OnGossipHello(Player* player, GameObject* go)
	{
		if (Creature* tyralius = go->FindCreatureWithDistance(NPC_CAPTAIN_TYRALIUS, 1.0f))
		{
			go->UseDoorOrButton();

			player->KilledMonsterCredit(NPC_CAPTAIN_TYRALIUS, 0);

			tyralius->AI()->Talk(SAY_FREE);
			tyralius->DespawnAfterAction(8*IN_MILLISECONDS);
		}
		return true;
	}
};

enum Zeppit
{
    NPC_WARP_CHASER                 = 18884,
    SPELL_GATHER_WARP_CHASER_BLOOD  = 39244,
    QUEST_BLOODY_IMPOSSIBLE         = 10924,
    ITEM_WARP_CHASER_BLOOD     		= 31813,
};

class npc_zeppit : public CreatureScript
{
public:
	npc_zeppit() : CreatureScript("npc_zeppit") { }

    struct npc_zeppitAI: public QuantumBasicAI
    {
        npc_zeppitAI(Creature* creature) : QuantumBasicAI(creature) { }

        void UpdateAI(uint32 const /*diff*/)
        {
            if (me->IsSummon())
            {
                Player* player = me->ToTempSummon()->GetSummoner()->ToPlayer();
                if (player && player->GetQuestStatus(QUEST_BLOODY_IMPOSSIBLE) != QUEST_STATUS_INCOMPLETE)
                    return;
                
                if (Creature* target = me->FindCreatureWithDistance(NPC_WARP_CHASER, 20.0f, false))
                {
                    me->CastSpell(target, SPELL_GATHER_WARP_CHASER_BLOOD);
                    target->DespawnAfterAction();
                    player->AddItem(ITEM_WARP_CHASER_BLOOD, 1);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_zeppitAI(creature);
    }
};

enum ElementalPowerExtractorData
{
    NPC_WARP_ABERRATION             = 18865,
    NPC_SUNDERED_RUMBLER            = 18881,
    SPELL_ARCANE_SHIELD             = 36640,
    SPELL_WARP_STORM                = 36577,
    SPELL_SUMMON_SUNDERED_SHARD     = 35310,
    SPELL_ELEMENTAL_POWER_EXTRACTOR = 34520,
    SPELL_CREATE_ELEMENTAL_POWER    = 34524,
    GO_ELEMENTAL_POWER              = 183933,
};

class npc_ele_power_extractor : public CreatureScript
{
public:
    npc_ele_power_extractor() : CreatureScript("npc_ele_power_extractor") { }

    struct npc_ele_power_extractorAI : public QuantumBasicAI
    {
        npc_ele_power_extractorAI(Creature* creature) : QuantumBasicAI(creature) {}

        bool hit;
        bool add;
        bool shield;
        uint32 WarpStormTimer;
        uint32 ArcaneShieldTimer;

        void Reset()
        {
            hit = false;
            add = false;
            shield = false;
            WarpStormTimer = urand(2500,3000);
            ArcaneShieldTimer = urand(500,1500);
        }

        void UpdateAI (const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetEntry() == NPC_WARP_ABERRATION)
            {
                if (ArcaneShieldTimer <= diff && !shield)
                {
                    DoCast(me, SPELL_ARCANE_SHIELD, true);
                    shield = true;
                }
				else
					ArcaneShieldTimer -= diff;

                if (WarpStormTimer <= diff)
                {
                    if (Unit* target = me->GetVictim())
                    {
                        DoCast(target, SPELL_WARP_STORM, true);
                        WarpStormTimer = urand(20000,25000);
                    }
                }
				else
					WarpStormTimer -= diff;
            }

            if (me->GetEntry() == NPC_SUNDERED_RUMBLER)
            {
                if (HealthBelowPct(15) && !add)
                {
                    DoCast(me, SPELL_SUMMON_SUNDERED_SHARD, true);
                    add = true;
                }
            }
            DoMeleeAttackIfReady();
        }

        void SpellHit (Unit* /*caster*/, const SpellInfo* spell)
        {
           if (spell->Id == SPELL_ELEMENTAL_POWER_EXTRACTOR)
		   {
               hit = true;
		   }
        }

        void JustDied (Unit* /*killer*/)
        {
            if (hit)
			{
                me->SummonGameObject(GO_ELEMENTAL_POWER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+2, 0.0f, 0, 0, 0, 0, 25000);
			}
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ele_power_extractorAI(creature);
    }
};

void AddSC_netherstorm()
{
    new go_manaforge_control_console();
    new npc_manaforge_control_console();
    new npc_commander_dawnforge();
    new at_commander_dawnforge();
    new npc_professor_dabiri();
    new mob_phase_hunter();
    new npc_bessy();
    new npc_maxx_a_million_escort();
	new item_belmara_luminrath_frostweaver_dathric();
	new go_captain_tyralius_prison();
	new spell_belmara_luminrath_frostweaver_dathric();
	new npc_zeppit();
	new npc_ele_power_extractor();
}