# Compiler Construction course 

## Table content 
- [Language](#dynamic-language)
- [Lexical](#lexical-analysis)
- [Syntax](#syntax-analysis)
- [Semantics](#semantic-analysis)
- [Interpretator](#interpretation)

## Dynamic Language 

[Description](ProjectD.pdf)

## Language
The project involves the construction of a compiler for a custom language designed specifically for educational purposes in a Compiler Construction course.

## Lexical Analysis
### Key Words
if, in, is, or, and, end, for, int, not, var, xor, bool, else, func, loop, real, then, true, empty, false, print, while, return, string

Lexical analysis is implemented in the folder `src/lexical`. This component handles the process of converting source code into tokens, recognizing keywords, identifiers, and other lexical units.

## Syntax Analysis
Our Abstract Syntax Tree (AST) parser employs a bottom-up approach. Implementation required a set of functions for each expression type, facilitating recursive parsing of nested structures.

## Semantics
Semantic analysis involves a function that accepts the AST as a parameter. It initializes a state structure that manages expressions on the stack, frame information, closures, and related fields.

## Interpreter
Following semantic analysis, the interpreter is invoked using the `run_interpreter` function. This interpreter accepts the AST and initializes an execution state (`op_state`), responsible for managing operations, error handling, return objects, and memory (frame, stack, temp).

The interpreter traverses nodes and operations, executing actions based on their type:

* For nodes, actions correspond to main type expressions, operations, and statements.
* For blocks, it checks constructs like conversions, arithmetic operations (e.g., add, multiply), and their subtypes.

## Dynamic Language
The project's language is designed to be dynamic, supporting various constructs and types, with a focus on educational aspects of compiler construction.

## Description
This repository contains the implementation of a compiler for a custom language developed as part of a Compiler Construction course. It includes components for lexical analysis (src/lexical), syntax analysis utilizing a bottom-up AST parser, semantic analysis for validating code semantics, and an interpreter for executing parsed code. The language supports dynamic features and is designed to facilitate learning key concepts in compiler design and implementation.