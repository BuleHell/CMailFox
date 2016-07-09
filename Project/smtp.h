 
 #include "rsrc.h"
#include "base64.h"
#ifndef _SMTP_H_
#define _SMTP_H_

const char MIME[]={ "From: %s <%s>\r\nTo: <%s>\r\nSubject: %s\r\nMIME-Version: 1.0\r\nContent-Type: text/plain;\r\ncharset=\"8bit\"\r\n\r\n%s"
};


void init();
void smtpCONN(char *filetosend,char *mailsendto);

#endif
