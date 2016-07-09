#include <boost/regex.hpp>
#include <string>
#include <iostream>
 
#include <ctype.h>
#define ENTER "\r\n\r\n"
#define MAILEND "\r\n.\r\n"
#define FROM "From"
#define  TITLE "Subject"
#define CTYPE "Content-Type:"

#define MPARTALT "alternative"
#define MPARTMIX "mixed"

#define CTE "Content-Transfer-Encoding:"
#include <fstream>
#include "base64.h"
using std::string;
using namespace boost;
typedef string::size_type SSIZE;

 
void regexdividemail(const string & maildata ,SSIZE partpos);
void readmail(const char *filename,string &strbuf);

 
void mutipart(const string &mutistring,SSIZE  ss,const char * bon);
 
void mutipart(const string &mutistring,SSIZE  ss,SSIZE last);
bool ruglardx(const string &otstr,smatch &ma,const char *chtomatch,SSIZE sbegin,SSIZE pa);
void mailtext(const string &maildata,SSIZE  ss,SSIZE last);

void getmailinfo(const string &maildata,int mailnumber,FILE *_fp);

void  charsetcon(string &titoc);

void tolowercase(string &tostr);
