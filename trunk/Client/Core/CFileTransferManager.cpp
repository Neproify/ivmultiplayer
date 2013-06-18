//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransferManager.cpp
// Project: Client.Core
// Author(s):   jenksta
// License: See LICENSE in root directory
//
//==============================================================================
  
#include "CFileTransferManager.h"
#include "CClient.h"
#include <CLogFile.h>
  
#define TRANSFERBOX_WIDTH  350
#define TRANSFERBOX_HEIGHT 58
  
extern CClient * g_pClient;
  
CFileTransferManager::CFileTransferManager()
{
    // Set our thread user data
    m_transferThread.SetUserData<CFileTransferManager *>(this);
}
  
CFileTransferManager::~CFileTransferManager()
{
    // If our transfer thread is running stop it
    if(m_transferThread.IsRunning())
        m_transferThread.Stop();
}
  
void CFileTransferManager::Add(bool bIsResource, String strName, CFileChecksum checksum)
{
    // Get our GUI instance
    CGUI * pGUI = g_pClient->GetGUI();
  
    // Ensure we have our transfer info gui stuff created
    float fWidth = (float)pGUI->GetDisplayWidth();
    float fHeight = (float)pGUI->GetDisplayHeight();
  
    // Check if we have created image and text stuff for download, if not create it
    if(!m_pFileImage)
    {
        // Try to load the image download.png - NOW!
        try
        {
            CEGUI::ImagesetManager::getSingleton().createFromImageFile("Download", "Download.png");
        }
        catch(CEGUI::InvalidRequestException e)
        {
            String strFile = e.getMessage().c_str();
            strFile = strFile.SubStr(strFile.Find(" - ")+3, (unsigned int)-1);
            String str("IV:MP failed to load. (%s)", strFile.Get());
            MessageBox(NULL, str.C_String(), "IV:MP Error", MB_OK | MB_ICONERROR);
            ExitProcess(0);
        }
        catch(CEGUI::Exception e)
        {
            MessageBox(NULL, "IV:MP failed to load. Check CEGUI.log for details.", "IV:MP Error", MB_OK | MB_ICONERROR);
            ExitProcess(0);
        }
  
        m_pFileImage = pGUI->CreateGUIStaticImage(pGUI->GetDefaultWindow());
        m_pFileImage->setProperty("FrameEnabled", "false");
        m_pFileImage->setProperty("BackgroundEnabled", "false");
        m_pFileImage->setSize(CEGUI::UVector2(CEGUI::UDim(0, (386.0f*1.5f)), CEGUI::UDim(0, (114.0f*1.5f))));
        m_pFileImage->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/3),  CEGUI::UDim(0, fHeight/2+(fHeight/(float)6.5))));
        m_pFileImage->setProperty("Image", "set:Download image:full_image");
        m_pFileImage->setProperty("InheritsAlpha","false");
        m_pFileImage->setAlpha(0.90f);
        m_pFileImage->setVisible(false);
    }
    if(!m_pFileText)
    {
        m_pFileText = pGUI->CreateGUIStaticText(pGUI->GetDefaultWindow());
        m_pFileText->setText("Waiting for resources download");
        m_pFileText->setSize(CEGUI::UVector2(CEGUI::UDim(TRANSFERBOX_WIDTH, 0), CEGUI::UDim(TRANSFERBOX_HEIGHT, 0)));
        m_pFileText->setPosition(CEGUI::UVector2(CEGUI::UDim(0, fWidth/(float)2.75),  CEGUI::UDim(0, fHeight/2-(fHeight/4))));
        m_pFileText->setProperty("FrameEnabled", "false");
        m_pFileText->setProperty("BackgroundEnabled", "false");
        m_pFileText->setFont(pGUI->GetFont("tahoma",28U));
        m_pFileText->setVisible(false);
    }
  
    // Create our file transfer
    CFileTransfer * pFileTransfer = new CFileTransfer(bIsResource, m_strHost, m_usPort, strName, checksum);
  
    if(!pFileTransfer)
        return;
  
    // Lock our transfer list mutex
    m_transferListMutex.Lock();
  
    // Insert our new file transfer
    m_transferList.push_back(pFileTransfer);
  
    // Unlock our transfer list mutex
    m_transferListMutex.Unlock();
  
    CLogFile::Printf("Added file %s to transfer list", strName.Get());
  
    // Is our transfer thread not running?
    if(!m_transferThread.IsRunning())
    {
        // Start our transfer thread
        m_transferThread.Start(TransferThread);
    }
}
  
bool CFileTransferManager::Remove(String strName)
{
    // We can't remove any items from the list if the thread is running in case they're being used
    if(m_transferThread.IsRunning())
        return false;
  
    // Loop through our transfer list
    for(std::list<CFileTransfer *>::iterator i = m_transferList.begin(); i != m_transferList.end(); i++)
    {
        // Is this our file transfer?
        if((*i)->GetName() == strName)
        {
            // Delete this transfer
            SAFE_DELETE(*i);
  
            // Remove it from the transfer list
            m_transferList.remove(*i);
            return true;
        }
    }
  
    // File transfer does not exist
    return false;
}
  
