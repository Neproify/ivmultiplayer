//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScreenShot.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================
// Parts taken from code found on the Irrlicht forums

#include "CScreenShot.h"
#include "../../Vendor/lpng142/png.h"
#include "DXSDK/Include/d3d9.h"
#include "SharedUtility.h"
#include "CGame.h"
#include "CGUI.h"

#define SCREEN_SHOT_FORMAT D3DFMT_A8R8G8B8
#define SCREEN_SHOT_FORMAT_BYTES_PER_PIXEL (32 / 8)

unsigned long           CScreenShot::m_ulLastScreenShotTime = 0;
CThread                 CScreenShot::m_writeThread;
unsigned char         * CScreenShot::m_ucData = NULL;
unsigned int            CScreenShot::m_uiScreenWidth = 0;
unsigned int            CScreenShot::m_uiScreenHeight = 0;
CMutex                  CScreenShot::m_threadDataMutex;
CScreenShot::ThreadData CScreenShot::m_threadData;

extern IDirect3DDevice9 * g_pDevice;
extern CGUI             * g_pGUI;

CScreenShot::CScreenShot()
{
	Reset();
}

String CScreenShot::GetScreenShotPath()
{
	// Get the screen shot directory
	String strPath(SharedUtility::GetAbsolutePath("screenshots"));

	// Ensure the screen shot directory exists
	if(!SharedUtility::Exists(strPath))
		SharedUtility::CreateDirectory(strPath);

	// Append the screen shot name to the path
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	strPath.AppendF("/ivmp-%04d.%02d.%02d-%02d.%02d.%02d.png", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	return strPath;
}

void CScreenShot::WriteImageToFile(CThread * pThread)
{
	// Lock the thread data mutex now
	m_threadDataMutex.Lock();
	m_threadData.bWriting = true;

	// Get the screen shot path
	String strPath = GetScreenShotPath();

	// Open the screen shot file
	FILE * fScreenshot = fopen(strPath, "wb");

	// Ensure the screen shot file is open
	if(!fScreenshot)
	{
		// Delete the image data
		delete [] m_ucData;

		// Flag the write as not succeeded
		m_threadData.bSucceeded = false;
		m_threadData.strError = "Failed to open screen shot file";

		// Unlock the thread data mutex
		m_threadDataMutex.Unlock();
		return;
	}

	// Allocate the png write struct
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, /*(png_error_ptr)png_cpexcept_error*/NULL, NULL);

	// Ensure the png write struct was allocated
	if(!png_ptr)
	{
		// Delete the image data
		delete [] m_ucData;

		// Close the screen shot file
		fclose(fScreenshot);

		// Flag the write as not succeeded
		m_threadData.bSucceeded = false;
		m_threadData.strError = "Failed to allocate memory";

		// Unlock the thread data mutex
		m_threadDataMutex.Unlock();
		return;
	}

	// Allocate the png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);

	// Ensure the png info struct was allocated
	if(!info_ptr)
	{
		// Destroy the png write struct
		png_destroy_write_struct(&png_ptr, NULL);

		// Delete the image data
		delete [] m_ucData;

		// Close the screen shot file
		fclose(fScreenshot);

		// Flag the write as not succeeded
		m_threadData.bSucceeded = false;
		m_threadData.strError = "Failed to allocate memory";

		// Unlock the thread data mutex
		m_threadDataMutex.Unlock();
		return;
	}

	// Set the png file pointer
	png_init_io(png_ptr, fScreenshot);

	// Set the png write struct info
	png_set_IHDR(png_ptr, info_ptr, m_uiScreenWidth, m_uiScreenHeight, 8, 
		PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, 
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	// Get the line width
	unsigned int uiLineWidth = (m_uiScreenWidth * SCREEN_SHOT_FORMAT_BYTES_PER_PIXEL);

	// Allocate the temporary image data
	unsigned char * tmpImage = new unsigned char[m_uiScreenHeight * uiLineWidth];

	// Ensure the temporary image data was allocated
	if(!tmpImage)
	{
		// Destroy the png write and info struct
		png_destroy_write_struct(&png_ptr, &info_ptr);

		// Delete the image data
		delete [] m_ucData;

		// Close the screen shot file
		fclose(fScreenshot);

		// Flag the write as not succeeded
		m_threadData.bSucceeded = false;
		m_threadData.strError = "Failed to allocate memory";

		// Unlock the thread data mutex
		m_threadDataMutex.Unlock();
		return;
	}

	// Copy the image data into the temporary image data
	memcpy(tmpImage, m_ucData, (m_uiScreenHeight * uiLineWidth));

	// Allocate the row pointers array
	unsigned char ** ucRowPointers = new png_bytep[m_uiScreenHeight];

	// Ensure the row pointers array was allocated
	if(!ucRowPointers)
	{
		// Delete the temporary image data
		delete [] tmpImage;

		// Destroy the png write and info struct
		png_destroy_write_struct(&png_ptr, &info_ptr);

		// Delete the image data
		delete [] m_ucData;

		// Close the screen shot file
		fclose(fScreenshot);

		// Flag the write as not succeeded
		m_threadData.bSucceeded = false;
		m_threadData.strError = "Failed to allocate memory";

		// Unlock the thread data mutex
		m_threadDataMutex.Unlock();
		return;
	}

	// Fill row pointers array
	for(unsigned int i = 0; i < m_uiScreenHeight; ++i)
		ucRowPointers[i] = (tmpImage + (uiLineWidth * i));

	// Set the png rows
	png_set_rows(png_ptr, info_ptr, ucRowPointers);

	// Write the png
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);

	// Delete the row pointers array
	delete [] ucRowPointers;

	// Delete the temporary image data
	delete [] tmpImage;

	// Destroy the png write and info struct
	png_destroy_write_struct(&png_ptr, &info_ptr);

	// Delete the image data
	delete [] m_ucData;

	// Close the screen shot file
	fclose(fScreenshot);

	// Flag the write as succeeded
	m_threadData.bSucceeded = true;
	m_threadData.strWriteName = strPath;

	// Unlock the thread data mutex
	m_threadDataMutex.Unlock();
	/*return true;*/
}

