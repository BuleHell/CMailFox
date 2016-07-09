#include "base64.h"
#include <commctrl.h>
#include "rsrc.h"
#ifndef _CONTRL_H_
#define _CONTRL_H_
void initlistv(HWND hwnd,int cid);
 
void findfile(char *root);
void AddToListView(HWND hWnd,char *root,int id);
void AddListView(HWND hWnd,int id);
void OpenSaveFile(HWND hwnd,int id);
void readlist(HWND hwnd,NM_LISTVIEW*	phdr);
#endif
