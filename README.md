# Complain

Complain is an [XPLN]-to-LLVM compiler.


## Features

I couldn't implement all XPLN features in time. In its current state,
Complain supports the following language features:
- If statements
- While statements
- Assignments
- Arithmetic expressions
- Relational operators (EQ, GT, etc.)
- Logical operators (with the exception of 'not' being not implemented)
- Return statements (though only from the main function)
- Output statements

Complain does *not* yet support the following features:
- Function definitions
- Function calls
- Input statements
- Unary operator: not
- 'else' statement within if statements

Apart from these, following features were implemented differently
(either temporarily or permanently) from the language standard:
- Complain only supports integer type

Few things to note about the compiler:
- Error reporting is terribly inadequate. No information regarding
the error location is reported. Also some constraints of the
language may not be enforced by the compiler warnings/errors.
- Implementation of the output functionality assumes the existence of
C standard library. It makes use of 'printf'.


## Requirements

Build process has the following requirements:
- Bison
- Flex
- LLVM toolchain
- GNU Make

After satisfying the requirements, build with `make`.


## Usage:

Complain reads the input from stdin, and dumps the LLVM IR to stderr,
therefore source file and compiler output need to be properly
redirected:

```sh
$ ./cpln < input.xpln 2> output.ll
```

Then the resulting IR can be interpreted via 'lli':

```sh
$ lli output.ll
```

or it can be compiled to x86 ASM:

```sh
$ llc output.ll -o output.s
```

One thing to note is that usage of 'output' functionality requires
the compilation to have PIC relocation model:

```sh
$ llc output.ll -relocation-model=pic -o output.s
```

Finally the resulting ASM may be assembled with the assembler of choice:

```sh
$ gcc output.s -o output
```


## Testing:

Several tests are provided in the tests directory. However they do not
test all possible edge cases extensively. They are not automated tests,
rather one run them and observe whether they do "the expected thing".

[XPLN]: https://github.com/bozsahin/ceng444/blob/master/project-material/xpl-specs-fall2018.pdf

