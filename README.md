# Security_Aware_Linux_Scheduler

This is an implementation of a security aware linux scheduler.
The kernel takes actions against a class of side channel attacks (like rowhammer attack).
The scheduler reduces the length of the timeslice of the attacking process by reducing its priority, thereby reducing the system exposure to the malware. The idea is that once the attacking process gets less time to execure on the system, the damage can be reduced and there will be enough time for user notification or for an antivirus to kick in.

For more information refer to the report [CS3500_project_report.pdf](https://github.com/sarthak-kaps/Security_Aware_Linux_Scheduler_CS3500/blob/main/CS3500_Project_Report.pdf).
