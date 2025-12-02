
#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <stddef.h>

// Encoding bitflags for logic values
#define ZERO (1)
#define ONE  (2)
#define X    (4)
#define Z    (8)

//notGate(a[], fault)
int *notGate(int[], int);

int *nandGate(int[], int[], int);

int *norGate(int[], int[], int); 

int *xorGate(int[], int[], int[]);

char *numToType(int, char[5]);

#endif // SIMULATOR_HPP