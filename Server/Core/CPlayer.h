//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayer.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Main.h"
#include "Interfaces/InterfaceCommon.h"
#include "CVehicle.h"

class CPlayer : public CPlayerInterface
{
private:
	EntityId      m_playerId;
	bool		  m_bJoined;
	SQInstance  * m_pScriptingInstance;
	String        m_strName;
	bool          m_bSpawned;
	int           m_iModelId;
	CVehicle    * m_pVehicle;
	BYTE          m_byteVehicleSeatId;
	CControlState     m_previousControlState;
	CControlState     m_currentControlState;
	CVector3      m_vecPosition;
	float         m_fHeading;
	CVector3      m_vecMoveSpeed;
	bool          m_bDuckState;
	unsigned int  m_uHealth;
	unsigned int  m_uArmour;
	bool          m_bHelmet;
	eStateType    m_state;
	CVector3      m_vecSpawnPosition;
	float         m_fSpawnHeading;
	int           m_iMoney;
	unsigned int  m_uWeapon;
	unsigned int  m_uAmmo;
	AimSyncData   m_aimSyncData;
	unsigned int  m_uiColor;
	unsigned char m_ucClothes[11];
	bool		  m_bAnimating;
	char*		  m_szAnimGroup;
	char*		  m_szAnimSpec;
	float		  m_fAnimTime;
	bool		  m_bMobilePhoneUse;
	CVector3	  m_vecLastAim;
	CVector3	  m_vecLastShot;
	CVector3	  m_vecLastHeadMove;
	DimensionId   m_ucDimension;
	bool		  m_bDrop;
	unsigned int  m_iWantedLevel;
	CheckGTAFiles m_FileCheck;
	bool		  m_bRemoteControlLogedIn;

public:
	CPlayer(EntityId playerId, String strName);
	~CPlayer();

	EntityId       GetPlayerId() { return m_playerId; }
	SQInstance   * GetScriptingInstance() { return m_pScriptingInstance; }
	bool		   IsJoined() { return m_bJoined; }
	void		   SetJoined(bool bJoin) { m_bJoined = bJoin; }
	bool           IsOnFoot() { return (m_pVehicle == NULL); }
	bool           IsInVehicle() { return (m_pVehicle != NULL); }
	void		   SetFileCheck(CheckGTAFiles pFiles) { m_FileCheck = pFiles; }
	unsigned int   GetFileChecksum(int iFile);
	String         GetIp();
	void           Kick(bool bSendNotification = true);
	void           Ban(unsigned int uiSeconds);
	void           SetState(eStateType state);
	eStateType     GetState();
	void           AddForPlayer(EntityId playerId);
	void           DeleteForPlayer(EntityId playerId);
	void           AddForWorld();
	void           DeleteForWorld();
	void           SpawnForPlayer(EntityId playerId);
	void           KillForPlayer(EntityId playerId);
	void           SpawnForWorld();
	void           KillForWorld();
	void           SetVehicle(CVehicle * pVehicle) { m_pVehicle = pVehicle; }
	CVehicle     * GetVehicle() { return m_pVehicle; }
	void           SetVehicleSeatId(BYTE byteSeatId) { m_byteVehicleSeatId = byteSeatId; }
	BYTE           GetVehicleSeatId() { return m_byteVehicleSeatId; }
	void           StoreOnFootSync(OnFootSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	void           StoreInVehicleSync(CVehicle * pVehicle, InVehicleSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	void           StorePassengerSync(CVehicle * pVehicle, PassengerSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	void           StoreSmallSync(SmallSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	void           Process();
	bool           SetName(String strName);
	String         GetName();
	bool           IsSpawned();
	bool           SetModel(int iModelId);
	int            GetModel();
	void           SetCameraPos(const CVector3& vecPosition);
	void           SetCameraLookAt(const CVector3& vecPosition);
	void           SetPosition(const CVector3& vecPosition);
	void           GetPosition(CVector3& vecPosition);
	void           SetCurrentHeading(float fHeading);
	float          GetCurrentHeading();
	void           SetMoveSpeed(const CVector3& vecMoveSpeed);
	void           GetMoveSpeed(CVector3& vecMoveSpeed);
	void           SetDucking(bool bDuckState);
	bool           IsDucking();
	void		   GiveHelmet();
	void		   RemoveHelmet();
	void           SetHealth(unsigned int uHealth);
	unsigned int   GetHealth();
	void           SetArmour(unsigned int uArmour);
	unsigned int   GetArmour();
	void           SetSpawnLocation(const CVector3& vecPosition, float fHeading);
	void           GetSpawnLocation(CVector3& vecPosition, float * fHeading);
	bool           GiveMoney(int iMoney);
	bool           SetMoney(int iMoney);
	int            GetMoney();
	void           SetWeapon(unsigned int uWeapon);
	unsigned int   GetWeapon();
	void           SetAmmo(unsigned int uAmmo);
	unsigned int   GetAmmo();
	String         GetSerial();
	void           SetControlState(CControlState * controlState);
	void           GetPreviousControlState(CControlState * controlState);
	void           GetControlState(CControlState * controlState);
	void           SetColor(unsigned int color);	
	unsigned int   GetColor();
	unsigned short GetPing();
	void           ResetClothes();
	void           SetClothes(unsigned char ucBodyPart, unsigned char ucClothes);
	unsigned char  GetClothes(unsigned char ucBodyPart);
	void		   UseMobilePhone(bool bUse) { m_bMobilePhoneUse = bUse; }
	bool		   UpdateWeaponSync(CVector3 vecAim, CVector3 vecShotm, CVector3 vecLookAt);
	void		   UpdateHeadMoveSync(CVector3 vecHead);

	void		   SetDimension(DimensionId ucDimension);
	DimensionId    GetDimension() { return m_ucDimension; }
	void		   SetBlockWeaponDrop(bool drop) { m_bDrop = drop; }
	bool		   GetBlockWeaponDrop() { return m_bDrop; }

	void		   SetRemoteControl(bool bControl) { m_bRemoteControlLogedIn = bControl; }
	bool		   IsRemoteControl() { return m_bRemoteControlLogedIn; }

	void		   SetWantedLevel(int iWantedLevel) { m_iWantedLevel = iWantedLevel; }
	int			   GetWantedLevel() { return m_iWantedLevel; }
};
