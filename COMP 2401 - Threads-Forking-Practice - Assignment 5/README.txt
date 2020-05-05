Purpose:
	-To provide a sum of factors for a following set of unsigned long ints
	 included in a binary file.

Developer:
	-Kareem Tantawy, 101083898
	-December 6th, 2019

Files:
	-createBinary.c
	-prime_threads.c
	-multiFactor.h
	-numPrimeFactor.c
	-prime.bin
	-prime.txt

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
	-All specifications have been implemented
	-did not get a chance to create the make files, so the programs
	 will have to be compiled manually
	-You might recieve some warnings when compiling Task II, but
	 don't fret.
	-Task I only takes the first 10 values, but Task II takes n
	 values in the binary file as specified
	-There might be some edge cases that have been overlooked,
	so please be gentle
	

