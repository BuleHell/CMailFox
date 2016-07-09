#include "main.h"
#include "dialogs.h"
 
 UserOptionInfo USERMAIL; 
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
HMODULE dllmod= LoadLibrary(TEXT("Comctl32.dll"));
init();
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DLG1), NULL, Main_Proc);
     FreeLibrary(dllmod);
    return(0);
}
