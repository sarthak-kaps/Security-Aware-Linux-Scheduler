# Security Aware Linux Scheduler

This is an ideation/implementation of a security aware linux scheduler.<br>
The kernel i.e. the core operating system takes actions against a class of side channel attacks (like the rowhammer attack). <br>
The scheduler reduces the length of the timeslice given to the execution of the attacking process by reducing its priority, thereby reducing the system exposure to the malware. <br> 
The idea is that once the attacking process gets less time to execute on the system, the damage can be reduced and there will be enough time for user notification or for an antivirus to kick in.

The project was conducted on **Linux Kernel 4.19.160** and **Intel Core i7 9th generation** processor.

For a detailed view of the project kindly refer to the report [CS3500_project_report.pdf](https://github.com/sarthak-kaps/Security_Aware_Linux_Scheduler_CS3500/blob/main/CS3500_Project_Report.pdf).

## Details And Directory Structure

The main problem is to identify which process is an attacking process. In particular we are interested in attacking processes that do side channel attacks. <br>
In this project we explored the usage of **Hardware Performance Counters** (HPCs) to gather information of processes and use this to classify a process as attacking or not.<br>
Hardware performance counters are registers in the system that can store occurences of various microarchitectural events like -
  * cache hits
  * cache misses
  * TLB hits
  * TLB misses
  * page faults 

And many more ... <br>

The key idea is that attacking processes have a distinct footprint with respect to these HPCs.
We collected HPCs for multiple attacking and non-attacking processes. The HPCs collected for these processes can be treated as their features. <br>
The collection of HPCs was done using perf tools - https://github.com/brendangregg.<br>
We then used a simple machine learning algorithm to learn a classifier which separated the attacking processes against the non-attacking ones on select features. 
### Non Attacking Processes
For non attacking processes we referred to multiple programs from the MIBENCH collection - https://github.com/ekut-es/mibench.
The results after running perf tools on the processes is present in the folder [MIBENCH](https://github.com/sarthak-kaps/Security_Aware_Linux_Scheduler_CS3500/tree/main/MIBENCH). 

### Attacking Processes
For attacking processes we referred to - 
 * Meltdown - https://github.com/IAIK/meltdown
   * The results after running perf tools on the processes is present in the folder [Meltdown](https://github.com/sarthak-kaps/Security_Aware_Linux_Scheduler_CS3500/tree/main/Meltdown).  
 * Rowhammer - https://github.com/google/rowhammer-test
   * The results after running perf tools on the processes is present in the folder [Rowhammer](https://github.com/sarthak-kaps/Security_Aware_Linux_Scheduler_CS3500/tree/main/rowhammer).
