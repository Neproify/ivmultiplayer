//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CXLiveHook.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//          Contains public domain code from xliveless by listener
//
//==============================================================================

#include "CXLiveHook.h"
#include "Patcher/CPatcher.h"
#include "CGame.h"
#include <CLogFile.h>

#define FAKE_MAGIC 0xDEADDEAD

// XLive Functions
int WINAPI XWSAStartup(WORD wVersionRequested, LPWSADATA lpWsaData)
{
	lpWsaData->wVersion = 2;
	return 0;
}

void WINAPI XWSACleanup()
{
	return;
}

SOCKET WINAPI XSocketCreate(int af, int type, int protocol)
{
	return INVALID_SOCKET;
}

int WINAPI XSocketClose(SOCKET s)
{
	return 0;
}

int WINAPI XSocketShutdown(SOCKET s, int how)
{
	return 0;
}

int WINAPI XSocketIOCTLSocket(SOCKET s, long cmd, long * argp)
{
	return 0;
}

int WINAPI XSocketSetSockOpt(SOCKET s, int level, int optname, const char * optval, int optlen)
{
	return 0;
}

int WINAPI XSocketGetSockName(SOCKET s, sockaddr_in * name, int * namelen)
{
	if(namelen && name && *namelen == sizeof(sockaddr_in)) 
		memset(name, 0, sizeof(sockaddr_in));

	return 0;
}

SOCKET WINAPI XSocketBind(SOCKET s, sockaddr_in * addr, int * addrlen)
{
	return INVALID_SOCKET;
}

int WINAPI XSocketConnect(SOCKET s, sockaddr_in * addr, int * addrlen)
{
	return 0;
}

int WINAPI XSocketListen(SOCKET s, int backlog)
{
	return 0;
}

SOCKET WINAPI XSocketAccept(SOCKET s, sockaddr_in * addr, int * addrlen)
{
	return INVALID_SOCKET;
}

int WINAPI XSocketSelect(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, const struct timeval * timeout)
{
	return 0;
}

int WINAPI XSocketRecv(SOCKET s, char * buf, int len, int flags)
{
	return 0;
}

int WINAPI XSocketRecvFrom(SOCKET s, char * buf, int len, int flags, sockaddr_in * from, int fromlen)
{
	return 0;
}

int WINAPI XSocketSend(SOCKET s, char * buf, int len, int flags)
{
	return 0;
}

int WINAPI XSocketSendTo(SOCKET s, char * buf, int len, int flags, sockaddr_in * to, int tolen)
{
	return 0;
}

int WINAPI XSocketInet_Addr(const char * cp)
{
	return 0;
}

int WINAPI XWSAGetLastError()
{
	return WSAENETDOWN; // 0 ?
}

WORD WINAPI XSocketNTOHS(u_short netshort)
{
	return((netshort & 0xFF00) >> 8) | ((netshort & 0xFF) << 8);
}

DWORD WINAPI XSocketNTOHL(u_long netlong)
{
	return((netlong & 0xFF000000) >> 24) | ((netlong & 0x00FF0000) >> 8) | ((netlong & 0x0000FF00) << 8) | ((netlong & 0x000000FF) << 24);
}

int WINAPI XNetStartup(void *)
{
	return 0;
}

int WINAPI XNetCleanup()
{
	return 0;
}

int WINAPI XNetXnAddrToInAddr(DWORD, DWORD, DWORD * p)
{
	*p = 0;
	return 0;
}

