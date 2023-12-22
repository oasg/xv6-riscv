#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char*
fmtname(char *path)
{
  //static char buf[DIRSIZ+1];
  char *p;
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}
void
find(char *path,char *filename)
{
  char buf[512], *p;
  int fd;   //文件描述符
  struct dirent de;
  struct stat st;
  if((fd = open(path, 0)) < 0){//使用open来获取文件描述符
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){ 
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    //printf("length: %d :%s : %s\n",strlen(fmtname(path)),fmtname(path),filename);
    if(strcmp(fmtname(path),filename)==0){
        printf("%s\n",path);
    }
    break;
  case T_DIR:  //是一个目录
    //printf("is a dir\n");
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path); //path copy to buf
    p = buf+strlen(buf);  //end of the path
    *p++ = '/';
    //read this dir
    while(read(fd,&de,sizeof(de))==sizeof(de)){
      if(de.inum == 0) continue;
      memmove(p,de.name,DIRSIZ); //de.name -> p
      p[DIRSIZ]=0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if(fmtname(buf)[0]!='.'){
         //printf("buf:%s\n",buf);
         find(buf,filename);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc<3){
        printf("find [path] [file name..]");
    }
    for(int i = 2;i<argc;i++){
        find(argv[1],argv[i]);
    }
    exit(1);

}
