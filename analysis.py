''' input -*| '''
def pmos_transistor(input):
    match (input) :
        #0001
        case 1:
            return 0b0010 # 1
        
        #0010
        case 2:
            return 0b1000 # z

        #0100
        case 4:
            return 0b1010 # 1,z
        
        case default:
            raise Exception("Invalid input to pmos transistor")


''' input -| '''
def nmos_transistor(input):
    match (input):
        #0001
        case 1:
            return 0b1000 # z
        
        #0010
        case 2:
            return 0b0001 # 0

        #0100
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
    output |= a & 0b0100 | a & 0b0100 # if a or b has x, x is in the output.
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



    return (bin(add_parallel_transistor_results(add_parallel_transistor_results(tran_1_result[0], tran_2_result[0]), add_series_transistor_results(tran_3_result[0], tran_4_result[0]))),
              bin(add_parallel_transistor_results(add_parallel_transistor_results(tran_1_result[1], tran_2_result[1]), add_series_transistor_results(tran_3_result[1], tran_4_result[1]))))

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
    match (fault):
        case 0:
            return a | b
        
        case 1:
            pass
        
        case 2:
            pass
        
        case 3:
            pass
        
        case 4:
            pass

        case default:
            pass

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