DWORD WINAPI XNetServerToInAddr(DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XNetUnregisterInAddr(DWORD)
{
	return 0;
}

int WINAPI XNetGetConnectStatus(DWORD)
{
	return 0;	
}

DWORD WINAPI XNetQosListen(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XNetQosLookup(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XNetQosRelease(DWORD)
{
	return 0; 
}

DWORD WINAPI XNetGetTitleXnAddr(DWORD * pAddr)
{
	*pAddr = 0x0100007F; // 127.0.0.1
	return 4; 
}

DWORD WINAPI XNetGetEthernetLinkStatus()
{
	return 1; 
}

DWORD WINAPI XNetSetSystemLinkPort(DWORD)
{
	return 0; 
}

int WINAPI XNotifyGetNext(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XNotifyPositionUI(DWORD dwPosition)
{
	return 0;
}

DWORD WINAPI XGetOverlappedExtendedError(void *)
{
	return 0;
}

DWORD WINAPI XGetOverlappedResult(void *, DWORD * pResult, DWORD)
{
	if(pResult)
		*pResult = 0;	// 0 elements enumerated

	return 0;
}

int WINAPI XLiveInitialize(DWORD)
{
	return 0;
}

int WINAPI XLiveInput(DWORD * p)
{
	p[5] = 0;
	return 1;	// -1 ?
}

int WINAPI XLiveRender()
{
	return 0;
}

int WINAPI XLiveUninitialize()
{
	return 0;
}

int WINAPI XLiveOnCreateDevice(DWORD, DWORD)
{
	return 0;
}

int WINAPI XLiveOnResetDevice(DWORD)
{
	return 0;
}

int WINAPI XHVCreateEngine(DWORD, DWORD, DWORD)
{
	// Disable live voice
	return -1;
}

DWORD WINAPI XLivePBufferAllocate(int size, FakeProtectedBuffer ** pBuffer)
{
	*pBuffer = (FakeProtectedBuffer *)malloc(size + 16);

	if(!*pBuffer) 
		return E_OUTOFMEMORY;

	// Some arbitrary number
	(*pBuffer)->dwMagic = FAKE_MAGIC;
	(*pBuffer)->iSize = size;
	return 0;
}

DWORD WINAPI XLivePBufferFree(FakeProtectedBuffer * pBuffer)
{
	if(pBuffer && pBuffer->dwMagic == FAKE_MAGIC)
		free(pBuffer);

	return 0;
}

DWORD WINAPI XLivePBufferGetByte(FakeProtectedBuffer * pBuffer, int offset, BYTE * value)
{
	if(!pBuffer || pBuffer->dwMagic != FAKE_MAGIC || !value || offset < 0 || offset > pBuffer->iSize)
		return 0;

	*value = pBuffer->byteData[offset];
	return 0;
}

DWORD WINAPI XLivePBufferSetByte(FakeProtectedBuffer * pBuffer, int offset, BYTE value)
{
	if(!pBuffer || pBuffer->dwMagic != FAKE_MAGIC || offset < 0 || offset > pBuffer->iSize)
		return 0;

	pBuffer->byteData[offset] = value;
	return 0;
}

DWORD WINAPI XLivePBufferGetDWORD(FakeProtectedBuffer * pBuffer, DWORD dwOffset, DWORD * pdwValue)
{
	if(!pBuffer || pBuffer->dwMagic != FAKE_MAGIC || dwOffset < 0 || dwOffset > (DWORD)(pBuffer->iSize - 4) || !pdwValue)
		return 0;

	*pdwValue = *(DWORD *)(pBuffer->byteData + dwOffset);
	return 0;
}

DWORD WINAPI XLivePBufferSetDWORD(FakeProtectedBuffer * pBuffer, DWORD dwOffset, DWORD dwValue)
{
	if(!pBuffer || pBuffer->dwMagic != FAKE_MAGIC || dwOffset < 0 || dwOffset > (DWORD)(pBuffer->iSize - 4))
		return 0;

	*(DWORD *)(pBuffer->byteData + dwOffset) = dwValue;
	return 0;
}

int WINAPI XLiveGetUpdateInformation(DWORD)
{
	// No update
	return -1;
}

int WINAPI XLiveUpdateSystem(DWORD)
{
	// No update
	return -1;
}

int WINAPI XLivePreTranslateMessage(DWORD)
{
	return 0;
}

DWORD WINAPI XLiveProtectData(BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE h)
{
	*pDataSize = dwInDataSize;

	if(*pDataSize >= dwInDataSize && pOutBuffer)
		memcpy(pOutBuffer, pInBuffer, dwInDataSize);

	return 0;
}

DWORD WINAPI XLiveUnprotectData(BYTE * pInBuffer, DWORD dwInDataSize, BYTE * pOutBuffer, DWORD * pDataSize, HANDLE h)
{
	*pDataSize = dwInDataSize;

	if(*pDataSize >= dwInDataSize && pOutBuffer)
		memcpy(pOutBuffer, pInBuffer, dwInDataSize);

	return 0;
}

DWORD WINAPI XLiveCreateProtectedDataContext(DWORD * dwType, HANDLE * pHandle)
{
	if(pHandle)
		*pHandle =(HANDLE)1;

	return 0;
}

DWORD WINAPI XLiveQueryProtectedDataInformation(HANDLE h, DWORD * p)
{
	return 0;
}

DWORD WINAPI XLiveCloseProtectedDataContext(HANDLE h)
{
	return 0;
}

int WINAPI XShowPlayerReviewUI(DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XShowGuideUI(DWORD)
{
	return 1;
}

int WINAPI XCloseHandle(DWORD)
{
	return 0;
}

int WINAPI XShowGamerCardUI(DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XCancelOverlapped(DWORD)
{
	return 0;
}

int WINAPI XEnumerate(HANDLE hEnum, void * pvBuffer, DWORD cbBuffer, DWORD *, void * pOverlapped)
{
	// Some error?
	return 0;
}

int WINAPI XShowSigninUI(DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserGetXUID(DWORD, DWORD * pXuid)
{
	pXuid[0] = pXuid[1] = 0x10001000; 

	// ???
	return 0;
}

int WINAPI XUserGetSigninState(DWORD)
{
	// eXUserSigninState_SignedInLocally
	return 1;
}

int WINAPI XUserGetName(DWORD dwUserId, char * pBuffer, DWORD dwBufLen)
{
	if(dwBufLen < 6)
		return 1;

	strcpy(pBuffer, "IV:MP");
	return 0;
}

int WINAPI XUserAreUsersFriends(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserCheckPrivilege(DWORD user, DWORD priv, PBOOL b)
{
	*b = false;
	return 0;
}

int WINAPI XUserGetSigninInfo(DWORD dwUser, DWORD type, DWORD * pInfo)
{
	// Some arbitrary id for offline user, INVALID_XUID for online user
	pInfo[0] = pInfo[1] = (type == 2) ? ((dwUser + 1) * 0x10001000) : 0;
	return 0;
}

HANDLE WINAPI XNotifyCreateListener(DWORD l, DWORD h)
{
	// Any non-zero value (Zero treated as fatal error).
	return(HANDLE)1;
}

int WINAPI XUserSetProperty(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserSetContext(DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XUserWriteAchievements(DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XUserCreateAchievementEnumerator(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum)
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

DWORD WINAPI XUserReadStats(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD * pcbResults, DWORD * pResults, void *)
{
	CLogFile::Print("[XLiveHook] Start reading XUser stats...");
	*pcbResults = 0;
	*pResults = static_cast<DWORD>(0); // have to cast it, need to test if it's working now, some crashdumps pointed at this..
	CLogFile::Print("[XLiveHook] Successfully finished reading XUser stats...");
	return 0;
}

DWORD WINAPI XUserCreateStatsEnumeratorByRank(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum)
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

DWORD WINAPI XUserCreateStatsEnumeratorByXuid(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PHANDLE phEnum)
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

int WINAPI XUserSetContextEx(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XLivePBufferGetByteArray(FakeProtectedBuffer * pBuffer, int offset, BYTE * destination, int size)
{
	if(!pBuffer || pBuffer->dwMagic != FAKE_MAGIC || !destination || offset < 0 || offset+size > pBuffer->iSize)
		return 0;

	memcpy(destination, pBuffer->byteData+offset, size);
	return 0;
}

DWORD WINAPI XLivePBufferSetByteArray(FakeProtectedBuffer * pBuffer, int offset, BYTE * source, int size)
{
	if(!pBuffer || pBuffer->dwMagic != FAKE_MAGIC || !source || offset < 0 || offset+size > pBuffer->iSize)
		return 0;

	memcpy(pBuffer->byteData+offset, source, size);
	return 0;
}

int WINAPI XLiveInitializeEx(void *, int)
{
	return 0;
}

DWORD WINAPI XSessionCreate(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return -1;
}

DWORD WINAPI XStringVerify(DWORD, DWORD, DWORD, DWORD, DWORD, WORD * pResult, DWORD)
{
	*pResult = 0;
	return 0;
}

DWORD WINAPI XStorageUploadFromMemory(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XOnlineStartup()
{
	return 0; 
}

int WINAPI XOnlineCleanup()
{
	return 0;
}

DWORD WINAPI XFriendsCreateEnumerator(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XUserMuteListQuery(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

int WINAPI XInviteGetAcceptedInfo(DWORD, DWORD)
{
	return 1; 
}

int WINAPI XInviteSend(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionWriteStats(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

int WINAPI XSessionStart(DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XSessionSearchEx(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionModify(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionMigrateHost(DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

int WINAPI XOnlineGetNatType()
{
	return 0; 
}

DWORD WINAPI XSessionLeaveLocal(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XSessionJoinRemote(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionJoinLocal(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionGetDetails(DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

int WINAPI XSessionFlushStats(DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XSessionDelete(DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XUserReadProfileSettings(DWORD dwTitleId, DWORD dwUserIndex, DWORD dwNumSettingIds, DWORD * pdwSettingIds, DWORD * pcbResults, XUSER_READ_PROFILE_SETTINGS * pResults, DWORD pOverlapped)
{
	if(*pcbResults < 1036) 
	{
		// TODO: make correct calculation by IDs.
		*pcbResults = 1036;
		return ERROR_INSUFFICIENT_BUFFER;
	}

	memset(pResults, 0, *pcbResults);
	pResults->dwLength = *pcbResults - sizeof(XUSER_READ_PROFILE_SETTINGS);
	pResults->pSettings =(BYTE *)pResults + sizeof(XUSER_READ_PROFILE_SETTINGS);
	return 0;
}

int WINAPI XSessionEnd(DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XSessionArbitrationRegister(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) 
{ 
	return 0; 
}

DWORD WINAPI XTitleServerCreateEnumerator(DWORD, DWORD, DWORD, PHANDLE phEnum) 
{
	*phEnum = INVALID_HANDLE_VALUE;
	return 0;
}

DWORD WINAPI XSessionLeaveRemote(DWORD, DWORD, DWORD, DWORD)
{ 
	return 0; 
}

DWORD WINAPI XUserWriteProfileSettings(DWORD, DWORD, DWORD, DWORD)
{
	return 0;
}

DWORD WINAPI XStorageBuildServerPath(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return 0; 
}

DWORD WINAPI XStorageDownloadToMemory(DWORD dwUserIndex, DWORD, DWORD, DWORD, DWORD, DWORD, void *)
{
	return 0; 
}

DWORD WINAPI XLiveProtectedVerifyFile(HANDLE hContentAccess, VOID * pvReserved, PCWSTR pszFilePath)
{
	return 0;
}

DWORD WINAPI XLiveContentCreateAccessHandle(DWORD dwTitleId, void * pContentInfo, DWORD dwLicenseInfoVersion, void * xebBuffer, DWORD dwOffset, HANDLE * phAccess, void * pOverlapped)
{
	if(phAccess)
		*phAccess = INVALID_HANDLE_VALUE;

	// TODO: Fix it
	return E_OUTOFMEMORY;
}

DWORD WINAPI XLiveContentUninstall(void * pContentInfo, void * pxuidFor, void * pInstallCallbackParams)
{
	return 0;
}

DWORD WINAPI XLiveContentGetPath(DWORD dwUserIndex, void * pContentInfo, wchar_t * pszPath, DWORD * pcchPath)
{
	if(pcchPath)
		*pcchPath = 0;

	if(pszPath)
		*pszPath = 0;

	return 0;
}

DWORD WINAPI XLiveContentCreateEnumerator(DWORD, void *, DWORD *pchBuffer, HANDLE * phContent)
{
	if(phContent)
		*phContent = INVALID_HANDLE_VALUE;

	return 0;
}

DWORD WINAPI XLiveContentRetrieveOffersByDate(DWORD dwUserIndex, DWORD dwOffserInfoVersion, SYSTEMTIME * pstStartDate, void * pOffserInfoArray, DWORD * pcOfferInfo, void * pOverlapped)
{
	if(pcOfferInfo)
		*pcOfferInfo = 0;

	return 0;
} 

DWORD WINAPI XShowMarketplaceUI(DWORD dwUserIndex, DWORD dwEntryPoint, ULONGLONG dwOfferId, DWORD dwContentCategories)
{
	return 1;
}

DWORD WINAPI xlive_5367(HANDLE, DWORD, DWORD, BYTE *, DWORD)
{
	return 1;
}

DWORD WINAPI xlive_5372(HANDLE, DWORD, DWORD, DWORD, BYTE *, HANDLE)
{
	return 1;
}

static BYTE		dwDetourPatch[121];

void CXLiveHook::Install()
{
	// Detour XWSAStartup (xlive_1)
	dwDetourPatch[0] = CPatcher::InstallDetourPatchWithData("xlive.dll", 1, (DWORD)XWSAStartup);
	// Detour XWSACleanup (xlive_2)
	dwDetourPatch[1] = CPatcher::InstallDetourPatchWithData("xlive.dll", 2, (DWORD)XWSACleanup);
	// Detour XSocketCreate (xlive_3)
	dwDetourPatch[2] = CPatcher::InstallDetourPatchWithData("xlive.dll", 3, (DWORD)XSocketCreate);
	// Detour XSocketClose (xlive_4)
	dwDetourPatch[3] = CPatcher::InstallDetourPatchWithData("xlive.dll", 4, (DWORD)XSocketClose);
	// Detour XSocketShutdown (xlive_5)
	dwDetourPatch[4] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5, (DWORD)XSocketShutdown);
	// Detour XSocketIOCTLSocket (xlive_6)
	dwDetourPatch[5] = CPatcher::InstallDetourPatchWithData("xlive.dll", 6, (DWORD)XSocketIOCTLSocket);
	// Detour XSocketSetSockOpt (xlive_7)
	dwDetourPatch[6] = CPatcher::InstallDetourPatchWithData("xlive.dll", 7, (DWORD)XSocketSetSockOpt);
	// Detour XSocketGetSockName (xlive_9)
	dwDetourPatch[7] = CPatcher::InstallDetourPatchWithData("xlive.dll", 9, (DWORD)XSocketGetSockName);
	// Detour XSocketBind (xlive_11)
	dwDetourPatch[8] = CPatcher::InstallDetourPatchWithData("xlive.dll", 11, (DWORD)XSocketBind);
	// Detour XSocketConnect (xlive_12)
	dwDetourPatch[9] = CPatcher::InstallDetourPatchWithData("xlive.dll", 12, (DWORD)XSocketConnect);
	// Detour XSocketListen (xlive_13)
	dwDetourPatch[10] = CPatcher::InstallDetourPatchWithData("xlive.dll", 13, (DWORD)XSocketListen);
	// Detour XSocketAccept (xlive_14)
	dwDetourPatch[11] = CPatcher::InstallDetourPatchWithData("xlive.dll", 14, (DWORD)XSocketAccept);
	// Detour XSocketSelect (xlive_15)
	dwDetourPatch[12] = CPatcher::InstallDetourPatchWithData("xlive.dll", 15, (DWORD)XSocketSelect);
	// Detour XSocketRecv (xlive_18)
	dwDetourPatch[13] = CPatcher::InstallDetourPatchWithData("xlive.dll", 18, (DWORD)XSocketRecv);
	// Detour XSocketRecvFrom (xlive_20)
	dwDetourPatch[14] = CPatcher::InstallDetourPatchWithData("xlive.dll", 20, (DWORD)XSocketRecvFrom);
	// Detour XSocketSend (xlive_22)
	dwDetourPatch[15] = CPatcher::InstallDetourPatchWithData("xlive.dll", 22, (DWORD)XSocketSend);
	// Detour XSocketSendTo (xlive_24)
	dwDetourPatch[16] = CPatcher::InstallDetourPatchWithData("xlive.dll", 24, (DWORD)XSocketSendTo);
	// Detour XSocketInet_Addr (xlive_26)
	dwDetourPatch[17] = CPatcher::InstallDetourPatchWithData("xlive.dll", 26, (DWORD)XSocketInet_Addr);
	// Detour XWSAGetLastError (xlive_27)
	dwDetourPatch[18] = CPatcher::InstallDetourPatchWithData("xlive.dll", 27, (DWORD)XWSAGetLastError);
	// Detour XSocketNTOHS (xlive_38)
	dwDetourPatch[19] = CPatcher::InstallDetourPatchWithData("xlive.dll", 38, (DWORD)XSocketNTOHS);
	// Detour XSocketNTOHL (xlive_39)
	dwDetourPatch[20] = CPatcher::InstallDetourPatchWithData("xlive.dll", 39, (DWORD)XSocketNTOHL);
	// Detour XNetStartup (xlive_51)
	dwDetourPatch[21] = CPatcher::InstallDetourPatchWithData("xlive.dll", 51, (DWORD)XNetStartup);
	// Detour XNetCleanup (xlive_52)
	dwDetourPatch[22] = CPatcher::InstallDetourPatchWithData("xlive.dll", 52, (DWORD)XNetCleanup);
	// Detour XNetXnAddrToInAddr (xlive_57)
	dwDetourPatch[23] = CPatcher::InstallDetourPatchWithData("xlive.dll", 57, (DWORD)XNetXnAddrToInAddr);
	// Detour XNetServerToInAddr (xlive_58)
	dwDetourPatch[24] = CPatcher::InstallDetourPatchWithData("xlive.dll", 58, (DWORD)XNetServerToInAddr);
	// Detour XNetUnregisterInAddr (xlive_63)
	dwDetourPatch[25] = CPatcher::InstallDetourPatchWithData("xlive.dll", 63, (DWORD)XNetUnregisterInAddr);
	// Detour XNetGetConnectStatus (xlive_66)
	dwDetourPatch[26] = CPatcher::InstallDetourPatchWithData("xlive.dll", 66, (DWORD)XNetGetConnectStatus);
	// Detour XNetQosListen (xlive_69)
	dwDetourPatch[27] = CPatcher::InstallDetourPatchWithData("xlive.dll", 69, (DWORD)XNetQosListen);
	// Detour XNetQosLookup (xlive_70)
	dwDetourPatch[28] = CPatcher::InstallDetourPatchWithData("xlive.dll", 70, (DWORD)XNetQosLookup);
	// Detour XNetQosRelease (xlive_72)
	dwDetourPatch[29] = CPatcher::InstallDetourPatchWithData("xlive.dll", 72, (DWORD)XNetQosRelease);
	// Detour XNetGetTitleXnAddr (xlive_73)
	dwDetourPatch[30] = CPatcher::InstallDetourPatchWithData("xlive.dll", 73, (DWORD)XNetGetTitleXnAddr);
	// Detour XNetGetEthernetLinkStatus (xlive_75)
	dwDetourPatch[31] = CPatcher::InstallDetourPatchWithData("xlive.dll", 75, (DWORD)XNetGetEthernetLinkStatus);
	// Detour XNetSetSystemLinkPort (xlive_84)
	dwDetourPatch[32] = CPatcher::InstallDetourPatchWithData("xlive.dll", 84, (DWORD)XNetSetSystemLinkPort);
	// Detour XNotifyGetNext (xlive_651)
	dwDetourPatch[33] = CPatcher::InstallDetourPatchWithData("xlive.dll", 651, (DWORD)XNotifyGetNext);
	// Detour XNotifyPositionUI (xlive_652)
	dwDetourPatch[34] = CPatcher::InstallDetourPatchWithData("xlive.dll", 652, (DWORD)XNotifyPositionUI);
	// Detour XGetOverlappedExtendedError (xlive_1082)
	dwDetourPatch[35] = CPatcher::InstallDetourPatchWithData("xlive.dll", 1082, (DWORD)XGetOverlappedExtendedError);
	// Detour XGetOverlappedResult (xlive_1083)
	dwDetourPatch[36] = CPatcher::InstallDetourPatchWithData("xlive.dll", 1083, (DWORD)XGetOverlappedResult);
	// Detour XLiveInitialize (xlive_5000)
	dwDetourPatch[37] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5000, (DWORD)XLiveInitialize);
	// Detour XLiveInput (xlive_5001)
	dwDetourPatch[38] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5001, (DWORD)XLiveInput);
	// Detour XLiveRender (xlive_5002)
	dwDetourPatch[39] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5002, (DWORD)XLiveRender);
	// Detour XLiveUninitialize (xlive_5003)
	dwDetourPatch[40] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5003, (DWORD)XLiveUninitialize);
	// Detour XLiveOnCreateDevice (xlive_5005)
	dwDetourPatch[41] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5005, (DWORD)XLiveOnCreateDevice);
	// Detour XLiveOnResetDevice (xlive_5007)
	dwDetourPatch[42] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5007, (DWORD)XLiveOnResetDevice);
	// Detour XHVCreateEngine (xlive_5008)
	dwDetourPatch[43] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5008, (DWORD)XHVCreateEngine);
	// Detour XLivePBufferAllocate (xlive_5016)
	dwDetourPatch[44] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5016, (DWORD)XLivePBufferAllocate);
	// Detour XLivePBufferFree (xlive_5017)
	dwDetourPatch[45] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5017, (DWORD)XLivePBufferFree);
	// Detour XLivePBufferGetByte (xlive_5018)
	dwDetourPatch[46] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5018, (DWORD)XLivePBufferGetByte);
	// Detour XLivePBufferSetByte (xlive_5019)
	dwDetourPatch[47] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5019, (DWORD)XLivePBufferSetByte);
	// Detour XLivePBufferGetDWORD (xlive_5020)
	dwDetourPatch[48] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5020, (DWORD)XLivePBufferGetDWORD);
	// Detour XLivePBufferSetDWORD (xlive_5021)
	dwDetourPatch[49] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5021, (DWORD)XLivePBufferSetDWORD);
	// Detour XLiveGetUpdateInformation (xlive_5022)
	dwDetourPatch[50] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5022, (DWORD)XLiveGetUpdateInformation);
	// Detour XLiveUpdateSystem (xlive_5024)
	dwDetourPatch[51] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5024, (DWORD)XLiveUpdateSystem);
	// Detour XLivePreTranslateMessage (xlive_5030)
	dwDetourPatch[52] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5030, (DWORD)XLivePreTranslateMessage);
	// Detour XLiveProtectData (xlive_5034)
	dwDetourPatch[53] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5034, (DWORD)XLiveProtectData);
	// Detour XLiveUnprotectData (xlive_5035)
	dwDetourPatch[54] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5035, (DWORD)XLiveUnprotectData);
	// Detour XLiveCreateProtectedDataContext (xlive_5036)
	dwDetourPatch[55] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5036, (DWORD)XLiveCreateProtectedDataContext);
	// Detour XLiveQueryProtectedDataInformation (xlive_5037)
	dwDetourPatch[56] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5037, (DWORD)XLiveQueryProtectedDataInformation);
	// Detour XLiveCloseProtectedDataContext (xlive_5038)
	dwDetourPatch[57] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5038, (DWORD)XLiveCloseProtectedDataContext);
	// Detour XShowPlayerReviewUI (xlive_5214)
	dwDetourPatch[58] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5214, (DWORD)XShowPlayerReviewUI);
	// Detour XShowGuideUI (xlive_5215)
	dwDetourPatch[59] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5215, (DWORD)XShowGuideUI);
	// Detour XCloseHandle (xlive_5251)
	dwDetourPatch[60] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5251, (DWORD)XCloseHandle);
	// Detour XShowGamerCardUI (xlive_5252)
	dwDetourPatch[61] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5252, (DWORD)XShowGamerCardUI);
	// Detour XCancelOverlapped (xlive_5254)
	dwDetourPatch[62] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5254, (DWORD)XCancelOverlapped);
	// Detour XEnumerate (xlive_5256)
	dwDetourPatch[63] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5256, (DWORD)XEnumerate);
	// Detour XShowSigninUI (xlive_5260)
	dwDetourPatch[64] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5260, (DWORD)XShowSigninUI);
	// Detour XUserGetXUID (xlive_5261)
	dwDetourPatch[65] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5261, (DWORD)XUserGetXUID);
	// Detour XUserGetSigninState (xlive_5262)
	dwDetourPatch[66] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5262, (DWORD)XUserGetSigninState);
	// Detour XUserGetName (xlive_5263)
	dwDetourPatch[67] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5263, (DWORD)XUserGetName);
	// Detour XUserAreUsersFriends (xlive_5264)
	dwDetourPatch[68] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5264, (DWORD)XUserAreUsersFriends);
	// Detour XUserCheckPrivilege (xlive_5265)
	dwDetourPatch[69] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5265, (DWORD)XUserCheckPrivilege);
	// Detour XUserGetSigninInfo (xlive_5267)
	dwDetourPatch[70] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5267, (DWORD)XUserGetSigninInfo);
	// Detour XNotifyCreateListener (xlive_5270)
	dwDetourPatch[71] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5270, (DWORD)XNotifyCreateListener);
	// Detour XUserSetProperty (xlive_5276)
	dwDetourPatch[72] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5276, (DWORD)XUserSetProperty);
	// Detour XUserSetContext (xlive_5277)
	dwDetourPatch[73] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5277, (DWORD)XUserSetContext);
	// Detour XUserWriteAchievements (xlive_5278)
	dwDetourPatch[74] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5278, (DWORD)XUserWriteAchievements);
	// Detour XUserCreateAchievementEnumerator (xlive_5280)
	dwDetourPatch[75] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5280, (DWORD)XUserCreateAchievementEnumerator);
	// Detour XUserReadStats (xlive_5281)
	dwDetourPatch[76] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5281, (DWORD)XUserReadStats);
	// Detour XUserCreateStatsEnumeratorByRank (xlive_5284)
	dwDetourPatch[77] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5284, (DWORD)XUserCreateStatsEnumeratorByRank);
	// Detour XUserCreateStatsEnumeratorByXuid (xlive_5286)
	dwDetourPatch[78] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5286, (DWORD)XUserCreateStatsEnumeratorByXuid);
	// Detour XUserSetContextEx (xlive_5292)
	dwDetourPatch[79] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5292, (DWORD)XUserSetContextEx);
	// Detour XLivePBufferGetByteArray (xlive_5294)
	dwDetourPatch[80] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5294, (DWORD)XLivePBufferGetByteArray);
	// Detour XLivePBufferSetByteArray (xlive_5295)
	dwDetourPatch[81] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5295, (DWORD)XLivePBufferSetByteArray);
	// Detour XLiveInitializeEx (xlive_5297)
	dwDetourPatch[82] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5297, (DWORD)XLiveInitializeEx);
	// Detour XSessionCreate (xlive_5300)
	dwDetourPatch[83] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5300, (DWORD)XSessionCreate);
	// Detour XStringVerify (xlive_5303)
	dwDetourPatch[84] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5303, (DWORD)XStringVerify);
	// Detour XStorageUploadFromMemory (xlive_5305)
	dwDetourPatch[85] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5305, (DWORD)XStorageUploadFromMemory);
	// Detour XOnlineStartup (xlive_5310)
	dwDetourPatch[86] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5310, (DWORD)XOnlineStartup);
	// Detour XOnlineCleanup (xlive_5311)
	dwDetourPatch[87] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5311, (DWORD)XOnlineCleanup);
	// Detour XFriendsCreateEnumerator (xlive_5312)
	dwDetourPatch[88] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5312, (DWORD)XFriendsCreateEnumerator);
	// Detour XUserMuteListQuery (xlive_5314)
	dwDetourPatch[89] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5314, (DWORD)XUserMuteListQuery);
	// Detour XInviteGetAcceptedInfo (xlive_5315)
	dwDetourPatch[90] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5315, (DWORD)XInviteGetAcceptedInfo);
	// Detour XInviteSend (xlive_5316)
	dwDetourPatch[91] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5316, (DWORD)XInviteSend);
	// Detour XSessionWriteStats (xlive_5317)
	dwDetourPatch[92] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5317, (DWORD)XSessionWriteStats);
	// Detour XSessionStart (xlive_5318)
	dwDetourPatch[93] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5318, (DWORD)XSessionStart);
	// Detour XSessionSearchEx (xlive_5319)
	dwDetourPatch[94] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5319, (DWORD)XSessionSearchEx);
	// Detour XSessionModify (xlive_5322)
	dwDetourPatch[95] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5322, (DWORD)XSessionModify);
	// Detour XSessionMigrateHost (xlive_5323)
	dwDetourPatch[96] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5323, (DWORD)XSessionMigrateHost);
	// Detour XOnlineGetNatType (xlive_5324)
	dwDetourPatch[97] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5324, (DWORD)XOnlineGetNatType);
	// Detour XSessionLeaveLocal (xlive_5325)
	dwDetourPatch[98] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5325, (DWORD)XSessionLeaveLocal);
	// Detour XSessionJoinRemote (xlive_5326)
	dwDetourPatch[99] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5326, (DWORD)XSessionJoinRemote);
	// Detour XSessionJoinLocal (xlive_5327)
	dwDetourPatch[100] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5327, (DWORD)XSessionJoinLocal);
	// Detour XSessionGetDetails (xlive_5328)
	dwDetourPatch[101] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5328, (DWORD)XSessionGetDetails);
	// Detour XSessionFlushStats (xlive_5329)
	dwDetourPatch[102] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5329, (DWORD)XSessionFlushStats);
	// Detour XSessionDelete (xlive_5330)
	dwDetourPatch[103] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5330, (DWORD)XSessionDelete);
	// Detour XUserReadProfileSettings (xlive_5331)
	dwDetourPatch[104] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5331, (DWORD)XUserReadProfileSettings);
	// Detour XSessionEnd (xlive_5332)
	dwDetourPatch[105] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5332, (DWORD)XSessionEnd);
	// Detour XSessionArbitrationRegister (xlive_5333)
	dwDetourPatch[106] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5333, (DWORD)XSessionArbitrationRegister);
	// Detour XTitleServerCreateEnumerator (xlive_5335)
	dwDetourPatch[107] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5335, (DWORD)XTitleServerCreateEnumerator);
	// Detour XSessionLeaveRemote (xlive_5336)
	dwDetourPatch[108] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5336, (DWORD)XSessionLeaveRemote);
	// Detour XUserWriteProfileSettings (xlive_5337)
	dwDetourPatch[109] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5337, (DWORD)XUserWriteProfileSettings);
	// Detour XStorageBuildServerPath (xlive_5344)
	dwDetourPatch[110] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5344, (DWORD)XStorageBuildServerPath);
	// Detour XStorageDownloadToMemory (xlive_5345)
	dwDetourPatch[111] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5345, (DWORD)XStorageDownloadToMemory);
	// Detour XLiveProtectedVerifyFile (xlive_5349)
	dwDetourPatch[112] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5349, (DWORD)XLiveProtectedVerifyFile);
	// Detour XLiveContentCreateAccessHandle (xlive_5350)
	dwDetourPatch[113] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5350, (DWORD)XLiveContentCreateAccessHandle);
	// Detour XLiveContentUninstall (xlive_5352)
	dwDetourPatch[114] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5352, (DWORD)XLiveContentUninstall);
	// Detour XLiveContentGetPath (xlive_5355)
	dwDetourPatch[115] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5355, (DWORD)XLiveContentGetPath);
	// Detour XLiveContentCreateEnumerator (xlive_5360)
	dwDetourPatch[116] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5360, (DWORD)XLiveContentCreateEnumerator);
	// Detour XLiveContentRetrieveOffersByDate (xlive_5361)
	dwDetourPatch[117] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5361, (DWORD)XLiveContentRetrieveOffersByDate);
	// Detour XShowMarketplaceUI (xlive_5365)
	dwDetourPatch[118] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5365, (DWORD)XShowMarketplaceUI);
	// Detour xlive_5367 (xlive_5367)
	dwDetourPatch[119] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5367, (DWORD)xlive_5367);
	// Detour xlive_5372 (xlive_5372)
	dwDetourPatch[120] = CPatcher::InstallDetourPatchWithData("xlive.dll", 5372, (DWORD)xlive_5372);
}

