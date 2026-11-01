# Brainfuck Interpreter / Transpiler (C++)

A small console tool for the Brainfuck esoteric language. Two modes:

1. **Interpret** — runs Brainfuck code right away
2. **Transpile** — converts Brainfuck code into a C++ file (`out.cpp`),
   which compiles with any C++ compiler and runs on its own

Brainfuck is an esoteric language with only 8 commands, operating on a tape
of 30,000 byte cells and a data pointer. Despite its minimalism it is
Turing-complete. See [esolangs.org/wiki/Brainfuck](https://esolangs.org/wiki/Brainfuck).

## Build

```
g++ BrainFuck.cpp -o bf
```

## Usage

```
./bf
```

The tool asks for a mode (`1` — transpile, `2` — interpret), then reads
one line of Brainfuck code.

Example (mode 2, Hello World):

```
What do you want to do?
2
Enter your code:
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
Hello World!
```

Example (mode 1): the tool writes `out.cpp`, which can be built and run
separately, without the tool:

```
g++ out.cpp -o out
./out
```

## Features

- Two modes in one binary: interpreter and BF → C++ transpiler
- Bracket pairs (`[`, `]`) are precomputed before execution; unbalanced
  brackets are reported (`Wrong Cycle`, exit code 1)
- Step limit (10,000,000 instructions) stops infinite loops like `+[]`
  (`Too many!`, exit code 2)
- EOF handling: end of input stores 0 in the cell, so loops like `,[.,]`
  terminate by themselves
- Ring tape: the pointer wraps around at both tape boundaries
- 8-bit cells with wrap-around on overflow (255 + 1 = 0)

## How it works

**Interpreter.** The tape is a `std::vector<unsigned char>`, the pointer `p`
is the data address, the loop index `i` is the instruction pointer. Before
execution, all bracket pairs are found and stored in a jump table, so `[` and
`]` become O(1) jumps instead of a re-scan on every loop iteration.

**Transpiler.** Every Brainfuck symbol is translated into one C++ statement
(`+` → `tape[p]++;`, `[` → `while (tape[p]) {`, ...). Bracket pairing is
delegated to the C++ compiler itself — the transpiler emits `while` and `}`
and does not need its own jump table. The generated file is self-contained:
raw arrays, no dependencies beyond `<iostream>`.

## Roadmap

- Optional tape-bounds check instead of silent wrap-around
- Compile to custom bytecode + a small VM
- Simple optimizer: fold runs like `+++` into `tape[p] += 3`
