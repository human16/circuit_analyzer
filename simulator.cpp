#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h> 

#define ZERO (1)
#define ONE (2)
#define X (4)
#define Z (8)


// -*|
int pmosTransistor(int input) {
    int output = 0;

    if (input & ZERO) {
        output |= ONE;
    }
    if (input & ONE) {
        output |= Z;
    }
    if (input & X || input & X) {
        output |= (ONE | Z);
    }


    if (output == 0) {
        fprintf(stderr, "Invalid input to pmos: 0x%x\n", input);
    }

    return output;
}

// -|
int nmosTransistor(int input) {
    int output = 0;

    if (input & ZERO) {
        output |= Z;
    }
    if (input & ONE) {
        output |= ZERO;
    }
    if (input & X || input & Z) {
        output |= (ZERO | Z);
    }

    if (output == 0) {
        fprintf(stderr, "Invalid input to nmos: 0x%x\n", input);
    }

    return output;
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
        fprintf(stderr, "Invalid through transistor logic");
        return 0;
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
        transistor_2[0] = nmosTransistor(a[0]);
        transistor_2[1] = nmosTransistor(a[0]);
    } else {
        transistor_2[0] = nmosTransistor(a[0]);
        transistor_2[1] = nmosTransistor(a[1]);
    }


    int *results = new int[2];
    results[0] = addTransistorLogic(transistor_1[0], transistor_2[0]);
    results[1] = addTransistorLogic(transistor_1[1], transistor_2[1]);

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
    // keep indices 1..4 as used below by sizing array to 5 rows
    int transistor[5][2];

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

    int transistor_1_2_intersection[] = {addTransistorLogic(transistor[1][0], transistor[2][0]), addTransistorLogic(transistor[1][1], transistor[2][1])};

    int after_transistor_3_4[] = {throughTransistorLogic(transistor[3][0], transistor[4][0]), throughTransistorLogic(transistor[3][1], transistor[4][1])};

    int *result = new int[2];
    result[0] = addTransistorLogic(transistor_1_2_intersection[0], after_transistor_3_4[0]);
    result[1] = addTransistorLogic(transistor_1_2_intersection[1], after_transistor_3_4[1]);
    return result;
}

/*
       GND
        |
     ___|____
    |        |
 A-| 1    B-| 2
    |________|_____ Output
        |
    A-*| 3
        |
    B-*| 4
        |
       VDD
*/

int *norGate(int a[], int b[], int fault) {
    // keep indices 1..4 as used below by sizing array to 5 rows
    int transistor[5][2];

    if (fault == 1) {
        transistor[1][0] = nmosTransistor(a[0]);
        transistor[1][1] = nmosTransistor(a[0]);
    } else {
        transistor[1][0] = nmosTransistor(a[0]);
        transistor[1][1] = nmosTransistor(a[1]);
    }
    if (fault == 2) {
        transistor[2][0] = nmosTransistor(b[0]);
        transistor[2][1] = nmosTransistor(b[0]);
    } else {
        transistor[2][0] = nmosTransistor(b[0]);
        transistor[2][1] = nmosTransistor(b[1]);
    }
    if (fault == 3) {
        transistor[3][0] = pmosTransistor(a[0]);
        transistor[3][1] = pmosTransistor(a[0]);
    } else {
        transistor[3][0] = pmosTransistor(a[0]);
        transistor[3][1] = pmosTransistor(a[1]);
    }
    if (fault == 4) {
        transistor[4][0] = pmosTransistor(b[0]);
        transistor[4][1] = pmosTransistor(b[0]);
    } else {
        transistor[4][0] = pmosTransistor(b[0]);
        transistor[4][1] = pmosTransistor(b[1]);
    }

    int transistor_3_4_intersection[] = {addTransistorLogic(transistor[3][0], transistor[4][0]), addTransistorLogic(transistor[3][1], transistor[4][1])};

    int after_transistor_1_2[] = {throughTransistorLogic(transistor[1][0], transistor[2][0]), throughTransistorLogic(transistor[1][1], transistor[2][1])};

    int *result = new int[2];
    result[0] = addTransistorLogic(transistor_3_4_intersection[0], after_transistor_1_2[0]);
    result[1] = addTransistorLogic(transistor_3_4_intersection[1], after_transistor_1_2[1]);
    return result;
}

/*
A ___________________
        |        |   \           _____
B ______|___|\___|NAND|__________\    \ 
    |   |   |/   |___/            |NOR |____|\___output
    |   |    1     2          ___/____/     |/
    |   |                     |    5         6
    |   |___________          |
    |           |   \         |
    |_______|\__|NAND|________|
            |/  |___/     
             3    4       
*/

