//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayerManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPlayerManager.h"
#include "CClient.h"
#include "CRemotePlayer.h"
#include <CLogFile.h>

extern CClient * g_pClient;

DWORD dwPlayerModelHashes[] = 
{
	Scripting::MODEL_PLAYER, Scripting::MODEL_M_Y_MULTIPLAYER, Scripting::MODEL_F_Y_MULTIPLAYER, Scripting::MODEL_SUPERLOD,
	Scripting::MODEL_IG_ANNA, Scripting::MODEL_IG_ANTHONY, Scripting::MODEL_IG_BADMAN, Scripting::MODEL_IG_BERNIE_CRANE,
	Scripting::MODEL_IG_BLEDAR, Scripting::MODEL_IG_BRIAN, Scripting::MODEL_IG_BRUCIE, Scripting::MODEL_IG_BULGARIN,
	Scripting::MODEL_IG_CHARISE, Scripting::MODEL_IG_CHARLIEUC, Scripting::MODEL_IG_CLARENCE, Scripting::MODEL_IG_DARDAN,
	Scripting::MODEL_IG_DARKO, Scripting::MODEL_IG_DERRICK_MC, Scripting::MODEL_IG_DMITRI, Scripting::MODEL_IG_DWAYNE,
	Scripting::MODEL_IG_EDDIELOW, Scripting::MODEL_IG_FAUSTIN, Scripting::MODEL_IG_FRANCIS_MC, Scripting::MODEL_IG_FRENCH_TOM,
	Scripting::MODEL_IG_GORDON, Scripting::MODEL_IG_GRACIE, Scripting::MODEL_IG_HOSSAN, Scripting::MODEL_IG_ILYENA, Scripting::MODEL_IG_ISAAC,
	Scripting::MODEL_IG_IVAN, Scripting::MODEL_IG_JAY, Scripting::MODEL_IG_JASON, Scripting::MODEL_IG_JEFF, Scripting::MODEL_IG_JIMMY,
	Scripting::MODEL_IG_JOHNNYBIKER, Scripting::MODEL_IG_KATEMC, Scripting::MODEL_IG_KENNY, Scripting::MODEL_IG_LILJACOB,
	Scripting::MODEL_IG_LILJACOBW, Scripting::MODEL_IG_LUCA, Scripting::MODEL_IG_LUIS, Scripting::MODEL_IG_MALLORIE, Scripting::MODEL_IG_MAMC,
	Scripting::MODEL_IG_MANNY, Scripting::MODEL_IG_MARNIE, Scripting::MODEL_IG_MEL, Scripting::MODEL_IG_MICHAEL, Scripting::MODEL_IG_MICHELLE,
	Scripting::MODEL_IG_MICKEY, Scripting::MODEL_IG_PACKIE_MC, Scripting::MODEL_IG_PATHOS, Scripting::MODEL_IG_PETROVIC,
	Scripting::MODEL_IG_PHIL_BELL, Scripting::MODEL_IG_PLAYBOY_X, Scripting::MODEL_IG_RAY_BOCCINO, Scripting::MODEL_IG_RICKY,
	Scripting::MODEL_IG_ROMAN, Scripting::MODEL_IG_ROMANW, Scripting::MODEL_IG_SARAH, Scripting::MODEL_IG_TUNA, Scripting::MODEL_IG_VINNY_SPAZ,
	Scripting::MODEL_IG_VLAD, Scripting::MODEL_M_Y_GAFR_LO_01, Scripting::MODEL_M_Y_GAFR_LO_02, Scripting::MODEL_M_Y_GAFR_HI_01,
	Scripting::MODEL_M_Y_GAFR_HI_02, Scripting::MODEL_M_Y_GALB_LO_01, Scripting::MODEL_M_Y_GALB_LO_02, Scripting::MODEL_M_Y_GALB_LO_03,
	Scripting::MODEL_M_Y_GALB_LO_04, Scripting::MODEL_M_M_GBIK_LO_03, Scripting::MODEL_M_Y_GBIK_HI_01, Scripting::MODEL_M_Y_GBIK_HI_02,
	Scripting::MODEL_M_Y_GBIK02_LO_02, Scripting::MODEL_M_Y_GBIK_LO_01, Scripting::MODEL_M_Y_GBIK_LO_02, Scripting::MODEL_M_Y_GIRI_LO_01,
	Scripting::MODEL_M_Y_GIRI_LO_02, Scripting::MODEL_M_Y_GIRI_LO_03, Scripting::MODEL_M_M_GJAM_HI_01, Scripting::MODEL_M_M_GJAM_HI_02,
	Scripting::MODEL_M_M_GJAM_HI_03, Scripting::MODEL_M_Y_GJAM_LO_01, Scripting::MODEL_M_Y_GJAM_LO_02, Scripting::MODEL_M_Y_GKOR_LO_01,
	Scripting::MODEL_M_Y_GKOR_LO_02, Scripting::MODEL_M_Y_GLAT_LO_01, Scripting::MODEL_M_Y_GLAT_LO_02, Scripting::MODEL_M_Y_GLAT_HI_01,
	Scripting::MODEL_M_Y_GLAT_HI_02, Scripting::MODEL_M_Y_GMAF_HI_01, Scripting::MODEL_M_Y_GMAF_HI_02, Scripting::MODEL_M_Y_GMAF_LO_01,
	Scripting::MODEL_M_Y_GMAF_LO_02, Scripting::MODEL_M_O_GRUS_HI_01, Scripting::MODEL_M_Y_GRUS_LO_01, Scripting::MODEL_M_Y_GRUS_LO_02,
	Scripting::MODEL_M_Y_GRUS_HI_02, Scripting::MODEL_M_M_GRU2_HI_01, Scripting::MODEL_M_M_GRU2_HI_02, Scripting::MODEL_M_M_GRU2_LO_02,
	Scripting::MODEL_M_Y_GRU2_LO_01, Scripting::MODEL_M_M_GTRI_HI_01, Scripting::MODEL_M_M_GTRI_HI_02, Scripting::MODEL_M_Y_GTRI_LO_01,
	Scripting::MODEL_M_Y_GTRI_LO_02, Scripting::MODEL_F_O_MAID_01, Scripting::MODEL_F_O_BINCO, Scripting::MODEL_F_Y_BANK_01, Scripting::MODEL_F_Y_DOCTOR_01,
	Scripting::MODEL_F_Y_GYMGAL_01, Scripting::MODEL_F_Y_FF_BURGER_R, Scripting::MODEL_F_Y_FF_CLUCK_R, Scripting::MODEL_F_Y_FF_RSCAFE,
	Scripting::MODEL_F_Y_FF_TWCAFE, Scripting::MODEL_F_Y_FF_WSPIZZA_R, Scripting::MODEL_F_Y_HOOKER_01, Scripting::MODEL_F_Y_HOOKER_03,
	Scripting::MODEL_F_Y_NURSE, Scripting::MODEL_F_Y_STRIPPERC01, Scripting::MODEL_F_Y_STRIPPERC02, Scripting::MODEL_F_Y_WAITRESS_01,
	Scripting::MODEL_M_M_ALCOHOLIC, Scripting::MODEL_M_M_ARMOURED, Scripting::MODEL_M_M_BUSDRIVER, Scripting::MODEL_M_M_CHINATOWN_01,
	Scripting::MODEL_M_M_CRACKHEAD, Scripting::MODEL_M_M_DOC_SCRUBS_01, Scripting::MODEL_M_M_DOCTOR_01, Scripting::MODEL_M_M_DODGYDOC,
	Scripting::MODEL_M_M_EECOOK, Scripting::MODEL_M_M_ENFORCER, Scripting::MODEL_M_M_FACTORY_01, Scripting::MODEL_M_M_FATCOP_01,
	Scripting::MODEL_M_M_FBI, Scripting::MODEL_M_M_FEDCO, Scripting::MODEL_M_M_FIRECHIEF, Scripting::MODEL_M_M_GUNNUT_01, Scripting::MODEL_M_M_HELIPILOT_01,
	Scripting::MODEL_M_M_HPORTER_01, Scripting::MODEL_M_M_KOREACOOK_01, Scripting::MODEL_M_M_LAWYER_01, Scripting::MODEL_M_M_LAWYER_02,
	Scripting::MODEL_M_M_LOONYBLACK, Scripting::MODEL_M_M_PILOT, Scripting::MODEL_M_M_PINDUS_01, Scripting::MODEL_M_M_POSTAL_01,
	Scripting::MODEL_M_M_SAXPLAYER_01, Scripting::MODEL_M_M_SECURITYMAN, Scripting::MODEL_M_M_SELLER_01, Scripting::MODEL_M_M_SHORTORDER,
	Scripting::MODEL_M_M_STREETFOOD_01, Scripting::MODEL_M_M_SWEEPER, Scripting::MODEL_M_M_TAXIDRIVER, Scripting::MODEL_M_M_TELEPHONE,
	Scripting::MODEL_M_M_TENNIS, Scripting::MODEL_M_M_TRAIN_01, Scripting::MODEL_M_M_TRAMPBLACK, Scripting::MODEL_M_M_TRUCKER_01,
	Scripting::MODEL_M_O_JANITOR, Scripting::MODEL_M_O_HOTEL_FOOT, Scripting::MODEL_M_O_MPMOBBOSS, Scripting::MODEL_M_Y_AIRWORKER,
	Scripting::MODEL_M_Y_BARMAN_01, Scripting::MODEL_M_Y_BOUNCER_01, Scripting::MODEL_M_Y_BOUNCER_02, Scripting::MODEL_M_Y_BOWL_01,
	Scripting::MODEL_M_Y_BOWL_02, Scripting::MODEL_M_Y_CHINVEND_01, Scripting::MODEL_M_Y_CLUBFIT, Scripting::MODEL_M_Y_CONSTRUCT_01,
	Scripting::MODEL_M_Y_CONSTRUCT_02, Scripting::MODEL_M_Y_CONSTRUCT_03, Scripting::MODEL_M_Y_COP, Scripting::MODEL_M_Y_COP_TRAFFIC,
	Scripting::MODEL_M_Y_COURIER, Scripting::MODEL_M_Y_COWBOY_01, Scripting::MODEL_M_Y_DEALER, Scripting::MODEL_M_Y_DRUG_01,
	Scripting::MODEL_M_Y_FF_BURGER_R, Scripting::MODEL_M_Y_FF_CLUCK_R, Scripting::MODEL_M_Y_FF_RSCAFE, Scripting::MODEL_M_Y_FF_TWCAFE,
	Scripting::MODEL_M_Y_FF_WSPIZZA_R, Scripting::MODEL_M_Y_FIREMAN, Scripting::MODEL_M_Y_GARBAGE, Scripting::MODEL_M_Y_GOON_01,
	Scripting::MODEL_M_Y_GYMGUY_01, Scripting::MODEL_M_Y_MECHANIC_02, Scripting::MODEL_M_Y_MODO, Scripting::MODEL_M_Y_NHELIPILOT,
	Scripting::MODEL_M_Y_PERSEUS, Scripting::MODEL_M_Y_PINDUS_01, Scripting::MODEL_M_Y_PINDUS_02, Scripting::MODEL_M_Y_PINDUS_03, 
	Scripting::MODEL_M_Y_PMEDIC, Scripting::MODEL_M_Y_PRISON, Scripting::MODEL_M_Y_PRISONAOM, Scripting::MODEL_M_Y_ROMANCAB, Scripting::MODEL_M_Y_RUNNER,
	Scripting::MODEL_M_Y_SHOPASST_01, Scripting::MODEL_M_Y_STROOPER, Scripting::MODEL_M_Y_SWAT, Scripting::MODEL_M_Y_SWORDSWALLOW,
	Scripting::MODEL_M_Y_THIEF, Scripting::MODEL_M_Y_VALET, Scripting::MODEL_M_Y_VENDOR, Scripting::MODEL_M_Y_FRENCHTOM, Scripting::MODEL_M_Y_JIM_FITZ,
	Scripting::MODEL_F_O_PEASTEURO_01, Scripting::MODEL_F_O_PEASTEURO_02, Scripting::MODEL_F_O_PHARBRON_01, Scripting::MODEL_F_O_PJERSEY_01,
	Scripting::MODEL_F_O_PORIENT_01, Scripting::MODEL_F_O_RICH_01, Scripting::MODEL_F_M_BUSINESS_01, Scripting::MODEL_F_M_BUSINESS_02,
	Scripting::MODEL_F_M_CHINATOWN, Scripting::MODEL_F_M_PBUSINESS, Scripting::MODEL_F_M_PEASTEURO_01, Scripting::MODEL_F_M_PHARBRON_01,
	Scripting::MODEL_F_M_PJERSEY_01, Scripting::MODEL_F_M_PJERSEY_02, Scripting::MODEL_F_M_PLATIN_01, Scripting::MODEL_F_M_PLATIN_02,
	Scripting::MODEL_F_M_PMANHAT_01, Scripting::MODEL_F_M_PMANHAT_02, Scripting::MODEL_F_M_PORIENT_01, Scripting::MODEL_F_M_PRICH_01,
	Scripting::MODEL_F_Y_BUSINESS_01, Scripting::MODEL_F_Y_CDRESS_01, Scripting::MODEL_F_Y_PBRONX_01, Scripting::MODEL_F_Y_PCOOL_01,
	Scripting::MODEL_F_Y_PCOOL_02,
	Scripting::MODEL_F_Y_PEASTEURO_01,
	Scripting::MODEL_F_Y_PHARBRON_01,
	Scripting::MODEL_F_Y_PHARLEM_01,
	Scripting::MODEL_F_Y_PJERSEY_02,
	Scripting::MODEL_F_Y_PLATIN_01,
	Scripting::MODEL_F_Y_PLATIN_02,
	Scripting::MODEL_F_Y_PLATIN_03,
	Scripting::MODEL_F_Y_PMANHAT_01,
	Scripting::MODEL_F_Y_PMANHAT_02,
	Scripting::MODEL_F_Y_PMANHAT_03,
	Scripting::MODEL_F_Y_PORIENT_01,
	Scripting::MODEL_F_Y_PQUEENS_01,
	Scripting::MODEL_F_Y_PRICH_01,
	Scripting::MODEL_F_Y_PVILLBO_02,
	Scripting::MODEL_F_Y_SHOP_03,
	Scripting::MODEL_F_Y_SHOP_04,
	Scripting::MODEL_F_Y_SHOPPER_05,
	Scripting::MODEL_F_Y_SOCIALITE,
	Scripting::MODEL_F_Y_STREET_02,
	Scripting::MODEL_F_Y_STREET_05,
	Scripting::MODEL_F_Y_STREET_09,
	Scripting::MODEL_F_Y_STREET_12,
	Scripting::MODEL_F_Y_STREET_30,
	Scripting::MODEL_F_Y_STREET_34,
	Scripting::MODEL_F_Y_TOURIST_01,
	Scripting::MODEL_F_Y_VILLBO_01,
	Scripting::MODEL_M_M_BUSINESS_02,
	Scripting::MODEL_M_M_BUSINESS_03,
	Scripting::MODEL_M_M_EE_HEAVY_01,
	Scripting::MODEL_M_M_EE_HEAVY_02,
	Scripting::MODEL_M_M_FATMOB_01,
	Scripting::MODEL_M_M_GAYMID,
	Scripting::MODEL_M_M_GENBUM_01,
	Scripting::MODEL_M_M_LOONYWHITE,
	Scripting::MODEL_M_M_MIDTOWN_01,
	Scripting::MODEL_M_M_PBUSINESS_01,
	Scripting::MODEL_M_M_PEASTEURO_01,
	Scripting::MODEL_M_M_PHARBRON_01,
	Scripting::MODEL_M_M_PINDUS_02,
	Scripting::MODEL_M_M_PITALIAN_01,
	Scripting::MODEL_M_M_PITALIAN_02,
	Scripting::MODEL_M_M_PLATIN_01,
	Scripting::MODEL_M_M_PLATIN_02,
	Scripting::MODEL_M_M_PLATIN_03,
	Scripting::MODEL_M_M_PMANHAT_01,
	Scripting::MODEL_M_M_PMANHAT_02,
	Scripting::MODEL_M_M_PORIENT_01,
	Scripting::MODEL_M_M_PRICH_01,
	Scripting::MODEL_M_O_EASTEURO_01,
	Scripting::MODEL_M_O_HASID_01,
	Scripting::MODEL_M_O_MOBSTER,
	Scripting::MODEL_M_O_PEASTEURO_02,
	Scripting::MODEL_M_O_PHARBRON_01,
	Scripting::MODEL_M_O_PJERSEY_01,
	Scripting::MODEL_M_O_STREET_01,
	Scripting::MODEL_M_O_SUITED,
	Scripting::MODEL_M_Y_BOHO_01,
	Scripting::MODEL_M_Y_BOHOGUY_01,
	Scripting::MODEL_M_Y_BRONX_01,
	Scripting::MODEL_M_Y_BUSINESS_01,
	Scripting::MODEL_M_Y_BUSINESS_02,
	Scripting::MODEL_M_Y_CHINATOWN_03,
	Scripting::MODEL_M_Y_CHOPSHOP_01,
	Scripting::MODEL_M_Y_CHOPSHOP_02,
	Scripting::MODEL_M_Y_DODGY_01,
	Scripting::MODEL_M_Y_DORK_02,
	Scripting::MODEL_M_Y_DOWNTOWN_01,
	Scripting::MODEL_M_Y_DOWNTOWN_02,
	Scripting::MODEL_M_Y_DOWNTOWN_03,
	Scripting::MODEL_M_Y_GAYYOUNG,
	Scripting::MODEL_M_Y_GENSTREET_11,
	Scripting::MODEL_M_Y_GENSTREET_16,
	Scripting::MODEL_M_Y_GENSTREET_20,
	Scripting::MODEL_M_Y_GENSTREET_34,
	Scripting::MODEL_M_Y_HARDMAN_01,
	Scripting::MODEL_M_Y_HARLEM_01,
	Scripting::MODEL_M_Y_HARLEM_02,
	Scripting::MODEL_M_Y_HARLEM_04,
	Scripting::MODEL_M_Y_HASID_01,
	Scripting::MODEL_M_Y_LEASTSIDE_01,
	Scripting::MODEL_M_Y_PBRONX_01,
	Scripting::MODEL_M_Y_PCOOL_01,
	Scripting::MODEL_M_Y_PCOOL_02,
	Scripting::MODEL_M_Y_PEASTEURO_01,
	Scripting::MODEL_M_Y_PHARBRON_01,
	Scripting::MODEL_M_Y_PHARLEM_01,
	Scripting::MODEL_M_Y_PJERSEY_01,
	Scripting::MODEL_M_Y_PLATIN_01,
	Scripting::MODEL_M_Y_PLATIN_02,
	Scripting::MODEL_M_Y_PLATIN_03,
	Scripting::MODEL_M_Y_PMANHAT_01,
	Scripting::MODEL_M_Y_PMANHAT_02,
	Scripting::MODEL_M_Y_PORIENT_01,
	Scripting::MODEL_M_Y_PQUEENS_01,
	Scripting::MODEL_M_Y_PRICH_01,
	Scripting::MODEL_M_Y_PVILLBO_01,
	Scripting::MODEL_M_Y_PVILLBO_02,
	Scripting::MODEL_M_Y_PVILLBO_03,
	Scripting::MODEL_M_Y_QUEENSBRIDGE,
	Scripting::MODEL_M_Y_SHADY_02,
	Scripting::MODEL_M_Y_SKATEBIKE_01,
	Scripting::MODEL_M_Y_SOHO_01,
	Scripting::MODEL_M_Y_STREET_01,
	Scripting::MODEL_M_Y_STREET_03,
	Scripting::MODEL_M_Y_STREET_04,
	Scripting::MODEL_M_Y_STREETBLK_02,
	Scripting::MODEL_M_Y_STREETBLK_03,
	Scripting::MODEL_M_Y_STREETPUNK_02,
	Scripting::MODEL_M_Y_STREETPUNK_04,
	Scripting::MODEL_M_Y_STREETPUNK_05,
	Scripting::MODEL_M_Y_TOUGH_05,
	Scripting::MODEL_M_Y_TOURIST_02,
};

