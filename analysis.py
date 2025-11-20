''' input -*| '''
def pmos_transistor(input):
    match (input) :
        #0001 - 0
        case 1:
            return 0b0010 # 1
        
        #0010 - 1
        case 2:
            return 0b1000 # z

        #0100 - x
        case 4:
            return 0b1010 # 1,z
        
        case default:
            raise Exception("Invalid input to pmos transistor")


''' input -| '''
def nmos_transistor(input):
    match (input):
        #0001 - 0
        case 1:
            return 0b1000 # z
        
        #0010 - 1
        case 2:
            return 0b0001 # 0

        #0100 - x
        case 4:
            return 0b1001 # 0,z
        
        case default:
            raise Exception("Invalid input to nmos transistor")


def add_parallel_transistor_results(a, b):
    output = 0
    if (a & 0b1000): # if a has z, all inputs of b are included in output
        output |= b

    if (b & 0b1000): # if b has z, all inputs of a are included in output
        output |= a

    output |= a & 0b0100 | b & 0b0100 # if a or b has x, x is in the output.
    output |= a & b # if a and b both have the same input, it is included

    if ((a & 0b0010 and b & 0b0001) or (b & 0b0010 and a & 0b0001)): 
        output |= 0b0100 # if a and b conflict (0, 1) or (1, 0), x is included.
    
    return output

def add_series_transistor_results(a, b):
    output = 0
    output |= 0b1000 & a | b & 0b1000 # z is included if it's in the inputs
    output |= a & b # if a and b have the same input, it is included

    # Then check for x - only if there's no z
    if (a & 0b0100 and b & 0b0111) or (b & 0b0100 and a & 0b0111):
        output |= 0b0100
        return output
    
    # If no z or x, handle normal states
    if ((a & 0b0010 and b & 0b0001) or (b & 0b0010 and a & 0b0001)): 
        output |= 0b0100 # if a and b conflict (0, 1) or (1, 0), x is included
    return output

def binary_to_units(binary):
    output = []
    if (binary & 0b0001):
        output.append("0")
    if (binary & 0b0010):
        output.append("1")
    if (binary & 0b0100):
        output.append("x")
    if (binary & 0b1000):
        output.append("z")
    return output




'''   VDD
       |
   __*| 1
  |    | <- tran_1_result
A-|    |_____ Output
  |    | <- tran_2_result
  |___| 2
       |
      GND    '''

def not_gate(a, fault):
    #asserting the result after the first transistor
    if (fault == 1):
        #transistor 1 is slow, so it will product the same result twice
        tran_1_result = (pmos_transistor(a[0]), pmos_transistor(a[0]))
    else: 
        tran_1_result = (pmos_transistor(a[0]), pmos_transistor(a[1]))

    #asserting the result after the second transistor
    if (fault == 2):
        #transistor 2 is slow, so it will product the same result twice
        tran_2_result = (nmos_transistor(a[0]), nmos_transistor(a[0]))
    else: 
        tran_2_result = (nmos_transistor(a[0]), nmos_transistor(a[1]))
    
    return (bin(add_parallel_transistor_results(tran_1_result[0], tran_2_result[0])),
              bin(add_parallel_transistor_results(tran_1_result[1], tran_2_result[1])))
    

'''
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
'''

def nand_gate(a, b, fault):
    if (fault == 1):
        #transistor 1 is slow, so it will product the same result twice
        tran_1_result = (pmos_transistor(a[0]), pmos_transistor(a[0]))
    else: 
        tran_1_result = (pmos_transistor(a[0]), pmos_transistor(a[1]))

    #asserting the result after the second transistor
    if (fault == 2):
        #transistor 2 is slow, so it will product the same result twice
        tran_2_result = (pmos_transistor(b[0]), pmos_transistor(b[0]))
    else: 
        tran_2_result = (pmos_transistor(b[0]), pmos_transistor(b[1]))
    
    #asserting the result after the third transistor
    if (fault == 3):
        #transistor 3 is slow, so it will product the same result twice
        tran_3_result = (nmos_transistor(a[0]), nmos_transistor(a[0]))
    else: 
        tran_3_result = (nmos_transistor(a[0]), nmos_transistor(a[1]))

    #asserting the result after the fourth transistor
    if (fault == 4):
        #transistor 4 is slow, so it will product the same result twice
        tran_4_result = (nmos_transistor(b[0]), nmos_transistor(b[0]))
    else: 
        tran_4_result = (nmos_transistor(b[0]), nmos_transistor(b[1]))



    return ((add_parallel_transistor_results(add_parallel_transistor_results(tran_1_result[0], tran_2_result[0]), add_series_transistor_results(tran_3_result[0], tran_4_result[0]))),
              (add_parallel_transistor_results(add_parallel_transistor_results(tran_1_result[1], tran_2_result[1]), add_series_transistor_results(tran_3_result[1], tran_4_result[1]))))

'''
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
'''


def nor_gate(a, b, fault):
    if (fault == 1):
        #transistor 1 is slow, so it will product the same result twice
        tran_1_result = (nmos_transistor(a[0]), nmos_transistor(a[0]))
    else: 
        tran_1_result = (nmos_transistor(a[0]), nmos_transistor(a[1]))

    #asserting the result after the second transistor
    if (fault == 2):
        #transistor 2 is slow, so it will product the same result twice
        tran_2_result = (nmos_transistor(b[0]), nmos_transistor(b[0]))
    else: 
        tran_2_result = (nmos_transistor(b[0]), nmos_transistor(b[1]))
    
    #asserting the result after the third transistor
    if (fault == 3):
        #transistor 3 is slow, so it will product the same result twice
        tran_3_result = (pmos_transistor(a[0]), pmos_transistor(a[0]))
    else: 
        tran_3_result = (pmos_transistor(a[0]), pmos_transistor(a[1]))

    #asserting the result after the fourth transistor
    if (fault == 4):
        #transistor 4 is slow, so it will product the same result twice
        tran_4_result = (pmos_transistor(b[0]), pmos_transistor(b[0]))
    else: 
        tran_4_result = (pmos_transistor(b[0]), pmos_transistor(b[1]))



    return ((add_parallel_transistor_results(add_parallel_transistor_results(tran_1_result[0], tran_2_result[0]), add_series_transistor_results(tran_3_result[0], tran_4_result[0]))),
              (add_parallel_transistor_results(add_parallel_transistor_results(tran_1_result[1], tran_2_result[1]), add_series_transistor_results(tran_3_result[1], tran_4_result[1]))))


