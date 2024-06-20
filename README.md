The CPU Scheduler will evaluate the system's resources (RAM and CPU availability) before assigning processes to either of the two CPUs according to defined rules and priorities.

Specifications
  Resource Evaluation: Before process allocation, the scheduler checks if sufficient RAM and CPU resources are available.
  RAM Allocation: The system has a total of 2048 MB of RAM. High priority processes are allocated 512 MB to ensure their uninterrupted execution. The remaining memory is available for other processes.
  CPU Allocation:
    CPU-1: Handles processes with the highest priority (priority 0) using the First-Come, First-Served (FCFS) algorithm.
    CPU-2: Manages user processes with priorities 1 (high), 2 (medium), and 3 (low). Processes are assigned based on priority, with different scheduling algorithms for each queue:
      High Priority (1): Shortest Job First (SJF)
      Medium Priority (2): Round Robin with a quantum time of 8 seconds
      Low Priority (3): Round Robin with a quantum time of 16 seconds