DWORD SkinIdToModelHash(int modelid)
{
	if(modelid >= 0 || modelid < sizeof(dwPlayerModelHashes))
		return dwPlayerModelHashes[modelid];

	return 0x00;
}

int ModelHashToSkinId(DWORD modelhash)
{
	for(int i = 0; i < sizeof(dwPlayerModelHashes); i++)
	{
		if(modelhash == dwPlayerModelHashes[i])
			return i;
	}

	return -1;
}

CPlayerManager::CPlayerManager()
{
	for(EntityId x = 0; x < MAX_PLAYERS; x++)
	{
		m_bActive[x] = false;
		m_bCreated[x] = false;
		m_pPlayers[x] = NULL;
	}
}

CPlayerManager::~CPlayerManager()
{
	for(EntityId x = 0; x < MAX_PLAYERS; x++)
	{
		if(m_bActive[x] && !m_pPlayers[x]->IsLocalPlayer())
			Remove(x);
	}
}

void CPlayerManager::Add(EntityId playerId, String sPlayerName)
{
	if(IsPlayerLimitReached())
	{
		g_pClient->GetChatWindow()->AddErrorMessage("[WARNING] Failed to add player %d(%s), playerlimit already reached[FUNC:%s()]",playerId,sPlayerName.Get(),__FUNCTION__);
		return;
	}

	if(DoesExist(playerId))
	{
		CLogFile::Printf("Player %d was not deleted.", playerId);
		Remove(playerId);
	}

	if(!m_pPlayers[playerId])
		m_pPlayers[playerId] = new CRemotePlayer();

	m_pPlayers[playerId]->SetPlayerId(playerId);
	m_pPlayers[playerId]->SetName(sPlayerName);
	m_bActive[playerId] = true;
	CEvents * pEvents = g_pClient->GetEvents();

	if(pEvents)
	{
		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		pEvents->Call("playerConnect", &pArguments);
	}
}