int *xorGate(int a[], int b[], int faults[]) {
    int *gate_1 = notGate(b, faults[0]);
    int *gate_2 = nandGate(a, gate_1, faults[1]);
    int *gate_3 = notGate(a, faults[2]);
    int *gate_4 = nandGate(gate_3, b, faults[3]);
    int *gate_5 = norGate(gate_2, gate_4, faults[4]);
    int *gate_6 = notGate(gate_5, faults[5]);
    // The current gate network produces the XNOR of A and B. Invert
    // the resulting signal mask to obtain XOR semantics.
    auto invertMask = [](int m) {
        int r = 0;
        if (m & ZERO) r |= ONE;
        if (m & ONE) r |= ZERO;
        if (m & X) r |= X;
        if (m & Z) r |= Z;
        return r;
    };

    int *result = new int[2];
    result[0] = invertMask(gate_6[0]);
    result[1] = invertMask(gate_6[1]);
    return result;
}

char *numToType(int num, char buffer[5]) {
    buffer[0] = '-';
    buffer[1] = '-';
    buffer[2] = '-';
    buffer[3] = '-';
    buffer[4] = '\0';
    
    if (num & ZERO) buffer[0] = '0';
    if (num & ONE) buffer[1] = '1';
    if (num & X) buffer[2] = 'x';
    if (num & Z) buffer[3] = 'z';
    
    return buffer;
}

// Simple test harness for XOR gate using Option A encoding
int main() {
    for (int a_0 = ZERO; a_0 <= ONE; a_0++) {
        for (int a_1 = ZERO; a_1 <= ONE; a_1++) {
            for (int b_0 = ZERO; b_0 <= ONE; b_0++) {
                for (int b_1 = ZERO; b_1 <= ONE; b_1++) {
                    int a[] = {a_0, a_1};
                    int b[] = {b_1, b_1};
                    for (int gate_1_fault = 1; gate_1_fault <= 2; gate_1_fault++) {
                        int faults[] = {gate_1_fault, 0, 0, 0, 0, 0};
                        int *results = xorGate(a, b, faults);
                        char buffer1[5];
                        char buffer2[5];
                        printf("for values: a:(%d, %d), b:(%d, %d) gate 1 fault: %d -> (%s, %s)\n", (a_0-1), (a_1-1), (b_0-1), (b_1-1), gate_1_fault, numToType(results[0], buffer1), numToType(results[1], buffer2));
                    }
                    for (int gate_2_fault = 1; gate_2_fault <= 4; gate_2_fault++) {
                        int faults[] = {0, gate_2_fault, 0, 0, 0, 0};
                        int *results = xorGate(a, b, faults);
                        char buffer1[5];
                        char buffer2[5];
                        printf("for values: a:(%d, %d), b:(%d, %d) gate 2 fault: %d -> (%s, %s)\n", (a_0-1), (a_1-1), (b_0-1), (b_1-1), gate_2_fault, numToType(results[0], buffer1), numToType(results[1], buffer2));
                    }
                    for (int gate_3_fault = 1; gate_3_fault <= 2; gate_3_fault++) {
                        int faults[] = {0, 0, gate_3_fault, 0, 0, 0};
                        int *results = xorGate(a, b, faults);
                        char buffer1[5];
                        char buffer2[5];
                        printf("for values: a:(%d, %d), b:(%d, %d) gate 3 fault: %d -> (%s, %s)\n", (a_0-1), (a_1-1), (b_0-1), (b_1-1), gate_3_fault, numToType(results[0], buffer1), numToType(results[1], buffer2));
                    }
                    for (int gate_4_fault = 1; gate_4_fault <= 4; gate_4_fault++) {
                        int faults[] = {0, 0, 0, gate_4_fault, 0, 0};
                        int *results = xorGate(a, b, faults);
                        char buffer1[5];
                        char buffer2[5];
                        printf("for values: a:(%d, %d), b:(%d, %d) gate 4 fault: %d -> (%s, %s)\n", (a_0-1), (a_1-1), (b_0-1), (b_1-1), gate_4_fault, numToType(results[0], buffer1), numToType(results[1], buffer2));
                    }
                    for (int gate_5_fault = 1; gate_5_fault <= 4; gate_5_fault++) {
                        int faults[] = {0, 0, 0, 0, gate_5_fault, 0};
                        int *results = xorGate(a, b, faults);
                        char buffer1[5];
                        char buffer2[5];
                        printf("for values: a:(%d, %d), b:(%d, %d) gate 5 fault: %d -> (%s, %s)\n", (a_0-1), (a_1-1), (b_0-1), (b_1-1), gate_5_fault, numToType(results[0], buffer1), numToType(results[1], buffer2));
                    }
                    for (int gate_6_fault = 1; gate_6_fault <= 2; gate_6_fault++) {
                        int faults[] = {0, 0, 0, 0, 0, gate_6_fault};
                        int *results = xorGate(a, b, faults);
                        char buffer1[5];
                        char buffer2[5];
                        printf("for values: a:(%d, %d), b:(%d, %d) gate 6 fault: %d -> (%s, %s)\n", (a_0-1), (a_1-1), (b_0-1), (b_1-1), gate_6_fault, numToType(results[0], buffer1), numToType(results[1], buffer2));
                    }
                }
            }
        }
    }
}