//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: Main.cpp
// Project: Client.Launcher
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include <StdInc.h>

int ShowMessageBox(const char * szText, UINT uType = (MB_ICONEXCLAMATION | MB_OK))
{
	return MessageBox(NULL, szText, MOD_NAME, uType);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Get the GTA IV install directory from the registry
	char szInstallDirectory[MAX_PATH];
	bool bFoundCustomDirectory = false;

	// Check if protocol 'ivmp' and 'ivmultiplayer' is avaiable in registry
	if(!SharedUtility::ReadRegistryString(HKEY_CLASSES_ROOT, "ivmp", NULL, "", NULL, NULL)
		|| !SharedUtility::ReadRegistryString(HKEY_CLASSES_ROOT, "ivmultiplayer", NULL, "", NULL, NULL))
	{
		// Update
		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmp","","IVMultiplayer",strlen("IVMultiplayer"));
		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmultiplayer","","IVMultiplayer",strlen("IVMultiplayer"));
		
		String strcommand = String("\"%s\" \"%%1\"",SharedUtility::GetAbsolutePath("Client.Launcher.exe"));

		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmp","Url Protocol","",0);
		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmp\\shell\\open\\command\\","",strcommand.GetData(),strcommand.GetLength());
		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmp\\DefaultIcon","",String("Client.Launcher.exe,1").GetData(),strlen("Client.Launcher.exe,1"));

		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmultiplayer","Url Protocol","",0);
		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmultiplayer\\shell\\open\\command\\","",strcommand.GetData(),strcommand.GetLength());
		SharedUtility::WriteRegistryString(HKEY_CLASSES_ROOT,"ivmultiplayer\\DefaultIcon","",String("Client.Launcher.exe,1").GetData(),strlen("Client.Launcher.exe,1"));
	}

	// TODO: Steam registry entry support
	if(!SharedUtility::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\Rockstar Games\\Grand Theft Auto IV", 
		"InstallFolder", NULL, szInstallDirectory, sizeof(szInstallDirectory)) || 
		!SharedUtility::Exists(szInstallDirectory))
	{
		if(!SharedUtility::ReadRegistryString(HKEY_CURRENT_USER, "Software\\IVMP", "gtaivdir", NULL, 
			szInstallDirectory, sizeof(szInstallDirectory)) || 
			!SharedUtility::Exists(szInstallDirectory))
		{
			if(ShowMessageBox("Failed to retrieve GTA IV install directory from registry. Specify your GTA IV path now?", 
				(MB_ICONEXCLAMATION | MB_OKCANCEL)) == IDOK)
			{
				// Taken from http://vcfaq.mvps.org/sdk/20.htm
				BROWSEINFO browseInfo = { 0 };
				browseInfo.lpszTitle = "Pick a Directory";
				ITEMIDLIST * pItemIdList = SHBrowseForFolder(&browseInfo);

				if(pItemIdList != NULL)
				{
					// Get the name of the selected folder
					if(SHGetPathFromIDList(pItemIdList, szInstallDirectory))
						bFoundCustomDirectory = true;

					// Free any memory used
					IMalloc * pIMalloc = 0;
					if(SUCCEEDED(SHGetMalloc(&pIMalloc)))
					{
						pIMalloc->Free(pItemIdList);
						pIMalloc->Release();
					}
				}
			}

			if(!bFoundCustomDirectory)
			{
				ShowMessageBox("Failed to retrieve GTA IV install directory from registry. Cannot launch IV: Multiplayer.");
				return 1;
			}
		}
	}

	// Get the full path to LaunchGTAIV.exe
	String strApplicationPath("%s\\LaunchGTAIV.exe", szInstallDirectory);

	// Check if LaunchGTAIV.exe exists
	if(!SharedUtility::Exists(strApplicationPath.Get()))
	{
		ShowMessageBox("Failed to find LaunchGTAIV.exe. Cannot launch IV: Multiplayer.");
		return 1;
	}

	// If we have a custom directory save it
	if(bFoundCustomDirectory)
		SharedUtility::WriteRegistryString(HKEY_CURRENT_USER, "Software\\IVMP", "gtaivdir", szInstallDirectory, strlen(szInstallDirectory));

	// Get the full path of the client core
	String strClientCore(SharedUtility::GetAbsolutePath(CLIENT_CORE_NAME DEBUG_SUFFIX LIBRARY_EXTENSION));

	// Check if the client core exists
	if(!SharedUtility::Exists(strClientCore.Get()))
	{
		ShowMessageBox("Failed to find " CLIENT_CORE_NAME DEBUG_SUFFIX LIBRARY_EXTENSION ". Cannot launch IV: Multiplayer.");
		return 1;
	}

	// Get the full path of the launch helper
	String strLaunchHelper(SharedUtility::GetAbsolutePath(CLIENT_LAUNCH_HELPER_NAME DEBUG_SUFFIX LIBRARY_EXTENSION));

	// Check if the launch helper exists
	if(!SharedUtility::Exists(strLaunchHelper.Get()))
	{
		ShowMessageBox("Failed to find " CLIENT_LAUNCH_HELPER_NAME DEBUG_SUFFIX LIBRARY_EXTENSION". Cannot launch IV: Multiplayer.");
		return 1;
	}
	
	// Check if GTAIV is already running
	if(SharedUtility::IsProcessRunning("GTAIV.exe"))
	{
		if(ShowMessageBox("GTAIV is already running and needs to be terminated before IV: Multiplayer can be started. Do you want to do that now?", MB_ICONQUESTION | MB_YESNO ) == IDYES)
		{
			if(!SharedUtility::_TerminateProcess("GTAIV.exe"))
			{
				ShowMessageBox("GTAIV.exe could not be terminated. Cannot launch IV: Multiplayer.");
				return 1;
			}
		}
		else
		{
			ShowMessageBox("GTAIV.exe is already running. Cannot launch IV: Multiplayer.");
			return 1;
		}
	}

	// Check if LaunchGTAIV.exe is already running
	if(SharedUtility::IsProcessRunning("LaunchGTAIV.exe"))
	{
		if(ShowMessageBox("LaunchGTAIV is already running and needs to be terminated before IV: Multiplayer can be started. Do you want to do that now?", MB_ICONQUESTION | MB_YESNO ) == IDYES)
		{
			if(!SharedUtility::_TerminateProcess("LaunchGTAIV.exe"))
			{
				// Wait until we've successfully terminated the process
				Sleep(3000);
				if(SharedUtility::IsProcessRunning("LaunchGTAIV.exe"))
				{
					if(!SharedUtility::_TerminateProcess("LaunchGTAIV.exe"))
					{
						ShowMessageBox("LaunchGTAIV.exe could not be terminated. Cannot launch IV: Multiplayer.");
						return 1;
					}
				}
			}
		}
		else
		{
			ShowMessageBox("LaunchGTAIV.exe is already running. Cannot launch IV: Multiplayer.");
			return 1;
		}
	}

	// Check if we have an server connect command
	String strServer;
	String strPort;
	String strNewCommandLine = String(lpCmdLine);
	std::string strServerCheck = String(lpCmdLine);

	int iOffset = 0;
	bool bFound = false;
	std::size_t found = strServerCheck.find("-ivmp");// -[1]i[2]v[3]m[4]p[5]*space*[6]***.***.***.***
	if(found != std::string::npos)
	{
		iOffset = 6;
		bFound = true;
	}

	// check for ivmp commandline
	if(!bFound)
	{
		 found = strServerCheck.find("ivmp://");
		 iOffset = 7;
		 bFound = true;
	}

	// check for ivmultiplayer commandline
	if(!bFound)
	{
		 found = strServerCheck.find("ivmultiplayer://");
		 iOffset = 16;
		 bFound = true;
	}

	// Open default clientsettings
	CSettings::Open(SharedUtility::GetAbsolutePath("clientsettings.xml"));

	if(bFound)
	{
		std::string strServerInst = strServerCheck.substr(found+iOffset,strServerCheck.length());
		std::size_t pos2 = strServerInst.find(":");

		if(pos2 != std::string::npos) 
		{
			// Grab our connect data
			strServer = String("%s",strServerInst.substr(0,pos2).c_str());
			strPort = String("%s",strServerInst.substr(pos2+1,strServerInst.length()).c_str());

			// Parse the command line
			CSettings::ParseCommandLine(GetCommandLine());

			// Write connect data to settings xml
			CVAR_SET_STRING("currentconnect_server",strServer.Get());
			CVAR_SET_INTEGER("currentconnect_port",strPort.ToInteger());
			
			// Generate new commandline
			strNewCommandLine = String("%s -ivmp %s %s", lpCmdLine, strServer.Get(), strPort.Get());
		}
	}
	else
	{
		//TODO: remove old entries if they were not removed by the client
		CVAR_SET_STRING("currentconnect_server","0.0.0.0");
		CVAR_SET_INTEGER("currentconnect_port",9999);
	}
	
	// Close settings...
	CSettings::Close();

	// Generate the command line
	String strCommandLine("%s %s", strApplicationPath.Get(), strNewCommandLine.Get());

	// Start LaunchGTAIV.exe
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	if(!CreateProcess(strApplicationPath.Get(), (char *)strCommandLine.Get(), NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, 
		SharedUtility::GetAppPath(), &siStartupInfo, &piProcessInfo))
	{
		ShowMessageBox("Failed to start LaunchGTAIV.exe. Cannot launch IV: Multiplayer.");
		return 1;
	}

	// Inject LauncherLibrary.dll into LaunchGTAIV.exe
	int iReturn = SharedUtility::InjectLibraryIntoProcess(piProcessInfo.hProcess, strLaunchHelper.Get());

	// Did the injection fail?
	if(iReturn > 0)
	{
		// Terminate the process
		TerminateProcess(piProcessInfo.hProcess, 0);

		// Show the error message
		String strError("Unknown error. Cannot launch IV: Multiplayer.");

		if(iReturn == 1)
			strError = "Failed to write library path into remote process. Cannot launch IV: Multiplayer.";
		else if(iReturn == 2)
			strError = "Failed to create remote thread in remote process. Cannot launch IV: Multiplayer.";
		else if(iReturn == 3)
			strError = "Failed to open the remote process, Cannot launch IV: Multiplayer.";

		ShowMessageBox(strError.Get());
		return 1;
	}

	// Resume the LaunchGTAIV.exe thread
	ResumeThread(piProcessInfo.hThread);
	return 0;
}
