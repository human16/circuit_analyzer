# Sections:
[Functions](#functions)

[Data Type](#data-type)

# Data Type

In this simulation, instead of only considering `1` and `0`, we need to also consider `z` and `x` (high impedence and variable value), to do this, along with considering `1` and `0`, we will use a bitwise operation on a 4 bit number `n`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`z x 1 0`<br>
`n = _ _ _ _` where `n` is our number and we can use bitwise operations to check if a value is `true` or `false`

Example:
Say we want to express the set of `{z, 1, 0}`, this would make our `n = 1011`. Now, we can convert to base-10, where we would have `n = 11`.


# Functions:
[Transitor](#transistor-functions)

[Gate Logic](#gate-logic-functions)

## Transistor Functions

[pmos](#pmos_transistor)

[nmos](#nmos_transistor)

These functions takes in one variable `input` which is expected to be from the set {`0`, `1`, `x`}


The following assumptions is made:

* `input` cannot be `z`, because if it is, the gate testing stage will supply the previous `input` (which is the assumption for `z` as a transistor "input")

### pmos_transistor()
Imitates the pmos logic

Since pmos transistors are always connected to VDD (1), we can make assumptions about their logic

The output for each input will be:

* `0`: transistor is connected, `0010` (`1`)
* `1`: transistor is disconnected, `1000` (`z`)
* `x`: transistor is either closed or opened, `1010` (`1`, `z`)


### nmos_transistor()
Imitates the nmos logic

Since nmos transistors are always conencted to GND (0), we can make assumptions about their logic

The output for each input will be:

* `0`: transistor is disconnected, `1000` (`z`)
* `1`: transistor is connected, `0001`(`1`)
* `x`: transistor is either closed or opened, `1001` (`0`, `z`)

## Gate Logic Functions

These functions take initial gate inputs and secondary inputs in a tuple along with the fault input.

The fault input will determine which transistor will be simulated as fault according to the design diagram above the function. We will use `input == 0` to denote no faults. If the input is not within the valid range of transistors, it will be assigned to `0`

These functions will output a tuple `(initial_result, secondary_result)`