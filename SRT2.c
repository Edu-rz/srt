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
    printf("\n");
    printf("\t%d", process->id);
    printf("\t\t%d", process->arrival_time);
    printf("\t%d", process->burst_time);
    printf("\t\t%d", process->waiting_time);
}

// Dado un puntero y el número de procesos, imprime todos los procesos
void DisplayAllProcesses(struct Process* processes, int n_processes) {
    printf("\n\tid\tarrival_time\tburst_time\twaiting_time");
    for(int i = 0; i < n_processes; i++) {
        DisplayProcess(&processes[i]);
    }
    printf("\n");
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

// Ordenar DIRECTAMENTE el arreglo de procesos por tiempo de ráfaga (no utilizar un arreglo auxiliar, modificarlo directamente)
void SortProcessArrayByBurstTime(int length, struct Process* array[length]) {
    for(int i = 0; i < length - 1; i++) {
        for(int j = 0; j < length - i - 1; j++) {
            if(array[j]->burst_time > array[j + 1]->burst_time) {
                struct Process* temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// Almacena en display2 información importante para luego mostrarlo en pantalla con el formato adecuado
void UpdateDisplay2(struct LinkedList* remaminingProcessesList, int n_processes, int display2[100][n_processes], int time) {
    if(remaminingProcessesList->head == NULL) return;
    struct Node* curProcess = remaminingProcessesList->head;
    struct Process* current_processes[n_processes];

    int i = 0;
    while(curProcess != NULL) {
        current_processes[i] = curProcess->data;
        curProcess = curProcess->next;
        i++;
    }

    SortProcessArrayByBurstTime(i, current_processes);

    // Una vez ordenado el arreglo, guardar la id de los procesos en display2[time]
    for(int j = 0; j < i; j++) {
        display2[time][j] = current_processes[j]->id;
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

// Retorna el tiempo promedio de ejecución
float AverageServiceTime(struct Process* processes, int n_processes){;
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

// Retornar el tiempo promedio de retorno normalizado
float AverageNormalizeReturnTime(float aver_tiemposerv, float ave_ex) {
    return ave_ex/aver_tiemposerv;
}


int main() {
    int n_processes;
    char* path = "C:/Users/joaqu/Downloads/EF_EQUIPO05/PROGRAMA_EQUIPO05/data.txt";
    
    //Ingresar cantidad maxima de procesos
    // printf("\nEnter the Total Number of Processes:\t");
    // scanf("%d", &n_processes);
    n_processes = GetLinesOfText(path);

    struct Process* processes = ScanProcessesByText(path);
    // struct Process* processes = ScanProcessesByInput(n_processes);
    
    float ave_ser = AverageServiceTime(processes, n_processes);

    //Inicializar Display 1 
    int display[100];
    Initialize1DArray(100, display, -1);
    //Inicializar Display 2
    int display2[100][n_processes];
    Initialize2DArray(100, n_processes, (int*)display2, -1); 


    if(n_processes <= 0){
        exit(0);
    }


    DisplayAllProcesses(processes, n_processes);
    int left_processes = n_processes;
    int time = 0;
    while(left_processes > 0) {
        
        //Obtener la cola de listos
        struct LinkedList* remaminingProcessesList = GetListWithRemainingProcesses(remaminingProcessesList, processes, n_processes, time);

        if(remaminingProcessesList->head == NULL) { //si la lista esta vacia
            time ++;
            continue;
        }

        ///Obtener el proceso con menor tiempo de ráfaga
        struct Process* currentProcess = GetProcessWithLessBurstTime(remaminingProcessesList, time);
        currentProcess->burst_time--;
        
        //Añadir 1 tiempo de espera a todos los procesos en la cola de listos, que no se están ejecutando 
        AddWaitingTime(remaminingProcessesList, currentProcess->id);
        
        // Tomar registros de los procesos en ejecución
        display[time] = currentProcess->id;
        // Tomar registro de la cola de listos
        UpdateDisplay2(remaminingProcessesList, n_processes, display2, time);

        if(currentProcess->burst_time == 0) {
            left_processes--;
            currentProcess->finished_time = time + 1;
        }
        time ++;
    }

    PrintDisplay(display, processes, n_processes, time);
    PrintQueueDisplay(n_processes, time, display2);

    float ave_wait = AverageWaitTime(processes, n_processes);
    float normalized_ret = AverageNormalizeReturnTime(ave_ser + ave_wait, ave_ser);

    printf("\nTIEMPO DE RETORNO PROMEDIO: %0.4f", ave_ser + ave_wait);
    printf("\nTIEMPO DE ESPERA PROMEDIO: %0.4f", ave_wait);
    printf("\nTIEMPO DE SERVICIO: %0.4f", ave_ser);
    printf("\nTIEMPO DE RETORNO NORMALIZADO: %0.4f", normalized_ret);
    
    return 0;
}

