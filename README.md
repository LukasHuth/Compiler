# Compiler

## After I ran into some problems with the code generation in my Rust Compiler, I decided to fully restart in C and see how good I can do it there to potentially make it better, because I now know for what things I have to look.

## Disclaimer

```
Every file found in src/headers/llvm and src/headers/llvm-c is from the official llvm project and not from me.
```

## compilation of .bc file

```
clang main.bc -o output
```

## if the compilation of .ll file is needed for debug reasons

```
llc -filetype=obj main.ll -o output.o
clang output.o -o output
```

## the basic syntax will be:

### functions
`func <name>(<argname>: <type>): <type> -> { <body> }`

### declaration
`<name>: <type>;`

### initialization
`<name>: <type> = <value>;`

## TODO

- add llvm/lib [instructions here](https://llvm.org/docs/GettingStarted.html#llvm-lib)