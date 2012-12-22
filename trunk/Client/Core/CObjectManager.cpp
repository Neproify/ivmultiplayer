//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
//			  XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "CObjectManager.h"
#include <CLogFile.h>

unsigned long lastTimeObjectProcessed = 0;

void CObjectManager::Process()
{
	for(EntityId i = 0; i < this->GetMax(); ++i)
	{
		CObject * pObject = this->Get(i);
		if(pObject) {
			if(pObject->IsMoving()) {
				CVector3 target = pObject->GetMoveTarget();
				CVector3 start = pObject->GetStartPosititon();
				float pps = 60.0f / ((SharedUtility::GetTime() - lastTimeObjectProcessed));
				float xSpeed = ((target.fX - start.fX) * pps / pObject->GetMoveSpeed()); 
				float ySpeed = ((target.fY - start.fY) * pps / pObject->GetMoveSpeed());
				float zSpeed = ((target.fZ - start.fZ) * pps / pObject->GetMoveSpeed());
				
				CVector3 vecMove(xSpeed, ySpeed, zSpeed);

				CVector3 pos;
				pObject->GetPosition(pos);
				pos.fX += xSpeed;
				pos.fY += ySpeed;
				pos.fZ += zSpeed;
				pObject->SetPosition(pos);
				if((target-pos).Length() <= vecMove.Length()) {
					pObject->SetIsMoving(false);
					pObject->SetPosition(target);
				}
			}
			if(pObject->IsRotating()) {
				CVector3 target = pObject->GetMoveTargetRot();
				CVector3 start = pObject->GetStartRotation();
				float pps = 60.0f / ((SharedUtility::GetTime() - lastTimeObjectProcessed));
				float xSpeed = ((target.fX - start.fX) * pps / pObject->GetMoveSpeed()); 
				float ySpeed = ((target.fY - start.fY) * pps / pObject->GetMoveSpeed());
				float zSpeed = ((target.fZ - start.fZ) * pps / pObject->GetMoveSpeed());

				CVector3 vecMove(xSpeed, ySpeed, zSpeed);
				
				CVector3 rot;
				pObject->GetRotation(rot);
				rot.fX += xSpeed;
				rot.fY += ySpeed;
				rot.fZ += zSpeed;
				pObject->SetRotation(rot);
				if((target-rot).Length() <= vecMove.Length()) {
					pObject->SetIsRotating(false);
					pObject->SetRotation(target);
				}
			}
		}
	}

	lastTimeObjectProcessed = SharedUtility::GetTime();
}
