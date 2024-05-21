#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100
#define MAX_QUEUE_SIZE 100

typedef struct {
    char id[5];
    int arrival_time;
    int priority;
    int cpu_burst;
    int remaining_time;
} Process;

typedef struct {
    Process processes[MAX_QUEUE_SIZE];
    int size;
    char scheduling_type[10];
} Queue;

Process process_list[MAX_PROCESSES];
Queue queues[4];  
int process_count = 0;
FILE *output_file;
int ram = 512;  

void read_input_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    while (fscanf(file, "%[^,],%d,%d,%d,%d,%d\n", 
                  process_list[process_count].id,
                  &process_list[process_count].arrival_time,
                  &process_list[process_count].priority,
                  &process_list[process_count].cpu_burst,
                  &process_list[process_count].remaining_time,
                  &process_list[process_count].remaining_time) != EOF) {
        process_list[process_count].remaining_time = process_list[process_count].cpu_burst;
        process_count++;
    }
    fclose(file);
}

void add_to_queue(Queue *queue, Process process) {
    queue->processes[queue->size++] = process;
    fprintf(output_file, "Process %s is queued to be assigned to CPU-%d.\n", process.id, process.priority == 0 ? 1 : 2);
}

void sort_by_arrival_time(Queue *queue) {
    for (int i = 0; i < queue->size - 1; i++) {
        for (int j = 0; j < queue->size - i - 1; j++) {
            if (queue->processes[j].arrival_time > queue->processes[j+1].arrival_time) {
                Process temp = queue->processes[j];
                queue->processes[j] = queue->processes[j+1];
                queue->processes[j+1] = temp;
            }
        }
    }
}

void sort_by_cpu_burst(Queue *queue) {
    for (int i = 0; i < queue->size - 1; i++) {
        for (int j = 0; j < queue->size - i - 1; j++) {
            if (queue->processes[j].cpu_burst > queue->processes[j+1].cpu_burst) {
                Process temp = queue->processes[j];
                queue->processes[j] = queue->processes[j+1];
                queue->processes[j+1] = temp;
            }
        }
    }
}

void fcfs_schedule(Queue *queue) {
    sort_by_arrival_time(queue);
}

void sjf_schedule(Queue *queue) {
    sort_by_cpu_burst(queue);
}

void rr_schedule(Queue *queue, int quantum) {
    int time = 0;
    while (queue->size > 0) {
        Process current = queue->processes[0];
        fprintf(output_file, "Process %s assigned to CPU-2. Starting execution using %d MB of RAM.\n", current.id, ram);
        printf("%s-", current.id);
        if (current.remaining_time <= quantum) {
            time += current.remaining_time;
            current.remaining_time = 0;
            for (int i = 0; i < queue->size - 1; i++) {
                queue->processes[i] = queue->processes[i+1];
            }
            queue->size--;
            fprintf(output_file, "Process %s has completed execution and terminated.\n", current.id);
            ram -= 10; // For demonstration, decrease RAM by 10 MB after each process
            fprintf(output_file, "RAM After: %d MB\n", ram);
        } else {
            current.remaining_time -= quantum;
            time += quantum;
            for (int i = 0; i < queue->size - 1; i++) {
                queue->processes[i] = queue->processes[i+1];
            }
            queue->processes[queue->size - 1] = current;
            fprintf(output_file, "Process %s is queued to be assigned to CPU-2 again.\n", current.id);
        }
    }
}

void assign_queues() {
    for (int i = 0; i < process_count; i++) {
        Process p = process_list[i];
        if (p.priority == 0) {
            add_to_queue(&queues[0], p);
            strcpy(queues[0].scheduling_type, "FCFS");
        } else if (p.priority == 1) {
            add_to_queue(&queues[1], p);
            strcpy(queues[1].scheduling_type, "SJF");
        } else if (p.priority == 2) {
            add_to_queue(&queues[2], p);
            strcpy(queues[2].scheduling_type, "RR-q8");
        } else if (p.priority == 3) {
            add_to_queue(&queues[3], p);
            strcpy(queues[3].scheduling_type, "RR-q16");
        }
    }
}

void print_schedule() {
    printf("CPU-1 que1(priority-0)(FCFS):");
    fcfs_schedule(&queues[0]);
    for (int i = 0; i < queues[0].size; i++) {
        printf("%s-", queues[0].processes[i].id);
        fprintf(output_file, "Process %s assigned to CPU-1. RAM Before: %d MB\n", queues[0].processes[i].id, ram);
        fprintf(output_file, "Process %s has completed execution and terminated.\n", queues[0].processes[i].id);
        ram -= 10;  // For demonstration, decrease RAM by 10 MB after each process
        fprintf(output_file, "RAM After: %d MB\n", ram);
    }
    printf("\n");

    printf("CPU-2 que2(priority-1)(SJF):");
    sjf_schedule(&queues[1]);
    for (int i = 0; i < queues[1].size; i++) {
        printf("%s-", queues[1].processes[i].id);
        fprintf(output_file, "Process %s assigned to CPU-2. RAM Before: %d MB\n", queues[1].processes[i].id, ram);
        fprintf(output_file, "Process %s has completed execution and terminated.\n", queues[1].processes[i].id);
        ram -= 10;  // For demonstration, decrease RAM by 10 MB after each process
        fprintf(output_file, "RAM After: %d MB\n", ram);
    }
    printf("\n");

    printf("CPU-2 que3(priority-2)(RR-q8):");
    rr_schedule(&queues[2], 8);
    printf("\n");

    printf("CPU-2 que4(priority-3)(RR-q16):");
    rr_schedule(&queues[3], 16);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    output_file = fopen("output.txt", "w");
    if (!output_file) {
        perror("Error opening output file");
        return 1;
    }

    read_input_file(argv[1]);
    assign_queues();
    print_schedule();

    fclose(output_file);
	return 0;
}