r'''

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
'''

def xor(a, b, gates_faults):
    gate_1 = not_gate(a, gates_faults[0])
    



if __name__ == "__main__":
    '''print(not_gate((0b0001, 0b0010), 0))
    print(not_gate((0b0001, 0b0010), 1)) # (0,1), 1 
    print(not_gate((0b0001, 0b0010), 2))
    print(not_gate((0b0010, 0b0001), 1))
    print(not_gate((0b0010, 0b0001), 2)) '''
    print(nand_gate((0b0010, 0b0001), ((0b0010), (0b0010)), 1))
    print(nand_gate((0b0010, 0b0001), ((0b0010), (0b0010)), 2))
    print(nand_gate((0b0010, 0b0001), ((0b0010), (0b0010)), 3))
    print(nand_gate((0b0010, 0b0001), ((0b0010), (0b0010)), 4))
    
    # Test inputs
    a = (0b0001, 0b0010)  # 0, 1
    b = (0b0001, 0b0010)  # 0, 1
    
    # Test XOR gate with no faults
    print("\n" + "="*60)
    print("XOR GATE TESTING - FAULT ANALYSIS")
    print("="*60)
    print(f"\nTest Input: a={binary_to_units(0b0011)}, b={binary_to_units(0b0011)}")
    print("(i.e., both inputs can be 0 or 1)")
    
    print("\n--- NO FAULTS (baseline) ---")
    result = xor(a, b, 0, 0, 0, 0, 0, 0)
    print(f"Result: {binary_to_units(result)}")
    print(f"Binary: {bin(result)}")
    
    # NOT gate faults (NOT_1 and NOT_3 and NOT_6)
    print("\n--- STAGE 1: NOT(B) GATE FAULTS ---")
    
    print("\nNOT_1 fault (transistor 1 slow):")
    result = xor(a, b, 1, 0, 0, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNOT_1 fault (transistor 2 slow):")
    result = xor(a, b, 2, 0, 0, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    # NAND gate 2 faults
    print("\n--- STAGE 2: NAND(A, NOT(B)) GATE FAULTS ---")
    
    print("\nNAND_2 fault (transistor 1 slow):")
    result = xor(a, b, 0, 1, 0, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNAND_2 fault (transistor 2 slow):")
    result = xor(a, b, 0, 2, 0, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNAND_2 fault (transistor 3 slow):")
    result = xor(a, b, 0, 3, 0, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNAND_2 fault (transistor 4 slow):")
    result = xor(a, b, 0, 4, 0, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    # NOT gate 3 faults
    print("\n--- STAGE 3: NOT(A) GATE FAULTS ---")
    
    print("\nNOT_3 fault (transistor 1 slow):")
    result = xor(a, b, 0, 0, 1, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNOT_3 fault (transistor 2 slow):")
    result = xor(a, b, 0, 0, 2, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    # NAND gate 4 faults
    print("\n--- STAGE 4: NAND(NOT(A), B) GATE FAULTS ---")
    
    print("\nNAND_4 fault (transistor 1 slow):")
    result = xor(a, b, 0, 0, 0, 1, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNAND_4 fault (transistor 2 slow):")
    result = xor(a, b, 0, 0, 0, 2, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNAND_4 fault (transistor 3 slow):")
    result = xor(a, b, 0, 0, 0, 3, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNAND_4 fault (transistor 4 slow):")
    result = xor(a, b, 0, 0, 0, 4, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    # NOR gate 5 faults
    print("\n--- STAGE 5: NOR(NAND2, NAND4) GATE FAULTS ---")
    
    print("\nNOR_5 fault (transistor 1 slow):")
    result = xor(a, b, 0, 0, 0, 0, 1, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNOR_5 fault (transistor 2 slow):")
    result = xor(a, b, 0, 0, 0, 0, 2, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNOR_5 fault (transistor 3 slow):")
    result = xor(a, b, 0, 0, 0, 0, 3, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNOR_5 fault (transistor 4 slow):")
    result = xor(a, b, 0, 0, 0, 0, 4, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    # NOT gate 6 faults
    print("\n--- STAGE 6: NOT(NOR) GATE FAULTS ---")
    
    print("\nNOT_6 fault (transistor 1 slow):")
    result = xor(a, b, 0, 0, 0, 0, 0, 1)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nNOT_6 fault (transistor 2 slow):")
    result = xor(a, b, 0, 0, 0, 0, 0, 2)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    # Multiple faults
    print("\n--- COMBINED FAULTS ---")
    
    print("\nBoth NOT gates faulty (NOT_1 and NOT_3):")
    result = xor(a, b, 1, 0, 1, 0, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nBoth NAND gates faulty (NAND_2 and NAND_4):")
    result = xor(a, b, 0, 1, 0, 1, 0, 0)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\nAll gates faulty (one fault in each):")
    result = xor(a, b, 1, 1, 1, 1, 1, 1)
    print(f"  Result: {binary_to_units(result)}")
    print(f"  Binary: {bin(result)}")
    
    print("\n" + "="*60)