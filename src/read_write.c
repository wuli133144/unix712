#include"../inc/common.h"



int main(){

   
   char buf[BUFSIZE];
   sprintf(buf,"%s","kakakkaka\n");
   
   int fd;
   fd=open("1.txt","r+");
   Write_s(fd,buf,sizeof(buf));
   bzero(buf,BUFSIZE);
   Read_s(fd,buf,sizeof(buf));
   fputs(stdout,buf);
    return  0;
}