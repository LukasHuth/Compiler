# Compiler

## After I ran into some problems with the code generation in my Rust Compiler, I decided to fully restart in C and see how good I can do it there to potentially make it better, because I now know for what things I have to look.

## compilation of .ll file

```
llc -filetype=obj out.ll -o out.o
clang out.o -o out
```

## the basic syntax will be:

### functions
`func <name>(<argname>: <type>): <type> -> { <body> }`

### declaration
`<name>: <type>;`

### initialization
`<name>: <type> = <value>;`

## TODO

- syntatic analysis test if start function is int and existing