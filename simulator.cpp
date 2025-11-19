#include <stdio.h>
#include <iostream>
#include <stdexcept>

#define ZERO (0b0001)
#define ONE (0b0010)
#define X (0b0100)
#define Z (0b1000)


// -*|
int pmosTransistor(int input) {
    switch(input) {
        //0001
        case 1:
            return ONE; //1            
        
        //0010 
        case 2:
            return Z; //z
            
        //0100 (x)
        case 4:
            return ONE | Z; //1, z
        
        default:
            throw std::invalid_argument("Invalid input to pmos transistor");
    }
}

// -|
int nmosTransistor(int input) {
    switch(input) {
        //0001
        case 1:
            return Z; //z            
        
        //0010 
        case 2:
            return ZERO; //0
            
        //0100 (x)
        case 4:
            return ZERO | Z; //0, z
        
        default:
            throw std::invalid_argument("Invalid input to cmos transistor");
    }
}

int addTransistorLogic(int a, int b) {
    int output = 0;
    if (a & Z) { //if a has z, all inputs of b are included in output
        output |= b;
    }
    
    if (b & Z) { //if b has z, all inputs of a are included in output
        output |= a;
    }
    
    output |= a & X | b & X; // if a or b has x, x is in the output
    output |= a & b; // any overlap of a & b will be included
    
    if ((a & ONE && b & ZERO) || (b & X && a & ZERO)) {
        output |= X; // if a and b conflict (0, 1) or (1, 0), x is included
    }

    return output;
}

int throughTransistorLogic(int a, int b) {
    int output = 0;
    
    output |= a & Z | b & Z; // z is included if it's in the input;
    output |= a & b; // any overlap of a & b will be included;
    
    // check for x - only if there's no z
    if ((a & X && b & (ZERO | ONE | X)) || (b & X && a & (ZERO | ONE | X))) {
        output |= X;
    }

    // if a and b conflict (0, 1) or (1, 0), x is included

    if ((a & ONE && b & ZERO) || (b & ONE && a & ZERO)) {
        throw std::invalid_argument("0 and 1 given to through transistor logic");
    }

    return output;
}

/*    VDD
       |
   __*| 1
  |    | <- tran_1_result
A-|    |_____ Output
  |    | <- tran_2_result
  |___| 2
       |
      GND
*/

int *notGate(int a[], int fault) {
    int transistor_1[2];
    if (fault == 1) {
        transistor_1[0] = pmosTransistor(a[0]);
        transistor_1[1] = pmosTransistor(a[0]);
    } else {
        transistor_1[0] = pmosTransistor(a[0]);
        transistor_1[1] = pmosTransistor(a[1]);
    }

    int transistor_2[2];
    if (fault == 2) {
        transistor_1[0] = nmosTransistor(a[0]);
        transistor_1[1] = nmosTransistor(a[0]);
    } else {
        transistor_1[0] = nmosTransistor(a[0]);
        transistor_1[1] = nmosTransistor(a[1]);
    }


    int results[] = {addTransistorLogic(transistor_1[0], transistor_2[0]), addTransistorLogic(transistor_1[1], transistor_2[1])};
    
    return results;
}

/*
       VDD
        |
     ___|____
    |        |
A-*| 1   B-*| 2
    |________|_____ Output
        | <- tran_3_result
     A-| 3
        | <- tran_4_result
     B-| 4
        |
       GND
*/

int *nandGate(int a[], int b[], int fault) {
    int transistor[4][2];

    if (fault == 1) {
        transistor[1][0] = pmosTransistor(a[0]);
        transistor[1][1] = pmosTransistor(a[0]);
    } else {
        transistor[1][0] = pmosTransistor(a[0]);
        transistor[1][1] = pmosTransistor(a[1]);
    }
    if (fault == 2) {
        transistor[2][0] = pmosTransistor(b[0]);
        transistor[2][1] = pmosTransistor(b[0]);
    } else {
        transistor[2][0] = pmosTransistor(b[0]);
        transistor[2][1] = pmosTransistor(b[1]);
    }
    if (fault == 3) {
        transistor[3][0] = nmosTransistor(a[0]);
        transistor[3][1] = nmosTransistor(a[0]);
    } else {
        transistor[3][0] = nmosTransistor(a[0]);
        transistor[3][1] = nmosTransistor(a[1]);
    }
    if (fault == 4) {
        transistor[4][0] = nmosTransistor(b[0]);
        transistor[4][1] = nmosTransistor(b[0]);
    } else {
        transistor[4][0] = nmosTransistor(b[0]);
        transistor[4][1] = nmosTransistor(b[1]);
    }

    int transistor_1_2_intersection[] = {addTransistorLogic(transistor[1][0], transistor[2][0]), addTransistorLogic(transistor[2][1], transistor[2][1])};

    int after_transistor_3_4[] = {throughTransistorLogic(transistor[3][0], transistor[4][0]), throughTransistorLogic(transistor[3][0], transistor[4][1])};

    int *result = new int[2];
    result[0] = addTransistorLogic(transistor_1_2_intersection[0], after_transistor_3_4[0]);
    result[1] = addTransistorLogic(transistor_1_2_intersection[1], after_transistor_3_4[1]);
    return result;
}

int *norGate(int a[], int b[], int fault) {
    int transistor[4][2];

    if (fault == 1) {
        transistor[1][0] = pmosTransistor(a[0]);
        transistor[1][1] = pmosTransistor(a[0]);
    } else {
        transistor[1][0] = pmosTransistor(a[0]);
        transistor[1][1] = pmosTransistor(a[1]);
    }
    if (fault == 2) {
        transistor[2][0] = pmosTransistor(b[0]);
        transistor[2][1] = pmosTransistor(b[0]);
    } else {
        transistor[2][0] = pmosTransistor(b[0]);
        transistor[2][1] = pmosTransistor(b[1]);
    }
    if (fault == 3) {
        transistor[3][0] = nmosTransistor(a[0]);
        transistor[3][1] = nmosTransistor(a[0]);
    } else {
        transistor[3][0] = nmosTransistor(a[0]);
        transistor[3][1] = nmosTransistor(a[1]);
    }
    if (fault == 4) {
        transistor[4][0] = nmosTransistor(b[0]);
        transistor[4][1] = nmosTransistor(b[0]);
    } else {
        transistor[4][0] = nmosTransistor(b[0]);
        transistor[4][1] = nmosTransistor(b[1]);
    }
}