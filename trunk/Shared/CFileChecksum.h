//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileChecksum.h
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================
// Parts taken from http://www.flounder.com/checksum.htm

#pragma once

#include <CString.h>
#include <SharedUtility.h>
#include <stdio.h>

#define ADD_TEMPLATE(in, size) \
	/* Add to the checksum */ \
	Add((unsigned char *)&in, size);

class CChecksum
{
private:
	unsigned short m_r;
	unsigned short m_c1;
	unsigned short m_c2;
	unsigned int   m_uiChecksum;

public:
	CChecksum()
	{
		// Reset the vars
		Reset();
	}

	bool operator == (CChecksum checksum)
	{
		return (m_uiChecksum == checksum.m_uiChecksum);
	}

	bool operator != (CChecksum checksum)
	{
		return (m_uiChecksum != checksum.m_uiChecksum);
	}

	void Reset()
	{
		m_r = 55665;
		m_c1 = 52845;
		m_c2 = 22719;
		m_uiChecksum = 0;
	}

	unsigned int GetChecksum() { return m_uiChecksum; }

	void Add(unsigned char * ucData, unsigned int uiLength)
	{
		for(unsigned int i = 0; i < uiLength; i++)
		{
			// Generate the cipher
			unsigned char ucCipher = (unsigned char)(ucData[i] ^ (m_r >> 8));
			m_r = ((ucCipher + m_r) * m_c1 + m_c2);

			// Add the cipher to the checksum
			m_uiChecksum += ucCipher;
		}
	}

	void Add(const bool &bData) { ADD_TEMPLATE(bData, sizeof(bool)); }
	void Add(const char &cData) { ADD_TEMPLATE(cData, sizeof(char)); }
	void Add(const unsigned char &ucData) { ADD_TEMPLATE(ucData, sizeof(unsigned char)); }
	void Add(const short &sData) { ADD_TEMPLATE(sData, sizeof(short)); }
	void Add(const unsigned short &usData) { ADD_TEMPLATE(usData, sizeof(unsigned short)); }
	void Add(const int &iData) { ADD_TEMPLATE(iData, sizeof(int)); }
	void Add(const unsigned int &uiData) { ADD_TEMPLATE(uiData, sizeof(unsigned int)); }
	void Add(const long &lData) { ADD_TEMPLATE(lData, sizeof(long)); }
	void Add(const unsigned long &ulData) { ADD_TEMPLATE(ulData, sizeof(unsigned long)); }
	void Add(const float &fData) { ADD_TEMPLATE(fData, sizeof(float)); }
	void Add(const double &dData) { ADD_TEMPLATE(dData, sizeof(double)); }
};

class CFileChecksum : public CChecksum
{
public:
	CFileChecksum() : CChecksum()
	{

	}

	bool Calculate(String strFilePath)
	{
		// Reset
		Reset();

		// Ensure the file exists
		if(!SharedUtility::Exists(strFilePath.Get()))
			return false;

		// Open the file
		FILE * fFile = fopen(strFilePath.Get(), "rb");

		// Did the file not open successfully?
		if(!fFile)
			return false;

		// Read file data in 32 byte blocks
		unsigned int uiBytesRead = 0;
		unsigned char ucData[32];

		while((uiBytesRead = fread(ucData, 1, sizeof(ucData), fFile)) > 0)
		{
			// Add the read data to the checksum
			Add(ucData, uiBytesRead);
		}

		// Close the file
		fclose(fFile);
		return true;
	}
};
