//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CVFS.h
// Project: Client.VFS
// Author(s): adamix
// License: See LICENSE in root directory
//
//============================================================================== 

#include "CVFSInterface.h"

class CVFS : public CVFSInterface
{
private:
	SVFSFunctions * m_funcs;
	bool m_bConnected;
public:
	CVFS();
	~CVFS();
        SVFSFunctions * GetFunctions() { return m_funcs; };
		void SetKey(char * szKey);
        void ConnectDisk(const char * szFileName);
        void DisconnectDisk();
		void CreateDisk(const char * szFileName, int sizeInMB);
		void FormatDisk();
		bool IsDiskConnected() { return m_bConnected; };
		void DecryptDisk(const char * szNewFileName);
};
