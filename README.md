# Assignment Focused on Creating Multiple Threads and Forks for Task
#### SCHOOL PROJECT - COMP 2401 - Fall 2019 Semester 

##### Submission README
Purpose:
To provide a sum of factors for a following set of unsigned long ints
included in a binary file.

Developer:
Kareem Tantawy
December 6th, 2019

Files:
1. createBinary.c
1. prime_threads.c
1. multiFactor.h
1. numPrimeFactor.c
1. prime.bin
1. prime.txt

Instructions:

	Task I:
		-Compilation Example:
			gcc multiFactor.c
			gcc -o numPrimeFactors numPrimeFactors.c
		-Execution:
			./a.out <binary file name>

	Task II:
		-Compilation Example:
			gcc prime_threads.c -lpthread
		-Execution:
			./a.out <binary file name> index1 index2 ...

Limitations:
1. All specifications have been implemented
1. did not get a chance to create the make files, so the programs
	 will have to be compiled manually
1. You might recieve some warnings when compiling Task II, but
	 don't fret.
1. Task I only takes the first 10 values, but Task II takes n
	 values in the binary file as specified
1. There might be some edge cases that have been overlooked,
	 so please be gentle
	

