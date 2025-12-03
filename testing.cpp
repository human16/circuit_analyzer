#include <stdio.h>
#include <string.h>
#include "simulator.hpp"

void resultAnalysis(int *results, int *default_results, int a[], int b[], int numFault, int numGate) {
    char buffer1[5];
    char buffer2[5];
    if (memcmp(results, default_results, sizeof(int) * 2) != 0) {
        printf("for values: a:(%d, %d), b:(%d, %d) gate %d fault index: %d -> (%s, %s)\n", (a[0]-1), (a[1]-1), (b[0]-1), (b[1]-1), (numGate+1), numFault, numToType(results[0], buffer1), numToType(results[1], buffer2));
    }
}

void runFault(int a[], int b[], int faultArrSize, int *faultArr, int *numFaultsArr, int *(*gate)(int[], int[], int[])) {
    for(int faultIdx = 0; faultIdx < faultArrSize; faultIdx++) {
        int *default_results = (*gate)(a, b, faultArr);
        for(int numFault = 1; numFault <= numFaultsArr[faultIdx]; numFault++) {
            faultArr[faultIdx] = numFault;
            int *results = (*gate)(a, b, faultArr);
            resultAnalysis(results, default_results, a, b, numFault, faultIdx);
        }
        //resetting to array of 0s
        faultArr[faultIdx] = 0;
    }
}

int main() {
    int vals[2] = {ZERO, ONE};
    for (int a0 = 0; a0 < 2; a0++) {
        for (int a1 = 0; a1 < 2; a1++) {
            for (int b0 = 0; b0 < 2; b0++) {
                for (int b1 = 0; b1 < 2; b1++) {
                    //testing xor
                    int faultArr[8] = {0};
                    int numFaultArr[] = {2, 4, 2, 2, 4, 2, 4, 2};
                    int a[] = {vals[a0], vals[b0]};
                    int b[] = {vals[a1], vals[b1]};
                    runFault(a, b, 8, faultArr, numFaultArr, &xorGate);
                }
            }
        }
    }
    return 0;
}