# Ober Cab Services

There are N cabs, M riders and K payment servers. M, N and K are user inputs and rest of the variables are randomly generated using `rand` function in C with seed current time. Each rider arrives at a random time and books a cab (either premier or pool). All riders and payment servers are threads. One mutex lock per cab and payment server is used. One mutex lock is used to keep track of remaining riders.

## Cabs
 - Each cab is a struct.
 
 - cabState denotes the current status of the cab.
 	- 0=waitState : can accept both pool and premier customers.
 	- 1=onRidePremier : currently riding a premier passenger
 	- 2=onRidePoolOne : currently riding a single pool passenger and can accept another pool passenger (priority will be given to it in case a person books a pool cab)
 	- 3=onRidePoolTwo : filled pool cab
- cablock is a mutex lock
## Riders
Implemented using struct `rider` and for each of the M riders, a new thread is created.

 - rider's arrival time is simulated using sleep.

 - arrival time is a random number between 1 and 10.

 - wait time is a random number between 1 and 10.

 - ride time is a random number between 1 and 10.

 - if the rider is pool, it searches for a poolOne type cab and if it does not find any it searches for wait cab.

 - if he does not find any cab he tries again untill his current time exceeds his maxWaitTime(Note: maxwaittime does not include time taken by payment server).

 - after getting a cab he sleeps for ride time and then leaves the cab.

 - it then changes status ofreadytopay int in rider struct.

## Payment Servers
For each of the K servers, a new thread is created.

 - it waits for it to be activated. status to change from 0 to 1.

 - it then sleeps for 2 seconds

 - payment is then accepted. status is changed from 1 to 0.

 - it then decrements the variable total_no_of_riders(keeps track of remaining riders).

 - if no rider is left, Then code ends. 
