#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define MAX 4
typedef struct {
    int i;
} matrix_coordinates;

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 


void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    matrix_coordinates *mc = args;
    for(int j = 0; j < MAX; j++) {
        matSumResult[mc->i][j] = matA[mc->i][j] + matB[mc->i][j];
    }
    
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    matrix_coordinates *mc = args;
    for(int j = 0; j < MAX; j++) {
        matDiffResult[mc->i][j] = matA[mc->i][j] - matB[mc->i][j];
    }
    
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    
    matrix_coordinates *mc = args;
    
    for(int j = 0; j < MAX; j++) {
        int tot = 0;
        for(int k =0; k < MAX; k++) {
            tot += matA[j][k] * matB[k][mc->i];
        }
        matProductResult[j][mc->i] = tot;
    }

    return NULL;
}

//computes matrix operations on a cell. Calls each op function
//Afterwards, releases the memory allocation
void* cellCompute(void* args) {
    computeSum(args);
    computeDiff(args);
    computeProduct(args);
    free(args);
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn? 4
int main(int argc, char *argv[]) {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    // if(argc < 2) {
    //     printf("You need to pass the size of the matrices for the program to work");
    //     return 1;
    // }
    // MAX = atoi(argv[1]);
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t tid[MAX];
    int *index[MAX];
    
    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    
    for(int i = 0; i < MAX; i++) {
        matrix_coordinates *mc = malloc(sizeof(matrix_coordinates));
        mc->i = i;
        pthread_create(&tid[i], NULL, cellCompute, mc);
    }

    // 5. Wait for all threads to finish.

    for(int i = 0; i < MAX; i++) {
        pthread_join(tid[i], NULL);
    }
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}