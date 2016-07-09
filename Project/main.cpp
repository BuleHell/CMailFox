 
#include "main.h"
#include "dialogs.h"
#include <windowsx.h>
#define CSIZE 512

#include<mmsystem.h>
 
HWND g_hwnd;
extern UserOptionInfo USERMAIL;
void Form1_OnSize(HWND hwnd, UINT state, int cx, int cy);
BOOL     CALLBACK Dlgoption(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
char g_pwd[CSIZE],g_uname[CSIZE],g_domain[CSIZE],g_text[CSIZE];
char g_all[10][CSIZE];
 void WrightIni(HWND hwnd);
void getInfo(HWND hwnd);
 void ReadIni(HWND hwnd );
bool SetUserMail(HWND hwnd );
void Form1_OnSize(HWND hwnd, UINT state, int cx, int cy);
////////////////////////////////////////////////////////////////////////////////
//  Form1_OnSize


BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        //BEGIN MESSAGE CRACK
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
        HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
        HANDLE_MSG(hWnd,WM_SIZE, Form1_OnSize);
        case WM_NOTIFY:
        {
        	if(wParam==IDC_MAILLIST) 
        	{
        NM_LISTVIEW*	lpnmitem = (NM_LISTVIEW*) lParam;
        readlist(hWnd,lpnmitem);
      
			}
		}
        return true;
    }



    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//  Main_OnInitDialog
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    // Set app icons
    HICON hIcon = LoadIcon((HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE) ,MAKEINTRESOURCE(IDI_ICONAPP));
    SendMessage(hwnd, WM_SETICON, TRUE,  (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, FALSE, (LPARAM)hIcon);
    g_hwnd=hwnd;
    //
    // Add initializing code here
    // GetModuleFileName(NULL,USERMAIL.ExePath,512);

  
    initlistv(hwnd, IDC_MAILLIST);
      SetUserMail(hwnd);
AddListView(hwnd,IDC_MAILLIST);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//  Main_OnCommand
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{



    switch (id)
    {
    case IDC_OK:
    {
        getInfo(hwnd);
     
    }
    break;
    case IDC_CREATEMAIL:
        DialogBox( (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE)
                   ,MAKEINTRESOURCE(IDD_DLG2),hwnd,DlgCreateMail);
        break;
    case IDM_OPTION:
        DialogBox( (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE)
                   ,MAKEINTRESOURCE(IDD_MAILOPTIONDLG),hwnd,Dlgoption);
        break;
    case IDC_RECVMAIL:
  pop3CONN(USERMAIL.pop3server,USERMAIL.pop3port);
  AddListView( hwnd,IDC_MAILLIST);
 
	
        break;
    default:
        break;
    }


}

////////////////////////////////////////////////////////////////////////////////
//  Main_OnClose
void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}


//从对话框中得到要发送的信息 保存到文件中 



BOOL     CALLBACK DlgCreateMail(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_INITDIALOG:

        return TRUE;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_SENDIT:
        {
            getInfo(hWnd);
            MessageBox(hWnd,"Send","",0);
            EndDialog(hWnd,1);
 
        }
            break;
                   case IDC_ADDAT:
        {
        OpenSaveFile(hWnd,IDC_COMBO);
 
        }
            break; 
            
  
    case WM_CLOSE:
        EndDialog(hWnd,0);
        break;
    case WM_DESTROY:
        EndDialog(hWnd,0);
        break;

    }

	}
	    return FALSE;
}
void Form1_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    MoveWindow(GetDlgItem(hwnd,IDC_MAILLIST),180,90,cx-190,cy/3,true);
    MoveWindow(GetDlgItem(hwnd,IDC_EDITMAIL),180,cy/3+90,cx-190,cy-cy/3-90,true);


}

BOOL     CALLBACK Dlgoption(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_INITDIALOG:
 		ReadIni( hWnd );
        return TRUE;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
    
		case IDC_OK:
		{
			WrightIni( hWnd);
			SetUserMail(hWnd);
			
		 EndDialog(hWnd,0);
		}
		return TRUE;
		break;
	 
		case IDC_OP_CANCEL:
		{
			 EndDialog(hWnd,0);
	 
		return TRUE;
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd,0);
        break;
    case WM_DESTROY:
        EndDialog(hWnd,0);
        break;

    }
   
}
 return FALSE;
}
