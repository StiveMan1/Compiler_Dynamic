# Compiler Construction course 

## Table content 
- [Language](#dynamic-language)
- [Lexical](#lexical-analysis)
- [Syntax](#syntax-analysis)
- [Semantics](#semantic-analysis)
- [Interpretator](#interpretation)

## Dynamic Language 

[Description](ProjectD.pdf)

## Lexical analysis

### Key words
`if in is or and end for int not var xor bool else func loop real then true empty false print while return string`

The Lexical analysis implemented in folder `src/lexical`.

## Syntax analysis

Our AST parser utilizes bottom-up approach. For this we had to implement a list of functions for all expression types that recursively call each other.

In the tree each node represents a construct met in the text. Each node has child tokens and other nodes as well as its value if there is any.

## Semantic analysis

Semantic scan function accepts AST as a parameter. At the beginning it creates a new state.

State - struct that stores expressions in the stack, information about frame, closure and other fields.

## Interpretation 

After performing a semantic analysis, we launch an interpreter by calling run_interpreter function. It accepts AST as a parameter and copies its content to newly created op_state.

State stores a list of operations to traverse, informations about error and return objects, and memories for frame, stack and temp.

Then, it starts traversing nodes and operations, and performs actions depending on their type: Node or Block.

For Node we deal with MainType expressions, operations and statements.

For Block we check such BlockType constructs as Convert, Arithmetics, etc. Some of them can have subtypes, for example, in Arithmetics we have add, multiply, etc.
