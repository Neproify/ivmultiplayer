//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVehicleInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"
#include "CPlayerInterface.h"

class CVehicleInterface
{
public:
	virtual EntityId                  GetVehicleId() = 0;
	virtual void                      Reset() = 0;
	// FIXME
	/*virtual void                      SetDriver(CPlayerInterface * pDriver) = 0;
	virtual CPlayerInterface * GetDriver() = 0;
	virtual void                      SetPassenger(BYTE byteSeatId, CPlayerInterface * pPassenger) = 0;
	virtual CPlayerInterface * GetPassenger(BYTE byteSeatId) = 0;
	virtual void                      SetOccupant(BYTE byteSeatId, CPlayerInterface * pOccupant) = 0;
	virtual CPlayerInterface * GetOccupant(BYTE byteSeatId) = 0;*/
	virtual void                      Respawn() = 0;
	virtual void                      SetModel(int iModelId) = 0;
	virtual int                       GetModel() = 0;
	virtual void                      SetHealth(unsigned int uHealth) = 0;
	virtual unsigned int              GetHealth() = 0;
	virtual void                      SetPosition(const CVector3& vecPosition) = 0;
	virtual void                      GetPosition(CVector3& vecPosition) = 0;
	virtual void                      SetRotation(const CVector3& vecRotation) = 0;
	virtual void                      GetRotation(CVector3& vecRotation) = 0;
	virtual void                      SetDirtLevel(float fDirtLevel) = 0;
	virtual float                     GetDirtLevel() = 0;
	virtual void                      SetTurnSpeed(const CVector3& vecTurnSpeed) = 0;
	virtual void                      GetTurnSpeed(CVector3& vecTurnSpeed) = 0;
	virtual void                      SetMoveSpeed(const CVector3& vecMoveSpeed) = 0;
	virtual void                      GetMoveSpeed(CVector3& vecMoveSpeed) = 0;
	virtual void                      SetColors(BYTE byteColor1, BYTE byteColor2, BYTE byteColor3, BYTE byteColor4) = 0;
	virtual void                      GetColors(BYTE &byteColor1, BYTE &byteColor2, BYTE &byteColor3, BYTE &byteColor4) = 0;
	virtual void                      SoundHorn(unsigned int iDuration) = 0;
	virtual void                      SetSirenState(bool bSirenState) = 0;
	virtual bool                      GetSirenState() = 0;
	virtual bool                      SetLocked(unsigned char ucLocked) = 0;
	virtual unsigned char             GetLocked() = 0;
	virtual void                      SetIndicatorState(bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight) = 0;
	virtual bool                      GetIndicatorState(unsigned char ucSlot) = 0;
	virtual void                      SetComponentState(unsigned char ucSlot, bool bOn) = 0;
	virtual bool                      GetComponentState(unsigned char ucSlot) = 0;
	virtual void                      ResetComponents(bool bNotify = true) = 0;
	virtual void                      SetVariation(unsigned char ucVariation) = 0;
	virtual unsigned char             GetVariation() = 0;
};
