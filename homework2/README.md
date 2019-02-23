# Advanced-Embedded-Software-Development

**FILEIO:**  
This folder consists of a C code which was used to practice and get familiarized with the file operational skills in order to implement a device driver or writing programs that require user interaction. This code was executed on command line in UBUNTU. Commands such as ltrace, strace and perf stat were used to analyze the code and its statistics. This same code was then cross-compiled using buildroot by making an overlay directory for beaglebone. The executable was then run on beaglebone and the same commands were executed as on UBUNTU to analyze the differences.

**MODIFIED FILES FOR SYSTEM CALLS AND CODE:**   
This folder consists of a systemcall.c user space C code file and all the other kernel space .c,.h and .tbl system files which were altered in order to create and run a new system call. The definition for system call in kernel space was written in sys.c. The system call copies an array of numbers into the kernel space and sorts them in descending order in the kernel space and then again copies them back to the user space. The array elements have been printed thrice - one is the initial array, second is the array after copying it in the kernel space and the third is the sorted array obtained in user space.   
The system call(defined in sys.c) supports the following features:   
->Pointer to the initial buffer and sorted buffer.    
->Validation of all input parameters.   
->Use of kmalloc(i.e assigning dynamic memory).    
->Printing Information to kernel Log using printk.(The printk data can be viewed by using the command "dmesg" on the command line)   
->All errors returning appropriate error values(defined in errno.h).   
->Sorting algorithm using O(nlogn) performance.    
->Returns the amount of time taken by the system call to execute. (using time.h)

**PROCESS ID LOG FILE AND CODE:**  
This folder consists of a C code file named processinfo.c which is the same as systemcall.c user space code in the previous folder with slight additions.   
Additions - The program displays the following information by using the appropriate system calls:   
->Current Process ID.   
->Current User ID.    
->Current Date and Time.   
In addition to this, this program was run every 10 minutes and the output was directed to a text file which is in the folder. CRONTAB was used in order to run this program every 10 minutes. 