bool CScreenShot::Take()
{
	// Ensure we are not already taking a screen shot
	if(m_writeThread.IsRunning())
	{
		// Flag the write as not succeeded
		m_threadData.strError = "Screen shot already being taken";
		return false;
	}

	// Only allow one screen shot per second to avoid abuse
	if((SharedUtility::GetTime() - m_ulLastScreenShotTime) < 1000)
	{
		// Flag the write as not succeeded
		m_threadData.strError = "You must wait 1 second between screen shots";
		return false;
	}

	// Get the screen display mode
	D3DDISPLAYMODE displayMode;
	g_pDevice->GetDisplayMode(0, &displayMode);

	// Create the image surface to store the front buffer image
	IDirect3DSurface9 * pSurface;

	if(FAILED(g_pDevice->CreateOffscreenPlainSurface(displayMode.Width, displayMode.Height, SCREEN_SHOT_FORMAT, D3DPOOL_SCRATCH, &pSurface, NULL)))
	{
		// Flag the write as not succeeded
		m_threadData.strError = "Failed to create surface";
		return false;
	}

	// Read the front buffer into the image surface
	if(FAILED(g_pDevice->GetFrontBufferData(0, pSurface)))
	{
		// Release the surface
		pSurface->Release();

		// Flag the write as not succeeded
		m_threadData.strError = "Failed to get front buffer";
		return false;
	}

	// Get the screen width and height
	D3DVIEWPORT9 viewport;
	g_pDevice->GetViewport(&viewport);
	unsigned int uiScreenWidth = viewport.Width;
	unsigned int uiScreenHeight = viewport.Height;

	// Create the client rect
	RECT clientRect;
	{
		POINT clientPoint;
		clientPoint.x = 0;
		clientPoint.y = 0;
		ClientToScreen(CGame::GetHWnd(), &clientPoint);
		clientRect.left   = clientPoint.x;
		clientRect.top	  = clientPoint.y;
		clientRect.right  = (clientRect.left + uiScreenWidth);
		clientRect.bottom = (clientRect.top  + uiScreenHeight);
	}

	// Lock the surface
	D3DLOCKED_RECT lockedRect;

	if(FAILED(pSurface->LockRect(&lockedRect, &clientRect, D3DLOCK_READONLY)))
	{
		// Release the surface
		pSurface->Release();

		// Flag the write as not succeeded
		m_threadData.strError = "Failed to lock surface";
		return false;
	}

	// Get the line width
	unsigned int uiLineWidth = (uiScreenWidth * SCREEN_SHOT_FORMAT_BYTES_PER_PIXEL);

	// Allocate the image data
	unsigned char * ucData =  new unsigned char[uiScreenHeight * uiLineWidth];

	// Ensure the image data was allocated
	if(!ucData)
	{
		// Unlock the surface
		pSurface->UnlockRect();

		// Release the surface
		pSurface->Release();

		// Flag the write as not succeeded
		m_threadData.strError = "Failed to allocate memory";
		return false;
	}

	// Sort the image data (d3d pads the image, so we need to copy the correct number of bytes)
	unsigned int * dP = (unsigned int *)ucData;
	unsigned char * sP = (unsigned char *)lockedRect.pBits;

	// If the display mode format doesn't promise anything about the Alpha value
	// and it appears that it's not presenting 255, then we should manually
	// set each pixel alpha value to 255
	if((displayMode.Format == D3DFMT_X8R8G8B8) && (0xFF000000 != (*dP & 0xFF000000)))
	{
		for(unsigned int y = 0; y < uiScreenHeight; y++)
		{
			for(unsigned int x = 0; x < uiScreenWidth; x++)
			{
				*dP = *((unsigned int *)sP) | 0xFF000000;
				dP++;
				sP += 4;
			}

			sP += lockedRect.Pitch - (4 * uiScreenWidth);
		}
	}
	else
	{
		for(unsigned int y = 0; y < uiScreenHeight; y++)
		{
			memcpy(dP, sP, uiScreenWidth * 4);

			sP += lockedRect.Pitch;
			dP += uiScreenWidth;
		}
	}

	// Unlock the surface
	pSurface->UnlockRect();

	// Release the surface
	pSurface->Release();

	// Write the image to a file
	m_ucData = ucData;
	m_uiScreenWidth = uiScreenWidth;
	m_uiScreenHeight = uiScreenHeight;
	m_writeThread.Start(WriteImageToFile);

	// Set the last screen shot time
	m_ulLastScreenShotTime = SharedUtility::GetTime();
	return true;
}

