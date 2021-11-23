#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"


// Estructura PROCESS
struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int finished_time;
};

//FUNCIONES PARA UN LINKED LIST

// Estructura NODE
struct Node {
    struct Process* data;
    struct Node* next;
};

// LinkedList
// Node1  ->  Node2   -> Node3 ->  Node4 -> NULL
//  1           2         3   ...

// Estructura LINKED LIST
struct LinkedList {
    struct Node* head;
};

// Aloja en memoria una nueva estructura Node. Simulación de un constructor para `Node`
struct Node* CreateNode(struct Process* data) {
    struct Node* newNode = NULL;
    newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
//Crea un nodo que almacena el valor `process` y lo añade a la lista dada `list`
void Append(struct LinkedList* list, struct Process* process) {
    if(list->head == NULL) { // en caso la lista este vacia
        list->head = CreateNode(process); // añadir el primer elemento
        return;
    }

    struct Node* currentNode = list->head;
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }
    currentNode->next = CreateNode(process);
}
// Aloja en memoria una nueva estructura LinkedList. Simulación de un constructor para `LinkedList`
struct LinkedList* CreateLinkedList() {
    struct LinkedList* list = NULL;
    list = (struct LinkedList*) malloc(sizeof(struct LinkedList));
    list->head = NULL;
    return list;
}

// FUNCIONES PARA EL ALGORITMO SRT

// Dado un número x (`limit`), pide al usuario ingresar x procesos, retorna el puntero del arreglo de procesos
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

// Dada la dirección de un archivo txt, retorna la cantidad de líneas que contiene.
int GetLinesOfText(char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error: %s not exists, enter a valid path", path);
        return 0;
    }
    
    char line[20];
    int i = 0;
    while(fgets(line, sizeof line, file) != NULL) {
        i++;
    }
    fclose(file);
    return i;
}

// Dada la dirección de un archivo txt, registra todos los valores contenidos en este.
struct Process* ScanProcessesByText(char* path) {
    FILE* file = fopen(path, "r");
    const char delimiter[1] = ","; 
    if (file == NULL) {
        printf("Error: %s not exists, enter a valid path", path);
        return NULL;
    }

    struct Process* processes = malloc(sizeof(struct Process) * GetLinesOfText(path));

    char line[20];
    int i = 0;
    while(fgets(line, sizeof line, file) != NULL) {
        char* token = strtok(line, delimiter);
        int arrival_time = atoi(token);
        int burst_time = atoi(strtok(NULL, delimiter));
        processes[i].arrival_time = arrival_time;
        processes[i].burst_time = burst_time;
        processes[i].waiting_time = 0;
        processes[i].finished_time = -1;
        processes[i].id = i;
        i++;
    }

    fclose(file);

    return processes;
}

// Dado un puntero de un proceso, imprime sus datos en el terminal
void DisplayProcess(struct Process* process) {
    printf("\nDISPLAY PROCESS DATA\n");
    printf("  id -> %d  \n", process->id);
    printf("  arrival_time -> %d  \n", process->arrival_time);
    printf("  burst_time -> %d  \n", process->burst_time);
    printf("  waiting_time -> %d  \n", process->waiting_time);
    printf("\n");
}

// Dado un puntero y el número de procesos, imprime todos los procesos
void DisplayAllProcesses(struct Process* processes, int n_processes) {
    for(int i = 0; i < n_processes; i++) {
        DisplayProcess(&processes[i]);
    }
}

// Dada la cola de listos, añade una unidad de tiempo a todos los procesos que no se están ejecutando
void AddWaitingTime(struct LinkedList* list, int current_process_id) {
    struct Node* currentNode = list->head;
    
    while(currentNode != NULL) {
        struct Process* curProcess = currentNode->data;
        if(curProcess->id != current_process_id) {
            curProcess->waiting_time ++;
        }
        currentNode = currentNode->next;
    }
}

// Retorna el tiempo promedio de ejecución
float AverageExeTime(struct Process* processes, int n_processes){;
    float prom;
    for (int i = 0; i < n_processes; i++){
        prom += processes[i].burst_time;
    }
    prom /= (float)n_processes;
    return prom;
}

// Retorna el tiempo promedio de espera
float AverageWaitTime(struct Process* processes, int n_processes){
    float prom;
    for (int i = 0; i < n_processes; i++){
        prom += processes[i].waiting_time;
    }
    prom /= (float)n_processes;
    return prom;
}

