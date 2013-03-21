//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayer.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CServer.h"
#include "Interfaces/InterfaceCommon.h"
#include "CVehicle.h"
#include "Scripting/CScriptingManager.h"

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
	int			  m_iHealth;
	unsigned int  m_uArmour;
	bool          m_bHelmet;
	bool		  m_bControls;
	eStateType    m_state;
	CVector3      m_vecSpawnPosition;
	float         m_fSpawnHeading;
	float		  m_fGravity;
	int           m_iMoney;
	bool		  m_bPhysicsEnabled;
	unsigned int  m_uWeapon;
	unsigned int  m_uAmmo;
	AimSyncData   m_aimSyncData;
	unsigned int  m_uiColor;
	unsigned char m_ucClothes[11];
	bool		  m_bAnimating;
	bool		  m_bDrunk;
	char*		  m_szAnimGroup;
	char*		  m_szAnimSpec;
	float		  m_fAnimTime;
	bool		  m_bMobilePhoneUse;
	CVector3	  m_vecLastAim;
	CVector3	  m_vecLastShot;
	CVector3	  m_vecLastHeadMove;
	DimensionId   m_ucDimension;
	bool		  m_bWeaponDropEnabled;
	unsigned int  m_uiWantedLevel;
	CheckGTAFiles m_FileCheck;

