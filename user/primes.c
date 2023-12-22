#include "kernel/types.h"
#include "user/user.h"
void print_data(int data[]){
    for(int i =0;i<35;i++){
        printf("%d ",data[i]);
    }
    printf("\n");
}
void do_prime(int *data){
    //print_data(data);
    //return;
    int base = -1;
    int flag = 1;
    int ind=2;
    while(flag&&ind<35){
        if(data[ind]>1){
            base = data[ind];
            flag = 0;
        }
        ++ind;
    }
    if(base == -1) return;
    printf("prime %d\n",base);  //print base
    for(int i = base;i<35;i++){  //
        if(data[i]%base == 0){
            data[i] = 0;
        }
    }
    int p[2];
    pipe(p);
    if(fork() == 0){  //child
        close(p[1]);
        int * r_data=(int*)malloc(sizeof(int)*35);
        read(p[0],r_data,35);
        do_prime(data);
     }else{   
        close(p[0]);     //present
        write(p[1],data,35);
        wait(0); 
    }
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  int data[35];
  for(int i =0 ;i < 35;i++){
    data[i] = i;
  }
  data[1] = 0;
  if(fork() == 0){  //child
    close(p[1]);
    int * r_data=(int*)malloc(sizeof(int)*35);
    read(p[0],r_data,35);
    do_prime(data);
  }else{   
    close(p[0]);     //present
    write(p[1],data,35);
    wait(0); 
  }
  exit(0);
}
