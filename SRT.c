#include <stdio.h>
#include <stdlib.h>
//#include "List3.h"
struct Process {
      int id;
      int arrival_time;
      int burst_time;
      int waiting_time;
};

struct Process* CreateProcess() {
    struct Process* newProcess = NULL;
    newProcess = (struct Process*) malloc(sizeof(struct Process));
    return newProcess;
}

struct Process* ScanProcessesByInput(int limit) {
    struct Process* processes = malloc(sizeof(struct Process) * limit);
    for(int i = 0; i < limit; i++) {
        int arrival_time, burst_time;
        printf("\nEnter Arrival Time:\t");
        scanf("%d", &arrival_time);

        printf("Enter Burst Time:\t");
        scanf("%d", &burst_time);

        processes[i].arrival_time = arrival_time;
        processes[i].burst_time = burst_time;
        processes[i].id = i;
    }

    return processes;
}

void DisplayProcess(struct Process* process) {
    printf("\nDISPLAY PROCESS DATA\n");
    printf("  id -> %d  \n", process->id);
    printf("  arrival_time -> %d  \n", process->arrival_time);
    printf("  burst_time -> %d  \n", process->burst_time);
    printf("  waiting_time -> %d  \n", process->waiting_time);
    printf("\n");
}

void DisplayAllProcesses(struct Process* processes, int n_processes) {
    for(int i = 0; i < n_processes; i++) {
        DisplayProcess(&processes[i]);
    }
}

struct Process* GetProcessWithLessBurstTime(struct Process* processes, int n_processes, int time) {
    int processLessBurstTimeIdx = -1;
    int processLessBurstTime = 10000;
    for(int i=0; i < n_processes; i++) {
        if(processes[i].burst_time < processLessBurstTime && processes[i].arrival_time <= time && processes[i].burst_time > 0) {
            processLessBurstTime = processes[i].burst_time;
            processLessBurstTimeIdx = i;
        }
    }

    if(processLessBurstTimeIdx == -1)
        return NULL;
    
    return &processes[processLessBurstTimeIdx];
}

struct Process* AddWaitingTime(struct Process* processes, int n_processes, int time, int current_process_id) {
    for(int i=0; i < n_processes; i++) {
        if(processes[i].id != current_process_id && processes[i].arrival_time <= time && processes[i].burst_time > 0) {
            processes[i].waiting_time ++;
        }
    }
}

void PrintCurrentState(struct Process* processes, int n_processes, int time, int current_process_id) {
    printf("  %02d  ", time);
    
    for(int i=0; i < n_processes; i++) {
        if(processes[i].id == current_process_id) {
            printf("X ");
        } else if(processes[i].arrival_time <= time && processes[i].burst_time > 0) {
            printf("W ");
        } else {
            printf("  ");
        }
    }
    printf("\n");
}

int main() {
    int n_processes;
    
    //Ingresar cantidad maxima de procesos
    printf("\nEnter the Total Number of Processes:\t");
    scanf("%d", &n_processes);

    struct Process* processes = ScanProcessesByInput(n_processes);
    DisplayAllProcesses(processes, n_processes);
    
    int left_processes = n_processes;
    int time = 0;
    while(left_processes > 0) {
        //printf("\n - TIME %d\n", time);
        struct Process* currentProcess = GetProcessWithLessBurstTime(processes, n_processes, time);
        if(currentProcess == NULL) {
            time ++;
            continue;
        }

        currentProcess->burst_time--;
        AddWaitingTime(processes, n_processes, time, currentProcess->id);

        PrintCurrentState(processes, n_processes, time, currentProcess->id);
        if(currentProcess->burst_time == 0) {
            left_processes--;
        }
        time ++;
    }
    
    //DisplayAllProcesses(processes, n_processes);

    return 0;
}
