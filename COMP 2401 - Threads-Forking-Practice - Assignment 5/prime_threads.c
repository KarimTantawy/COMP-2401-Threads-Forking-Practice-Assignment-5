// file is prime_threads.c
/********************************************************************/


/****************************************************************/
// Includes

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
/***************************************************************/
// Structures


// holds the thread information for each thread
typedef struct threadData { 
    pthread_t thread_id;			// system thread id
    int tid;                        // inernal id assigned by the program
} ThreadData; 


/******************************************************************/
// Define


#define NUM_THREADS 5


/******************************************************************/
// Function Prototypes

int numPrimeFactors(unsigned long number);

/***************************************************************/
// Global Variables
volatile unsigned long totalNumPrimeFactors = 0;		// global value that holds the final result
volatile unsigned long *recordsToProcess = NULL;     // array that will hold al the numbers to be processed
volatile int numRecords = 0;                         // number of records in the array recordsToProcess
sem_t mutex;

/*************************************************************/
/*
Purpose compute the number of prime factors of a given number
input:
number - the number to be processed

output: 
None

Return:
the number of prime factors of number
*/
int numPrimeFactors(unsigned long number)
{
    unsigned long i;
    unsigned long quot;
    int numFactors = 0;

    // base case
    if (number == 1) return(0);
    if (number == 2 || number == 3) return(1);

    // check for the first factor until integer is less then number 
    for (i = 2; i <= number; i++) {
        if (i % 1000 == 0) {        
            if (number / i == 1) return(1);   // cannot have any more factrs other than self
        }
        if (number % i == 0) {
            quot = number / i;
            if (quot == 1) return(1);   // cannot have any more factrs other than self
            numFactors = 1 + numPrimeFactors(quot);
            return(numFactors);
        }
    }
    return(0);
}
/*************************************************************************/
//Purpose: the main function of the threads

void *threadMain()
{
	//get number of values that we want to factor
	int size = sizeof(recordsToProcess)/sizeof(unsigned long);
	//check if there are any values left to factor
	while(numRecords-1 >= 0)
	{
		//wait for our turn
		if (sem_wait(&mutex) < 0) 
		{
	 		printf("Error: on semaphore wait.\n");
	 		exit(1);
	 	}
		//check if there are still values left to process
		if(numRecords-1 >= 0)
		{
			printf("processing %lu, at index %d\n", recordsToProcess[numRecords-1], numRecords-1);
			//add value of factor to sum 		
			totalNumPrimeFactors += numPrimeFactors(recordsToProcess[numRecords-1]);
			numRecords--;
		}
		//now it's another threads turn
	 	if (sem_post(&mutex) < 0) 
		{
	 		printf("Error: on semaphore post.\n");
	 		exit(1);
	 	}
	}

	return 0; 
}

/*************************************************************************/
int main(int argc, char ** argv)
{
	//variable declarations
	FILE *fNumbers;
    pthread_t tid[NUM_THREADS];
	int* retThreads[NUM_THREADS];
	int processAll = -1;
	
    // check command line is correct - if not show how to use the program
    if (argc <= 2)
	{ 
		printf("Instructions: \n");
		printf("	A file name for a binary file with at least 10 unsigned long numbers\n");
		printf("	must be included as the second command line parameter\n");
		printf("	Indices of the desired values to be processed must be included as well.\n");
		printf("Usage: multiFactor <fileName> index1 index2...\n\n");

		return -2;
	}
	//number of indices desired by user
	int numOfIndices = argc - 2;
	int indices[numOfIndices];	
	//check if user desires all numbers in the file to be processed
	if(atoi(argv[2]) == -1)
	{
		processAll = 0;
	}
	else
	{
		//insert all values at n indice to be processed
		for(int i = 0; i < numOfIndices; i++)
		{
			indices[i] = atoi(argv[i+2]);
		}
	}
    // init mutex 
	if(sem_init(&mutex, 0, 1) < 0)
	{
		printf("Error: Unable to initialize semaphore.");
		exit(1);
	}	

    // read the records from the file
	fNumbers = fopen(argv[1], "rb");
	
	if(fNumbers == NULL)
	{
		printf("file %s.bin does not exist.\n", argv[1]);
		//correct return code		
		return -1;
	}
	
	long size;	

	if(processAll == 0)
	{
		//get number of elements in the binary file
		//this was included as a bonus for task I, if you would like
		//to give me some of the bonus marks, that would be really appreciated :)
		fseek(fNumbers, 0, SEEK_END);
		
		long end = ftell(fNumbers);
	
		size = end / sizeof(unsigned long);

		//allocate space for the files
		recordsToProcess = (unsigned long*)malloc(size*sizeof(unsigned long));
		//reset file pointer to beginning
		fseek(fNumbers, 0, SEEK_SET);
		//read all values
		fread(recordsToProcess, sizeof(unsigned long), size, fNumbers);
	}
	else
	{
		//find only appropriate values at desired indices
		unsigned long curNumber;
		size = numOfIndices;
		recordsToProcess = (unsigned long*)malloc(size*sizeof(unsigned long));
		//printf("SIZE = %ld\n", size);
		for(int i = 0; i < size; i++)
		{
			//set file pointer to correct position
			fseek(fNumbers, sizeof(unsigned long)*indices[i], SEEK_SET);
			fread(&curNumber, sizeof(unsigned long), 1, fNumbers);
			recordsToProcess[i] = curNumber;	
		}
	}
	//set numRecords
	numRecords = (int)size;
	printf("\nNumber of values: %ld\n\n", size);
	//display numbers to be processed
	printf("Values:\n");
	for(int i = 0; i < size; i++)
	{
		printf("%lu\n", recordsToProcess[i]);
	}
	printf("\n");
	//create 5 threads to process values
	for(int i = 0; i < NUM_THREADS; i++)
	{
		int rc = pthread_create(&tid[i], NULL, threadMain, NULL);
 		if (rc != 0) 
		{
 			printf("Error - pthread_create() return code: %d\n", rc);
			exit(1);
		} 
	}
	
    // wait for the threads to terminate
	for(int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(tid[i], NULL);
	}
    
    // print the numer of prime factors
	printf("\nTotal number of prime factors is %ld.\n", totalNumPrimeFactors);
	//free allocated memory
	free(recordsToProcess);
}




