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

#include <string>
#include <vector>
#include <time.h>
#include <Shlwapi.h>
#include "PluginInterface.h"
#include "SelectNLaunch.h"
#include "b64.h"

#define NB_BASE_CMD 2
const TCHAR PLUGIN_NAME[] = TEXT("Select 'N' Launch");
int nbFunc = NB_BASE_CMD; // "Edit Commands" & "About"

HINSTANCE _hInst;
NppData nppData;
FuncItem *funcItem;
generic_string confPath;

const char confContent[] = "\
; Select 'N' Launch plugin allows you to convert the selected text to whatever you want, launch it on the fly\n\
; You should define the document type (by extension) you want to launch and voila.\n\
;\n\
; What Select 'N' Launch plugin does is writing the selected text into a temporary file with the extension you defined,\n\
; then run shellExecute to launch it.\n\
;\n\
; Example: if you need to check a certificate in a XAdES signature document,\n\
; or a PDF document encoded in base 64 in your text file, you just need to define your command name, the extension (crt or pdf in our context),\n\
; and buildin convertion function (optional, \"b64decode\" is needed for launching a PDF document if your pdf is encoded in base64).\n\
;\n\
; built-in convertion function :\n\
; 1. b64decode\n\
\n\
[Selection to PDF]\n\
ext=pdf\n\
\n\
[Base64 to PDF]\n\
ext=pdf\n\
convertFunc=b64decode\n\
\n\
[Selection to Certificate]\n\
ext=crt\n\
\n\
[Selection to CRL]\n\
ext=crl\n\
\n";

const int maxNbCmd = 20;
CmdParam cmdParam[maxNbCmd];

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID /*lpReserved*/)
{
	switch (reasonForCall)
	{
		case DLL_PROCESS_ATTACH:
		{
			_hInst = (HINSTANCE)hModule;
			cmdParam[0]._pMainFunc = selectNlaunch_00;
			cmdParam[1]._pMainFunc = selectNlaunch_01;
			cmdParam[2]._pMainFunc = selectNlaunch_02;
			cmdParam[3]._pMainFunc = selectNlaunch_03;
			cmdParam[4]._pMainFunc = selectNlaunch_04;
			cmdParam[5]._pMainFunc = selectNlaunch_05;
			cmdParam[6]._pMainFunc = selectNlaunch_06;
			cmdParam[7]._pMainFunc = selectNlaunch_07;
			cmdParam[8]._pMainFunc = selectNlaunch_08;
			cmdParam[9]._pMainFunc = selectNlaunch_09;
			cmdParam[10]._pMainFunc = selectNlaunch_10;
			cmdParam[11]._pMainFunc = selectNlaunch_11;
			cmdParam[12]._pMainFunc = selectNlaunch_12;
			cmdParam[13]._pMainFunc = selectNlaunch_13;
			cmdParam[14]._pMainFunc = selectNlaunch_14;
			cmdParam[15]._pMainFunc = selectNlaunch_15;
			cmdParam[16]._pMainFunc = selectNlaunch_16;
			cmdParam[17]._pMainFunc = selectNlaunch_17;
			cmdParam[18]._pMainFunc = selectNlaunch_18;
			cmdParam[19]._pMainFunc = selectNlaunch_19;
		}
		break;

		case DLL_PROCESS_DETACH:
		{
			delete [] funcItem;
		}
		break;

		case DLL_THREAD_ATTACH:
		break;

		case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
	TCHAR confDir[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)confDir);
	confPath = confDir;
	confPath += TEXT("\\SelectNLaunch.ini");
	
	if (!::PathFileExists(confPath.c_str()))
	{
		FILE *f = generic_fopen(confPath.c_str(), TEXT("w"));
		if (f)
		{
			fwrite(confContent, sizeof(confContent[0]), strlen(confContent), f);
			fflush(f);
			fclose(f);
		}
		else
		{
			generic_string msg = confPath + TEXT(" is absent, and this file cannot be create.");
			::MessageBox(nppData._nppHandle, confPath.c_str(), TEXT("Not present"), MB_OK);
		}
	}
	nbFunc += getCmdsFromConf(confPath.c_str(), cmdParam, maxNbCmd);
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	bool insertSeparator = false;
	if (nbFunc > NB_BASE_CMD)
	{
		nbFunc += 1;
		insertSeparator = true;
	}
	*nbF = nbFunc;
	funcItem = new FuncItem[nbFunc];

	// User customizable commands
	int i = 0;
	for ( ; i < nbFunc - (insertSeparator?NB_BASE_CMD+1:NB_BASE_CMD) ; )
	{
		funcItem[i]._pFunc = cmdParam[i]._pMainFunc;
		lstrcpy(funcItem[i]._itemName, cmdParam[i]._cmdName);
		funcItem[i]._init2Check = false;
		funcItem[i]._pShKey = NULL;
		i++;
	}

	// Separator if necessary
	if (insertSeparator)
	{
		funcItem[i]._pFunc = NULL;
		lstrcpy(funcItem[i]._itemName, TEXT("-SEPARATOR-"));
		funcItem[i]._init2Check = false;
		funcItem[i]._pShKey = NULL;
		i++;
	}

	// Edit Commands
	funcItem[i]._pFunc = editCommands;
	lstrcpy(funcItem[i]._itemName, TEXT("Edit User Commands"));
	funcItem[i]._init2Check = false;
	funcItem[i]._pShKey = NULL;
	i++;

	// About
	funcItem[i]._pFunc = about;
	lstrcpy(funcItem[i]._itemName, TEXT("About"));
	funcItem[i]._init2Check = false;
	funcItem[i]._pShKey = NULL;

	return funcItem;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification* /*notifyCode*/)
{	
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
	return TRUE;
}
#endif //UNICODE

// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

HWND getCurrentScintillaHandle() {
    int currentEdit;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	return (currentEdit == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
};



int getCmdsFromConf(const TCHAR *confPathValue, CmdParam * /*cmdParamValue*/, int /*maxNbCmd*/)
{
	TCHAR cmdNames[MAX_PATH];
	::GetPrivateProfileSectionNames(cmdNames, MAX_PATH, confPathValue);
	TCHAR *pFn = cmdNames;

	int i = 0;
	while (*pFn)
	{
		TCHAR extVal[64];
		GetPrivateProfileString(pFn, TEXT("ext"), TEXT("invalid"), extVal, 64, confPathValue);

		if (lstrcmp(extVal, TEXT("invalid")) != 0)
		{
			lstrcpy(cmdParam[i]._cmdName, pFn);
			lstrcpy(cmdParam[i]._ext, extVal);

			TCHAR convertFunc[64];
			GetPrivateProfileString(pFn, TEXT("convertFunc"), TEXT("invalid"), convertFunc, 64, confPathValue);

			if (lstrcmp(convertFunc, TEXT("b64decode")) == 0)
			{
				cmdParam[i]._convertFuncType = BASE64DECODE;
			}
			i++;
		}

		pFn += lstrlen(pFn) + 1;
	}
	
	return i;
}

bool b64decode(char **pText, size_t *textLen) 
{
	//::MessageBoxA(NULL, "b64decode", "I'm in", MB_OK);
	if (*textLen == 0) return false;

    char * pBase64Text = new char[*textLen];

    // this line is added to walk around Scintilla 201 bug
    *textLen = *textLen + 1;

	size_t m = 0, n = 0;
	for ( ; m < *textLen ; )
	{
		if ((*pText)[m] == 0x0A || (*pText)[m] == 0x0D)
			m++;
		else
			pBase64Text[n++] = (*pText)[m++];
	}

	size_t asciiLen = getAsciiLenFromBase64Len(n-1);
	if (!asciiLen)
	{
		::MessageBox(nppData._nppHandle, TEXT("Length of selected text (not including EOL) to be decoded is invalid.\rIt should be mod 4."), TEXT("Base64"), MB_OK);
		return false;
	}

	char * pAsciiText = new char[asciiLen + 1];

	int len = base64ToAscii(pAsciiText, pBase64Text);

	if (len == -1)
	{
		::MessageBox(nppData._nppHandle, TEXT("Problem!"), TEXT("Base64"), MB_OK);
		return false;
	}

	delete [] *pText;
	delete [] pBase64Text;
	*pText = pAsciiText;
	*textLen = len;
	return true;
};


void selectNlaunch(TCHAR *ext, int convertFunc)
{
	if (!ext || lstrcmp(ext, TEXT("")) == 0)
		return;

	HWND hCurrScintilla = getCurrentScintillaHandle();

    size_t start = ::SendMessage(hCurrScintilla, SCI_GETSELECTIONSTART, 0, 0);
	size_t end = ::SendMessage(hCurrScintilla, SCI_GETSELECTIONEND, 0, 0);
	if (end < start)
	{
		size_t tmp = start;
		start = end;
		end = tmp;
	}

	size_t textLen = end - start;
    if (textLen == 0) return;

    char * pText = new char[textLen+1];
    ::SendMessage(hCurrScintilla, SCI_GETSELTEXT, 0, (LPARAM)pText);

	if (convertFunc >= 0)
	{
		switch (convertFunc)
		{
			case BASE64DECODE :
				b64decode(&pText, &textLen);
			break;
		}
	}

	generic_string timestampFilename(TEXT(""));
	const int temBufLen = 32;
	TCHAR tmpbuf[temBufLen];
	time_t ltime = time(0);
	struct tm *today;

	today = localtime(&ltime);
	generic_strftime(tmpbuf, temBufLen, TEXT("%Y%m%d%H%M%S"), today);
	TCHAR tempPath[256];
	::GetTempPath(256, tempPath);
	timestampFilename += tempPath;
	timestampFilename += tmpbuf;
	timestampFilename += TEXT(".");
	timestampFilename += ext;


	FILE *f = generic_fopen(timestampFilename.c_str(), TEXT("wb"));
	if (f)
	{
		fwrite(pText, sizeof(pText[0]), textLen, f);
		fflush(f);
		fclose(f);
		::ShellExecute(NULL, TEXT("open"), timestampFilename.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	delete [] pText;
}

void selectNlaunch_00() {selectNlaunch( cmdParam[0]._ext, cmdParam[0]._convertFuncType);}
void selectNlaunch_01() {selectNlaunch( cmdParam[1]._ext, cmdParam[1]._convertFuncType);}
void selectNlaunch_02() {selectNlaunch( cmdParam[2]._ext, cmdParam[2]._convertFuncType);}
void selectNlaunch_03() {selectNlaunch( cmdParam[3]._ext, cmdParam[3]._convertFuncType);}
void selectNlaunch_04() {selectNlaunch( cmdParam[4]._ext, cmdParam[4]._convertFuncType);}
void selectNlaunch_05() {selectNlaunch( cmdParam[5]._ext, cmdParam[5]._convertFuncType);}
void selectNlaunch_06() {selectNlaunch( cmdParam[6]._ext, cmdParam[6]._convertFuncType);}
void selectNlaunch_07() {selectNlaunch( cmdParam[7]._ext, cmdParam[7]._convertFuncType);}
void selectNlaunch_08() {selectNlaunch( cmdParam[8]._ext, cmdParam[8]._convertFuncType);}
void selectNlaunch_09() {selectNlaunch( cmdParam[9]._ext, cmdParam[9]._convertFuncType);}
void selectNlaunch_10() {selectNlaunch(cmdParam[10]._ext, cmdParam[10]._convertFuncType);}
void selectNlaunch_11() {selectNlaunch(cmdParam[11]._ext, cmdParam[11]._convertFuncType);}
void selectNlaunch_12() {selectNlaunch(cmdParam[12]._ext, cmdParam[12]._convertFuncType);}
void selectNlaunch_13() {selectNlaunch(cmdParam[13]._ext, cmdParam[13]._convertFuncType);}
void selectNlaunch_14() {selectNlaunch(cmdParam[14]._ext, cmdParam[14]._convertFuncType);}
void selectNlaunch_15() {selectNlaunch(cmdParam[15]._ext, cmdParam[15]._convertFuncType);}
void selectNlaunch_16() {selectNlaunch(cmdParam[16]._ext, cmdParam[16]._convertFuncType);}
void selectNlaunch_17() {selectNlaunch(cmdParam[17]._ext, cmdParam[17]._convertFuncType);}
void selectNlaunch_18() {selectNlaunch(cmdParam[18]._ext, cmdParam[18]._convertFuncType);}
void selectNlaunch_19() {selectNlaunch(cmdParam[19]._ext, cmdParam[19]._convertFuncType);}

BOOL CALLBACK dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM /*lParam*/) 
{
	switch (message) 
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
            {
                case IDCLOSE :
			    {
					::EndDialog(hwnd, 0);
					return  TRUE;
				}
			}
			return FALSE;
	}
	return FALSE;
}

void editCommands()
{
	if (!::PathFileExists(confPath.c_str()))
	{
		generic_string msg = confPath + TEXT("is not present.\\rPlease create this file manually.");
		::MessageBox(nppData._nppHandle, msg.c_str(), TEXT("Configuration file is absent"), MB_OK);
		return;
	}
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)confPath.c_str());
}

void about()
{
	HWND hSelf = ::CreateDialogParam(_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), nppData._nppHandle, (DLGPROC)dlgProc, (LPARAM)NULL);
		    
	// Go to center
	RECT rc;
	::GetClientRect(nppData._nppHandle, &rc);
	POINT center;
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	center.x = rc.left + w/2;
	center.y = rc.top + h/2;
	::ClientToScreen(nppData._nppHandle, &center);

	RECT dlgRect;
	::GetClientRect(hSelf, &dlgRect);
	int x = center.x - (dlgRect.right - dlgRect.left)/2;
	int y = center.y - (dlgRect.bottom - dlgRect.top)/2;

	::SetWindowPos(hSelf, HWND_TOP, x, y, (dlgRect.right - dlgRect.left), (dlgRect.bottom - dlgRect.top), SWP_SHOWWINDOW);
}