bool CPlayerManager::Remove(EntityId playerId)
{
	if(!DoesExist(playerId))
	{
		CLogFile::Printf("Tried to remove inexistent player (%d).", playerId);
		return false;
	}

	if(m_pPlayers[playerId]->IsLocalPlayer())
	{
		CLogFile::Printf("Tried to remove local player (%d).", playerId);
		return false;
	}

	m_bActive[playerId] = false;

	// Now the player won't be synced anymore, now we can delete him
	// --

	// Teleport him away so we can't see him anymore
	m_pPlayers[playerId]->Teleport(CVector3(0.0f, 0.0f, -20.0f));

	// Destroy him now...
	m_pPlayers[playerId]->Destroy();
	m_bCreated[playerId] = false;

	// Enable that if we have to destroy stuff  in cremoteplayer
	/*CNetworkPlayer * pPlayer = GetAt(playerId);
	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);
		if(pRemotePlayer)
			pRemotePlayer->Destroy();

		SAFE_DELETE(pRemotePlayer);
	}*/

	SAFE_DELETE(m_pPlayers[playerId]); // Handles CRemotePlayer Class...
	return true;
}

void CPlayerManager::Spawn(EntityId playerId, int iModelId, CVector3 vecSpawnPos, float fSpawnHeading, int ucDimension)
{
	if(!DoesExist(playerId))
		return;

	if(m_pPlayers[playerId]->IsLocalPlayer())
		return;

	if(!m_bCreated[playerId])
	{
		m_bCreated[playerId] = true;
	}

	CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer*>(m_pPlayers[playerId]);
	pRemotePlayer->Spawn(iModelId, vecSpawnPos, fSpawnHeading, ucDimension, true);
	pRemotePlayer->SetModel(SkinIdToModelHash(iModelId));
	
	// CrackHD: prevents player to scream (voice) after spawn sometimes...
	// Scripting::CancelCurrentlyPlayingAmbientSpeech(pRemotePlayer->GetScriptingHandle());
	// Doesn't affects problem.

	CEvents * pEvents = g_pClient->GetEvents();

	if(pEvents)
	{
		CSquirrelArguments pArguments;
		pArguments.push(playerId);
		pEvents->Call("playerSpawn", &pArguments);
	}
}

