# memo of isa 3

## 3.1.1

> To illustrate the concepts of static analysis, we do not require a language with a very large set of features.

## 3.1.2

There're several kind of semantics( also called concrete semantics in the rest of this chapter ).
For instance, __trace semantics__, __denotational semantics__.

Before we can select which semantics to use, we discuss the family of properties of interest.

### Family of properties of interest

We focus on __reachability__ properties as in chapter 2.
Examples:

1. absence of run-time errors
2. verification of user assertions
   - execution should reach assertion point but should not meet the assertion condition

> Other classes of properties will be addressed in chapter 9.

Checking reachability properties would be:

1. pre-condition -> post-condition
2. check post-condition

Focusing the post-condition would be enough.

### An Input-Output Semantics

Input-output semantics, which characterize the effect of executing a program using a mathematical function that maps input states into corresponding output state.

> program semantics : set of input states -> set of output states

Note that `input` is non-deterministic execution( we bay observe infinitely many output states, so output is a set of states).
Input is also a set of states for compositionality.

This is a very basic form of __denotational__ semantics. This behaves like __interpreter__.

Definition of interpreter would be:

> program and input state -> output state

This definition is very close to that of input-output semantics.
The main difference is that interpreter inputs single state and output single state.
Besides this point, the interpreter essentially implements the input-output semantics.

**compositional** : the semantics of a command composed of several sub-commands(e.g. sequence or conditional) can be defined simply by composing the semantics of the sub-commands.

e.g.:

> C := C_1; C_2

Semantics of C can be defined by that of C_1 and C_2.
