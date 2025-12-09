#include <stdio.h>
#include <string.h>
#include "simulator.hpp"

int vals[4] = {ZERO, ONE, X, Z};

char valToChar(int val) {
    if (val == ZERO) {
        return '0';
    }
    if (val == ONE) {
        return '1';
    }
    if (val == X) {
        return 'x';
    }
    if (val == Z) {
        return 'z';
    }
    return NULL;
}

//prints the results of the test if they are different from the expected results
int resultAnalysis(int *results, int *default_results, int a[], int b[], int numFault, int numGate, char *name) {
    char buffer1[5];
    char buffer2[5];
    if (memcmp(results, default_results, sizeof(int) * 2) != 0 || (results[1] & (X | Z)) != 0) {
        printf("%s: for values: a:(%c, %c), b:(%c, %c) gate %d fault index: %d -> (%s, %s)\n", name, valToChar(a[0]), valToChar(a[1]), valToChar(b[0]), valToChar(b[1]), (numGate+1), numFault, numToType(results[0], buffer1), numToType(results[1], buffer2));
        return 1;
    }
    return 0;
}

//test the all the possible faults on the given gate
int runFault(int a[], int b[], int faultArrSize, int *faultArr, int *numFaultsArr, int *(*gate)(int[], int[], int[]), char *name) {
    int output = 0;
    for(int faultIdx = 0; faultIdx < faultArrSize; faultIdx++) {
        int *default_results = (*gate)(a, b, faultArr);
        for(int numFault = 1; numFault <= numFaultsArr[faultIdx]; numFault++) {
            faultArr[faultIdx] = numFault;
            int *results = (*gate)(a, b, faultArr);
            output += resultAnalysis(results, default_results, a, b, numFault, faultIdx, name);
        }
        //resetting to array of 0s
        faultArr[faultIdx] = 0;
    }
    return output;
}

int main() {
    int nor_faults = 0;
    int nand_faults = 0;
    int nand_xor_faults = 0;
    int nor_xor_faults = 0;
    for (int a0 = 0; a0 < 2; a0++) {
        for (int a1 = 0; a1 < 4; a1++) {
            int a[] = {vals[a0], vals[a1]};

            for (int b0 = 0; b0 < 2; b0++) {
                for (int b1 = 0; b1 < 4; b1++) {
                    //testing NOR and NAND
                    int b[] = {vals[b0], vals[b1]};
                    int *defNorResult = norGate(a, b, 0);
                    int *defNandResult = nandGate(a, b, 0);
                    for (int fault = 0; fault < 1; fault++) {
                        int *norResult = norGate(a, b, fault);
                        int *nandResult = nandGate(a, b, fault);
                        //resultAnalysis(norResult, defNorResult, a, b, fault, 0, "NOR");
                        //resultAnalysis(nandResult, defNandResult, a, b, fault, 0, "NAND");

                    }

                    //testing nand xor (using same a & b)
                    int nandFaultArr[4] = {0};
                    int nandNumFaultArr[5] = {4};
                    nand_xor_faults += runFault(a, b, 4, nandFaultArr, nandNumFaultArr, &nandXorGate, "NAND-XOR");

                    //testing nor xor 
                    int norFaultArr[5] = {0};
                    int norNumFaultArr[5] = {4};
                    nor_xor_faults += runFault(a, b, 5, norFaultArr, norNumFaultArr, &norXorGate, "NOR-XOR");
                }
            }
        }
    }
    printf("NAND-XOR: %d faults\n", nand_xor_faults);
    printf("NOR-XOR: %d faults\n", nor_xor_faults);
    return 0;
}