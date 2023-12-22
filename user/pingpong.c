#include "kernel/types.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{
  int pc[2], pp[2];
  pipe(pc);
  pipe(pp);

  if (fork() == 0){// 子进程
    close(pp[1]);
    char from_parent;
    read(pp[0],&from_parent,1);
    if(from_parent=='c'){
      printf("%d: received ping\n", getpid());
      char reply = '1';
      close(pc[0]); //write
      write(pc[1],&reply,1);
    }
  } else {
    char send_to_child = 'c';
    close(pp[0]);
    write(pp[1], &send_to_child, 1);
    close(pc[1]);
    char receive_from_child;
    read(pc[0],&receive_from_child,1);
    if(receive_from_child=='1'){
      printf("%d: received pong\n", getpid());
    }
  }

  exit(0);
}