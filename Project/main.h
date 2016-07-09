
#include "smtp.h"
#include "resource.h"
#ifndef _MAIN_H
#define _MAIN_H
#include "contrl.h"
#include "pop3.h"
 

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Main_OnClose(HWND hwnd);
 
BOOL     CALLBACK DlgCreateMail(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


#endif
