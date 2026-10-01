#include "bot_state_file_codec.h"
#include <string.h>
#include <stdlib.h>
#define MAX_FIELD 65535u
static int field_ok(const char*s,size_t n){return s&&n>0&&n<=MAX_FIELD&&memchr(s,0,n)==NULL;}
static int put_field(FILE*f,const char*s){size_t n=strlen(s);return field_ok(s,n)&&fprintf(f,"%zu:",n)>=0&&fwrite(s,1,n,f)==n;}
static int put_record(FILE*f,const char*s,const char*k,const char*v){return put_field(f,s)&&put_field(f,k)&&put_field(f,v)&&fputc('\n',f)!=EOF;}
static int read_field(FILE*f,char*out,size_t cap){unsigned long n=0;int ch;if(!out||cap<2)return 0;while((ch=fgetc(f))>='0'&&ch<='9'){n=n*10u+(unsigned)(ch-'0');if(n>MAX_FIELD)return 0;}if(ch!=':'||n==0||n+1>cap)return 0;if(fread(out,1,n,f)!=n)return 0;out[n]=0;return 1;}
static int load_record(FILE*f,bot_state*s){char a[MAX_FIELD+1],b[MAX_FIELD+1],c[MAX_FIELD+1];int ch;if(!read_field(f,a,sizeof(a))||!read_field(f,b,sizeof(b))||!read_field(f,c,sizeof(c)))return 0;ch=fgetc(f);if(ch!='\n'||!bot_state_set(s,a,b,c))return 0;return 1;}
static int emit(const char*s,const char*k,const char*v,void*ctx){return put_record((FILE*)ctx,s,k,v);}
int bot_state_file_write(FILE*f,const bot_state*s){if(!f||!s)return 0;if(fputs("BSTATE1\n",f)==EOF)return 0;return bot_state_foreach(s,emit,f);}
int bot_state_file_read(FILE*f,bot_state*s){char h[9];int ch;if(!f||!s)return 0;if(fread(h,1,8,f)!=8||memcmp(h,"BSTATE1\n",8)!=0)return 0;bot_state_clear(s);while((ch=fgetc(f))!=EOF){ungetc(ch,f);if(!load_record(f,s)){bot_state_clear(s);return 0;}}return 1;}
