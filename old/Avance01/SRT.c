#include <stdio.h>
#include <stdlib.h>
#include "util.h"
 
struct Process {
      int id;
      int arrival_time;
      int burst_time;
      int waiting_time;
      int finished_time;
};
 
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
        processes[i].waiting_time = 0;
        processes[i].finished_time = -1;
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
 
void PrintDisplay(int display[], struct Process* processes, int n_processes, int last_time) {
    printf("\n ---   TIMELINE DISPLAY   ---   \n");
    // imprimir del 0 al 100 con un for
    printf("   ");
    for(int i = 0; i < last_time; i++) {
        printf("%.2d ", i);
    }
    printf("\n");
    for(int i = 0; i < n_processes; i++) {
        printf("%d  ", processes[i].id);
        for(int time = 0; time < last_time; time++) {
            if(display[time] == processes[i].id) {
                ColorYellow();
                printf("X  ");
                ColorReset();
            } else if(processes[i].arrival_time <= time && time < processes[i].finished_time) {
                ColorRed();
                printf("W  ");
                ColorReset();
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("\n");
}
 
float AverageExeTime(struct Process* processes, int n_processes){;
    float prom;
    for (int i = 0; i < n_processes; i++){
        prom += processes[i].burst_time;
    }
    prom /= (float)n_processes;
    return prom;
}
 
float AverageWaitTime(struct Process* processes, int n_processes){
    float prom;
    for (int i = 0; i < n_processes; i++){
        prom += processes[i].waiting_time;
    }
 
    prom /= (float)n_processes;
    return prom;
}
 
int main() {
    int n_processes;
    float ave_ex, ave_wait;
    int display[100];
    for(int i = 0; i < 100; i++) { //  inicializar la display con -1
        display[i] = -1;
    }
 
    //Ingresar cantidad máxima de procesos
    printf("\nEnter the Total Number of Processes:\t");
    scanf("%d", &n_processes);
 
    struct Process* processes = ScanProcessesByInput(n_processes);
    ave_ex = AverageExeTime(processes, n_processes); 
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
        display[time] = currentProcess->id;
      
 
        if(currentProcess->burst_time == 0) {
            left_processes--;
            currentProcess->finished_time = time + 1;
        }
        time ++;
    }
 
    PrintDisplay(display, processes, n_processes, time);
    DisplayAllProcesses(processes, n_processes);
   
    ave_wait = AverageWaitTime(processes, n_processes);
    if(n_processes != 0){
        printf("TIEMPO DE EJECUCION PROMEDIO: %0.2f", ave_ex);
        printf("\nTIEMPO DE ESPERA PROMEDIO: %0.2f", ave_wait);
    }
   
    return 0;
}
