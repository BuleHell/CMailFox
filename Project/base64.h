#include <string.h>
#include <stdio.h>
#include <string>
 
#define LEN 128
#define CSIZE 512
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"shlwapi.lib")
#ifndef _BASE64_H_
#define _BASE64_H_
struct UserOptionInfo
{
	char username[CSIZE];
	char userpass[CSIZE];
	char e64uname[CSIZE];
	char e64upass[CSIZE];
	char usermailaddr[CSIZE];
	char smtpserver[CSIZE];
	char pop3server[CSIZE];
	char savepath[CSIZE];	
	int pop3port,smtpport,mailnumber;
	char ExePath[CSIZE];
};


void translatebase64(char *ori,char *des);
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) ;
std::string base64_decode(std::string const& encoded_string);
void erroroutput(FILE  *fp,char *errorcode);
char * decode(const char *src, int src_len,int *reint);
int base64(char *s,const int len,char *d);
void utf8togb2312(const char *utf8code,char *gb2312);
#endif
