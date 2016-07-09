#include "regexpress.h"

#define BUFSIZE 4000
#define MTYPE 6
#define MBOUN 3
#define SEOF string::npos
extern UserOptionInfo USERMAIL;
bool  attch(const string &maildata,SSIZE denterpos );

void regexdividemail(const string & maildata ,SSIZE partpos)
{

    const char MIMEH[60]=
    {
        "^Content-Type:\\s*(.*?)/(.*?);\\s*(.*?)=\"(.*?)\""
    };


    SSIZE denterpos= maildata.find(ENTER);

    if (attch(maildata,denterpos))
    {
        return;
    }

    smatch what,swhat;




    string::const_iterator start = maildata.begin();
    string::const_iterator end = maildata.begin()+denterpos;

    regex express(MIMEH,boost::regex::icase);
    bool hr=regex_search(start, end, what, express );

    string maintype= what[1].str();
    tolowercase(maintype);
    string subtype=what[2].str();
    tolowercase(subtype);
    string bon=what[3].str();

    string name=what[4].str();

    printf("%s %s %s %s\r\n",maintype.c_str(),subtype.c_str(),bon.c_str(),name.c_str());
    if (hr)
    {

        if (MPARTALT==subtype)
        {
            mutipart(maildata,denterpos,name.c_str());

        }
        else if (MPARTMIX==subtype||"mutilpart"==maintype)
        {

            SSIZE nstart= maildata.find(name,denterpos);

            string::const_iterator sstart = maildata.begin()+nstart;
            string::const_iterator eend = maildata.end();
            const char ALT[]=	"\r\n\\s*(.*?)\\s*-*%s";
            char alt[512];
            sprintf (alt,ALT,name.c_str());

            regex exp (alt,boost::regex::icase);
            while ( regex_search(sstart, eend, swhat, exp ) )
            {

                regexdividemail(swhat[1].str() ,0);

                sstart = swhat[0].second;
            }
        }
        else if (maintype=="text")
        {
            mailtext(maildata,0,0);
        }

    }
    else
    {

        mailtext(maildata,0,0);

    }

}

void readmail(const char *filename,string &strbuf)
{

    FILE *fp=fopen(filename,"rb");
    if (fp==NULL)
    {
        return;
    }

    char buf[BUFSIZE+1];
    size_t nRead=0;
    strbuf="";
    while (1)
    {
        nRead=	fread(buf,1,BUFSIZE,fp);

        if (nRead<=0)
        {
            break;
        }
        buf[nRead]='\0';
        strbuf+=buf;
    }

//std::cout<<strbuf<<std::endl;
    fclose(fp);
}


void mutipart(const string &mutistring,SSIZE  ss,const char * bon)
{

    const char ALT[]=	"%s\\s*(.*?)\\s*-*%s";
    char alt[512];
    sprintf (alt,ALT,bon,bon);

    smatch what;

    regex express(alt,boost::regex::icase);
    string::const_iterator start = mutistring.begin()+ss ;
    string::const_iterator end = mutistring.end();
    bool hr= regex_search(start, end, what, express );
    if (!hr)
    {
        return;


    }


    string str=what[1].str();

    mailtext(str,0,0);

}


bool ruglardx(const string &otstr,smatch &ma,const char *chtomatch,SSIZE sbegin,SSIZE  pa)
{


    regex express(chtomatch,boost::regex::icase);
    string::const_iterator start = otstr.begin() ;
    string::const_iterator end = otstr.begin()+sbegin+4;
    return regex_search(start, end, ma, express );
}

void mailtext(const  string &maildata,SSIZE  ss,SSIZE last)
{

    static const char MATCH[4][128] =
    {
        "^Content-Transfer-Encoding:\\s*(.*?)\\s*\r\n",
        "charset=\\s*\"(.*?)\"",
        "^Content-Type:\\s*(.*?)\r\n"

    };
    SSIZE denterpos= maildata.find(ENTER);
    if (denterpos==SEOF)
    {
        return;
    }
    puts(maildata.c_str());
    smatch what;
    string typeinfo[5];
    for (int i=0;i<3;i++)
    {



        regex express(MATCH[i],boost::regex::icase);
        string::const_iterator start = maildata.begin() ;
        string::const_iterator end = maildata.begin()+denterpos+4;
        bool hr =regex_search(start, end, what, express );

        if (hr)
        {
            typeinfo[i]=what[1].str();
        }


    }

    string decodestr;
    printf("content type %s\r\n",typeinfo[0].c_str());

    if (typeinfo[0]=="base64")
    {
        decodestr= base64_decode( maildata.substr(denterpos+4) );
        puts(" i found base64");

    }
    else if (typeinfo[0]=="quoted-printable")
    {
        decodestr=maildata.substr(denterpos+4);

    }
    else
    {

        decodestr=maildata.substr(denterpos+4);


    }
    char filename[256];
    sprintf(filename,"%s%04d.mmb",USERMAIL.savepath,USERMAIL.mailnumber );
    FILE *fp=fopen(filename,"wb");
    if (fp==NULL)
    {
        return;
    }
    fwrite(decodestr.c_str(),1,decodestr.size(),fp);
    fclose(fp);
}


