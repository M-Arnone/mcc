# Notes — *Writing a C Compiler* (Nora Sandler)

My understanding notes, chapter by chapter, while building `mcc`.

## Table of Contents

- [Introduction](#introduction)
- **Part I: The Basics**
  - [1. A Minimal Compiler](#1-a-minimal-compiler)
    - [1.1 The four stages and the driver](#11-the-four-stages-and-the-driver)
    - [1.2 The lexer](#12-the-lexer)
    - [1.3 The parser](#13-the-parser)
    - [1.4 Assembly generation](#14-assembly-generation)
    - [1.5 Code emission](#15-code-emission)
  - [2. Unary Operators](#2-unary-operators)
  - [3. Binary Operators](#3-binary-operators)
  - [4. Logical and Relational Operators](#4-logical-and-relational-operators)
  - [5. Local Variables](#5-local-variables)
  - [6. If Statements and Conditional Expressions](#6-if-statements-and-conditional-expressions)
  - [7. Compound Statements](#7-compound-statements)
  - [8. Loops](#8-loops)
  - [9. Functions](#9-functions)
  - [10. File-scope Variables and Storage-class Specifiers](#10-file-scope-variables-and-storage-class-specifiers)
- **Part II: Types Beyond Int**
  - [11. Long Integers](#11-long-integers)
  - [12. Unsigned Integers](#12-unsigned-integers)
  - [13. Floating-Point Numbers](#13-floating-point-numbers)
  - [14. Pointers](#14-pointers)
  - [15. Arrays and Pointer Arithmetic](#15-arrays-and-pointer-arithmetic)
  - [16. Characters and Strings](#16-characters-and-strings)
  - [17. Supporting Dynamic Memory Allocation](#17-supporting-dynamic-memory-allocation)
  - [18. Structures](#18-structures)
- **Part III: Optimizations**
  - [19. Optimizing TACKY Programs](#19-optimizing-tacky-programs)
  - [20. Register Allocation](#20-register-allocation)
- [Next Steps](#next-steps)
- [Appendix A: Debugging Assembly Code with GDB or LLDB](#appendix-a-debugging-assembly-code-with-gdb-or-lldb)
- [Appendix B: Assembly Generation and Code Emission Tables](#appendix-b-assembly-generation-and-code-emission-tables)

---

## Introduction

_To do._

---

# Part I: The Basics

## 1. A Minimal Compiler

Chapter goal: compile the simplest possible program.

```c
int main(void) {
    return 2;
}
```

### 1.1 The four stages and the driver

The **driver** (`main` in `mcc.c`) orchestrates everything:

1. **Preprocessing**: `gcc -E -P file.c -o file.i` (delegated to gcc).
2. **Compilation** (my job): lexer → parser → assembly generation → emit the `.s` file.
3. **Assembling and linking**: `gcc file.s -o file` (delegated to gcc).

The `--lex`, `--parse` and `--codegen` options stop compilation **after** the requested stage, without producing any file. Exit code 0 = success, anything else = error. The book's test suite checks these exit codes.

Each external stage runs in a child process (`fork` + `execlp`), and the parent waits for it with `waitpid`.

### 1.2 The lexer

_To do._

#### Reading the file: two separate steps

_To do._

#### The algorithm: a read head that moves forward

_To do._

#### Storing a token

_To do._

#### C pitfalls I ran into

_To do._

### 1.3 The parser

_To do._

### 1.4 Assembly generation

_To do._

### 1.5 Code emission

_To do._

---

## 2. Unary Operators

_To do._

Question to think about: the `--` token requires looking at **two** characters (`p[0]` and `p[1]`). Why does loading the whole file into memory make this easy?

## 3. Binary Operators

_To do._

## 4. Logical and Relational Operators

_To do._

## 5. Local Variables

_To do._

## 6. If Statements and Conditional Expressions

_To do._

## 7. Compound Statements

_To do._

## 8. Loops

_To do._

## 9. Functions

_To do._

## 10. File-scope Variables and Storage-class Specifiers

_To do._

---

# Part II: Types Beyond Int

## 11. Long Integers

_To do._

## 12. Unsigned Integers

_To do._

## 13. Floating-Point Numbers

_To do._

## 14. Pointers

_To do._

## 15. Arrays and Pointer Arithmetic

_To do._

## 16. Characters and Strings

_To do._

## 17. Supporting Dynamic Memory Allocation

_To do._

## 18. Structures

_To do._

---

# Part III: Optimizations

## 19. Optimizing TACKY Programs

_To do._

## 20. Register Allocation

_To do._

---

## Next Steps

_To do._

## Appendix A: Debugging Assembly Code with GDB or LLDB

_To do._ (On macOS: LLDB.)

## Appendix B: Assembly Generation and Code Emission Tables

_To do._
