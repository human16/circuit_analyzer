# input -*|

def pmos_transistor(input):
    match (input) :
        case "0":
            return ("1")
        
        case "1":
            return ("0")

        case "x":
            return ("0", "1")
        
        case default:
            raise Exception("Invalid input to pmos transistor")


# input -|
def nmos_transistor(input):
    match (input) :
        case "0":
            return ("0")
        
        case "1":
            return ("1")

        case "x":
            return ("0", "1")
        
        case default:
            raise Exception("Invalid input to nmos transistor")



'''
      VDD
       |
   __*| 1
  |    |
A-|    |_____ Output
  |    |
  |___| 2
       |
      GND    


'''

def not_gate_logic(a, fault):
    match (fault): 
        case 0:
            pass

        case 1:
            pass

        case 2:
            pass

        case default:
            raise Exception("NOT only has 2 transistors but fault != {0, 1, 2}")

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