void getmailinfo(const string &maildata,int mailnumber,FILE *_fp)
{
    const char MIMEH[4][50]=
    {
        "^From:\\s*(.*?)\\s*\r\n"  ,
        "^Subject:\\s*(.*?)\\s*\r\n",
        "^To:\\s*(.*?)\\s*\r\n",
        "^Date:\\s*(.*?)\\s*\r\n"

    };

    SSIZE partpos  = maildata.find(ENTER);
    string infomation[4];
    string::const_iterator start = maildata.begin();
    string::const_iterator end = maildata.begin()+partpos+4;
    smatch what;
    for (int o=0;o<4;o++)
    {
        regex express(MIMEH[o],boost::regex::icase);
        bool hr=regex_search(start, end, what, express );
        if (hr)
        {

            infomation[o]=what[1].str();
            charsetcon(infomation[o]);
        }


    }

    fprintf(_fp,"_mmb_from=%s_mmb_from= _mmb_subject=%s_mmb_subject=  _mmb_to=%s_mmb_to= _mmb_data=%s_mmb_data= _mmn_num=%04d_mmn_num= \n",
            infomation[0].c_str(),	infomation[1].c_str(),	infomation[2].c_str(),	infomation[3].c_str(),mailnumber
           );

    USERMAIL.mailnumber=mailnumber;
    regexdividemail(maildata,0);

}

void  charsetcon(string &titoc)
{

    char MIMEH[40]="=\\?(.*?)\\?(.)\\?(.*?)\\?";

    smatch what;
    string::const_iterator start = titoc.begin();
    string::const_iterator end = titoc.end();
    regex express(MIMEH,boost::regex::icase);
    bool hr=regex_search(start,end, what, express );

    if (hr)
    {

        char chcode=tolower( what[2].str()[0]);

        if (chcode=='b')
        {
            titoc=	base64_decode(what[3].str());


            char aca[512];
            if (what[1].str()=="utf-8")
            {
                utf8togb2312(titoc.c_str(),aca);
                titoc=aca;
            }
        }
        else if (chcode=='p')
        {
            titoc=what[3].str();
        }
    }


}

void tolowercase(string &tostr)
{
    for (int i=0;i<tostr.size();i++)
    {
        tostr[i]=tolower(tostr[i]);
    }
}

//检查是否有附件
bool  attch(const  string &maildata,SSIZE denterpos )
{

    const char attchment[60]= "^Content-Disposition:\\s*attachment;\\s*filename=\"(.*?)\"";
    smatch what;

    bool battch= ruglardx(maildata,what,attchment,denterpos,0);

    string filenamen ;
    if (battch)
    {

        filenamen=what[1].str();
    }
    else
    {
        return 0;
    }


    char filename[512];

    charsetcon(filenamen);


    sprintf(filename,"%s%04d%s",USERMAIL.savepath,USERMAIL.mailnumber,filenamen.c_str());
    //	puts(maildata.c_str()+denterpos+4);
    FILE *ffp=fopen(filename,"wb+");
    if (ffp==NULL)
    {
        puts("cannot openfile");
        return 0;
    }
    smatch ms;
    char cte[50]=    "^Content-Transfer-Encoding:\\s*base64\\s*\r\n";

    regex express(cte,boost::regex::icase);
    string::const_iterator start = maildata.begin() ;
    string::const_iterator end = maildata.end();
    bool hr =regex_search(start, end, ms, express );

    if (hr )
    {
        int  reint;
        puts("附件");
        char *ff= decode(maildata.c_str()+denterpos+4, maildata.size()-4-denterpos,&reint);
        fwrite(ff,1,reint,ffp);
        free(ff);
    }
    else
    {
        fwrite(maildata.c_str()+denterpos+4,1,maildata.size()-4-denterpos,ffp);
    }
    fclose(ffp);
    return 1;
}
