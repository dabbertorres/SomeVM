# SomeVM
=======

A register-based bytecode interpreter VM.

Has two modes:
  * REPL mode (though the Print part is manual!).
  * Run mode(?) (like reading from a file and just running it, etc)

~~REPL mode is functional,~~ with a currently assembly-like sort of intermediate language (IL).
(Apparently I removed REPL mode at some point...)
example hello world:
```asm
  > load $0 "hello, world"
  > print $0
  "hello, world"
  > 
```

Run mode is functional, using the same IL (hesitantly named "SVML"):
```asm
load $0 "hello, world"
print $0
```

Basic arithmetic (+, -, *, /, %) is supported, as well as logical operators (!, <, <=, >, >=, ==, !=).
Also, a "print" instruction.

Basic syntax:
* integers prepended with a '$' is an index to a register.
* Anything between a pair of double quotes (") is a string
* "true" and "false" (no quotes) are bools.
* Instructions and their arguments are simply separated by spaces

Current VM instructions:
* \<instruction\> - \<description\>
  * \<argument list\>
* load - loads a constant or another the value of another register to the given register
  * register to load to
  * a value OR another register to load from

* add - adds the values at the two registers and stores the result
  * register to write to
  * first register to add
  * second register to add
* sub - subtracts the values at the two registers and stores the result (first - second)
  * register to write to
  * first register to subtract
  * second register to subtract
* mult - multiplies the values at the two registers and stores the result
  * register to write to
  * first register to multiply
  * second register to multiply
* div - divides the values at the two registers and stores the result (first / second)
  * register to write to
  * first register to divide
  * second register to divide
* mod - performs modulus on the values at the two registers and stores the result (first % second)
  * register to write to
  * first register to perform modulo with
  * second register to perform modulo with
* neg - negates (equivalent to multiplying by -1) the value at the given register and stores the result
  * register to write to
  * register with the value to negate

* fadd - adds the (floating point) values at the two registers and stores the result
  * register to write to
  * first register to add
  * second register to add
* fsub - subtracts the (floating point) values at the two registers and stores the result (first - second)
  * register to write to
  * first register to subtract
  * second register to subtract
* fmult - multiplies the (floating point) values at the two registers and stores the result
  * register to write to
  * first register to multiply
  * second register to multiply
* fdiv - divides the (floating point) values at the two registers and stores the result (first / second)
  * register to write to
  * first register to divide
  * second register to divide
* fneg - negates (equivalent to multiplying by -1) the value at the given register and stores the result
  * register to write to
  * register with the value to negate

* casti - casts a value to an integer
  * register to write to
  * register to read from
* castf - casts a value to a floating point value
  * register to write to
  * register to read from

* lt - stores the result of first < second
  * register to write to
  * first register to compare with
  * second register to compare with
* lteq - stores the result of first <= second
  * register to write to
  * first register to compare with
  * second register to compare with
* gt - stores the result of first > second
  * register to write to
  * first register to compare with
  * second register to compare with
* gteq - stores the result of first >= second
  * register to write to
  * first register to compare with
  * second register to compare with
* eq - stores the result of first == second
  * register to write to
  * first register to compare with
  * second register to compare with
* neq - stores the result of first != second
  * register to write to
  * first register to compare with
  * second register to compare with

* flt - stores the result of (floating point) first < second
  * register to write to
  * first register to compare with
  * second register to compare with
* flteq - stores the result of (floating point) first <= second
  * register to write to
  * first register to compare with
  * second register to compare with
* fgt - stores the result of (floating point) first > second
  * register to write to
  * first register to compare with
  * second register to compare with
* fgteq - stores the result of (floating point) first >= second
  * register to write to
  * first register to compare with
  * second register to compare with
* feq - stores the result of (floating point) first == second
  * register to write to
  * first register to compare with
  * second register to compare with
* fneq - stores the result of (floating point) first != second
  * register to write to
  * first register to compare with
  * second register to compare with

* not - inverts the boolean value at the given register and stores the result
  * register to write to
  * register to invert
* and - boolean ands the values at the given registers and stores the result
  * register to write to
  * register of first boolean
  * register of second boolean
* or - boolean ors the values at the given registers and stores the result
  * register to write to
  * register of first boolean
  * register of second boolean
* xor - boolean xors the values at the given registers and stores the result
  * register to write to
  * register of first boolean
  * register of second boolean

* bnot - bitwise inverts the value at the given register and stores the result
  * register to write to
  * register to invert
* band - bitwise ands the values at the given registers and stores the result
  * register to write to
  * register of first value
  * register of second value
* bor - bitwise ors the values at the given registers and stores the result
  * register to write to
  * register of first value
  * register of second value
* bxor - bitwise xors the values at the given registers and stores the result
  * register to write to
  * register of first value
  * register of second second
* bsl - bitwise shifts-left the value at the given registers and stores the result
  * register to write to
  * register of value to shift
  * register of amount to shift by
* bsr - bitwise shifts-right the value at the given registers and stores the result
  * register to write to
  * register of value to shift
  * register of amount to shift by

* jmpt - if the test is true, jumps to the given instruction index (only within the current call frame)
  * registry index to test if true
  * instruction index to jump to
* jmpf - if the test is false, jumps to the given instruction index (only within the current call frame)
  * registry index to test if false
  * instruction index to jump to
* rjmpt - if the test is true, jumps to a instruction by the given value (only within the current call frame)
  * registry index to test if true
  * instruction offset to jump by
* rjmpf - if the test is false, jumps to a instruction by the given value (only within the current call frame)
  * registry index to test if false
  * instruction offset to jump by
* jmp - moves execution to the specified instruction (only within the current call frame)
  * instruction index
* rjmp - moves execution to the instruction at the given offset (only within the current call frame)
  * offset to jump by

* ~~call - creates a new call frame and moves execution to it~~
  * number of arguments to send to the function
  * registry index of the start of the arguments
  * function index (TODO: or function name)
* ~~ret - returns from the current call frame to the previous (if no previous, exits)~~
  * number of values to return
  * registry index of start of returns

* nop - "No operation" - self explanatory
  * any arguments are ignored
* print - prints out the value at the given register
  * register to print from
