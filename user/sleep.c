#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc == 2){
        sleep(atoi(argv[1]));
        exit(1);
    }else{
        printf("sleep [nums]\n");
    }
  exit(0);
}