/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2015 QuantumCore <http://vk.com/quantumcore>
 *
 * Copyright (C) 2010-2012 MaNGOS project <http://getmangos.com>
 *
 */

#ifndef DEF_THE_BLACK_MORASS_H
#define DEF_THE_BLACK_MORASS_H

enum Data
{
	DATA_MEDIVH                 = 1,
	DATA_MEDIVH_DEATH           = 2,
	DATA_CHRONO_LORD_DEJA_DEATH = 3,
	DATA_TEMPORUS_DEATH         = 4,
	DATA_AEONUS_DEATH           = 5,
	DATA_WAVE                   = 6,
	DATA_SHIELD_PERCENT         = 7,
};

enum Creatures
{
	NPC_MEDIVH                = 15608,
	NPC_SHADOW_ENFORCER       = 17023,
	NPC_SHADOW_ACOLYTE        = 17024,
	NPC_DARK_PORTAL_CRYSTAL   = 18553,
	NPC_PURPLE_GROUND_RUNE    = 18562,
	NPC_TIME_KEEPER           = 17918,
	NPC_TIME_RIFT             = 17838,
	NPC_RIFT_LORD             = 17839,
	NPC_RIFT_KEEPER           = 21104,
	NPC_INFINITE_ASSASSIN     = 17835,
	NPC_INFINITE_CHRONOMANCER = 17892,
	NPC_INFINITE_EXECUTIONER  = 18994,
	NPC_INFINITE_VANQUISHER   = 18995,
	NPC_INFINITE_WHELP        = 21818,
	NPC_INFINITE_CHRONO_LORD  = 21697,
	NPC_INFINITE_TIMEREAVER   = 21698,
	NPC_CHRONO_LORD_DEJA      = 17879,
	NPC_TEMPORUS              = 17880,
	NPC_AEONUS                = 17881,
};

enum InstanceSpells
{
	SPELL_GROUND_RUNES          = 32570,
	SPELL_MEDIVH_SHIELD         = 31635,
	SPELL_MEDIVH_CHANNEL        = 31556,
	SPELL_CORRUPT_MEDIVH_1      = 31326,
	SPELL_CORRUPT_MEDIVH_2      = 37853,
	SPELL_TIME_RIFT_CHANNEL     = 31387,
	SPELL_CREATE_BEACON         = 34975,
	SPELL_BLACK_CRYSTAL_AURA    = 32563,
	SPELL_SUMMON_PORTAL_CRYSTAL = 32564,
	SPELL_BANISH_PURPLE         = 32566,
	SPELL_BANISH_GREEN          = 32567,
	SPELL_TRANSLOCATE           = 32568,
	SPELL_SANDBREATH_TIMEKEEPER = 31478,
	SPELL_QUEST_10297           = 37215,
};

enum Quests
{
	QUEST_THE_BLACK_MORASS        = 10296,
	QUEST_THE_OPENING_DARK_PORTAL = 10297,
	QUEST_THE_MASTERS_TOUCH       = 9836,
};

enum InstanceWorldStates
{
	WORLD_STATE_MEDIVH_SHIELD = 2540,
	WORLD_STATE_BLACK_MORASS  = 2541,
	WORLD_STATE_RIFT_OPENED   = 2784,
};

static float RiftLocations[4][3]=
{
    {-1954.875f, 7199.664f, 22.361f},
    {-1884.577f, 7105.649f, 22.641f},
    {-1953.659f, 7013.159f, 22.436f},
    {-2036.578f, 7024.884f, 23.153f},
};

const Position RuneSpawnPos[1] =
{
	{-2022.71f, 7119.21f, 22.6643f, 3.13749f},
};

const Position DarkCrystalSpawnPos[6] =
{
	{-2023.76f, 7122.83f, 22.6624f, 4.97766f},
	{-2021.75f, 7115.63f, 22.6624f, 1.81268f},
	{-2022.68f, 7112.44f, 22.6582f, 1.60455f},
	{-2029.55f, 7119.11f, 22.6623f, 0.0573177f},
	{-2022.8f, 7125.92f, 22.6623f, 4.85847f},
	{-2015.88f, 7119.3f, 22.6623f, 3.15023f},
};

const Position ShadowAcolyteSpawnPos[1] =
{
	{-2034.16f, 7121.24f, 22.666f, 6.05232f},
};

const Position ShadowEnforcerSpawnPos[36] =
{
	{-2062.85f, 7117.1f, 29.7449f, 6.14424f},
	{-2074.31f, 7113.92f, 30.5878f, 6.16857f},
	{-2065.74f, 7114.0f, 30.2247f, 6.27538f},
	{-2066.24f, 7120.01f, 30.3771f, 6.05154f},
	{-2065.2f, 7125.34f, 30.345f, 6.16935f},
	{-2064.1f, 7130.08f, 30.1889f, 6.25651f},
	{-2073.82f, 7118.31f, 30.5874f, 6.19606f},
	{-2072.04f, 7126.79f, 30.5845f, 6.14737f},
	{-2073.27f, 7122.39f, 30.5889f, 6.10967f},
	{-2070.91f, 7131.79f, 30.5453f, 6.03348f},
	{-2061.98f, 7122.07f, 29.7405f, 6.13639f},
	{-2060.91f, 7127.76f, 30.025f, 6.15445f},
	{-2041.55f, 7130.5f, 23.4676f, 6.19426f},
	{-2042.01f, 7123.46f, 23.2618f, 6.1061f},
	{-2043.9f, 7116.87f, 23.5502f, 6.08646f},
	{-2084.62f, 7119.1f, 34.5882f, 6.20434f},
	{-2084.03f, 7124.12f, 34.5849f, 6.12815f},
	{-2083.08f, 7129.83f, 34.5261f, 6.14229f},
	{-2037.42f, 7125.19f, 22.6942f, 6.13546f},
	{-2038.26f, 7119.12f, 22.7424f, 6.08981f},
	{-2046.4f, 7120.62f, 24.3011f, 6.04148f},
	{-2044.88f, 7127.37f, 24.2721f, 6.09331f},
	{-2069.47f, 7116.66f, 30.5088f, 6.11121f},
	{-2068.9f, 7123.27f, 30.5625f, 6.14262f},
	{-2068.08f, 7128.48f, 30.4594f, 6.12695f},
	{-2050.59f, 7117.28f, 25.6799f, 0.0085299f},
	{-2047.83f, 7131.72f, 26.1019f, 6.07558f},
	{-2050.1f, 7127.66f, 26.4112f, 6.16589f},
	{-2050.67f, 7122.76f, 26.0196f, 6.1761f},
	{-2047.49f, 7113.76f, 24.5597f, 6.19685f},
	{-2036.05f, 7130.58f, 22.6944f, 6.10011f},
	{-2038.99f, 7114.02f, 22.7598f, 6.07262f},
	{-2055.75f, 7127.91f, 28.8408f, 0.0487594f},
	{-2056.29f, 7123.15f, 28.2748f, 6.18094f},
	{-2056.64f, 7117.79f, 27.9916f, 6.18801f},
	{-2056.11f, 7113.1f, 27.6298f, 0.0982998f},
};

const Position EnforcerPortalMovePos[1] =
{
	{-2096.63f, 7127.26f, 34.5882f, 2.98587f},
};

#endif