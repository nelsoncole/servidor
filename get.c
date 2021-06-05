#include "server.h"


#define DEL1 "GET /"
#define DEL2 "\t "

#define DEL3 "Accept: "

char header1[256] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: \0";
char header2[256] = "HTTP/1.1 200 OK\nContent-Type: text/css\nContent-Length: \0";
char header3[256] = "HTTP/1.1 200 OK\nContent-Type: application/javascript\nContent-Length: \0";
char header4[256] = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: \0";

// char *__cdecl itoa(int _Val,char *_DstBuf,int _Radix)

int motor(char *s1, const char *s2, int len)
{
        char *us1 = ( char *)s1;
        char _DstBuf[256];
        memset(_DstBuf, 0, sizeof(_DstBuf));

        strcpy(us1, s2);

        itoa(len , _DstBuf, 10);

        strcat(us1, _DstBuf);
        strcat(us1, "\n\n");

        return  strlen(us1);

}

char *strdel(const char *s1, const char *s2)
{
        char *us1 = (char *)s1;

        int count = strlen(s2);



        while(*us1)
        {
                if( strncmp(us1, s2, count) == 0)
                {
                        return us1 + count;
                }
                us1 ++;
        }

        return us1;
}


int get_type(const char *s1, const char *s2)
{
        char *us1 = (char *)s1;

        us1 = strdel(us1, s2);

        char *tmp = (char *) strchr(us1, ',');

        if(tmp != 0)
        {
                *tmp = '\0';
        }

        if( strcmp(us1, "text/html") == 0) return 1;

        if( strcmp(us1, "text/css") == 0) return 2;

        if( strcmp(us1, "application/javascript") == 0) return 3;

        if( strcmp(us1, "image/png") == 0) return 4;

        return 0;
}


int get( void *buf, void *receive, int count)
{
        int of;
        int type = 0;
        char *h = NULL;
        char pathname[256];
        memset(pathname, 0, 256);

        char _rcv[512];
        char *rcv = _rcv;

        memcpy(rcv, receive, strlen(receive));
        for( int i=0; i < count; i ++ )
        {
                putchar(rcv[i]);
        }

        putchar('\n');

        rcv = strtok(rcv, DEL1);

        if(*rcv == 'H') {
                strcpy(pathname, "index.html");
        } else strcpy(pathname, rcv);

        FILE *f = fopen(pathname, "r+b");
        if(!f) {
                printf("\t[-] fopen(\"%s\", \"r+b\") falhou!\n\n",pathname);
                return 0;
	}

        fseek(f,0, SEEK_END);
        int f_size = ftell(f);
        rewind(f);

        type = get_type(receive, DEL3);

        switch(type){
                case 1:
                        h = header1;
                break;
                case 2:
                        h = header2;
                break;
                case 3:
                        h = header3;
                break;
                case 4:
                        h = header4;
                break;
                default:
                        h = header1;
                break;
        }

        of = motor(buf, h, f_size);

        int r = fread(buf + of, 1, f_size, f);
        fclose(f);
        return r + of;
}

