#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int finished_time;
};

//LISTAAAAAAAAAAaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

struct Node {
    struct Process* data;
    struct Node* next;
};

struct LinkedList {
    struct Node* head;
};

struct Node* CreateNode(struct Process* data) {
    struct Node* newNode = NULL;
    newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void Append(struct LinkedList* list, struct Process* process) {
    if(list->head == NULL) {
        list->head = CreateNode(process);
        return;
    }

    struct Node* currentNode = list->head;
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }
    currentNode->next = CreateNode(process);
}

void PrintList(struct LinkedList* list) {
    printf("[ ");
    struct Node* currentNode = list->head;
    while (currentNode != NULL) {
        printf("%d ", currentNode->data);
        currentNode = currentNode->next;
    }
    printf("]\n");
}

struct LinkedList* CreateLinkedList() {
    struct LinkedList* list = NULL;
    list = (struct LinkedList*) malloc(sizeof(struct LinkedList));
    list->head = NULL;
    return list;
}

// PROGRAMAAAAAAAAAAAAAAA

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

void AddWaitingTime(struct LinkedList* list, int time, int current_process_id) {
    struct Node* currentNode = list->head;
    
    while(currentNode != NULL) {
        struct Process* curProcess = currentNode->data;
        if(curProcess->id != current_process_id) {
            curProcess->waiting_time ++;
        }
        currentNode = currentNode->next;
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

struct LinkedList* GetListWithRemainingProcesses(struct LinkedList* list, struct Process* processes, int n_processes, int time) {
    struct LinkedList* remainingProcesses = CreateLinkedList();
    for(int i=0; i < n_processes; i++) {
        if(processes[i].arrival_time <= time && processes[i].burst_time > 0) {
            Append(remainingProcesses, &processes[i]);
        }
    }
    return remainingProcesses;
}

struct Process* GetProcessWithLessBurstTime(struct LinkedList* remainingProcesses, int time) {
    int processLessBurstTime = 10000;
    struct Process* proc = NULL;
    struct Node* currentNode = remainingProcesses->head;

    while(currentNode != NULL) {
        struct Process* curProcess = currentNode->data;
        if(curProcess->burst_time < processLessBurstTime) {
            processLessBurstTime = curProcess->burst_time;
            proc = curProcess;
        }
        currentNode = currentNode->next;
    }

    return proc;
}

void PrintDisplay(int display[], struct Process* processes, int n_processes) {
    for(int i = 0; i < n_processes; i++) {
        for(int time = 0; time < 100; time++) {
            if(display[time] == processes[i].id) {
                printf("X");
            } else if(processes[i].arrival_time <= time && time < processes[i].finished_time) {
                printf("W");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main() {
    int n_processes;
    float ave_ex, ave_wait; //AQUI
    int display[100];
    for(int i = 0; i < 100; i++) {
        display[i] = -1;
    }
    
    //Ingresar cantidad maxima de procesos
    printf("\nEnter the Total Number of Processes:\t");
    scanf("%d", &n_processes);
    struct Process* processes = ScanProcessesByInput(n_processes);
    ave_ex = AverageExeTime(processes, n_processes); //AQUI
    //DisplayAllProcesses(processes, n_processes);
       
    int left_processes = n_processes;
    int time = 0;
    while(left_processes > 0) {
        struct LinkedList* remaminingProcessesList = GetListWithRemainingProcesses(remaminingProcessesList, processes, n_processes, time);

        if(remaminingProcessesList->head == NULL) { //si la lista esta vacia
            PrintCurrentState(processes, n_processes, time, -1);
            time ++;
            continue;
        }

        struct Process* currentProcess = GetProcessWithLessBurstTime(remaminingProcessesList, time);
        currentProcess->burst_time--;
        AddWaitingTime(remaminingProcessesList, time, currentProcess->id);
        display[time] = currentProcess->id;
        PrintCurrentState(processes, n_processes, time, currentProcess->id);
        if(currentProcess->burst_time == 0) {
            left_processes--;
            currentProcess->finished_time = time + 1;
        }
        time ++;
    }

    PrintDisplay(display, processes, n_processes);
    //DisplayAllProcesses(processes, n_processes);
    
    // ave_wait = AverageWaitTime(processes, n_processes); //AQUI
    // //AQUIII
    // if(n_processes != 0){
    //     printf("TIEMPO DE EJECUCION PROMEDIO: %0.2f", ave_ex); 
    //     printf("\nTIEMPO DE ESPERA PROMEDIO: %0.2f", ave_wait); 
    // }
    
    return 0;
}
