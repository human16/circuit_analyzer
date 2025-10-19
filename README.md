# Functions:

## Transistor functions

These functions takes in one variable `input` which is expected to be from the set {`"0"`, `"1"`, `"x"`}

The functions can return the tuples `("0")`, `("1")`, or `("0", "1")`

The following assumptions are made:
* if `input == x` the function will return the result for input `"0"` and `"1"`, which will always be the tuple `("0", "1")`

* `input` cannot be `z`, because if it is, the gate testing stage will supply the previous `input` (which is the assumption for `z` as a transistor "input")

### pmos_transistor()
Imitates the pmos logic

### noms_transistor()
Imitates the nmos logic