// Insertar a una lista, todos los procesos listos, y retornar el puntero de aquella lista
struct LinkedList* GetListWithRemainingProcesses(struct LinkedList* list, struct Process* processes, int n_processes, int time) {
    struct LinkedList* remainingProcesses = CreateLinkedList();
    for(int i=0; i < n_processes; i++) {
        if(processes[i].arrival_time <= time && processes[i].burst_time > 0) {
            Append(remainingProcesses, &processes[i]);
        }
    }
    return remainingProcesses;
}

// Dada la cola de listos, retornar el proceso con menor tiempo de ráfaga
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

// Almacena en display2 información importante para luego mostrarlo en pantalla con el formato adecuado
void UpdateDisplay2(struct LinkedList* remaminingProcessesList, int n_processes, int display2[100][n_processes], int time) {
    struct Node* curProcess = remaminingProcessesList->head;
    int i = 0;
    while(curProcess != NULL) {
        display2[time][i] = curProcess->data->id;
        curProcess = curProcess->next;
        i++;
    }
}

// Imprimir en pantalla la linea de tiempo con los estados de los procesos
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

// Imprimir en pantalla la cola de listos de todos los procesos
void PrintQueueDisplay(int n_processes, int last_time, int display2[100][n_processes]) {
    printf("\n ---   QUEUE DISPLAY   ---   \n");
    printf("   ");
    ColorBlue();
    for(int i = 0; i < last_time; i++) {
        printf("%.2d ", i);
    }
    ColorReset();
    printf("\n");
    for(int j = 0; j < n_processes; j++) {
        printf("   ");
        for(int i = 0; i < last_time; i++) {
            if(display2[i][j] == -1) {
                printf("   ");
            }else {
                printf("%.2d ", display2[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}


// Rellenar un arreglo de 1 dimensión con el parametro `value`
void Initialize1DArray(int length, int array[length], int value) {

}

// Rellenar un arreglo de 2 dimensiones con el parametro `value`
void Initialize2DArray(int length, int height, int array[length][height], int value) {
    
}

// Retornar el tiempo promedio de servicio
float AverageServiceTime() {

}

// Retornar el tiempo promedio de retorno normalizado
float AverageNormalizeReturnTime() {

}



int main() {
    int n_processes;
    float ave_ex, ave_wait;
    char* path = "C:/Users/joaqu/Documents/GitHub/SRT/data.txt";
    
    int display[100];
    for(int i = 0; i < 100; i++) { //  inicializar la display con -1
        display[i] = -1;
    }

    //Ingresar cantidad maxima de procesos
    // printf("\nEnter the Total Number of Processes:\t");
    // scanf("%d", &n_processes);
    n_processes = GetLinesOfText(path);

    struct Process* processes = ScanProcessesByText(path);
    // struct Process* processes = ScanProcessesByInput(n_processes);
    
    ave_ex = AverageExeTime(processes, n_processes); //AQUI
    DisplayAllProcesses(processes, n_processes);

    int display2[100][n_processes];
    for(int i = 0; i < 100; i++) {//inicializar display2 con -1
        for(int j = 0; j < n_processes; j++) {
            display2[i][j] = -1;
        }
    }

    int left_processes = n_processes;
    int time = 0;
    while(left_processes > 0) {
        // Proceso 1 2 3 4 5
        // 2 4 5
        struct LinkedList* remaminingProcessesList = GetListWithRemainingProcesses(remaminingProcessesList, processes, n_processes, time);

        if(remaminingProcessesList->head == NULL) { //si la lista esta vacia
            time ++;
            continue;
        }

        struct Process* currentProcess = GetProcessWithLessBurstTime(remaminingProcessesList, time);
        currentProcess->burst_time--;
        AddWaitingTime(remaminingProcessesList, currentProcess->id);
        
        // Tomar registros de los procesos en ejecución y la cola de listos
        display[time] = currentProcess->id;
        UpdateDisplay2(remaminingProcessesList, n_processes, display2, time);

        if(currentProcess->burst_time == 0) {
            left_processes--;
            currentProcess->finished_time = time + 1;
        }
        time ++;
    }

    PrintDisplay(display, processes, n_processes, time);
    PrintQueueDisplay(n_processes, time, display2);
    ave_wait = AverageWaitTime(processes, n_processes);

    if(n_processes > 0){
        printf("TIEMPO DE EJECUCION PROMEDIO: %0.4f", ave_ex);
        printf("\nTIEMPO DE ESPERA PROMEDIO: %0.4f", ave_wait);
        //Mostrar Tiempo de servicio
        //Mostrar Tiempo de retorno normalizado
    }
    
    return 0;
}
