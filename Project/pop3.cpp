#include "pop3.h"
#include <fstream>
#include <ctype.h>
#define USER "USER"
#define PASS "PASS"
#define BUFSIZE 4000

extern UserOptionInfo USERMAIL;
MailInfo MyMail;
UserInfo MyUser;
using  namespace std;

void dividemail(string &data);
bool IsOK(char *);
void connandgetinfo(SOCKET s);
 

//连接pop3服务器
void pop3CONN(const char *domain,const int port)
{

    hostent * remoteip=gethostbyname(domain);
    if (NULL==remoteip)
    {
        puts("getip error");
        return;
    }
    puts(domain);
    SOCKADDR_IN saddr;
    ::memcpy(&	saddr.sin_addr,remoteip->h_addr,4);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(port);
    puts(inet_ntoa(saddr.sin_addr));
    SOCKET s=socket(AF_INET,SOCK_STREAM,0);
    int nRe=connect(s,(sockaddr*)&saddr,sizeof(saddr));
    if (nRe!=0)
    {
        puts("connect error\n");
        return;
    }
    connandgetinfo(s);

}


//会话过程 验证密码
bool negociation(SOCKET s,const char *password,const char *username)
{
    char upass[256];
    char uname[256];
    char recvbuf[512];
    char  rre[512];
    ::sprintf(upass,"%s %s\r\n",PASS,password);
    ::sprintf(uname,"%s %s\r\n",USER,username);
    int len=0;
    len=::strlen(uname);
    send(s,uname,len,0);


    recv(s,recvbuf,512,0);
    ::memset(rre,0,10);
    sscanf(recvbuf,"%s",rre);

    if (!IsOK(rre))
    {

        return false;
    }
    send(s,upass,::strlen(upass),0);
    ::memset(rre,0,10);
    recv(s,recvbuf,512,0);
    sscanf(recvbuf,"%s",rre);
    if (!IsOK(rre))
    {
        return false;
    }

    return true;
}

//发送stat 返回服务器上邮件数目
int getstat(SOCKET s)
{
    char Sendchar[CSIZE]="stat\r\n";
    send(s,Sendchar,::strlen(Sendchar),0);
    char buf[512];
    int ns=recv(s,buf,512,0);
    buf[ns]='\0';
    int num=0;

    sscanf(buf,"%s%d",Sendchar,&num);
    if(!IsOK(Sendchar))
    {
    	return 0;
	}
 
    return num;
}

//按邮件序号 接收邮件主体
bool getmail(int index,SOCKET s,FILE *fpp)
{

    char Sendchar[CSIZE], buf[BUFSIZE+1],temp[LEN];
    int maillength,la,nrr=0;

    ::sprintf(Sendchar,"retr %d\r\n",index);
    send(s,Sendchar,::strlen(Sendchar),0);


    int nRed=0	;
    char filename[256];

	
    nrr=0;
    while (1)
    {
        recv(s,buf+nrr,1,0);

        if (buf[nrr]=='\n')
        {
            break;
        }

        nrr++;

    }
	string str;
    buf[nrr]='\0';
    sscanf(buf,"%s%d",temp,&maillength);

    int ntotalrecv=0;
	char *douen=NULL; 
	
	     char file [CSIZE],chmb[CSIZE];
 
	
    while (1)
    {

        nRed=	recv(s,buf,BUFSIZE,0);
        buf[nRed]='\0';
     

        douen=strstr(buf,"\r\n.\r\n");
        if(douen!=NULL)
        {
        	*douen='\0';
        	   str+=buf;
        	  break;
		}
        str+=buf;
    	
       // if (str.find("\r\n.\r\n",str.size()-30)!=string::npos)
        {
         //   break;
        }
        ntotalrecv+=nRed;


    }
 
     getmailinfo(str, index,fpp);
  
    return 1;
}


void connandgetinfo(SOCKET s)
{
    bool Result=0;
    char buf[CSIZE];
    recv(s,buf,CSIZE,0);
    if (!IsOK(buf))
    {
        return;
    }
 
    if (!negociation(s,USERMAIL.userpass,USERMAIL.username))
    {
        puts("password error!\n");
        return;
    }
    int num=	getstat(s);
    char filename[CSIZE],chmb[CSIZE];
    
    ::sprintf(filename,"%smymail.lis",USERMAIL.savepath);
     
    FILE *fpp= fopen(filename,"w+");
    if(fpp==NULL)
    {
    	MessageBox(NULL,"文件保存失败！请设置好目录","严重错误",0);
    	return;
	}
    for (int i=1;i<=num;i++)
    { 
        if	(!getmail(i,s,fpp))
        {
            return;
        }

    }
    fclose(fpp);
    send(s,"quit\r\n",7,0);
}

bool IsOK(char * message)
{
    if (::tolower(message[1])=='o'&&::tolower(message[2])=='k' )
        return 1;
    else
        return 0;
}

 