void CXLiveHook::Uninstall()
{
	// Detour XWSAStartup (xlive_1)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[0], (DWORD)XWSAStartup);
	// Detour XWSACleanup (xlive_2)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[1], (DWORD)XWSACleanup);
	// Detour XSocketCreate (xlive_3)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[2], (DWORD)XSocketCreate);
	// Detour XSocketClose (xlive_4)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[3], (DWORD)XSocketClose);
	// Detour XSocketShutdown (xlive_5)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[4], (DWORD)XSocketShutdown);
	// Detour XSocketIOCTLSocket (xlive_6)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[5], (DWORD)XSocketIOCTLSocket);
	// Detour XSocketSetSockOpt (xlive_7)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[6], (DWORD)XSocketSetSockOpt);
	// Detour XSocketGetSockName (xlive_9)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[7], (DWORD)XSocketGetSockName);
	// Detour XSocketBind (xlive_11)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[8], (DWORD)XSocketBind);
	// Detour XSocketConnect (xlive_12)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[9], (DWORD)XSocketConnect);
	// Detour XSocketListen (xlive_13)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[10], (DWORD)XSocketListen);
	// Detour XSocketAccept (xlive_14)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[11], (DWORD)XSocketAccept);
	// Detour XSocketSelect (xlive_15)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[12], (DWORD)XSocketSelect);
	// Detour XSocketRecv (xlive_18)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[13], (DWORD)XSocketRecv);
	// Detour XSocketRecvFrom (xlive_20)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[14], (DWORD)XSocketRecvFrom);
	// Detour XSocketSend (xlive_22)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[15], (DWORD)XSocketSend);
	// Detour XSocketSendTo (xlive_24)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[16], (DWORD)XSocketSendTo);
	// Detour XSocketInet_Addr (xlive_26)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[17], (DWORD)XSocketInet_Addr);
	// Detour XWSAGetLastError (xlive_27)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[18], (DWORD)XWSAGetLastError);
	// Detour XSocketNTOHS (xlive_38)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[19], (DWORD)XSocketNTOHS);
	// Detour XSocketNTOHL (xlive_39)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[20], (DWORD)XSocketNTOHL);
	// Detour XNetStartup (xlive_51)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[21], (DWORD)XNetStartup);
	// Detour XNetCleanup (xlive_52)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[22], (DWORD)XNetCleanup);
	// Detour XNetXnAddrToInAddr (xlive_57)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[23], (DWORD)XNetXnAddrToInAddr);
	// Detour XNetServerToInAddr (xlive_58)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[24], (DWORD)XNetServerToInAddr);
	// Detour XNetUnregisterInAddr (xlive_63)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[25], (DWORD)XNetUnregisterInAddr);
	// Detour XNetGetConnectStatus (xlive_66)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[26], (DWORD)XNetGetConnectStatus);
	// Detour XNetQosListen (xlive_69)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[27], (DWORD)XNetQosListen);
	// Detour XNetQosLookup (xlive_70)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[28], (DWORD)XNetQosLookup);
	// Detour XNetQosRelease (xlive_72)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[29], (DWORD)XNetQosRelease);
	// Detour XNetGetTitleXnAddr (xlive_73)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[30], (DWORD)XNetGetTitleXnAddr);
	// Detour XNetGetEthernetLinkStatus (xlive_75)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[31], (DWORD)XNetGetEthernetLinkStatus);
	// Detour XNetSetSystemLinkPort (xlive_84)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[32], (DWORD)XNetSetSystemLinkPort);
	// Detour XNotifyGetNext (xlive_651)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[33], (DWORD)XNotifyGetNext);
	// Detour XNotifyPositionUI (xlive_652)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[34], (DWORD)XNotifyPositionUI);
	// Detour XGetOverlappedExtendedError (xlive_1082)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[35], (DWORD)XGetOverlappedExtendedError);
	// Detour XGetOverlappedResult (xlive_1083)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[36], (DWORD)XGetOverlappedResult);
	// Detour XLiveInitialize (xlive_5000)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[37], (DWORD)XLiveInitialize);
	// Detour XLiveInput (xlive_5001)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[38], (DWORD)XLiveInput);
	// Detour XLiveRender (xlive_5002)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[39], (DWORD)XLiveRender);
	// Detour XLiveUninitialize (xlive_5003)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[40], (DWORD)XLiveUninitialize);
	// Detour XLiveOnCreateDevice (xlive_5005)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[41], (DWORD)XLiveOnCreateDevice);
	// Detour XLiveOnResetDevice (xlive_5007)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[42], (DWORD)XLiveOnResetDevice);
	// Detour XHVCreateEngine (xlive_5008)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[43], (DWORD)XHVCreateEngine);
	// Detour XLivePBufferAllocate (xlive_5016)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[44], (DWORD)XLivePBufferAllocate);
	// Detour XLivePBufferFree (xlive_5017)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[45], (DWORD)XLivePBufferFree);
	// Detour XLivePBufferGetByte (xlive_5018)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[46], (DWORD)XLivePBufferGetByte);
	// Detour XLivePBufferSetByte (xlive_5019)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[47], (DWORD)XLivePBufferSetByte);
	// Detour XLivePBufferGetDWORD (xlive_5020)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[48], (DWORD)XLivePBufferGetDWORD);
	// Detour XLivePBufferSetDWORD (xlive_5021)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[49], (DWORD)XLivePBufferSetDWORD);
	// Detour XLiveGetUpdateInformation (xlive_5022)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[50], (DWORD)XLiveGetUpdateInformation);
	// Detour XLiveUpdateSystem (xlive_5024)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[51], (DWORD)XLiveUpdateSystem);
	// Detour XLivePreTranslateMessage (xlive_5030)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[52], (DWORD)XLivePreTranslateMessage);
	// Detour XLiveProtectData (xlive_5034)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[53], (DWORD)XLiveProtectData);
	// Detour XLiveUnprotectData (xlive_5035)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[54], (DWORD)XLiveUnprotectData);
	// Detour XLiveCreateProtectedDataContext (xlive_5036)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[55], (DWORD)XLiveCreateProtectedDataContext);
	// Detour XLiveQueryProtectedDataInformation (xlive_5037)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[56], (DWORD)XLiveQueryProtectedDataInformation);
	// Detour XLiveCloseProtectedDataContext (xlive_5038)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[57], (DWORD)XLiveCloseProtectedDataContext);
	// Detour XShowPlayerReviewUI (xlive_5214)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[58], (DWORD)XShowPlayerReviewUI);
	// Detour XShowGuideUI (xlive_5215)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[59], (DWORD)XShowGuideUI);
	// Detour XCloseHandle (xlive_5251)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[60], (DWORD)XCloseHandle);
	// Detour XShowGamerCardUI (xlive_5252)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[61], (DWORD)XShowGamerCardUI);
	// Detour XCancelOverlapped (xlive_5254)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[62], (DWORD)XCancelOverlapped);
	// Detour XEnumerate (xlive_5256)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[63], (DWORD)XEnumerate);
	// Detour XShowSigninUI (xlive_5260)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[64], (DWORD)XShowSigninUI);
	// Detour XUserGetXUID (xlive_5261)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[65], (DWORD)XUserGetXUID);
	// Detour XUserGetSigninState (xlive_5262)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[66], (DWORD)XUserGetSigninState);
	// Detour XUserGetName (xlive_5263)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[67], (DWORD)XUserGetName);
	// Detour XUserAreUsersFriends (xlive_5264)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[68], (DWORD)XUserAreUsersFriends);
	// Detour XUserCheckPrivilege (xlive_5265)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[69], (DWORD)XUserCheckPrivilege);
	// Detour XUserGetSigninInfo (xlive_5267)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[70], (DWORD)XUserGetSigninInfo);
	// Detour XNotifyCreateListener (xlive_5270)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[71], (DWORD)XNotifyCreateListener);
	// Detour XUserSetProperty (xlive_5276)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[72], (DWORD)XUserSetProperty);
	// Detour XUserSetContext (xlive_5277)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[73], (DWORD)XUserSetContext);
	// Detour XUserWriteAchievements (xlive_5278)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[74], (DWORD)XUserWriteAchievements);
	// Detour XUserCreateAchievementEnumerator (xlive_5280)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[75], (DWORD)XUserCreateAchievementEnumerator);
	// Detour XUserReadStats (xlive_5281)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[76], (DWORD)XUserReadStats);
	// Detour XUserCreateStatsEnumeratorByRank (xlive_5284)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[77], (DWORD)XUserCreateStatsEnumeratorByRank);
	// Detour XUserCreateStatsEnumeratorByXuid (xlive_5286)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[78], (DWORD)XUserCreateStatsEnumeratorByXuid);
	// Detour XUserSetContextEx (xlive_5292)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[79], (DWORD)XUserSetContextEx);
	// Detour XLivePBufferGetByteArray (xlive_5294)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[80], (DWORD)XLivePBufferGetByteArray);
	// Detour XLivePBufferSetByteArray (xlive_5295)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[81], (DWORD)XLivePBufferSetByteArray);
	// Detour XLiveInitializeEx (xlive_5297)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[82], (DWORD)XLiveInitializeEx);
	// Detour XSessionCreate (xlive_5300)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[83], (DWORD)XSessionCreate);
	// Detour XStringVerify (xlive_5303)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[84], (DWORD)XStringVerify);
	// Detour XStorageUploadFromMemory (xlive_5305)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[85], (DWORD)XStorageUploadFromMemory);
	// Detour XOnlineStartup (xlive_5310)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[86], (DWORD)XOnlineStartup);
	// Detour XOnlineCleanup (xlive_5311)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[87], (DWORD)XOnlineCleanup);
	// Detour XFriendsCreateEnumerator (xlive_5312)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[88], (DWORD)XFriendsCreateEnumerator);
	// Detour XUserMuteListQuery (xlive_5314)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[89], (DWORD)XUserMuteListQuery);
	// Detour XInviteGetAcceptedInfo (xlive_5315)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[90], (DWORD)XInviteGetAcceptedInfo);
	// Detour XInviteSend (xlive_5316)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[91], (DWORD)XInviteSend);
	// Detour XSessionWriteStats (xlive_5317)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[92], (DWORD)XSessionWriteStats);
	// Detour XSessionStart (xlive_5318)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[93], (DWORD)XSessionStart);
	// Detour XSessionSearchEx (xlive_5319)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[94], (DWORD)XSessionSearchEx);
	// Detour XSessionModify (xlive_5322)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[95], (DWORD)XSessionModify);
	// Detour XSessionMigrateHost (xlive_5323)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[96], (DWORD)XSessionMigrateHost);
	// Detour XOnlineGetNatType (xlive_5324)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[97], (DWORD)XOnlineGetNatType);
	// Detour XSessionLeaveLocal (xlive_5325)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[98], (DWORD)XSessionLeaveLocal);
	// Detour XSessionJoinRemote (xlive_5326)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[99], (DWORD)XSessionJoinRemote);
	// Detour XSessionJoinLocal (xlive_5327)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[100], (DWORD)XSessionJoinLocal);
	// Detour XSessionGetDetails (xlive_5328)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[101], (DWORD)XSessionGetDetails);
	// Detour XSessionFlushStats (xlive_5329)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[102], (DWORD)XSessionFlushStats);
	// Detour XSessionDelete (xlive_5330)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[103], (DWORD)XSessionDelete);
	// Detour XUserReadProfileSettings (xlive_5331)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[104], (DWORD)XUserReadProfileSettings);
	// Detour XSessionEnd (xlive_5332)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[105], (DWORD)XSessionEnd);
	// Detour XSessionArbitrationRegister (xlive_5333)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[106], (DWORD)XSessionArbitrationRegister);
	// Detour XTitleServerCreateEnumerator (xlive_5335)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[107], (DWORD)XTitleServerCreateEnumerator);
	// Detour XSessionLeaveRemote (xlive_5336)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[108], (DWORD)XSessionLeaveRemote);
	// Detour XUserWriteProfileSettings (xlive_5337)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[109], (DWORD)XUserWriteProfileSettings);
	// Detour XStorageBuildServerPath (xlive_5344)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[110], (DWORD)XStorageBuildServerPath);
	// Detour XStorageDownloadToMemory (xlive_5345)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[111], (DWORD)XStorageDownloadToMemory);
	// Detour XLiveProtectedVerifyFile (xlive_5349)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[112], (DWORD)XLiveProtectedVerifyFile);
	// Detour XLiveContentCreateAccessHandle (xlive_5350)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[113], (DWORD)XLiveContentCreateAccessHandle);
	// Detour XLiveContentUninstall (xlive_5352)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[114], (DWORD)XLiveContentUninstall);
	// Detour XLiveContentGetPath (xlive_5355)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[115], (DWORD)XLiveContentGetPath);
	// Detour XLiveContentCreateEnumerator (xlive_5360)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[116], (DWORD)XLiveContentCreateEnumerator);
	// Detour XLiveContentRetrieveOffersByDate (xlive_5361)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[117], (DWORD)XLiveContentRetrieveOffersByDate);
	// Detour XShowMarketplaceUI (xlive_5365)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[118], (DWORD)XShowMarketplaceUI);
	// Detour xlive_5367 (xlive_5367)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[119], (DWORD)xlive_5367);
	// Detour xlive_5372 (xlive_5372)
	CPatcher::UninstallDetourPatch(&dwDetourPatch[120], (DWORD)xlive_5372);
}