void CPlayerManager::Pulse()
{
	for(EntityId x = 0; x < MAX_PLAYERS; x++)
	{
		if(m_bActive[x])
			m_pPlayers[x]->Pulse();
		else
		{
			if(m_bCreated[x])
				m_pPlayers[x]->Teleport(CVector3(0.0f, 0.0f, -20.0f));
		}
	}
}

CNetworkPlayer * CPlayerManager::GetAt(EntityId playerId)
{
	if(!DoesExist(playerId))
		return NULL;

	return m_pPlayers[playerId];
}

CNetworkPlayer * CPlayerManager::GetFrom(IVPed * pIVPed)
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(m_bActive[i])
		{
			if(m_bCreated[i] && m_pPlayers[i]->GetGamePlayerPed()->GetPed() == pIVPed)
				return m_pPlayers[i];
		}
	}

	return NULL;
}

bool CPlayerManager::DoesExist(EntityId playerId)
{
	if(playerId < 0 || playerId >= MAX_PLAYERS)
		return false;

	return m_bActive[playerId];
}

void CPlayerManager::SetLocalPlayer(EntityId playerId, CNetworkPlayer * pPlayer)
{
	for(EntityId x = 0; x < MAX_PLAYERS; x++)
	{
		if(m_bActive[x] && m_pPlayers[x]->IsLocalPlayer())
		{
			m_bActive[x] = false;
			break;
		}
	}

	m_bActive[playerId] = true;
	m_bCreated[playerId] = true;
	m_pPlayers[playerId] = pPlayer;
	g_pClient->GetLocalPlayer()->SetPlayerId(playerId);
}

bool CPlayerManager::IsPlayerLimitReached( void )
{
	int count = 0;
	for(EntityId x = 0; x < MAX_PLAYERS; ++x) 
	{
		if(DoesExist(x))
			count++;
	}
	return count >= MAX_PLAYERS;
}
