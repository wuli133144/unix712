
#include"../inc/common.h"

#define STATUS          30

/*listen start*/
int Serve_listen(const char *path){
   
    int fd;
    int err;
    int ret;
    int len;
    UN_SOCK un;
    if(strlen(path)>=sizeof(un.sun_path)){
           err=ENAMETOOLONG;
           return -1;
    }
    
    fd=Socket(AF_UNIX,SOCKET_STREAM,0)

    unlink(path);//close path count
    
    memset(&un,0,sizeof(un));
    un.sun_family=AF_UNIX;

    SCPY(un.sun_path,path)
    len=offsetof(UN_SOCK,sun_path)+strlen(path);
    Bind(fd,(SOCK *)&un,len);
    Listen(fd,LISTEN_NUM);
    return fd;

}
/*@listen end@*/

/*@accept start@*/
int Serve_accept(int listenfd,uid_t *uid )
{      
    int clientfd;
    int err,rval;
    socklen_t len;
    UN_SOCK un;
    struct stat statbuf;
    char *name;
    time_t nowtm;

     
     name=(char *)Malloc(sizeof(un.sun_path)+1);
     len=sizeof(un);
     
     clientfd=Accept(listenfd,(SOCK *)&un,len)
     
     len=len-offsetof(UN_SOCK,sun_path);
    // SCPY(name,un.sun_path);
       memcpy(name,un.sun_path,len);
       name[len]=0;
       
       Stat(name,&statbuf);//get file attrbution
       //is socket fd
       if((S_ISSOCK(statbuf.st_mode))==0){
               err_dump("name is not socket fd");
       }
        
       if((statbuf.st_mode&(S_IRWXG|S_IRWXO))||(statbuf.st_mode&(S_IRWXU))) {
             err_dump("stat error!");
        }
      tm=time(NULL)-STATUS;
      if(statbuf.st_atime<tm||
         statbuf.st_ctime<tm||
         statbuf.st_mtime<tm){
             err_dump("time happen!");
      }


      if(uid==nullptr){
         return -1;
      }
      *uid=statbuf.st_uid;
      unlink(name);
      free(name);
      return clientfd;
}
/*@accept end@*/


/*@client_connect start@*/

int client_connect(const char *name){
    int fd;
    int err;
    int rval;
    socklen_t len;
    UN_SOCK un;
    memset(&un,0,sizeof(un));
    
    if(strlen(name)>sizeof(un.sun_path)+1){
        errno=ENAMETOOLONG;
        return -1;//error
    }

    fd=Socket(AF_UNIX,SOCKET_STREAM,0);
    
    un.sun_family=AF_UNIX;
    sprintf(un.sun_path,"%s%05ld",name,);
    SCPY(name,un.sun_path);
}
/*@client_connect end end@*/



int main(){




    return 0;
}