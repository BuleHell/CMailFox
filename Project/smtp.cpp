#include "smtp.h"
#define IDC_LST1 1002
#define BUFSIZE 4096
extern HWND g_hwnd;
extern UserOptionInfo USERMAIL;
extern char g_all[10][512];
void init()
{
    WSADATA wsad;
    int n=WSAStartup(MAKEWORD(2,2),&wsad);
    if (n!=0)
        MessageBox(NULL,"error inin network","",0);
}
 
bool SmtpNegociation(const char *e64username,const char *e64password,char *username,SOCKET s);
bool SendMail(SOCKET s,char *mailfrom,char *rept,char *filetosend);
void smtpCONN(char *filetosend,char *mailsendto)
{
 
    hostent * remoteip=gethostbyname(USERMAIL.smtpserver);
    if (NULL==remoteip)
    {


        return;
    }
 
    SOCKADDR_IN saddr;
    memcpy(&	saddr.sin_addr,remoteip->h_addr,4);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(USERMAIL.smtpport);

    SOCKET s=socket(AF_INET,SOCK_STREAM,0);
    int nRe=connect(s,(sockaddr*)&saddr,sizeof(saddr));
    if (nRe!=0)
    {
        MessageBox(g_hwnd,"连接失败","",0);
    
        return;
    }
  
    bool hr= SmtpNegociation(USERMAIL.e64uname,USERMAIL.e64upass,USERMAIL.username,s);
    if (!hr)
    {
        MessageBox(g_hwnd,"发送密码验证失败","",0);
   
        return;
    }
    hr=SendMail(s,USERMAIL.usermailaddr,mailsendto,filetosend);
    if(hr)
    {
     puts("mail succeed");
    }
 
    closesocket(s);
}

bool SmtpNegociation(const char *e64username,const  char *e64password,char *username,SOCKET s)
{

    char SendMsg[BUFSIZE],ReceiveMsg[CSIZE/2];
    int succeed=0,recved=0;
    
    recv(s,ReceiveMsg,BUFSIZE,0);
    sprintf(SendMsg,"EHLO %s\r\n",username);
    send(s,SendMsg,strlen(SendMsg),0);
    recv(s,ReceiveMsg,CSIZE/2,0);

 
    sprintf(SendMsg,"AUTH LOGIN\r\n");
    send(s,SendMsg,strlen(SendMsg),0);
    recv(s,ReceiveMsg,CSIZE/2,0);

    sprintf(SendMsg,"%s\r\n",e64username);
    send(s,SendMsg,strlen(SendMsg),0);
    recv(s,ReceiveMsg,CSIZE/2,0);

    sprintf(SendMsg,"%s\r\n",e64password);
    send(s,SendMsg,strlen(SendMsg),0);
     recv(s,ReceiveMsg,CSIZE/2,0);
 
    sscanf(ReceiveMsg,"%d",&succeed);
  
    printf("%d\n",succeed);
    return 1;

}

bool SendMail(SOCKET s,char *mailfrom,char *rept,char *filetosend)
{
 

    char Sendbuf[CSIZE],ReceiveMsg[CSIZE];
 
    sprintf(Sendbuf,"MAIL FROM:<%s>\r\n",USERMAIL.usermailaddr);
    send(s,Sendbuf,strlen(Sendbuf),0);
  
    recv(s,ReceiveMsg,CSIZE,0);
 puts(Sendbuf);
    sprintf(Sendbuf,"RCPT TO:<%s>\r\n",rept);
    send(s,Sendbuf,strlen(Sendbuf),0);
    puts(Sendbuf);
    recv(s,ReceiveMsg,CSIZE,0);
    sprintf(Sendbuf,"DATA\r\n");
    send(s,Sendbuf,strlen(Sendbuf),0);
   puts(Sendbuf);
    recv(s,ReceiveMsg,CSIZE,0);
 
    FILE *fp=fopen(filetosend,"rb");
    char sendbuf[CSIZE+1];
    if (fp==NULL)
    {
    	        MessageBox(NULL,"openfilefield","",0);
        return 0;
    }
    int nfileread=1;
    puts("邮件内容");
    while (1)
    {
        nfileread=	fread(sendbuf,1,CSIZE,fp ) ;
 
        if (nfileread<=0)
        {
 
            fclose(fp);
            break;
        }
        sendbuf[nfileread]='\0';
        printf("%s", sendbuf);
    
        send(s,sendbuf,nfileread,0);
    }

    recv(s,ReceiveMsg,CSIZE,0);
    int nisok=0;
    sscanf(ReceiveMsg,"%d",&nisok);
    puts(ReceiveMsg);
return 1;
   // return nisok==250?1:0;
}

void erroroutput(FILE  *fp,char *errorcode)
{
    fwrite(errorcode,strlen(errorcode),1,fp);

}
