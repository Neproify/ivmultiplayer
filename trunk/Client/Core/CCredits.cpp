//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CCredits.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CCredits.h"
#include <SharedUtility.h>

#define BACKGROUND_COLOR D3DCOLOR_RGBA(0, 0, 0, 0) // Black

// TODO: Effects enum with basic effects
// Fade in, slide in (left, right, up, down), Fade out, boom in, boom out

CreditText creditsItems[] = 
{
	{6000, "tahoma-bold", 40, "The IV: Multiplayer Team Presents"},
	{5000, "tahoma-bold", 40, "IV: Multiplayer"},
	{4000, "tahoma-bold", 40, "Programming By"},
	{4000, "tahoma-bold", 40, "Sam \"jenksta\" Jenkins"},
	{4000, "tahoma-bold", 40, "Sebastian \"Sebihunter\" Mühlbauer"},
	{4000, "tahoma-bold", 40, "Martin \"@FRi\" Konrad"},
	{4000, "tahoma-bold", 40, "MaVe"},
	{4000, "tahoma-bold", 40, "Marcus \"mabako\" Bauer"},
	{4000, "tahoma-bold", 40, "Vladislav \"adamix\" Grishin"},
	{4000, "tahoma-bold", 40, "Einstein"},
	{4000, "tahoma-bold", 40, "Christopher \"Christopher\" Martino"},
	{4000, "tahoma-bold", 40, "Sebas \"x86\" Lamers"},
	{4000, "tahoma-bold", 40, "Testing By"},
	{4000, "tahoma-bold", 40, "Boylett"},
	{4000, "tahoma-bold", 40, "Cameron \"Cameron\" Banfield"},
	{4000, "tahoma-bold", 40, "TrojaA"},
	{4000, "tahoma-bold", 40, "Pyrokid"},
	{4000, "tahoma-bold", 40, "Special Thanks To"},
	{4000, "tahoma-bold", 40, "Multi Theft Auto Team"},
	{4000, "tahoma-bold", 40, "aru"},
	{4000, "tahoma-bold", 40, "listener"},
	{4000, "tahoma-bold", 40, "s0beit"},
	{4000, "tahoma-bold", 40, "Cespur"},
	{4000, "tahoma-bold", 40, "Leopard"},
	{4000, "tahoma-bold", 40, "IV:MP Uses The Following Libraries"},
	{4000, "tahoma-bold", 40, "RakNet (http://www.jenkinssoftware.com)"},
	{4000, "tahoma-bold", 40, "CEGUI (http://www.cegui.org.uk)"},
	{4000, "tahoma-bold", 40, "Squirrel (http://www.squirrel-lang.org)"},
	{4000, "tahoma-bold", 40, "Mongoose (http://code.google.com/p/mongoose/)"},
	{4000, "tahoma-bold", 40, "TinyXML (http://sourceforge.net/projects/tinyxml/)"}
};

CCredits::CCredits(CGUI * pGUI)
{
	// Set the GUI pointer
	m_pGUI = pGUI;

	// Reset the start time
	m_ulStartTime = 0;

	// Reset the current font
	m_pCurrentFont = NULL;

	// Reset the current credit text
	m_pCurrentCreditText = NULL;

	// Pre-load all needed fonts
	// jenksta: don't think this is really needed
	//for(size_t i = 0; i < (sizeof(creditsItems) / sizeof(CreditText)); i++)
	//	pGUI->GetFont(creditsItems[i].strFont, creditsItems[i].uiFontSize);
}

CCredits::~CCredits()
{

}

