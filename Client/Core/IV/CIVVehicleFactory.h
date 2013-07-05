//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVVehicleFactory.cpp
// Project: Client.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVVehicle.h"
#include "../CGame.h"
#include "../COffsets.h"
#include <CLogFile.h>

class IVVehicleFactory
{
public:
	virtual 			~IVVehicleFactory();
	virtual IVVehicle*	Create(int modelIndex, int creator, int a3, int network);
	virtual int 		Delete(IVVehicle*);
	virtual IVVehicle*	Get_0(int a1); //Get index=0 
	virtual IVVehicle*	Get(int a1);
	virtual bool		m14(int a1, int a2, int a3);
};

class IVVehicleFactoryNY : public IVVehicleFactory
{

};