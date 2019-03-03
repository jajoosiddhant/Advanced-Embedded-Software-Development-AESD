# Advanced-Embedded-Software-Development

**Timer Module:**  
This folder consists of a timer_module.c code to create an out of tree kernel module in order to run it on Beaglebone Green. The kernel module uses a kernel timer to periodically wakeup every 500 msec by default. Each time timetimer wakes up, a function is executed which prints to the kernel log NAME and a count of how many times the kernel has been fired (this can be viewed using the "dmesg" command on the command line). 
The kernel module also takes two parameters: a string which needs to be displayed in place of NAME and the timer count time in milliseconds. These parameters need to be specified at the time of inserting the module (example: **sudo insmod timer _module.ko name=Siddhant period=1000**).   
Using the **lsmod** command would display all the current modules.   
**sudo rmmod timer_module.ko** is used to remove the module.    
The folder also consists of a **MAKEFILE** to cross-compile the module code for Beaglebone Green on UBUNTU using buildroot.    
The Makefile generates the **.ko** file.


**Module using Linked list:**   
This folder consists of an animal_sort.c code to create an out of tree kernel module in order to run it on Beaglebone Green. The objective of this program is to create animal-type “sorting” functions in a Linux kernel module. Based on command line parameters for initialization at module insertion, it will construct and initialize 2 internal lists using the Linux kernel data structure definition, constructs and macros(e.g. lists.h). The file which is used to supply 50 animal types is animal_list. This file is used to insert animal types to be used for sorting as parameters. The list may or may not consist of duplicate animal types. The module also takes the animal name and count as parameters and displays only the filtered list using the parameters.   
The module should keep a count of all the unique animal types using a data structure from list.h. Linked list has been used in this program for this situation. The module will print three lists: one being a list consisting of duplicates, second being an alphabetically sorted list and the third being a filtered list.
 
The Filter works in such a way:  
If none filter is passed, both the linked-list are necessarily same and all the elements are displayed in filtered list meaning no filter has been passed. If count parameter is passed, all the animals with greater than filter count should be present in filtered linked list and displayed. If animal type is passed, only that animal(if present in linked-list) should appear. If both filters are passed (animal type = Frog and count = 2), Frog, only if present more than twice should be present in filtered list and should be printed.   
All the parameters i.e the array of animal types, animal name and count can be passed in the animal_list file.  

The program supports the following features:    
->Displaying all the animal types obtained as parameters including the duplicates. 
->Allocating dynamic memory to construct nodes for the linked list as required.  
->Displaying all the animal types without the duplicates in a sorted array alphabetically.  
->Returns the number of nodes in the sorted linked list.     
->The total amount of dynamic memory allocated.  
->Displaying all the animal types as filtered list based on the parameters.  
->Deallocating Dynamic memory on module exit and printing the amount of memory freed on exit.
->Returns the number of nodes in the filtered linked list.  
->Prints the amount of time to insert and remove the module using *do_timeofday* system call.   
  
**sudo insmod animal_sort.ko $(cat animal_list)** is used to insert the module.    
Using the **lsmod** command would display all the current modules. 
**sudo rmmod animal_sort.ko** is used to remove the module. 
The folder also consists of a **MAKEFILE** to cross-compile the module code for Beaglebone Green on UBUNTU using buildroot. The Makefile generates the **.ko** file.

