//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CObjectManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            mabako
//            XForce
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

				float moveProgress = (SharedUtility::GetTime() - pObject->GetMoveStartTime()) / pObject->GetMoveDuration();

				float xSpeed = ((target.fX - start.fX) * moveProgress); 
				float ySpeed = ((target.fY - start.fY) * moveProgress);
				float zSpeed = ((target.fZ - start.fZ) * moveProgress);
				
				CVector3 vecMove(xSpeed, ySpeed, zSpeed);

				start.fX += xSpeed;
				start.fY += ySpeed;
				start.fZ += zSpeed;
				pObject->SetPosition(start);
				if((target-start).Length() <= vecMove.Length()) {
					pObject->SetIsMoving(false);
					pObject->SetPosition(target);
				}
			}
			if(pObject->IsRotating()) {
				CVector3 target = pObject->GetMoveTargetRot();
				CVector3 start = pObject->GetStartRotation();
				
				float moveProgress = (SharedUtility::GetTime() - pObject->GetMoveStartTime()) / pObject->GetMoveDuration();

				float xSpeed = ((target.fX - start.fX) * moveProgress); 
				float ySpeed = ((target.fY - start.fY) * moveProgress);
				float zSpeed = ((target.fZ - start.fZ) * moveProgress);
				
				CVector3 vecMove(xSpeed, ySpeed, zSpeed);

				start.fX += xSpeed;
				start.fY += ySpeed;
				start.fZ += zSpeed;
				pObject->SetRotation(start);
				if((target-start).Length() <= vecMove.Length()) {
					pObject->SetIsRotating(false);
					pObject->SetRotation(target);
				}
			}
		}
	}

	lastTimeObjectProcessed = SharedUtility::GetTime();
}