public:
	CPlayer(EntityId playerId, String strName);
	~CPlayer();

	// Internal (sync & etc)
	SQInstance   * GetScriptingInstance() { return m_pScriptingInstance; }
	EntityId       GetPlayerId() { return m_playerId; }
	void           StoreOnFootSync(OnFootSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	void           StoreInVehicleSync(CVehicle * pVehicle, InVehicleSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	void           StorePassengerSync(CVehicle * pVehicle, PassengerSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	void           StoreSmallSync(SmallSyncData * syncPacket, bool bHasAimSyncData, AimSyncData * aimSyncData);
	bool		   UpdateWeaponSync(CVector3 vecAim, CVector3 vecShotm, CVector3 vecLookAt);
	void		   UpdateHeadMoveSync(CVector3 vecHead);
	void           Process();
	void           AddForPlayer(EntityId playerId);
	void           DeleteForPlayer(EntityId playerId);
	void           AddForWorld();
	void           DeleteForWorld();
	void           SpawnForPlayer(EntityId playerId);
	void           KillForPlayer(EntityId playerId);
	void           SpawnForWorld();
	void           KillForWorld();
	void           SetVehicle(CVehicle * pVehicle) { m_pVehicle = pVehicle; }
	void           SetVehicleSeatId(BYTE byteSeatId) { m_byteVehicleSeatId = byteSeatId; }
	void           SetControlState(CControlState * controlState);
	void           GetPreviousControlState(CControlState * controlState) { *controlState = m_previousControlState; }
	void           GetControlState(CControlState * controlState) { *controlState = m_currentControlState; }
	void           SetState(eStateType state);
	void		   SetFileCheck(CheckGTAFiles pFiles) { m_FileCheck = pFiles; }

	// Functions:
	eStateType     GetState() { return m_state; }
	CVehicle     * GetVehicle() { return m_pVehicle; }
	bool           SetName(String strName);
	String         GetName() { return m_strName; }
	bool           IsSpawned() { return m_bSpawned; }
	bool           SetModel(int iModelId);
	int            GetModel() { return m_iModelId; }
	void           SetCoordinates(const CVector3& vecPosition);
	void           GetCoordinates(CVector3& vecPosition) { vecPosition = m_vecPosition; }
	void           SetHeading(float fHeading);
	float          GetHeading() { return m_fHeading; }
	void           SetVelocity(const CVector3& vecMoveSpeed);
	void           GetVelocity(CVector3& vecMoveSpeed) { vecMoveSpeed = m_vecMoveSpeed; }
	void           SetDucking(bool bDuckState);
	bool           IsDucking() { return m_bDuckState; }
	void		   GiveHelmet();//TODO: replace with toggle helmet + 1rpc
	void		   RemoveHelmet();
	void           SetHealth(int iHealth);
	int			   GetHealth();
	void		   SetGravity(float fGravity);
	float		   GetGravity() { return m_fGravity; }
	void           SetArmour(unsigned int uArmour);
	unsigned int   GetArmour() { return m_uArmour; }
	void		   SetTime(unsigned int ucHour, unsigned int ucMinute, unsigned int ucMinuteDuration);
	void		   SetWeather(BYTE byteWeatherId);
	void           SetSpawnLocation(const CVector3& vecPosition, float fHeading);
	void           GetSpawnLocation(CVector3& vecPosition, float * fHeading);
	void           SetColor(unsigned int color);	
	unsigned int   GetColor() { return m_uiColor; }
	void		   SetWantedLevel(unsigned int uiWantedLevel);
	unsigned int   GetWantedLevel() { return m_uiWantedLevel; }
	void		   DisplayText(float fX, float fY, String strText, unsigned int uiTime);
	void		   DisplayInfoText(String strText, unsigned int uiTime);
	void		   SetInvincible(bool bInvincible);
	bool		   WarpIntoVehicle(EntityId vehicleId, BYTE byteSeatId = 0);
	void		   RemoveFromVehicle(bool bAnimated);
	void		   TriggerClientEvent(CSquirrelArguments sqArgs);
	void		   UseMobilePhone(bool bUse);
	void		   SetBlockWeaponDrop(bool drop);
	bool		   GetBlockWeaponDrop() { return m_bWeaponDropEnabled; }
	void		   SetDimension(DimensionId ucDimension);
	DimensionId    GetDimension() { return m_ucDimension; }

	// Toggles
	void		   TogglePhysics(bool bEnabled);
	void		   ToggleFrozen(bool bPlayerFrozen, bool bCameraFrozen);
	void		   TogglePayAndSpray(bool bToggle);
	void		   ToggleAutoAim(bool bToggle);
	void		   ToggleDrunk(bool bToggle);
	void		   ToggleControls(bool bToggle);
	void		   ToggleHUD(bool bVisible);
	void		   ToggleRadar(bool bVisible);
	void		   ToggleNameTags(bool bToggle);
	void		   ToggleAreaNames(bool bToggle);

	// Camera
	void           SetCameraPos(const CVector3& vecPosition);
	void           SetCameraLookAt(const CVector3& vecPosition);

	// Weapons
	void		   GiveWeapon(unsigned int uiWeaponModelId, unsigned int uiAmmo);
	void           SetWeapon(unsigned int uWeapon);
	unsigned int   GetWeapon() { return m_uWeapon; }
	void           SetAmmo(unsigned int uAmmo);
	unsigned int   GetAmmo() { return m_uAmmo; }

	// Clothes
	void           SetClothes(unsigned char ucBodyPart, unsigned char ucClothes);
	unsigned char  GetClothes(unsigned char ucBodyPart);
	void           ResetClothes();

	// Money
	bool           GiveMoney(int iMoney);
	bool           SetMoney(int iMoney);
	int            GetMoney() { return m_iMoney; }

	// Camera
	void		   SetCameraBehind();
	void		   ResetCamera();

	// Networking
	String         GetSerial();
	unsigned short GetPing();
	String         GetIp();
	void           Kick(bool bSendNotification = true);
	void           Ban(unsigned int uiSeconds);

	// Other:
	bool		   IsDead() { return m_state == STATE_TYPE_DEATH; }
	bool		   IsJoined() { return m_bJoined; }
	void		   SetJoined(bool bJoin) { m_bJoined = bJoin; }
	bool           IsOnFoot() { return (m_pVehicle == NULL); }
	bool           IsInVehicle() { return (m_pVehicle != NULL); }
	BYTE           GetVehicleSeatId() { return m_byteVehicleSeatId; }
	unsigned int   GetFileChecksum(int iFile);
};
