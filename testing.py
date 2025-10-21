from analysis import *

# Try to use colorama for cross-platform color support; otherwise fall back to ANSI escapes.
try:
    import colorama
    colorama.init()
    RED = colorama.Fore.RED
    GREEN = colorama.Fore.GREEN
    YELLOW = colorama.Fore.YELLOW
    BLUE = colorama.Fore.BLUE
    RESET = colorama.Style.RESET_ALL
except Exception:
    # ANSI escapes (may work on many terminals); if that fails, use empty strings.
    RED = "\x1b[31m"
    GREEN = "\x1b[32m"
    YELLOW = "\x1b[33m"
    BLUE = "\x1b[34m"
    RESET = "\x1b[0m"

if __name__ == "__main__":
    for a_0 in range(2):
        for b_0 in range(2):
            for a_1 in range(2):
                for b_1 in range(2):
                    for fault in range(5):
                        result = nand_gate((a_0+1, a_1+1), (b_0+1, b_1+1), fault)
                        if (result[1] & 0b1100):
                            result = "(" + str(binary_to_units(int(result[0]))) + ", " + str(binary_to_units(int(result[1]))) + ")"
                            print(f"Nand | a_0: {YELLOW}{a_0}{RESET}, b_0:{YELLOW}{b_0}{RESET}, a_1:{YELLOW}{a_1}{RESET}, b_1:{YELLOW}{b_1}{RESET}, fault:{YELLOW}{fault}{RESET}, results: {result}")
    print("\n\n\n\n")
    for a_0 in range(2):
        for b_0 in range(2):
            for a_1 in range(2):
                for b_1 in range(2):
                    for fault in range(5):
                        result = nor_gate((a_0+1, a_1+1), (b_0+1, b_1+1), fault)
                        if (result[1] & 0b1100):
                            result = "(" + str(binary_to_units(int(result[0]))) + ", " + str(binary_to_units(int(result[1]))) + ")"
                            print(f"Nor | a_0: {YELLOW}{a_0}{RESET}, b_0:{YELLOW}{b_0}{RESET}, a_1:{YELLOW}{a_1}{RESET}, b_1:{YELLOW}{b_1}{RESET}, fault:{YELLOW}{fault}{RESET}, results: {result}")