#include "base64.h"

const char BASE64[65]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
 
static inline bool is_base64(unsigned char c) {
return (isalnum(c) || (c == '+') || (c == '/')|| (c == '\r')|| (c == '\n'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
std::string ret;
int i = 0;
int j = 0;
unsigned char char_array_3[3];
unsigned char char_array_4[4];

while (in_len--) {
char_array_3[i++] = *(bytes_to_encode++);
if (i == 3) {
char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
char_array_4[3] = char_array_3[2] & 0x3f;

for(i = 0; (i <4) ; i++)
ret += base64_chars[char_array_4[i]];
i = 0;
}
}

if (i)
{
for(j = i; j < 3; j++)
char_array_3[j] = '\0';

char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
char_array_4[3] = char_array_3[2] & 0x3f;

for (j = 0; (j < i + 1); j++)
ret += base64_chars[char_array_4[j]];

while((i++ < 3))
ret += '=';

}

return ret;

}


std::string base64_decode(std::string const& encoded_string) {
int in_len = encoded_string.size();
int i = 0;
int j = 0;
int in_ = 0;
unsigned char char_array_4[4], char_array_3[3];
std::string ret;

while (in_len--  && ( encoded_string[in_] != '=') &&( encoded_string[in_] != '.')&& is_base64(encoded_string[in_]))
 {
  if(encoded_string[in_]=='\r'||encoded_string[in_]=='\n')
  {
  	in_++;
  	continue;
  }
	char_array_4[i++] = encoded_string[in_];
	 in_++;
if (i ==4) {
for (i = 0; i <4; i++)
char_array_4[i] = base64_chars.find(char_array_4[i]);

char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	for (i = 0; (i < 3); i++)
	{
	ret += char_array_3[i];
	}
	i = 0;
	}
 
}

if (i) {
for (j = i; j <4; j++)
char_array_4[j] = 0;

for (j = 0; j <4; j++)
char_array_4[j] = base64_chars.find(char_array_4[j]);

char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

for (j = 0; (j < i - 1); j++)
 ret += char_array_3[j];
}

return ret;
}


 
char * decode(const char *src, int src_len,int *reint)

{
	
	int i = 0, j = 0;
	char *dst;
	char base64_decode_map[256] = {
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 62, 255, 255, 255, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255,
			255, 0, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255, 255, 26, 27, 28,
			29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
			49, 50, 51, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
	dst = (char *)malloc(src_len);
	char sss[5];
	
	int pos=0,num=0;
	while(pos<src_len)
	{
		num=0;i=0;
		while(num<4&&pos<src_len)
		{
			if(src[pos]=='\r'||src[pos]=='\n')
			{
				pos++;
				continue;
			}
			sss[num]=src[pos];
			num++;
			pos++;
		}
	 
			dst[j++] = base64_decode_map[sss[0]] << 2 |

				base64_decode_map[sss[1]] >> 4;
	 
			dst[j++] = base64_decode_map[sss[1]] << 4 |
				
				base64_decode_map[sss[2]] >> 2;
	 
			dst[j++] = base64_decode_map[sss[2]] << 6 |
				
				base64_decode_map[sss[ 3]];
	 
	}
		 *reint=j;
		return dst;
		
}

void utf8togb2312(const char *utf8code,char *gb2312)
{
	int utf8len=strlen(utf8code);
	wchar_t unicode[CSIZE];
int ncon=	MultiByteToWideChar(CP_UTF8,0,utf8code,utf8len,unicode,CSIZE);
unicode[ncon]='\0';
int nlen=	WideCharToMultiByte(CP_ACP,0,unicode,wcslen(unicode),gb2312,CSIZE,NULL,NULL);
gb2312[nlen]='\0';
}


int base64(char *s,const int len,char *d)
{
char CharSet[64]={
'A','B','C','D','E','F','G','H',
'I','J','K','L','M','N','O','P',
'Q','R','S','T','U','V','W','X',
'Y','Z','a','b','c','d','e','f',
'g','h','i','j','k','l','m','n',
'o','p','q','r','s','t','u','v',
'w','x','y','z','0','1','2','3',
'4','5','6','7','8','9','+','/'};
unsigned char In[3];
unsigned char Out[4];
int cnt=0;

 int ncount=0;

for(;ncount<len;)
{
	/*
if(cnt+4>76)
{
*d++='\n';
cnt=0;
}
*/
if(len-ncount>=3)
{
In[0]=s[ncount];
In[1]=s[ncount+1];
In[2]=s[ncount+2];
Out[0]=In[0]>>2;
Out[1]=(In[0]&0x03)<<4|(In[1]&0xf0)>>4;
Out[2]=(In[1]&0x0f)<<2|(In[2]&0xc0)>>6;
Out[3]=In[2]&0x3f;
*d=CharSet[Out[0]];
*(d+1)=CharSet[Out[1]];
*(d+2)=CharSet[Out[2]];
*(d+3)=CharSet[Out[3]];
ncount+=3;
d+=4;
}
else if(len-ncount==1)
{
In[0]=s[ncount];
Out[0]=In[0]>>2;
Out[1]=(In[0]&0x03)<<4|0;
*d=CharSet[Out[0]];
*(d+1)=CharSet[Out[1]];
*(d+2)='=';
*(d+3)='=';
ncount++;
d+=4;
}
else if(len-ncount==2)
{
In[0]=s[ncount];
In[1]=s[ncount+1];
Out[0]=In[0]>>2;
Out[1]=(In[0]&0x03)<<4|(In[1]&0xf0)>>4;
Out[2]=(In[1]&0x0f)<<2|0;
*d=CharSet[Out[0]];
*(d+1)=CharSet[Out[1]];
*(d+2)=CharSet[Out[2]];
*(d+3)='=';
ncount+=2;
d+=4;
}
cnt+=4;
}
*d='\0';
return 1;
}
