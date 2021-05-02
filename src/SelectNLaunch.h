//this file is part of SelectNLaunch
//Copyright (C)2010 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef NPP_TOOLS_H
#define NPP_TOOLS_H

#define VERSION_VALUE "2.1\0"
#define VERSION_DIGITALVALUE 2, 1, 0, 0

#define IDD_ABOUTBOX 250

#ifndef IDC_STATIC 
#define IDC_STATIC -1
#endif

#ifdef UNICODE
	#define NppMainEntry wWinMain
	#define generic_strtol wcstol
	#define generic_strncpy wcsncpy
	#define generic_stricmp wcsicmp
	#define generic_strncmp wcsncmp
	#define generic_strnicmp wcsnicmp
	#define generic_strncat wcsncat
	#define generic_strchr wcschr
	#define generic_atoi _wtoi
	#define generic_itoa _itow
	#define generic_atof _wtof
	#define generic_strtok wcstok
	#define generic_strftime wcsftime
	#define generic_fprintf fwprintf
	#define generic_sscanf swscanf
	#define generic_fopen _wfopen
	#define generic_fgets fgetws
	#define generic_stat _wstat
	#define generic_sprintf swprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESW
#else
	#define NppMainEntry WinMain
	#define generic_strtol strtol
	#define generic_strncpy strncpy
	#define generic_stricmp stricmp
	#define generic_strncmp strncmp
	#define generic_strnicmp strnicmp
	#define generic_strncat strncat
	#define generic_strchr strchr
	#define generic_atoi atoi
	#define generic_itoa itoa
	#define generic_atof atof
	#define generic_strtok strtok
	#define generic_strftime strftime
	#define generic_fprintf fprintf
	#define generic_sscanf sscanf
	#define generic_fopen fopen
	#define generic_fgets fgets
	#define generic_stat _stat
	#define generic_sprintf sprintf
	#define COPYDATA_FILENAMES COPYDATA_FILENAMESA
#endif

typedef std::basic_string<TCHAR> generic_string;

void printInt(int int2print) 
{
	TCHAR str[32];
	wsprintf(str, TEXT("%d"), int2print);
	::MessageBox(NULL, str, TEXT(""), MB_OK);
};

struct CmdParam {
	TCHAR _cmdName[nbChar];
	PFUNCPLUGINCMD _pMainFunc;
	TCHAR _ext[10];
	int _convertFuncType;

	CmdParam() {
		_cmdName[0] = '\0';
		_ext[0] = '\0';
		_convertFuncType = -1;
	};
};

// convertFuncType
#define BASE64DECODE 0

int getCmdsFromConf(const TCHAR *confPathValue, CmdParam *cmdParamValue, int maxNbCmd);
void selectNlaunch(TCHAR *ext, int converFuncType);
void selectNlaunch_00();
void selectNlaunch_01();
void selectNlaunch_02();
void selectNlaunch_03();
void selectNlaunch_04();
void selectNlaunch_05();
void selectNlaunch_06();
void selectNlaunch_07();
void selectNlaunch_08();
void selectNlaunch_09();
void selectNlaunch_10();
void selectNlaunch_11();
void selectNlaunch_12();
void selectNlaunch_13();
void selectNlaunch_14();
void selectNlaunch_15();
void selectNlaunch_16();
void selectNlaunch_17();
void selectNlaunch_18();
void selectNlaunch_19();

void editCommands();
void about();

#endif //NPP_TOOLS_H
