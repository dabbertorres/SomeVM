# SomeVM
A register-based bytecode interpreter VM.

Has two modes:
  * REPL mode (though the Print part is manual!).
  * Run mode(?) (like reading from a file and just running it, etc)

REPL mode is functional, with a currently assembly-like sort of intermediate language (IL).
example hello world:
```asm
  > const "hello, world"
  > load $0 %0
  > print $0
  "hello, world"
  > 
```

Run mode is functional, using the same IL (hesitantly named "SVML"):
```asm
const "hello, world"
load $0 %0
print $0
```

Basic arithmetic (+, -, *, /, %) is supported, as well as logical operators (!, <, <=, >, >=, ==, !=).
Also, a "print" instruction.

Basic syntax:
* Any integer prepended with a '$' is a register index.
* Anything between a pair of double quotes (") is a string
* "true" and "false" (no quotes) are bools.
* Instructions and their arguments are simply separated by spaces

Current VM instructions:
* \<instruction\> - \<description\>
  * \<argument list\>
* load - loads a constant or another the value of another register to the given register
  * register to load to
  * index into the constants list OR another register to load
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
* not - inverts the boolean value at the given register and stores the result
  * register to write to
  * register to invert
* less - stores the result of first < second
  * register to write to
  * first register to compare with
  * second register to compare with
* lesseq - stores the result of first <= second
  * register to write to
  * first register to compare with
  * second register to compare with
* gret - stores the result of first > second
  * register to write to
  * first register to compare with
  * second register to compare with
* greteq - stores the result of first >= second
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
* if - next instruction is a jump, executed if argument is false (the "else"). if argument is true, jump is skipped (the "then"). If an "else" exists, the end of the "then" will have a jump
  * registry index to test
* call - creates a new stack frame and moves execution to it
  * function index (will prolly change)
* ret - returns from the current stack frame to the previous (if no previous, exits)
  * starting register to return values from
  * ending register to return values from (inclusive). If larger than 255, nothing is returned
* jump - moves execution to the specified instruction (only within the current stack frame)
  * instruction index (if out of bounds, instantly returns from the current stack frame)
* noop - "No operation" - self explanatory
  * any arguments are ignored
* print - prints out the type and the value of the value at the specified register
  * register to print from
