/*
 * PROJECT:     ReactOS FreeLoader installer for Linux
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Installation functions
 * COPYRIGHT:   Copyright 2020 Arnav Bhatt (arnavbhatt2004@gmail.com)
 */

#include "install.h"
#include "volume.h"

#include <stdbool.h> 
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

/*
 * These includes are required to define
 * the fat_data and fat32_data arrays.
 */
#include "fat.h"
#include "fat32.h"

bool InstallBootSector(char* lpszVolumeType)
{
	unsigned char BootSectorBuffer[512];

	if (!ReadVolumeSector(0, BootSectorBuffer))
	{
		return false;
	}

	if (strcasecmp(lpszVolumeType, "fat32") == 0)
	{
		//
		// Update the BPB in the new boot sector
		//
		memcpy((fat32_data+3), (BootSectorBuffer+3), 87 /*fat32 BPB length*/);

		//
		// Write out new boot sector
		//
		if (!WriteVolumeSector(0, fat32_data))
		{
			return false;
		}

		//
		// Write out new extra sector
		//
		if (!WriteVolumeSector(14, (fat32_data+512)))
		{
			return false;
		}
    	}

	else if (strcasecmp(lpszVolumeType, "fat") == 0)
	{
		//
		// Update the BPB in the new boot sector
 		//
		memcpy((fat_data+3), (BootSectorBuffer+3), 59 /*fat BPB length*/);

		//
		// Write out new boot sector
		//
		if (!WriteVolumeSector(0, fat_data))
		{
			return false;
		}
	}

	else
	{
		printf("%s:%d: ", __FILE__, __LINE__);
		printf("File system type %s unknown.\n", lpszVolumeType);
		return false;
	}

	printf("%s boot sector installed.\n", lpszVolumeType);

	return true;
}
