---
marp: true
theme: custom
---

# Introduction to static analysis #3

## Seminar @ Gondow Lab.

<!--
class: title
-->

---

<!--
class: slides
_footer: ''
paginate: true
-->

# Goal of This Chapter

- The construction of a static analysis framework.
   - feature : general and can be used with different abstraction.
   - goal    : compute program invariants by static abstraction
- How to construct a static analysis step by step.
   - We use basic programming language that operates over numerical states.


---

# Outline of the book

> - 3.1 : fix the language and its semantics.(6p)
> - 3.2 : select an abstraction and fix their representation.(9p)
> - 3.3 : derive the abstract semantics of programs from their semantics and abstractions.(18p)
> - 3.4 : design of the interpreter.(2p)

---

# Overview

- Semantics (3.1)
   - Programming Language
   - Concrete Semantics
      - Concrete Semantics
      - Properties of Interest
      - Input-Output Semantics
- Abstraction (3.2)
- Computable Abstract Semantics (3.3)
- Interpreter (3.4)

---

# A Simple Programming Language (1/4)

We use simple programming language to illustrate the concepts of static analysis.

Some preparations:
- $\mathbb{X}$ : a finite set of variable( which is fixed )
- $\mathbb{V}$ : a set of scalar value
- $\mathbb{B}$ : a set of boolean value
   - $\mathbb{B} = \{ \mathbf{true}, \mathbf{false} \}$


---

# A Simple Programming Language (2/4)

Syntax of our language is:

- $n \quad \in\quad  \mathbb{V}$
- $\mathrm{x} \quad \in \quad \mathbb{X}$
- $\odot\quad  \coloncolonequals \quad + \enspace | \enspace - \enspace | \enspace * \enspace | \enspace \dots$
- $\lessdot \quad \coloncolonequals \quad < \enspace | \enspace \leq \enspace | \enspace == \enspace | \enspace \dots$
- $E \quad \coloncolonequals \quad n \enspace | \enspace \mathrm{x} \enspace | \enspace E \odot E$
- $B \quad \coloncolonequals \quad \mathrm{x} \lessdot n$
   - returns an element of $\mathbb{B}(= \{\mathbf{true}, \mathbf{false}\})$
- $C \quad \coloncolonequals \quad \mathbf{skip} \enspace | \enspace C; C \enspace | \enspace \mathrm{x} \colonequals E \enspace | \enspace \mathbf{input}(\mathrm{x}) \enspace | \enspace \mathbf{if}(B)\{C\} \mathbf{else}\{C\}$
- $P \quad \coloncolonequals \quad C$
---

# A Simple Programming Language (3/4)

- $n \quad \in\quad  \mathbb{V}$
   - scalar values
- $\mathrm{x} \quad \in \quad \mathbb{X}$
   - program variables
- $\odot\quad  \coloncolonequals \quad + \enspace | \enspace - \enspace | \enspace * \enspace | \enspace \dots$
   - binary operators
- $\lessdot \quad \coloncolonequals \quad < \enspace | \enspace \leq \enspace | \enspace == \enspace | \enspace \dots$
   - comparison operators
- $E \quad \coloncolonequals \quad n \enspace | \enspace \mathrm{x} \enspace | \enspace E \odot E$
   - scalar expressions
---

# A Simple Programming Language (4/4)

- $B \quad \coloncolonequals \quad \mathrm{x} \lessdot n$
   - returns an element of $\mathbb{B}(= \{\mathbf{true}, \mathbf{false}\})$
   - Boolean expressions
- $C \quad \coloncolonequals \quad \mathbf{skip} \enspace | \enspace C; C \enspace | \enspace \mathrm{x} \colonequals E \enspace | \enspace \mathbf{input}(\mathrm{x}) \enspace | \enspace \mathbf{if}(B)\{C\} \mathbf{else}\{C\}$
   - commands
- $P \quad \coloncolonequals \quad C$
   - program

---

# Concrete Semantics

There're several kind of semantics. For instance, __trace semantics__, __denotational semantics__.

- __trace semantics__ : describes program execution as a sequence of program state
- __denotational semantics__ : describes only input-output relation

Before we can select which semantics to use, we discuss the family of properties of interest.

---

# Properties of Interest

As in chapter 2, we focus on __reachability__ properties.

Examples:
1. absence of run-time errors
2. verification of user assertions
   - execution should reach assertion point but should not meet the assertion condition

More general properties will be addressed in chapter 9.

---

# Properties of Interest - reachability

Checking reachability properties would be:

1. pre-condition $\rightarrow$ post-condition ($\leftarrow$We need a semantic that capture this)
2. check post-condition

So we use **input-output semantics**( one of denotational semantics ).

---

# An Input-Output Semantics


- Input-output semantics :
   - set of input states $\longmapsto$ set of output states
   - use mathematical function to map
   - output is a set of states because:
      - of the non-deterministic execution of **input**
      - we may observe infinitely many output states from one input
   - input is also a set of states
      - for the sake of compositionality

<note>
**compositional** : the semantics of a command can be defined by composing the semantics of its sub-commands.

> $C \colonequals C_1; C_2$
</note>

---

# An Input-Output Semantics - compositionality

- Input-Output Semantics compositional.

<note>
compositional : the semantics of a command can be defined by composing the semantics of its sub-commands.

e.g
> $C \colonequals C_1; C_2$

Semantics of $C$ is defined by that of $C_1$ and $C_2$.
</note>

---

# An Input-Output Semantics vs Interpreter

Input-output Semantics and Interpreter have much in common:
- input-output : set of input state**s** $\longmapsto$ set of output state**s**
- interpreter  : a program and an input state $\longmapsto$ an output state

The main difference is:
- interpreter : inputs a single state and returns a single state

Essentially, interpreter implements the input-output semantics.
