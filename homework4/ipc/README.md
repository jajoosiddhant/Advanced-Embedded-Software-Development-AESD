# Advanced-Embedded-Software-Development
 
Each Folder Consists of a user space example of IPC bidirectionally communicating messages between two processes or two separate executables using **Pipes, Sockets, Shared Memory, Posix Queues.**It accepts a command line file name to log file entries.   
The processes send messages using **structures** consisting of two types of messages:  
->String and its length.  
->A command to show the status of an LED. (Actual LED control is not done)   
These strings and command were sent randomly from an array using a random function. The case 9 and 10 represents LED OFF and LED ON.  
Initially each code in each folder prints its PID and the IPC method with a timestamp being used to a log file. Then, a tag indicating whether it is sending or receiving along with the Message contents.  
Eg : [Timestamp]	Sending/Receiving:	<Message Contents>   
In addition to this, a signal handler has been implemented. Upon the receipt of a SIGTERM Signal, the processes display the signal that has interrupted and cleans and terminates the process accordingly. This is also reported in the log file.  
Mutexes have also been used as and when required in the programs.

The folders also consists of a **MAKEFILE** to cross-compile the module code for Beaglebone Green on UBUNTU using buildroot. Each Folder Consist of two text files - a correct working output in a log text file and a log text file when a **SIGTERM** signal is interrupted.      
  
  
