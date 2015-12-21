# SomeVM
A register-based bytecode interpreter VM.

Currently will run only in REPL mode (though the Print part is manual!).

REPL mode is functional, with a currently assembly-like sort of intermediate language (IL).
example hello world:
```asm
  load $0 "hello, world"
  print $0
```

Basic arithmetic (+, -, *, /, %) is supported, as well as logical operators (!, <, <=, >, >=, ==, !=).

Basic syntax:
* Any integer prepended with a '$' is a register index.
* Anything between a pair of double quotes (") is a string
* "true" and "false" (no quotes) are bools.
* Instructions and their arguments are simply separated by spaces

Current instructions:
* \<instruction\> - \<description\>
  * \<argument list\>
* print - prints out the type and the value of the value at the specified register
  * register to print from
* load - loads the provided literal or constant index to a register
  * register to load to
  * literal to load to the constants list OR an index into the constants list to load
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
