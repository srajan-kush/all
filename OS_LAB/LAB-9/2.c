#include <stdio.h>

#define MAX_PROCESSES 5  // Maximum number of processes
#define MAX_RESOURCES 3  // Maximum number of resources

// Function declarations
int isSafeState(int available[], int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int processes[], int numProcesses, int numResources);
void calculateNeed(int need[][MAX_RESOURCES], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], int numProcesses, int numResources);

int main() {
    int numProcesses, numResources;
    int i, j;

    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);

    printf("Enter the number of resources: ");
    scanf("%d", &numResources);

    int allocation[MAX_PROCESSES][MAX_RESOURCES];  // Matrix to store the allocation of resources to processes
    int max[MAX_PROCESSES][MAX_RESOURCES];         // Matrix to store the maximum resource requirement of processes
    int available[MAX_RESOURCES];                  // Vector to store available resources
    int need[MAX_PROCESSES][MAX_RESOURCES];        // Matrix to store the need of each process
    int processes[MAX_PROCESSES];                  // Array of processes

    // Input allocation matrix
    printf("\nEnter the allocation matrix:\n");
    for (i = 0; i < numProcesses; i++) {
        printf("Process %d: ", i);
        for (j = 0; j < numResources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Input maximum matrix
    printf("\nEnter the maximum matrix:\n");
    for (i = 0; i < numProcesses; i++) {
        printf("Process %d: ", i);
        for (j = 0; j < numResources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input available resources
    printf("\nEnter the available resources: ");
    for (i = 0; i < numResources; i++) {
        scanf("%d", &available[i]);
    }

    // Calculate the need matrix
    calculateNeed(need, max, allocation, numProcesses, numResources);

    // Print the need matrix
    printf("\nThe need matrix is:\n");
    for (i = 0; i < numProcesses; i++) {
        for (j = 0; j < numResources; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Check if the system is in a safe state
    if (isSafeState(available, allocation, need, processes, numProcesses, numResources)) {
        printf("\nThe system is in a safe state.\n");
        printf("Safe sequence: ");
        for (i = 0; i < numProcesses; i++) {
            printf("P%d ", processes[i]);
        }
        printf("\n");
    } else {
        printf("\nThe system is NOT in a safe state.\n");
    }

    return 0;
}

// Function to calculate the need matrix
void calculateNeed(int need[][MAX_RESOURCES], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], int numProcesses, int numResources) {
    int i, j;
    for (i = 0; i < numProcesses; i++) {
        for (j = 0; j < numResources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// Function to check if the system is in a safe state
int isSafeState(int available[], int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int processes[], int numProcesses, int numResources) {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int safeSeq[MAX_PROCESSES];
    int count = 0, i, j;

    // Initialize work as a copy of available
    for (i = 0; i < numResources; i++) {
        work[i] = available[i];
    }

    // Find a safe sequence
    while (count < numProcesses) {
        int found = 0;
        for (i = 0; i < numProcesses; i++) {
            if (!finish[i]) {
                // Check if process i can be satisfied
                for (j = 0; j < numResources; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }

                // If all resources for process i can be allocated
                if (j == numResources) {
                    // Allocate resources
                    for (j = 0; j < numResources; j++) {
                        work[j] += allocation[i][j];
                    }

                    // Add process to safe sequence
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        // If no process was found in this pass, system is in unsafe state
        if (!found) {
            return 0;  // Unsafe state
        }
    }

    // Safe sequence found
    for (i = 0; i < numProcesses; i++) {
        processes[i] = safeSeq[i];
    }

    return 1;  // Safe state
}
