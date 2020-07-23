#include <stc89c51.h>

/****小小调度器开始**********************************************/
#define MAXTASKS 3
volatile unsigned short timers[MAXTASKS];
#define _SS static unsigned char _lc=0; switch(_lc){default: 
#define _EE ;}; _lc=0; return 65535;

#define WaitX(tickets)  do { _lc=(__LINE__%255)+1; return (tickets) ;case (__LINE__%255)+1:;} while(0);
#define WaitUntil(A)    do { while(!(A)) WaitX(1);} while(0);
#define WaitUtilT(A,TimeOut)  do { static unsigned short _count=(TimeOut); do { WaitX(1); _count--; } while((!(A))&&(_count>0));} while(0);

#define RunTask(TaskName,TaskID)  do { if (timers[TaskID]==0) { unsigned short d=TaskName(); while(timers[TaskID]!=d) timers[TaskID]=d;} }  while(0); 
#define RunTaskA(TaskName,TaskID) do { if (timers[TaskID]==0) {unsigned short d=TaskName(); while(timers[TaskID]!=d) timers[TaskID]=d;   continue;} }while(0);   //前面的任务优先保证执行

#define CallSub(SubTaskName) do {unsigned short currdt; _lc=(__LINE__%255)+1; return 0; case (__LINE__%255)+1:  currdt=SubTaskName(); if(currdt!=65535) return currdt;} while(0);
#define InitTasks() do {unsigned char i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; } while(0);
#define UpdateTimers() do{unsigned char i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=65535)) timers[i-1]--;}} while(0);

#define SEM unsigned int 
//初始化信号量
#define InitSem(sem) do{sem=0;}while(0);
//等待信号量
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);
//发送信号量
#define SendSem(sem)  do {sem=0;} while(0);

/*****小小调度器结束*******************************************************/


sbit LED1 = P2^1;
sbit LED2 = P2^2;

sbit LED0 = P2^5;

unsigned short task0(){
_SS
  while(1){
   WaitX(50);
   LED0=!LED0;   
  }
_EE
}

unsigned short  task1(){
_SS
  while(1){
   WaitX(100);
   LED1=!LED1;   
  }
_EE
}

unsigned short task2(){
_SS
  while(1){
   WaitX(100);
   LED2=!LED2;   
  }
_EE
}

void InitT0()
{
        TMOD = 0x21;
        IE |= 0x82;  // 12t
        TL0=0Xff;
        TH0=0XDB;
        TR0 = 1;
}

void INTT0(void) interrupt 1 using 1
{
    TL0=0Xff;    //10ms 重装
    TH0=0XDB;//b7;    

    UpdateTimers();
}




void main()
{
        InitT0();
        InitTasks(); //初始化任务，实际上是给timers清零
        while(1){
//           RunTask(task0,0);
                RunTaskA(task1,1);//任务1具有比任务2高的运行权限                   
           RunTaskA(task2,2);//任务2具有低的运行权限                   
      }
}
