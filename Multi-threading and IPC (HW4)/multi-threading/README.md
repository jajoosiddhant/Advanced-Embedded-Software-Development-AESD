# Advanced-Embedded-Software-Development
 
This is a simple multithreading program that shares some data between multiple threads that should all write to the same log file. It accepts a command line file name to log file entries. In order to make this happen, mutexes have been used wherever required.  
Three Different Threads have been created - a master thread and two child threads.  
Each thread prints th its PID Number, Linux Thread ID and Posix Thread ID at the start of its execution with a timestamp.  
A signal hadnler has also been implemented. Threads exit if they receive a USR1 and USR2 signal telling them to exit. A timestamp signifying the receipt of the signal and a message is written to the log file.  

**CHILD THREAD 1:**   
Child thread 1 takes an input of characters from  a text file. It then sorts all the alphabets, keeps a count of all the alphabets using an hash map and displays the number of occurences in the log file. Only the alphabets whose occurences are less than 100 are logged into the log file. While counting the number of alphabets case sensitivity is ignored.  

**CHILD THREAD 2:**  
This thread reports the CPU Utilization (from /proc/stat) every 100 ,illiseconds to the log file using a POSIX Timer.
 
The folders also consists of a **MAKEFILE** to cross-compile the module code for Beaglebone Green on UBUNTU using buildroot. Each Folder Consist of two text files - a correct working output in a log text file and a log text file when a **SIGTERM** signal is interrupted.      
  
  