bool CFileTransferManager::Clear(bool bKillThread)
{
    // We can't remove any items from the list if the thread is running in case they're being used
    if(m_transferThread.IsRunning())
    {
        if(bKillThread)
            m_transferThread.Stop(false, true);
        else
            return false;
    }
  
    // Loop through our transfer list
    for(std::list<CFileTransfer *>::iterator i = m_transferList.begin(); i != m_transferList.end(); i++)
    {
        // Delete this transfer
        SAFE_DELETE(*i);
    }
  
    // Clear our transfer list
    m_transferList.clear();
    return true;
}
  
bool CFileTransferManager::IsComplete()
{
    // Is our transfer thread running?
    if(m_transferThread.IsRunning())
        return false;
  
    // Loop through our transfer list
    for(std::list<CFileTransfer *>::iterator i = m_transferList.begin(); i != m_transferList.end(); i++)
    {
        // Is this transfer not complete?
        if(!(*i)->IsComplete())
            return false;
    }
  
    // All transfer complete
    return true;
}
  
void CFileTransferManager::Process()
{
    // Is our transfer thread running?
    if(m_transferThread.IsRunning())
        return;
  
    // Loop through our transfer list
    for(std::list<CFileTransfer *>::iterator i = m_transferList.begin(); i != m_transferList.end(); i++)
    {
        // Get our file transfer pointer
        CFileTransfer * pFileTransfer = *i;
  
        // jenksta: TODO: Add two events for this and add a handler to CClient
        // TODO: Handler should have true/false return value to remove transfer from list or not?
        // Did our file transfer successfully?
        if(pFileTransfer->IsComplete())
        {
            if(pFileTransfer->HasSucceeded())
            {
                if(!pFileTransfer->IsResource())
                {
                    CLogFile::PrintDebugf("Adding client script: %s.", pFileTransfer->GetName().Get());
                    CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
                    pClientScriptManager->AddScript(pFileTransfer->GetName(), pFileTransfer->GetPath());
  
                    if(g_pClient->GetLocalPlayer()->GetFirstSpawn())
                        pClientScriptManager->Load(pFileTransfer->GetName());
                }
            }
            else
            {
                String strErrorMessage("Failed to download file %s (%s)", pFileTransfer->GetName().Get(), pFileTransfer->GetError().Get());
                CLogFile::Print(strErrorMessage);
                g_pClient->GetNetworkManager()->Disconnect();
 
                // Get our main menu
                CMainMenu * pMainMenu = g_pClient->GetMainMenu();
  
                pMainMenu->ResetNetworkStats();
                pMainMenu->SetDisconnectButtonVisible(false);
                pMainMenu->SetVisible(true);
  
                g_pClient->GetGUI()->ShowMessageBox(strErrorMessage.Get(), "File transfer failed");
            }
  
            m_transferList.remove(pFileTransfer);
            i = m_transferList.begin();
            // MRK : make sure to check for end of the list
            if (i == m_transferList.end())
                break;
        }
    }
}
  
void CFileTransferManager::TransferThread(CThread * pCreator)
{
    // Get our instance
    CFileTransferManager * pThis = pCreator->GetUserData<CFileTransferManager *>();
  
    // Lock our transfer list mutex
    pThis->m_transferListMutex.Lock();
  
    // Ensure clientfiles directories exist:    
    CLogFile::Print("TransferThread: Creating directories to download files to..");
    String strFolderName = SharedUtility::GetAbsolutePath("clientfiles");
    if(!SharedUtility::Exists(strFolderName))
        SharedUtility::CreateDirectoryA(strFolderName);
    if(!SharedUtility::Exists(strFolderName + "\\clientscripts"))
        SharedUtility::CreateDirectoryA(strFolderName + "\\clientscripts");
    if(!SharedUtility::Exists(strFolderName + "\\resources"))
        SharedUtility::CreateDirectoryA(strFolderName + "\\resources"); 
  
    // Loop through our transfer list
    CLogFile::Print("TransferThread: Downloading client files...");
    for(std::list<CFileTransfer *>::iterator i = pThis->m_transferList.begin(); i != pThis->m_transferList.end(); i++)
    {
        // Get our file transfer pointer
        CFileTransfer * pFileTransfer = *i;
  
        // Unlock our transfer list mutex
        pThis->m_transferListMutex.Unlock();
  
        // Download our file
        CLogFile::Printf("TransferThread: pFileTransfer(%s) Downloading...", pFileTransfer->GetName().Get());
        if(pFileTransfer->Download())
            CLogFile::Printf("TransferThread: pFileTransfer(%s) Done", pFileTransfer->GetName().Get());
        else
            CLogFile::Printf("TransferThread: pFileTransfer(%s) Failed: %s", pFileTransfer->GetName().Get(), pFileTransfer->GetError().Get());
  
        // Lock our transfer list mutex
        pThis->m_transferListMutex.Lock();
    }
    CLogFile::Print("TransferThread: Finished client files download");
  
    // Unlock our transfer list mutex
    pThis->m_transferListMutex.Unlock();
}