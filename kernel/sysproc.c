#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 buf;
  int size;
  uint64 bitadds;
  argaddr(0,&buf);
  argint(1,&size);
  argaddr(2,&bitadds);  //bits's addree
  //get the access bits
  struct  proc * p = myproc();
  //copy bits to kernal space
  uint32 bits;
  copyin(p->pagetable,(char*)&bits,bitadds,4);
  //max scan 64 pages
  for(int i = 0;i<size&&i<32;i++){
    pte_t * pte = walk(p->pagetable,buf+i*PGSIZE,0);
    if(1<<i & bits){
      *pte = *pte&(~PTE_A);  //clear bits when on sets
      bits = bits&~(1<<i);
    }
    if(*pte & PTE_A){
      bits = bits|(1<<i);
    }
  }
  copyout(p->pagetable,bitadds,(char* )&bits,4);
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