bool CScreenShot::IsDone()
{
	// Lock the thread data mutex
	m_threadDataMutex.Lock();
	bool bWriting = m_threadData.bWriting;

	// Unlock the thread data mutex
	m_threadDataMutex.Unlock();

	return (bWriting && !m_writeThread.IsRunning());
}

bool CScreenShot::HasSucceeded()
{
	// Lock the thread data mutex
	m_threadDataMutex.Lock();
	bool bSucceeded = m_threadData.bSucceeded;

	// Unlock the thread data mutex
	m_threadDataMutex.Unlock();
	return bSucceeded;
}

String CScreenShot::GetWriteName()
{
	// Lock the thread data mutex
	m_threadDataMutex.Lock();
	String strWriteName = m_threadData.strWriteName;

	// Unlock the thread data mutex
	m_threadDataMutex.Unlock();
	return strWriteName;
}

String CScreenShot::GetError()
{
	// Lock the thread data mutex
	m_threadDataMutex.Lock();
	String strError = m_threadData.strError;

	// Unlock the thread data mutex
	m_threadDataMutex.Unlock();
	return strError;
}

void CScreenShot::Reset()
{
	// Lock the thread data mutex
	m_threadDataMutex.Lock();

	// Reset the thread data
	m_threadData.bWriting = false;
	m_threadData.bSucceeded = false;
	m_threadData.strError = "";

	// Unlock the thread data mutex
	m_threadDataMutex.Unlock();
}
