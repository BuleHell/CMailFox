 
 #include <stdio.h>
#include "base64.h"
#include "regexpress.h"

#include "base64.h"
#ifndef _POP3_H_
#define _POP3_H_


struct UserInfo
{
	char uname[LEN];
	char upass[LEN];
};
typedef std::string str1;
struct MailInfo
{

str1 titel; 
str1 sdata;
str1 from;
str1 DATA;

};

 
void pop3CONN(const   char *domain,const int port);
bool negociation(SOCKET s,const char *password,const char *username);
int getstat(SOCKET s);
bool getmail(int index,SOCKET s,FILE *fpp);
void getmultipart(str1 &strdata);
#endif
