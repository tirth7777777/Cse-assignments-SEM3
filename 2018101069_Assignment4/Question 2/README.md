# Automated Biryani Serving

There are M robot chefs, N serving tables and K students. Each of them is a thread. There is a global mutex lock on total number of students and one mutex lock for each table.
M, N and K are user inputs and rest of the variables are randomly generated using `rand` function in C with seed current time.

## Robot Chefs 
Implemented using struct `robots` and for each of the M chefs, a new thread is created.

 - preparation time of robot is simulated by `sleep(<random int between 2 and 5>)`

 - it then creates biryani.

 - Then invokes readytoserve funtion and waits for all vessels to be finished

## Serving Tables
Implemented using struct `table` and for each of the N tables, a new thread is created. 

 - it waits for it to be filled (status to change from 0 to 1)

 - it then creates random number of slots between 1 to Capacity of each vessel.

 - it then changes state in table struct 
 - it also regularly updates the slots_available variable based on its capacity. For eg: if capacity becomes 5 from 6 then slots_available will also become 5(note: it happens only when slots_available > capacity of the table).

 - it continues torun untill program exits

 - after returning it status is again changed to 0 and it can now be rediscovered by robot chefs.

## Students
For each of the K students, a new thread Studentwait is created.

 - each student arrives at a random time between 1 and 10 seconds.

 - inside the function it searches for a table with available slot till it finds one.

 - it then occupies that slot and decreases the capacity and available number of slots of that table,it also decreases number of students left.

 - whenever total_no_of_students reaches zero, Program ends
