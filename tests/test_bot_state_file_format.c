#include <assert.h>
#include <string.h>
static int valid_field(const char*s,size_t n){return s&&n>0&&n<=65535&&memchr(s,'\0',n)==NULL;}
int main(void){assert(valid_field("user:alice",10));assert(valid_field("key",3));assert(!valid_field("",0));assert(!valid_field("x",65536));return 0;}