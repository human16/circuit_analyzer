#include <stdio.h>
#include <iostream>
#include <stdexcept>

int pmosTransistor(int input) {
    switch(input) {
        //0001
        case 1:
            return 0b0010; //1            
        
        //0010 
        case 2:
            return 0b1000; //z
            
        //0100 (x)
        case 4:
            return 0b1010; //1, z
        
        default:
            throw std::invalid_argument("Invalid input to pmos transistor");
    }
}

int cmosTransistor(int input) {
    switch(input) {
        //0001
        case 1:
            return 0b1000; //z            
        
        //0010 
        case 2:
            return 0b0001; //0
            
        //0100 (x)
        case 4:
            return 0b1001; //0, z
        
        default:
            throw std::invalid_argument("Invalid input to cmos transistor");
    }
}

int addParallelTransistor(int a, int b) {
    int output = 0;
    if (a & 0b1000) { //if a has z, all inputs of b are included in output
        output |= b;
    }
    
    if (b & 0b1000) { //if b has z, all inputs of a are included in output
        output |= a;
    }
    
    output |= a & 0b0100 | b & 0b0100; // if a or b has x, x is in the output
    output |= a & b; // any overlap of a & b will be included
    
    if ((a & 0b0010 && b & 0b0001) || (b & 0b0010 && a & 0b0001)) {
        output |= 0b0100; // if a and b conflict (0, 1) or (1, 0), x is included
    }

    return output;
}

int addSeriesTransistor(int a, int b) {
    int output = 0;
    
    output |= a & 0b1000 | b & 0b1000; // z is included if it's in the input;
    output |= a & b; // any overlap of a & b will be included;
    
    // check for x - only if there's no z
    if ((a & 0b0100 && b & 0b111) || (b & 0b0100 && a & 0b0111)) {
        output |= 0b0100;
    }

    // if a and b conflict (0, 1) or (1, 0), x is included
    
    output |= a & 0b0100 | b & 0b0100; // if a or b has x, x is in the output
    output |= a & b; // any overlap of a & b will be included
    
    if ((a & 0b0010 && b & 0b0001) || (b & 0b0010 && a & 0b0001)) {
        output |= 0b0100; // if a and b conflict (0, 1) or (1, 0), x is included
    }

    return output;
}