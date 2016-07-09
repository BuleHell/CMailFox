#include "contrl.h"
#include <stdio.h>
#include <shlwapi.h>
#include "pop3.h"
#include "smtp.h"
#include "regexpress.h"
#define SSIZ 6
char namelist[SSIZ][20]=
{
    "发件人",
    "标题",
    "收件人" ,
    "日期",
    "备注",
    "附件"
};

char szoption[8][512]=
{
    "username",
    "userpass",
    "usermailaddr",
    "smtpserver",
    "pop3server",
    "savepath",
    "pop3port","smtpport"
};
void AddItem(HWND hwndTV, LPSTR lpszItem, int nLevel);
extern UserOptionInfo USERMAIL;

void GetFileName(char *fullname,char *getname);
//初始化listview 控件 并保存EXE的文件路径
void initlistv(HWND hwnd,int cid)
{
    SendDlgItemMessage(hwnd,cid,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    LVCOLUMN lm;
    lm.mask=LVCF_TEXT|LVCF_WIDTH;
    lm.cx=120;
    for (int i=0;i<6;i++)
    {
        lm.pszText=namelist[i];
        SendDlgItemMessage(hwnd,cid,LVM_INSERTCOLUMN,i,LPARAM(&lm));

    }

    GetModuleFileName(NULL,USERMAIL.ExePath,512);
    PathRemoveFileSpec( USERMAIL.ExePath);
    int len=strlen (USERMAIL.ExePath);
    USERMAIL.ExePath[len]='\\';
    USERMAIL.ExePath[len+1]='\0';

    puts(USERMAIL.ExePath);
    HWND treehwnd=GetDlgItem(hwnd,IDC_MAILTREE);
    AddItem(treehwnd,"收件箱",1);
    AddItem(treehwnd,"发件箱",1);
    AddItem(treehwnd,"垃圾箱",1);
}

/*
void AddToListView(HWND hWnd,char *root,int id)
{

   LVITEM lvi={0},lvs={0};
   int nitem=0;
   char temp[512];
   SendDlgItemMessage(hWnd,id,LVM_DELETEALLITEMS,0,0);
   		lvi.mask=LVIF_TEXT;
   	WIN32_FIND_DATA fdata;
   DWORD errorcode = 0;
HANDLE hFile=FindFirstFile(root,&fdata);

while(hFile!=INVALID_HANDLE_VALUE&&errorcode!=ERROR_NO_MORE_FILES)
{

   		lvi.iItem=nitem;

   	strcpy(temp,fdata.cFileName);
   	lvi.pszText=temp;
   	SendDlgItemMessage(hWnd,id,LVM_INSERTITEM,0,LPARAM(&lvi));
   	for(int xx=0;xx<SSIZ;xx++)
   	{
   				strcpy(temp,fdata.cFileName);
    		lvs.iSubItem=xx;
    		lvs.pszText=temp;
    			SendDlgItemMessage(hWnd,id,LVM_SETITEMTEXT,nitem,LPARAM(&lvs));
   	}
   	nitem++;
   		FindNextFile(hFile,&fdata);
  errorcode=GetLastError();
 
}
 
 

}
*/


//读取 .lis文件 已下载的邮件列表 发送到listview
void AddListView(HWND hWnd,int id)
{
    char filename[512];
    sprintf(filename,"%smymail.lis",USERMAIL.savepath);

    FILE *fp=fopen(filename,"r");

    LVITEM lvi={0},lvs={0};
    int nitem=0;
    char temp[512];
    if (fp==NULL)
    {
        return;
    }
    char data[2048];

    string str;
    smatch what,swhat;
    SendDlgItemMessage(hWnd,id,LVM_DELETEALLITEMS,0,0);
    char MIMEH[512]= "_mmb_from=(.*?)_mmb_from=\\s*_mmb_subject=(.*?)_mmb_subject=\\s*_mmb_to=(.*?)_mmb_to= _mmb_data=(.*?)_mmb_data=\\s*_mmn_num=(.*?)_mmn_num=";

    regex express(MIMEH);

    int index=0;
    while (	fgets(data,2048,fp)!=NULL )
    {

        str=data;
        string::const_iterator start = str.begin();
        string::const_iterator end =str.end();
        bool hr=regex_search(start, end, what, express );
        if (hr)
        {
            lvi.iItem=nitem;
 
            strcpy(temp,what[1].str().c_str());
            lvi.pszText=temp;
            SendDlgItemMessage(hWnd,id,LVM_INSERTITEM,0,LPARAM(&lvi));
            for (int xx=0;xx<5;xx++)
            {
                strcpy(temp,what[xx+1].str().c_str());
                lvs.iSubItem=xx;
                lvs.pszText=temp;
                SendDlgItemMessage(hWnd,id,LVM_SETITEMTEXT,nitem,LPARAM(&lvs));
            }
            nitem++;
        }
    }
    fclose(fp);
}


//在listview 项被点击后 打开邮件的文本文件
void readlist(HWND hwnd,NM_LISTVIEW*	phdr)
{

    char text[128];
    if (phdr->hdr.code == NM_CLICK && phdr->iItem != 0xffffffff)
    {
		
        HWND hList = GetDlgItem(hwnd, 1004);
        ListView_GetItemText(hList, phdr->iItem,4, text, sizeof(text));
   
    }
    else
    {
        return;
    }
    char  filename[512];
    puts(text);

    sprintf(filename,"%s%s.mmb",USERMAIL.savepath,text);


    FILE *fp=fopen(filename,"rb");
    if (fp==NULL)
    {
        SetDlgItemText(hwnd,IDC_EDITMAIL,"无正文");
        puts("openfilefailed");
        return;
    }
    char szfilebuf[20000];
    int nread=	fread(szfilebuf,1,20000,fp);
    szfilebuf[nread]=0;
    SetDlgItemText(hwnd,IDC_EDITMAIL,szfilebuf);
    puts(szfilebuf);
    fclose(fp);
}

void WrightIni(HWND hwnd )
{


char szoption[8][512]=
{
    "username",
    "userpass",
    "usermailaddr",
    "smtpserver",
    "pop3server",
    "savepath",
    "pop3port","smtpport"
};
    char szgetoption[8][512] ;
    for (int i=0;i<8;i++)
    {
        GetDlgItemText(hwnd,i+6001,szgetoption[i],512);
    }


    char inifile[512];
    sprintf(inifile,"%soption.ini",USERMAIL.ExePath);
    for (int y=0;y<8;y++)
    {
        WritePrivateProfileString("MyMailBoxOption",szoption[y],szgetoption[y],inifile);
    }

}


//从配置文件中读取设置 发送到对话框控件
void ReadIni(HWND hwnd )
{

char szoption[8][512]=
{
    "username",
    "userpass",
    "usermailaddr",
    "smtpserver",
    "pop3server",
    "savepath",
    "pop3port","smtpport"
};


    char szgetoption[8][512];
    char inifile[512];
    sprintf(inifile,"%soption.ini",USERMAIL.ExePath);
    puts(USERMAIL.ExePath);
    for (int i=0;i<8;i++)
    {
        GetPrivateProfileString("MyMailBoxOption",szoption[i],"",szgetoption[i],512,inifile);
    }

    for (int y=0;y<8;y++)
    {
        SetDlgItemText(hwnd,y+6001,szgetoption[y]);
    }

}


//从配置文件中读取 设置全局变量
bool SetUserMail(HWND hwnd )
{


char szoption[8][512]=
{
    "username",
    "userpass",
    "usermailaddr",
    "smtpserver",
    "pop3server",
    "savepath",
    "pop3port","smtpport"
};


    char szgetoption[8][512];
    char inifile[512];
    sprintf(inifile,"%soption.ini",USERMAIL.ExePath);
    for (int f=0;f<8;f++)
    {
        GetPrivateProfileString("MyMailBoxOption",szoption[f],"",szgetoption[f],512,inifile);

    }

    strncpy(USERMAIL.username,szgetoption[0],512);
    strncpy(USERMAIL.userpass,szgetoption[1],512);
    strncpy(USERMAIL.usermailaddr,szgetoption[2],512);
    strncpy(USERMAIL.smtpserver,szgetoption[3],512);
    strncpy(USERMAIL.pop3server,szgetoption[4],512);
    strncpy(USERMAIL.savepath,USERMAIL.ExePath,512);
    USERMAIL.pop3port=atoi(szgetoption[6]) ;
    USERMAIL.smtpport=atoi(szgetoption[7]) ;
    base64(USERMAIL.username,strlen(USERMAIL.username),USERMAIL.e64uname);
    base64(USERMAIL.userpass,strlen(USERMAIL.userpass),USERMAIL.e64upass);
    return 1;
}


void OpenSaveFile(HWND hwnd,int id)
{
    char lpstr[512]="\0",lpf[512]="All Files\0*.*\0\0";
    OPENFILENAME lpofn;
    ZeroMemory (&lpofn,sizeof(lpofn));
    lpofn.Flags =  OFN_EXPLORER|OFN_LONGNAMES|OFN_PATHMUSTEXIST| OFN_FILEMUSTEXIST;


    lpofn.hwndOwner=hwnd;
    lpofn.lStructSize=sizeof(lpofn);
    HINSTANCE hInst= GetModuleHandle(NULL);

    lpofn.hInstance= hInst;
    lpofn.lpstrFile  =lpstr;
    lpofn.nMaxFile=512;
    lpofn.lpstrFilter=lpf;

    if (GetOpenFileName(&lpofn))
    {
    	SendDlgItemMessage(hwnd,id,CB_ADDSTRING,0,LPARAM(lpstr));
        int Attcount=GetDlgItemInt(hwnd,	IDC_ATTMOUNT,NULL,1);
        Attcount++;
        SetDlgItemInt(hwnd,IDC_ATTMOUNT,Attcount,1);
        
    }

}


void getInfo(HWND hwnd)
{
	
	
const char ATTACH[512]="Content-Type: application/octet-stream;\r\n      name=\"%s\"\r\nContent-Disposition: attachment; filename=\"%s\"\r\nContent-Transfer-Encoding: base64\r\n\r\n";

    char mailto[CSIZE],Mailto[CSIZE];
    char mailfrom[CSIZE],Mailfrom[CSIZE];
    char mailtitle[CSIZE],Mailtitle[CSIZE];
    SYSTEMTIME sys;

    GetLocalTime(&sys);
    char filename[CSIZE],szMailText[20000];

    GetDlgItemText(hwnd,IDCD_RECEIVER,mailto,CSIZE);

    GetDlgItemText(hwnd,IDCD_TITLE,mailtitle,CSIZE);
    GetDlgItemText(hwnd,IDC_MAILTEXTEDT,szMailText,20000);
    int Attcount=GetDlgItemInt(hwnd,	IDC_ATTMOUNT,NULL,1);
    const char chto[CSIZE]="To: <%s>\r\n";
    const char chfrom[CSIZE]="From: %s <%s>\r\n";
    const char subject[CSIZE]="Subject: %s\r\nX-mailer: MyMailBox[cn]\r\nMime-Version: 1.0\r\n";

    sprintf(Mailto,chto,mailto);
    sprintf(Mailfrom,chfrom,USERMAIL.username,USERMAIL.usermailaddr);
    sprintf(Mailtitle,subject,mailtitle);

    sprintf(filename,"%s%s%d%d%d%d%d%d.txt",USERMAIL.savepath,mailto,sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
    FILE *fp= fopen(filename,"wb+");
    if (fp==NULL)
    {
        MessageBox(hwnd,"创建邮件失败！","文件存贮失败",0);
        return;
    }
    fwrite(Mailfrom,strlen(Mailfrom),1,fp);

    fwrite(Mailto,strlen(Mailto),1,fp);
    fwrite(Mailtitle,strlen(Mailtitle),1,fp);
    if (Attcount<=0)  //无附件
    {
        char type[CSIZE]="Content-Type: text/plain;\r\n    charset=\"8bit\";\r\n\r\n";
        fwrite(type,strlen(type),1,fp);
        fwrite(szMailText,strlen(szMailText),1,fp);
    }
    else
    {
    	const char Mix[512]="Content-Type: multipart/mixed;\r\n";
    	const char Ki[512]="     boundary=\"%s\"\r\n";
    	
		const char Bound[512]="----=_Next%010d=--";
		
		
		
		char bon[512],BON[512];
		srand(GetTickCount());
		int r=rand()%100000;
		sprintf(bon,Bound,r);
		sprintf(BON,Ki,bon);		
		 fwrite(Mix,1,strlen(Mix),fp);
		 fwrite(BON,1,strlen(BON),fp);
				 fwrite("\r\n",1,strlen("\r\n"),fp);
				 

		char qthis[512]=	"This is a multi-part message in MIME format.\r\n\r\n";
		
					 fwrite(qthis,1,strlen(qthis),fp); 	
			 fwrite(bon,1,strlen(bon),fp); 	
			 	 				 fwrite("\r\n",1,strlen("\r\n"),fp);
			 char type[CSIZE]="Content-Type: text/plain;\r\n    charset=\"8bit\";\r\n\r\n";
        fwrite(type,strlen(type),1,fp);
        fwrite(szMailText,strlen(szMailText),1,fp);
			 	 
				 fwrite("\r\n",1,strlen("\r\n"),fp);
			 fwrite(bon,1,strlen(bon),fp);
			 		 fwrite("\r\n",1,strlen("\r\n"),fp); 	
    	char FileName[512],shortname[128]="fujian",Contype[512];
    	int nRead;
    	
    	for(int a=0;a<Attcount;a++)
    	{
    		SendDlgItemMessage(hwnd,IDC_COMBO,CB_GETLBTEXT,a,LPARAM(FileName));
    		FILE *filea=fopen(FileName,"rb");
    		if(filea==NULL)
    		{
    			continue;
			}
			GetFileName(FileName,shortname);
			sprintf(Contype,ATTACH,shortname,shortname);
			fwrite(Contype,1,strlen(Contype),fp);
    		char tt[4000],base[6000];
    		while(1) 
			{
				nRead=fread(tt,1,3900,filea);
				if(nRead<=0)
				{
				break;
				}
			base64(tt,nRead,base);
			fwrite(base,1,strlen(base),fp);
			
			}
    		fclose(filea);
    			 fwrite("\r\n",1,strlen("\r\n"),fp);
			 fwrite(bon,1,strlen(bon),fp); 
			 fwrite("\r\n",1,strlen("\r\n"),fp);
    		puts(FileName);
		}
	}
   fwrite("\r\n.\r\n",strlen("\r\n.\r\n"),1,fp);
    fclose(fp);
   smtpCONN(filename,mailto);
}


void GetFileName(char *fullname,char *getname)
{
	int slash=strlen(fullname),y;
	for( y=slash;y>=0;y--)
	{
		if(fullname[y]=='\\')
		{
			strncpy(getname,fullname+y+1,128);
			break;
		}
	}
}

void AddItem(HWND hwndTV, LPSTR lpszItem, int nLevel)
{
TVITEM tvi; 
    TVINSERTSTRUCT tvins; 
    static HTREEITEM hPrev = (HTREEITEM) TVI_FIRST; 
    static HTREEITEM hPrevRootItem = NULL; 
    static HTREEITEM hPrevLev2Item = NULL; 
    HTREEITEM hti; 
 
    tvi.mask = TVIF_TEXT | TVIF_IMAGE 
        | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
 
    tvi.pszText = lpszItem; 
    tvi.cchTextMax = lstrlen(lpszItem); 
 
    tvi.lParam = (LPARAM) nLevel; 
 
    tvins.item = tvi; 
    tvins.hInsertAfter = hPrev;  
    if (nLevel == 1) 
        tvins.hParent = TVI_ROOT; 
    else if (nLevel == 2) 
        tvins.hParent = hPrevRootItem; 
    else 
        tvins.hParent = hPrevLev2Item; 
  
    hPrev = (HTREEITEM) SendMessage(hwndTV, TVM_INSERTITEM, 0, 
         (LPARAM) (LPTVINSERTSTRUCT) &tvins); 
  
    if (nLevel == 1) 
        hPrevRootItem = hPrev; 
    else if (nLevel == 2) 
        hPrevLev2Item = hPrev; 
  
    if (nLevel > 1) { 
        hti = TreeView_GetParent(hwndTV, hPrev); 
        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
        tvi.hItem = hti; 
 
        TreeView_SetItem(hwndTV, &tvi); 
    } 


}

  
