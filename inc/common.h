


#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <signal.h>
#include<pthread.h>
#include<errno.h>
#include<pwd.h>
#include<grp.h>

#include<sys/socket.h>
#include<sys/un.h>


#include<stddef.h>


#define BUFSIZE              1024
#define NR_PIPE              2

#define  nullptr             0

#define LISTEN_NUM           10

typedef struct sockaddr_un   UN_SOCK;
typedef struct sockaddr_in   IN_SOCK;
typedef struct sockaddr      SOCK;



#define   PARA    (int argc,char **argv) 

#define err_exit(e) \
do{                 \
     perror(e);     \
     exit(0);       \
}while(0)
  
#define err_dump(err) do{                                 \
             fprintf(stderr,"%s %s",strerror(errno),err); \
             exit(0);                                     \
}while(0)


#ifdef  __SCP__N
   #define SCPYN(a, b) strncpy(a, b, sizeof(a))
#else
    #define SCPY(a,b)   strcpy(a,b)
#endif


#define EVER ;;

/*@getpwuid get user info@*/
struct  passwd *Getpwuid(int uid){
    struct passwd *pw_st=NULL;
    if((pw_st=getpwuid(uid))==NULL){
        err_exit("getpwuid");
    } 
    return pw_st;
}
/*@getpwuid end@*/


/*@skip start@*/
//find position 
static char *skip(char *start,char *op){
            while(*start&&strcpy(start,op)!=0){
                      start++;
            }
            if(*start!=0)
               start++;
  return start;
}
/*@skip end@*/

/*@getpwname get user info by username@*/
struct passwd *Getpwnam(const char *name){
     struct passwd *pw_st=NULL;
    if((pw_st=getpwnam(name))==NULL){
        err_exit("getpwuid");
    } 
    return pw_st;
}

/*@getpwname end@*/

/*@getpwent start @*/
struct passwd *Getpwent(){
     struct passwd *pw_st=NULL;
    if((pw_st=getpwent())==NULL){
        err_exit("getpwent");
    } 
    return pw_st;
}

/*@getpwent end@*/


/*@getprent start*/

 struct group * Getgrent(){
     struct group *pw_st=NULL;
    if((pw_st=getgrent())==NULL){
        err_exit("getprent");
    } 
    return pw_st;
 }


/*@getprent end@*/


/*@Fork ()start@*/

pid_t Fork(){
    int pid;
    if((pid=fork())<0){
        err_dump("fork error!");
    }
    return pid;
}


/**@fork end@*/

/*@close start@*/
int Close(int fd){
    int ret;
    if((ret=close(fd))!=0){
        err_dump("close");
    }
    return ret;
}
/*@close end@*/


/*@dup2_stdin start@*/


int dup2_stdin(int fd){
    if(fd!=STDIN_FILENO){

        if(dup2(fd,STDIN_FILENO)!=STDIN_FILENO){
            err_dump("dup2 stdin_fileno");
        }
        close(fd);
    }
    return 0;
}

/*@dup2_stdin end@*/


/*@dup2_stdout@*/
int dup2_stdout(int fd){
    if(fd==STDOUT_FILENO){
        if(dup2(fd,STDOUT_FILENO)!=STDOUT_FILENO){
            err_dump("dup2 stdout ");
        }
        close(fd);
    }
    return 0;
}


/*@dup2_stdout@*/



/*@pipe() start@*/
int Pipe(int fd[2]){
     int ret;
     if((ret=pipe(fd))==-1){
         err_dump("pipe error");
     }
     return ret;
}

/*@pipe()end@*/

/*@Socket start@*/

int Socket(int domain,int type,int prot){
    int ret;
    if((ret=socket(domain,type,prot))<0){
        err_dump("socket error!");
    }
    return ret;
}


/*@socket end@*/
/*@connectg start@*/
int Connect(int socketfd,SOCK*addr,socklen_t len){
    int ret;
    if((ret=connect(socketfd,addr,len))<0){
        err_dump("connect error");
    }
    return ret;
}
/*@connectg end@*/


/*@bind start@*/

int Bind(int socketfd,const SOCK *addr,socklen_t len){
    int rval;
    if((rval=bind(socketfd,addr,len))==-1){
        err_dump("bind error");
    }
    return rval;
}

/*@bing end@*/

/*@Listen start@*/
int Listen(int fd,int backlog){
    int rval;
    if((rval=listen(fd,backlog))<0){
           err_dump("listen error");
    }
    return rval;
}
/*@Listen end@*/


/*@Ser_accept Start@*/

int Ser_accept(int listfd,struct sockaddr * addr,socklen_t len){
          int rval;
          if((rval=accept(listfd,addr,len))<0){
              err_dump("accept");
          }
          return rval;
}
/*@ser_accept end@*/


/*@Accept start@*/
int Accept(int listenfd,struct sockaddr *addr,socklen_t len){
      int rval;
      if((rval=accept(listenfd,addr,len))<0){
          err_dump("accept error!");
      }
      return rval;
}

/*@Accept end@*/


/*@memory operation@*/

void  *Malloc(size_t sz){
  void *prval=nullptr;
  if((prval=malloc(sz))==nullptr){
      err_dump("malloc error!");
  } 
  return prval;
}

/*@memory operation@*/


/*@file operation start@*/
 
 int Stat(const char *filename,struct stat *st){
     int rval;
     if((rval=stat(filename,st))<0){
         err_dump("stat file failed");
     }
     return rval;
 }

/*@read_s start@*/
 size_t Read_s(int fd,void *buf,ssize_t sz){
     size_t nleft;
     ssize_t nread;
     nleft=sz;
     
     while(nleft>0){
     
         if((nread=read(fd,buf,nleft))<0){
             if(nleft==0){
                 return -1;
             }else{
                 break;
             }
         }else if(nread==0){
             break;//eof
         }else{
               nleft-=nread;
               buf+=nread;
         }
     }
      return sz-nleft;
 }


/*@read end@*/

/*@write start@*/

   size_t Write_s(int fd,void *buf,ssize_t sz){
            size_t nwrite;
            ssize_t nleft;
            nleft=sz;
            while(nleft>0){

                  if((nwrite=write(fd,buf,nleft))<0){
                       if(nleft==0){
                           return -1;
                       }else
                       break;
                  }else if(nwrite ==0){
                          break;//eof
                  }else{
                        nleft-=nwrite;
                        buf+=nwrite;
                  }
            }
       return sz-nleft;
   }


/*@write end@*/


/*@file operation end@*/

#include<time.h>

/*@time operation file@*/

 int Is_intime(stat st,int sec){
       time_t stat;
       stat=time(nullptr)-sec;
       if(st.st_aatime<stt||
          st.st_ctime<stat||
          st.st_mtime<stat){
             err_dump("time out");
         }
         return 0;
 } 
/*@time operation end@*/