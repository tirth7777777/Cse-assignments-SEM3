Task 1:

<1>:System call Waitx:
To add System Call we have to add lines in these 5 files
1)User.h
2)USYS.s
3)Sysproc.h
4)Syscall.c
5)Syscall.c
Also we have add function in proc.c and change rtime in trap.c

<2>:System Call Getpinfo
We need add function in proc.c and add ticks and numrun in trap.c


Task 2:

<1>:FCFS:
We implement FCFS in scheduler function and we check if Scheduler is FCFS or not by using #ifdef.
In FCFS we only use a for loop to get earliest process and execute process

<2>:PBS:
We implement PBS in scheduler function and we check if Scheduler is PBS or not by using #ifdef.
In PBS we only use a for loop to get highest priority process and execute process.In case of same process we use Round Robin Scheduler

<3>:MLFQ:
we implement MLFQ in scheduler function and we check if Scheduler is MLFQ or not by using #ifdef
In MLFQ we use 5 queues and we change queues after appropriate wait time and after time slice.We do that in function check in which argument is cpu .

