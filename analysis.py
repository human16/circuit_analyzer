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


def add_transistor_results(a, b):
    output = 0
    if (a & 0b1000): # if a has z, all inputs of b are included in output
        output |= b
    if (b & 0b1000): # if b has z, all inputs of a are included in output
        output |= a
    output |= a & 0b0100 | a & 0b0100 # if a or b has x, x is in the output.
    output |= a & b # if a and b both have the same input, it is included
    if ((a & 0b0010 and b & 0b0001) or (b & 0b0010 and a & 0b0001)): 
        output |= 0b0100 # if a and b conflict (0, 1) or (1, 0), x is included.

'''   VDD
       |
   __*| 1
  |    | <- tran_1_result
A-|    |_____ Output
  |    | <- tran_2_result
  |___| 2
       |
      GND    '''

def not_gate_logic(a, fault):
    #asserting the result after the first transistor
    if (fault == 1):
        #transistor 1 is slow, so it will product the same result twice
        tran_1_result = (pmos_transistor(a[0], a[0]))
    else: 
        tran_1_result = (pmos_transistor(a[0], a[1]))

    #asserting the result after the second transistor
    if (fault == 2):
        #transistor 1 is slow, so it will product the same result twice
        tran_2_result = (nmos_transistor(a[0], a[0]))
    else: 
        tran_2_result = (nmos_transistor(a[0], a[1]))
    
    output = (0, 0)
    

'''
       VDD
        |
     ___|____
    |        |
A-*| 1   B-*| 2
    |________|_____ Output
        |
     A-| 3
        |
     B-| 4
        |
       GND
'''

def nand_gate(a, b, fault):
    match (fault):
        case 0:
            return a & b
        
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

