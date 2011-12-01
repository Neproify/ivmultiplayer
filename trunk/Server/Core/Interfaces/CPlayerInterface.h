//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPlayerInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"

class CPlayerInterface
{
public:
	virtual EntityId       GetPlayerId() = 0;
	virtual eStateType     GetState() = 0;
	virtual void           SpawnForPlayer(EntityId playerId) = 0;
	virtual void           KillForPlayer(EntityId playerId) = 0;
	virtual void           SpawnForWorld() = 0;
	virtual void           KillForWorld() = 0;
	// FIXME
	/*virtual void              SetVehicle(CNetworkVehicle * pVehicle) { m_pVehicle = pVehicle; }
	virtual CNetworkVehicle * GetVehicle() { return m_pVehicle; }
	virtual void              SetVehicleSeatId(BYTE byteSeatId) { m_byteVehicleSeatId = byteSeatId; }
	virtual BYTE              GetVehicleSeatId() { return m_byteVehicleSeatId; }*/
	virtual bool           SetName(String strName) = 0;
	virtual String         GetName() = 0;
	virtual bool           IsSpawned() = 0;
	virtual bool           SetModel(int iModelId) = 0;
	virtual int            GetModel() = 0;
	virtual void           SetPosition(CVector3 vecPosition) = 0;
	virtual void           GetPosition(CVector3 * vecPosition) = 0;
	virtual void           SetCurrentHeading(float fHeading) = 0;
	virtual float          GetCurrentHeading() = 0;
	virtual void           SetMoveSpeed(CVector3 vecMoveSpeed) = 0;
	virtual void           GetMoveSpeed(CVector3 * vecMoveSpeed) = 0;
	virtual void           SetDucking(bool bDuckState) = 0;
	virtual bool           IsDucking() = 0;
	virtual void           SetHealth(unsigned int uHealth) = 0;
	virtual unsigned int   GetHealth() = 0;
	virtual void           SetArmour(unsigned int uArmour) = 0;
	virtual unsigned int   GetArmour() = 0;
	virtual void           SetSpawnLocation(CVector3 vecPosition, float fHeading) = 0;
	virtual void           GetSpawnLocation(CVector3 * vecPosition, float * fHeading) = 0;
	virtual bool           GiveMoney(int iMoney) = 0;
	virtual bool           SetMoney(int iMoney) = 0;
	virtual int            GetMoney() = 0;
	virtual void           SetWeapon(unsigned int uWeapon) = 0;
	virtual unsigned int   GetWeapon() = 0;
	virtual void           SetAmmo(unsigned int uAmmo) = 0;
	virtual unsigned int   GetAmmo() = 0;
	virtual String         GetSerial() = 0;
	virtual void           SetControlState(CControlState * controlState) = 0;
	virtual void           GetPreviousControlState(CControlState * controlState) = 0;
	virtual void           GetControlState(CControlState * controlState) = 0;
	virtual void           SetColor(unsigned int color) = 0;	
	virtual unsigned int   GetColor() = 0;
	virtual unsigned short GetPing() = 0;
	virtual void           ResetClothes() = 0;
	virtual void           SetClothes(unsigned char ucBodyPart, unsigned char ucClothes) = 0;
	virtual unsigned char  GetClothes(unsigned char ucBodyPart) = 0;
};
