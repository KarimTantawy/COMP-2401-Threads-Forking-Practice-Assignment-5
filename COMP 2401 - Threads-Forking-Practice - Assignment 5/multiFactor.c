#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
//prototype of morph method
int morph(char* number);

int main(int argc, char *argv[])
{
	//variable declarations
	FILE *fNumbers;
	unsigned long numArr[10];
	int status, child, parent, children[10];
	int factors[10];	
	int nums;
	//check if file name was provided, if not, provide instructions to user
	if (argc < 2)
	{ 
		printf("Instructions: \n");
		printf("	A file name for a binary file with at least 10 unsigned long numbers\n");
		printf("	must be included as the second command line parameter\n");
		printf("Usage: multiFactor fileName\n\n");
		//correct return code
		return -2;
	}	
	//find file with file name provided
	fNumbers = fopen(argv[1], "rb");
	//return error if file does not exist
	if(fNumbers == NULL)
	{
		printf("file %s.bin does not exist.\n", argv[1]);
		//correct return code		
		return -1;
	}
	//get first 10 ints from binary file and store it into an array
	nums = fread(numArr, sizeof(unsigned long), 10, fNumbers);
	//get parent id
	parent = getpid();

	for(int i = 0; i < 10; i++)
	{
		//spawn 10 children to handle each number
		if(getpid() == parent)
			children[i] = fork();
		//morph each child
		if(children[i] == 0)
		{
			char buffer [sizeof(unsigned long)*8+1];

			sprintf(buffer, "%lu", numArr[i]);

			morph(buffer);	
		}
		
	}
	//wait for 10 children to return
	for(int i = 0; i < 10; i++)
	{
		child = waitpid(-1, &status, 0);
		//get return value and add it to array of factors
		if (WIFEXITED(status))
		{
      		factors[i] = WEXITSTATUS(status);
		}
	}
	
	nums = 0;
	//sum up all of the factors
	for(int i = 0; i < 10; i++)
	{
		nums += factors[i];
	}
	//display sum
	printf("The sum of the factors of the 10 unsigned long numbers is, %d\n", nums);

	return 0;
}
//morph function
int morph(char* number)
{
	char* args[3];
		
	args[0] = "./numPrimeFactors";                                                                  
	args[1] = number;
	args[2] = NULL;
	//morph child into numPrimeFactors process and pass number as argument
	execvp("./numPrimeFactors", args);	
	
	return -1;
}
