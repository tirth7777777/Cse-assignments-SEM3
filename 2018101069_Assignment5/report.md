## In defs.h
* Declared function for made syscalls waitx, setpriority, getpinfo.
* Added struct proc_stat.

## In proc.c
* Added function for syscalls waitx, setpriority, getpinfo.
* Added scheduling algorithms.
* Added required functions and struct to implement queue.
* Added iniatialise function in userinit function.
* Initialised struct proc variables in allocproc.
* Changed necessary variables for schduler implementation.
* Added queue no value in printf of procdump.

## In proc.h
* Added useful variables in struct proc.

## In syscall.h
* Added declaration of sys_waitx, sys_set_priority, sys_getpinfo.
* Added them in static int array syscalls.

## In syscall.h
* Added #define for SYS_waitx, SYS_set_priority, SYS_getpinfo.

## In sysproc.h
* Added functions sys_waitx, sys_set_priority, sys_getpinfo.

## In trap.c 
* Added necesarry variable for scheduler in trap function.

## In user.h
* Added declaration of function for syscalls waitx, setpriority, getpinfo.
* Declared struct proc_stat.

## In user.S
* Added definition for calling syscall waitx, setpriority, getpinfo.

## New files added
* time.c
    * This is tester for waitx it prints time status of command.
    * write time [Command Name] to run command.

* benchmark.c
    * Run it by command benchmark.
    * Open OS by setting appropriate value of scheduler variable. Then run benchmark.
    * It shows pid of creation time as it creates them and wait time and run time when ends them.

* procstat.h
    * contains struct proc_stat.