String CCredits::GetCreditsString()
{
	String strCredits;

	// Loop through all credit items
	for(size_t i = 0; i < (sizeof(creditsItems) / sizeof(CreditText)); i++)
	{
		// Loop through all characters
		String strText = creditsItems[i].strText;
		unsigned int uiTextLength = strText.GetLength();

		for(size_t c = 0; c < uiTextLength; c++)
		{
			// Remove any color formatting
			if(uiTextLength >= 10 && (strText[c] == '[' && strText[c + 9] == ']'))
			{
				bool bValid = true;

				// Loop through all color chars
				for(size_t i = 0; i < 8; i++)
				{
					unsigned char cChar = strText[(c + i) + 1];

					// Make sure its 0-99, A-F or a-f
					if((cChar < '0' || cChar > '9') && (cChar < 'A' || cChar > 'F') && 
						(cChar < 'a' || cChar > 'f'))
					{
						// char is invalid
						bValid = false;
						break;
					}
				}

				// Was the color formatting valid?
				if(bValid)
				{
					// Increment the current char by 9
					c += 9;
					continue;
				}
			}

			// Add the current char to the credits string
			strCredits += strText[c];
		}

		// Add spaces to the credits string
		strCredits += "   ";
	}

	return strCredits;
}

void CCredits::Start()
{
	// Set the start time
	m_ulStartTime = SharedUtility::GetTime();
}

void CCredits::End()
{
	// Reset the start time
	m_ulStartTime = 0;

	// Reset the current credit text
	m_pCurrentCreditText = NULL;
}

void CCredits::Process()
{
	// Have we started the credits sequence?
	if(m_ulStartTime != 0)
	{
		// Do we want to exit the credits sequence?
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			// End the credits sequence
			End();
			return;
		}

		// Get the current time
		unsigned long ulCurrentTime = SharedUtility::GetTime();

		// Find the current credit item
		CreditText * pCreditText = NULL;
		unsigned long ulTotalFrameTime = 0;
		unsigned long ulStartTime = 0;
		unsigned long ulFinishTime = 0;

		for(size_t i = 0; i < (sizeof(creditsItems) / sizeof(CreditText)); i++)
		{
			// Set the start time
			ulStartTime = (m_ulStartTime + ulTotalFrameTime);

			// Increment the total frame time
			ulTotalFrameTime += creditsItems[i].ulTime;

			// Set the finish time
			ulFinishTime = (ulStartTime + ulTotalFrameTime);

			// Has this frame finished?
			if((ulCurrentTime - m_ulStartTime) > ulTotalFrameTime)
				continue;

			// Set the current frame
			pCreditText = &creditsItems[i];
			break;
		}

		// Do we not have a valid credit item?
		if(!pCreditText)
		{
			// Credits sequence has finished
			End();
			return;
		}

		// Has the credit text changed?
		if(m_pCurrentCreditText != pCreditText)
		{
			// Set the current font
			m_pCurrentFont = m_pGUI->GetFont(pCreditText->strFont, pCreditText->uiFontSize);

			// Set the current credit text
			m_pCurrentCreditText = pCreditText;
		}

		// Lets get a blank canvas...
		m_pGUI->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, BACKGROUND_COLOR, 1.0, 0);

		// Get the factor of time taken from the start of the credit item to the current time
		float fAlpha = Math::Unlerp(m_ulStartTime, ulCurrentTime, ulFinishTime);

		// Clamp the factor of time
		fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);

		// Get the scale value
		float fScale = (-(fAlpha - 1.0f) / 0.5f);

		// Get the font width and height
		float fFontWidth = (m_pCurrentFont->getTextExtent(pCreditText->strText.Get()) * fScale);
		float fFontHeight = (pCreditText->uiFontSize * fScale);

		// Get the text position
		float fX = (float)((m_pGUI->GetDisplayWidth() / 2) - (fFontWidth / 2));
		float fY = (float)((m_pGUI->GetDisplayHeight() / 2) - (fFontHeight / 2));

		// Draw the text
		m_pGUI->DrawText(pCreditText->strText, CEGUI::Vector2(fX, fY), (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255), m_pCurrentFont, true, true, NULL, 0.0, fScale, fScale);
	}
}
