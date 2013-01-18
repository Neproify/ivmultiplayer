//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: 3DLabelNatives.cpp
// Project: Server.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "..\C3DLabels.h"
#include <SharedUtility.h>


extern C3DLabelManager * g_p3DLabelManager;

void _3DLabelNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("create3DLabel", Create, 7, "sfffibf");
	pScriptingManager->RegisterFunction("delete3DLabel", Delete, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelPosition", SetPosition, 4, "ifff");
	pScriptingManager->RegisterFunction("get3DLabelPosition", GetPosition, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelColor", SetColor, 2, "ii");
	pScriptingManager->RegisterFunction("get3DLabelColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("get3DLabelDimension", GetDimension, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelText", SetText, 2, "is");
	pScriptingManager->RegisterFunction("get3DLabelText", GetText, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelVisible", SetVisible, 2, "ib");
	pScriptingManager->RegisterFunction("get3DLabelVisible", GetVisible, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelStreamingDistance", SetStreamingDistance, 2, "if");
	pScriptingManager->RegisterFunction("get3DLabelStreamingDistance", GetStreamingDistance, 1, "i");
};

// create3DLabel(text, x, y, z, color, visible, streamingdistance)
SQInteger _3DLabelNatives::Create(SQVM * pVM)
{
	float fStreamingDistance;
	CVector3 vecPosition;
	SQBool bVisible;
	SQInteger iColor;
	const char * szName;
	sq_getstring(pVM, -7, &szName);
	sq_getvector3(pVM, -6, &vecPosition);
	sq_getinteger(pVM, -3, &iColor);
	sq_getbool(pVM, -2, &bVisible);
	sq_getfloat(pVM, -1, &fStreamingDistance);
	bool bToggle = (bVisible != 0);
	printf("New Label(%s, %i, %f)", szName, bToggle, fStreamingDistance);
	sq_pushinteger(pVM, g_p3DLabelManager->Add(szName, vecPosition, iColor, bToggle, fStreamingDistance));

	return 1;
}

// delete3DLabel(labelId)
SQInteger _3DLabelNatives::Delete(SQVM * pVM)
{
	LabelId labelId;
	sq_getinteger(pVM, -1, (SQInteger*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		g_p3DLabelManager->Remove(labelId);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// set3DLabelPosition(labelId, x, y, z)
SQInteger _3DLabelNatives::SetPosition(SQVM * pVM)
{
	CVector3 vecPosition;
	LabelId labelId;
	sq_getinteger(pVM, -4, (SQInteger*)&labelId);
	sq_getvector3(pVM, -3, &vecPosition);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		g_p3DLabelManager->GetAt(labelId)->SetPosition(vecPosition);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// get3DLabelPosition(labelId)
SQInteger _3DLabelNatives::GetPosition(SQVM * pVM)
{
	LabelId labelId;
	sq_getinteger(pVM, -1, (SQInteger*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		CVector3 vecPosition = g_p3DLabelManager->GetAt(labelId)->GetPosition();

		CSquirrelArguments args;
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		sq_pusharg(pVM, CSquirrelArgument(args, true));
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// set3DLabelColor(labelId, color)
SQInteger _3DLabelNatives::SetColor(SQVM * pVM)
{
	SQInteger iColor;
	LabelId labelId;
	sq_getinteger(pVM, -2, (SQInteger*)&labelId);

	sq_getinteger(pVM, -1, &iColor);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
			g_p3DLabelManager->GetAt(labelId)->SetColor(iColor);
			sq_pushbool(pVM, true);
			return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// get3DLabelColor(labelId)
SQInteger _3DLabelNatives::GetColor(SQVM * pVM)
{
	LabelId labelId;
	sq_getinteger(pVM, -1, (SQInteger*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		sq_pushinteger(pVM, g_p3DLabelManager->GetAt(labelId)->GetColor());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// set3DLabelDimension(labelId, dimension)
SQInteger _3DLabelNatives::SetDimension(SQVM * pVM)
{
	SQInteger iDimension;
	LabelId labelId;
	sq_getinteger(pVM, -2, (SQInteger*)&labelId);

	sq_getinteger(pVM, -1, &iDimension);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
			g_p3DLabelManager->GetAt(labelId)->SetDimension(iDimension);
			sq_pushbool(pVM, true);
			return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// get3DLabelDimension(labelId)
SQInteger _3DLabelNatives::GetDimension(SQVM * pVM)
{
	LabelId labelId;
	sq_getinteger(pVM, -1, (SQInteger*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		sq_pushinteger(pVM, g_p3DLabelManager->GetAt(labelId)->GetDimension());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// set3DLabelText(labelId, text)
SQInteger _3DLabelNatives::SetText(SQVM * pVM)
{
	const char* szText;
	LabelId labelId;
	sq_getinteger(pVM, -2, (SQInteger*)&labelId);

	sq_getstring(pVM, -1, &szText);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
			g_p3DLabelManager->GetAt(labelId)->SetText(szText);
			sq_pushbool(pVM, true);
			return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// get3DLabelText(labelId)
SQInteger _3DLabelNatives::GetText(SQVM * pVM)
{
	LabelId labelId;
	sq_getinteger(pVM, -1, (SQInteger*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		sq_pushstring(pVM, g_p3DLabelManager->GetAt(labelId)->GetText(), g_p3DLabelManager->GetAt(labelId)->GetText().GetLength());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// set3DLabelVisible(labelId, visible)
SQInteger _3DLabelNatives::SetVisible(SQVM * pVM)
{
	SQBool bVisible;
	LabelId labelId;
	sq_getinteger(pVM, -2, (SQInteger*)&labelId);

	sq_getbool(pVM, -1, &bVisible);


	if(g_p3DLabelManager->DoesExist(labelId))
	{
		bool bToggleVisible = (bVisible != 0);
		g_p3DLabelManager->GetAt(labelId)->SetVisible(bToggleVisible);
		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// get3DLabelVisible(labelId)
SQInteger _3DLabelNatives::GetVisible(SQVM * pVM)
{
	LabelId labelId;
	sq_getinteger(pVM, -1, (SQInteger*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		sq_pushbool(pVM, g_p3DLabelManager->GetAt(labelId)->IsVisible());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// set3DLabelStreamingDistance(labelId, streamingDistance)
SQInteger _3DLabelNatives::SetStreamingDistance(SQVM * pVM)
{
	float fStreamingDistance;
	LabelId labelId;
	sq_getinteger(pVM, -2, (SQInteger*)&labelId);

	sq_getfloat(pVM, -1, &fStreamingDistance);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		g_p3DLabelManager->GetAt(labelId)->SetStreamingDistance(fStreamingDistance);
			sq_pushbool(pVM, true);
			return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// get3DLabelStreamingDistance(labelId)
SQInteger _3DLabelNatives::GetStreamingDistance(SQVM * pVM)
{
	LabelId labelId;
	sq_getinteger(pVM, -1, (SQInteger*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		sq_pushfloat(pVM, g_p3DLabelManager->GetAt(labelId)->GetStreamingDistance());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
