#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#define BUFFER_SIZE 20
char** getargs(char* str,int argc, char*argv[]){
    int count = argc+1;
    for(int i=0;str[i]!='\0';i++){
        if(str[i]==' '){
            count++;
        }
    }
    char **margv=(char**)malloc(sizeof(char*)*count);
    for(int i=0;i<count-1;i++){
        margv[i] = (char*)malloc(sizeof(char)*50);
    }
    margv[count-1] = ((char *)0); //null
    for(int i=1;i<argc;i++){
        memcpy(margv[i-1],argv[i],strlen(argv[i]));
    }
    int i =0;
    int num = argc-1;
    int ci = 0;
    while(str[i]!='\0'&&str[i]!='\n'){
        if(str[i]==' '){
            margv[num][ci] ='\0';
            num++;
            ci=0;
            i++;
        }
        margv[num][ci] = str[i];
        i++;
        ci++;
    }
    // for(int i =0;i<count;i++){
    //     printf("%s ",margv[i]);
    // }
    //printf("\n");
    return margv;
}
int
main(int argc, char *argv[])
{
    if(argc==1){
        printf("[args]|xargs [args]\n");
    }
    if(argc>=MAXARG){
        printf("two much arguments\n");
    }
    char buf[BUFFER_SIZE];
    read(0,buf,BUFFER_SIZE);
    char** m_args = getargs(buf,argc,argv);
    if(fork()==0){ //child
        if(exec(m_args[0],m_args)<0){
            printf("exec error:%s...\n",m_args[0]);
        }
    }
    wait(0);
    exit(0);
}