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

- The construction of a _static analysis framework_.
   - feature : general, can be used with different abstraction.
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

# A Simple Programming Language (1/2)

We use simple programming language to illustrate the concepts of static analysis.

Some preparations:
- $\mathbb{X}$ : a finite set of variable( which is fixed )
- $\mathbb{V}$ : a set of scalar value
- $\mathbb{B}$ : a set of boolean value
   - $\mathbb{B} = \{ \mathbf{true}, \mathbf{false} \}$


---

# A Simple Programming Language (2/2)

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

# A Simple Programming Language (3/2)

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

# A Simple Programming Language (4/2)

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

So we use ***input-output semantics***( one of denotational semantics ).

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

---

# An Input-Output Semantics - compositionality

- Input-Output Semantics ___compositional___.

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
- interpreter : inputs a ***single*** state and returns a ***single*** state

Essentially, interpreter implements the input-output semantics.

---

# Memory States(1/2)

- _program state_ should include:
   - _memory state_ : contents of the memory
   - _control state_ : a value of "program counter"( or next command to be executed )
- a state is defined by a memory state:
   - we use input-output semantics
   - input( output ) state is fully determined by the contents of memory

---

# Memory States(2/2)

- memory state $\mathbb{M}$ is defined by:
   - $\mathbb{M} = \mathbb{X} \longrightarrow \mathbb{V}$

example:

- $\mathbb{X} = \{\mathrm{x}, \mathrm{y}\}$
   - $\mathrm{x}$ : 2, $\mathrm{y}$ : 7
- $m \in \mathbb{M}$ is:
   - $m = \{ \mathrm{x} \mapsto 2, \mathrm{y} \mapsto 7\}$

<note>
簡単に言うと、memory stateとは変数からスカラー値のマッピングのこと
</note>

---

# Semantics of Scalar Expressions

How scalar expressions are evaluated.

- $\llbracket E \rrbracket(m)$ : semantics of expression $E$, in the memory state $m$.
   - $\llbracket E \rrbracket : \mathbb{M} \longrightarrow \mathbb{V}$
      - This is a function from memory states to scalar values

Semantics of each scalar expression is as follows:
- $\llbracket n \rrbracket (m) = n$
- $\llbracket \mathrm{ x } \rrbracket (m) = m(\mathrm{ x })$
   - $m(\mathrm{x})$ : value of x in the memory state $m$
- $\llbracket E_0 \odot E_1 \rrbracket (m) = f_{\odot}(\llbracket E_0 \rrbracket (m), \llbracket E_1 \rrbracket (m))$
   - $f_{\odot}$ : mathematical function associated to the binary operator $\odot$

---

# Semantics of Boolean Expressions

How Boolean expressions are evaluated.

- $\llbracket B \rrbracket = \mathbb{M} \longrightarrow \mathbb{B}$
   - This is a function from memory states to boolean values
- $\llbracket \mathrm{x} \lessdot n \rrbracket = f_{\lessdot}(m(\mathrm{x}), n)$
   - $f_{\lessdot}$ : mathematical function associated to the comparison operator $\lessdot$

---

# Semantics of Commands (1/6)

- $\llbracket C \rrbracket_{\mathscr{P}}$ : semantics of a command $C$
   - a set of input states to a set of output states( which is observed __after__ the command )
      - non-terminating executions are not observed
- $\wp(\mathbb{ M })$ : power set of memory states
   - intuitive explanation : "whether or not each variable is defined"
   - $M$ : an element of $\wp(\mathbb{ M })$, that is:
      - $M \in \wp(\mathbb{M})$

As a result, semantics of commands can be written as follows:

- $\llbracket C \rrbracket_{\mathscr{P}}$ : $\wp(\mathbb{ M }) \longrightarrow \wp(\mathbb{ M })$ 

---

# Semantics of Commands (2/6)

Semantics of commands is:

- $\llbracket \texttt{skip} \rrbracket_{\mathscr{P}}(M) = M$
   - identity function
- $\llbracket C_0 ; C_1 \rrbracket_{\mathscr{P}}(M) = \llbracket C_1 \rrbracket_{\mathscr{P}}(\llbracket C_0 \rrbracket_{\mathscr{P}}(M))$
   - composition of the semantics of each commands
- $\llbracket \mathrm{x} \colonequals E \rrbracket_{\mathscr{P}}(M) = \{m[\mathrm{x} \mapsto \llbracket E \rrbracket(m)] \enspace | \enspace m \in M\}$
   - the evaluation of assignment updates the value of $\mathrm{x}$ in the memory states with the result of the evaluation of $E$.
- $\llbracket \texttt{input} (\mathrm{x}) \rrbracket_{\mathscr{P}}(M) = \{ m[ \mathrm{x} \mapsto n ] \enspace | \enspace m \in M, n \in \mathbb{V} \}$
   - replace the value of $x$ with any possible scalar value $n$.

Quite easy.

---

# Semantics of Commands (3/6)

Before we define semantics of $\texttt{if-else}$ or $\texttt{while}$, we need some preparations.
- $\mathscr{F}_B$ : filtering function. We need to define this first.
   - This function filter out memory states

Definition is as follows:
- $\mathscr{F}_{B}(M) = \{m \in M \enspace | \enspace \llbracket B\rrbracket(m) = \mathbf{true}\}$
   - intuitive explanation : filter out memory states $m$ in which $B$ doesn't hold or can't be defined

---

# Semantics of Commands (4/6)

Semantics of $\texttt{if-else}$:
- $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket_{\mathscr{P}}(M) = \llbracket C_0 \rrbracket_{\mathscr{P}}(\mathscr{F}_{B}(M)) \cup \llbracket C_1 \rrbracket_{\mathscr{P}}(\mathscr{F}_{\neg B}(M))$
   - union of the results of each branch

---

# Semantics of Commands (5/6)

Semantics of $\texttt{while}$:
- $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \big)$
   - complicated...

Let $M_i$ be as follows:
- $M_i = \mathscr{F}_{\neg B} \big( ( \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B )^i (M) \big)$
   - intuitive explanation : $B$ evaluates to __true__ $i$ times and to __false__ for the last.
   - $\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B$ : filter memory states with $B$, then execute the command.

---

# Semantics of Commands (6/6)

> Semantics of $\texttt{while}$:
> - $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \big)$
>    - complicated...

Then, the set of output states would be $M_0 \cup M_1 \cup M_2 \dots$, that is :
- $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \cup_{i \geq 0} M_i = \cup_{i \geq 0} \mathscr{F}_{\neg B} \big( (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^i (M) \big)$

$\mathscr{F}_B$ commutes with the union, thus:
- $\cup_{i \geq 0} M_i = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^i (M) \big)$

Therefore,

- $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \big)$

---

# Overview

- Semantics (3.1)
- Abstraction (3.2)
   - The concept of abstraction
   - Non-relational abstraction
   - Relational abstraction
- Computable Abstract Semantics (3.3)
- Interpreter (3.4)

---

# Concrete, Abstract

We carefully distinguish between these:
- domain the program is defined ($\longrightarrow$ "___concrete___" qualifier for this)
- domain that is used for the analysis of program ($\longrightarrow$ "___abstract___" qualifier for this)

---

# Concrete Domain

<def>
Definition: Concrete Domain
<ul>
<li>

a set $\mathbb{C}$ : concrete domain, describes concrete behaviors
</li>
<li>

$\subseteq$ : order relation, compares program behaviors in the logical point of view
</li>
<ul>
<li>

$x \subseteq y$ means that $x$ implies behavior y, that is:
</li>
<ul>
<li>

$x$ expresses a stronger property than $y$.
</li>
</ul>
</ul>
</ul>
</def>

Example:
- $\mathbb{C} = \wp (\mathbb{M})$
   <!-- - choose $\wp (\mathbb{M})$ as the concrete domain to study the sets of reachability states. -->
   - $c \in \mathbb{C}$, $c = \{\mathrm{x} \mapsto 1, \mathrm{y} \mapsto 2\}$

---

# Abstract Domain (1/3)

Some preparations:
- $c$ : concrete element
- $a$ : abstract element
- $c \vDash a$ : $c$ satisfies the logical properties expressed by $a$

---

# Abstract Domain (2/3)

<def>
Definition: Abstract Domain and Abstract Relation

- **abstract domain** : a pair of a set $\mathbb{A}$ and an ordering relation $\sqsubseteq$ over that set.

Given a concrete domain $(\mathbb{C}, \subseteq)$, **abstraction** is defined by:
- $(\mathbb{A}, \sqsubseteq)$
- an abstract relation "$\vDash$" such that:
   - for all $c \in \mathbb{C}, a_0, a_1 \in \mathbb{A},$ if $c \vDash a_0$ and $a_0 \sqsubseteq a_1,$ then $c \vDash a_1;$ and
   - for all $c_0, c_1 \in \mathbb{C}, a \in \mathbb{A},$ if $c_0 \subseteq c_1$ and $c_1 \vDash a,$ then $c_0 \vDash a$.
</def>

---

# Abstract Domain (3/3)

##### Example 3.2 (Abstraction) :

- concrete domain : $\wp (\mathbb{M})$
- variable : $\mathrm{x}, \mathrm{y}$

<div class="twocols">
<p>

Elements of concrete domain :
- $M_0 = \{m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x}) < m(\mathrm{y}) \leq 8\}$
- $M_1 = \{m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x})\}$
</p>
<p class="break">

An element of abstract domain :
- $M^{\sharp}$ : over-approximates each value
   - $\mathrm{x}$ : [0, 10]
   - $\mathrm{y}$ : [0, 100]
</p>
</div>


Then,
- $M_0 \vDash M^{\sharp}$ : any memory state in $M_0$ is included in $M^{\sharp}$.
- $M_1 \nvDash M^{\sharp}$ : $(11,0)$ is an element of $M_1$, but doesn't satisfy $M^{\sharp}$

---

# Concretization Function (1/n)

Sometimes, $"\vDash"$ is not useful. Thus, we define concretization function.

<def>
<h4>
Definition 3.3 (Concretization function)
</h4>

A concretization function (or, for short, concretization) :
- $\gamma : \mathbb{A} \rightarrow \mathbb{C}$
   - for any abstract element $a$, $\gamma (a)$ satisfies $a$. ($\gamma (a) \vDash a$)
   - $\gamma (a)$ is the maximum element of $\mathbb{C}$ that satisfies $a$
</def>

---

# Concretization Function (2/n)

- A concretization function fully describe the abstraction relation:
   - $\forall c \in \mathbb{C}, \forall a \in \mathbb{A},$ $\quad\quad c \vDash a \iff c \subseteq \gamma (a)$
- Concretization function is also monotone.

#### Example 3.3 (Concretization function)

- same notion as example 3.2. ($M^{\sharp}, M_0, M_1$)
- There are memory states in $\gamma (M^{\sharp})$ that are not in $M_1$

> - $M_1 \nvDash M^{\sharp}$ : $(11,0)$ is an element of $M_1$, but doesn't satisfy $M^{\sharp}$


---

# Abstraction Function (1/3)


<def>
<h4>
   Definition 3.4 (Abstraction function)
</h4>

$c$ has a **best abstraction** if and only if there exists $a$ such that:
- $a$ is an abstraction of $c$
- any other abstraction of $c$ is greater than $c$.

Abstraction function (or for short, abstraction):
- $\alpha : \mathbb{C} \rightarrow \mathbb{A}$
   - This function maps each concrete element to its best abstraction

</def>

Abstraction function is:

<div class="twocols">
   <p>
      <ul>
         <li>the dual of concretization function</li>
      </ul>
   </p>

   <p class="break">
      <ul>
         <li>monotone</li>
      </ul>
   </p>

</div>

---

# Abstraction Function (2/3)

#### Example 3.4 (Abstraction function)
- same notion as example 3.2 and 3.3
- $M^{\sharp}$ is not a best abstraction of $M_0$
   - Best abstraction of $M_0$ is smaller than $M^{\sharp}$

> - $M_0 = \{m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x}) < m(\mathrm{y}) \leq 8\}$
> - $M_1 = \{m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x})\}$
> - $M^{\sharp}$ : over-approximates each value
>    - $\mathrm{x}$ : [0, 10]
>    - $\mathrm{y}$ : [0, 100]

---

# Abstraction Function (3/3)

Note:
- The existence of a best abstraction is not guaranteed in general.
- Abstract relations such that no concretization function can be defined will not arise in this book.

---

# Galois Connection (1/3)

When an abstraction relation defines both
- concretization function
- abstraction function

they are tightly related to each other (which we call ***Galois connection***).

---

# Galois Connection (2/3)

<def>
<h4>
Definition 3.5 (Galois connection):
</h4>

   **Galois connection** is a pair made of a concretization function $\gamma$ and an abstraction function $\alpha$ such that:
   - $\forall c \in \mathbb{C}, \forall a \in \mathbb{A}$
      - $\enspace \alpha (c) \sqsubseteq a \iff c \subseteq \gamma (a)$

   We write such a pair as follows:
   - $(\mathbb{C}, \subseteq) \xtofrom[\gamma]{\alpha}(\mathbb{A}, \sqsubseteq)$
</def>

---

# Galois Connection (3/3)

Some interesting properties (proof is in B.1):
- $\alpha$ and $\gamma$ are monotone function.
- $\forall c \in \mathbb{C}$
   - $c \subseteq \gamma (\alpha (c))$
   - applying the abstraction function and concretizing the result back yield a less precise result
- $\forall a \in \mathbb{A}$
   - $\alpha (\gamma (a)) \sqsubseteq a$
   - concretizing an abstract element and abstracting the result back refines the information available in the initial abstract element (which is known as *reduction*)

---

# Overview

- Semantics (3.1)
- Abstraction (3.2)
   - <gray>The concept of abstraction</gray>
   - Non-relational abstraction
   - <gray>Relational abstraction</gray>
- Computable Abstract Semantics (3.3)
- Interpreter (3.4)

---

# (Non-relational / Relational) Abstraction

- Non-relational : それぞれの変数を独立に抽象化する
- Relational : 変数間の関係も含めて抽象化する (relationalが表すとおり)

---

# Non-relational Abstraction

Non-relational abstraction proceeds in two steps:
1. For each variable, it collects the values that the variable may take.
2. Then, over-approximates each of these set of values with one abstract element per variable (*value abstraction*).

---

# Value Abstraction (1/n)


<def>
   <h4>
Definition 3.6 (Value abstraction)
   </h4>

A **value abstraction** is an abstraction of $(\wp (\mathbb{V}), \subseteq)$

</def>

As we saw in chapter 2, *interval* and *sign* constraints define value abstractions.

---

# Value Abstraction (2/n)

#### Example 3.5 (Signs) (Figure 3.5)

- sign abstraction domain $\mathbb{A}_{\mathscr{S}}$ : $[\geq 0]$, $[\leq 0]$, $[= 0]$
   - $\top$ : any set of values
   - $\bot$ : empty set of values
- concretization function
   - $\gamma_{\mathscr{S}}$ :
      - $[\geq 0]\enspace \longmapsto \enspace \{ n \in \mathbb{V} \enspace | \enspace n \geq 0\}$
      - $[\leq 0]\enspace \longmapsto \enspace \{ n \in \mathbb{V} \enspace | \enspace n \leq 0\}$
      - $[= 0]\enspace \longmapsto \enspace \{ 0 \}$
      - $\top \enspace \longmapsto \enspace \mathbb{V}$
      - $\bot \enspace \longmapsto \enspace \empty$

---

# Value Abstraction (3/n)

#### Example 3.6 (A variation on the lattice of sign, with no abstraction function)

- If we remove $[= 0]$ from the abstract domain above, it doesn't have best abstract function.
- concrete set $\{ 0 \}$
   - we can't define abstraction function of this
   - $[\leq]$ and $[\geq]$ are incomparable

As a consequence:
- in general, it is impossible to identify one element as a most precise (sound) one.

> Provided the analysis designer and user are aware of this fact, it is not a serious limitation, however.

---

# Value Abstraction (4/n)

#### Example 3.7 (Intervals) (Figure 3.5)

- intervals value abstract domain $\mathbb{A}_{\mathscr{S}}$ :
   - $\bot$ : the empty set of values
   - $(n_0, n_1)$ : 
      - $n_0$ : either $- \infty$ or a value
      - $n_1$ : either $+ \infty$ or a value
      - $n_0 \leq n_1$
- concretization function :
   - $\gamma_{\mathscr{S}}$ :
      - $\bot \longmapsto \empty$
      - $[n_0, n_1]\enspace \longmapsto \enspace \{n \in \mathbb{V} \enspace | \enspace n_0 \leq n \leq n_1\}$
      - $[n_0, + \infty]\enspace \longmapsto \enspace \{n \in \mathbb{V} \enspace | \enspace n_0 \leq n\}$
      - $[- \infty, n_1]\enspace \longmapsto \enspace \{n \in \mathbb{V} \enspace | \enspace n \leq n_1\}$
      - $(- \infty, + \infty) \longmapsto \mathbb{V}$

The order relation is defined by the inclusion of concrete interval.

$[0, 2]$ and $[1, 3]$ are incomparable.

---

# Value Abstraction (5/n)

#### Example 3.8 (Congruences)

- abstract domain of congruences :
   - describes sets of values using congruence relations
- abstract element :
   - $\bot$ : empty set of values
   - $(n, p)$ : set of values that are equal to $n$ modulo $p$.
      - $p = 0$ or $0 \leq n \lt p$
- concretization function :
   - $\gamma_{\mathscr{C}}$ :
      - $\bot \enspace \longmapsto \enspace \empty$
      - $(n, p) \enspace \longmapsto \enspace \{n + kp \enspace | \enspace k \in \mathbb{Z}\}$

Also,
- $\mathbb{A}_{\mathscr{C}}$ : set of abstract elements
- $\sqsubseteq_{\mathscr{C}}$ : ordering relation defined by
   - $a_0 \sqsubseteq_{\mathscr{C}} a_1 \iff \gamma_{\mathscr{C}} (a_0) \subseteq \gamma_{\mathscr{C}} (a_1)$

Note:
- This domain has abstraction function $\alpha_{\mathscr{C}}$.
- $(0, 1)$ : represents all integers.
- $(n, 0)$ : represents $n$.

---

# Non-relational Abstraction (1/4)

<def>
   <h4>
Definition 3.7 (Non-relational abstraction)
   </h4>

   Assume that a value abstraction is given, that is
   - a value abstraction : $(\mathbb{A}_{\mathscr{V}}, \sqsubseteq)$
   - concretization function $\gamma_{\mathscr{V}}$ : $\mathbb{A}_{\mathscr{V}} \rightarrow \wp (\mathbb{V})$
   - a least element : $\bot_{\mathscr{V}}$
   - a greatest element : $\top_{\mathscr{V}}$

   Then, non-relational abstraction is is defined by
   - set of abstract elements $\mathbb{A}_{\mathscr{N}}$ = $\mathbb{X} \rightarrow \mathbb{A}_{\mathscr{V}}$
   - order relation $\sqsubseteq_{\mathscr{A}}$ : defined by
   - point-wise extension of $\sqsubseteq_{\mathscr{V}}$
   - $M_0^{\sharp} \sqsubseteq_{\mathscr{N}} M_1^{\sharp}$ if and only if $\forall \mathrm{x} \in \mathbb{X}$, $M_0^{\sharp}(\mathrm{x}) \sqsubseteq_{\mathscr{V}} M_1^{\sharp}(\mathrm{x})$
   - concretization function $\gamma_{\mathscr{N}}$ defined by
   - $\gamma_{\mathscr{N}} : \quad M^{\sharp} \enspace \longmapsto \enspace \{m \in \mathbb{M} \enspace | \enspace \forall \mathrm{x} \in \mathbb{X}, m(\mathrm{x}) \in \gamma_{\mathscr{V}} (M^{\sharp}(\mathrm{x}))\}$

</def>

---

# Non-relational Abstraction (2/4)

Intuitive explanation:
- treats each variable independently
   - applies the value abstraction to each variable separately from the other
- order relation is point-wise

The ***least element*** of the non-relational abstract domain is
- the function that maps each variable to the least element $\bot_{\mathscr{V}}$ :
   - $\forall \mathrm{x} \in \mathbb{X}, \bot_{\mathscr{N}} (\mathrm{x}) = \bot_{\mathscr{V}}$

The ***greatest element*** $\top_{\mathscr{N}}$ can be defined similarly.


---

# Non-relational Abstraction (3/4)

- When the value abstraction has an abstraction function $\alpha_{\mathscr{V}}$:
   - the non-relational abstraction also has one.

It is defined as follows:
- $\alpha_{\mathscr{N}}$ : $M \longmapsto \Big( (\mathrm{x} \in \mathbb{X}) \longmapsto \alpha_{\mathscr{V}} (\{ m(\mathrm{x}) \enspace | \enspace m \in M\}) \Big)$

Note:
- $\bot_{\mathscr{N}}$ is the best abstraction of $\empty$


---

# Non-relational Abstraction (4/4)

#### Example 3.9 (Non-relational abstraction)

Assumption:
- $\mathbb{X} = \{\mathrm{x}, \mathrm{y}, \mathrm{z}\}$
- memory states
   - $m_0$ : $\quad \mathrm{x} \mapsto 25 \quad \mathrm{y} \mapsto 7 \quad \mathrm{z} \mapsto -12$
   - $m_1$ : $\quad \mathrm{x} \mapsto 28 \quad \mathrm{y} \mapsto -7 \quad \mathrm{z} \mapsto -11$
   - $m_2$ : $\quad \mathrm{x} \mapsto 20 \quad \mathrm{y} \mapsto 0 \quad \mathrm{z} \mapsto -10$
   - $m_3$ : $\quad \mathrm{x} \mapsto 35 \quad \mathrm{y} \mapsto 8 \quad \mathrm{z} \mapsto -9$

The best abstraction of $\{m_0, m_1, m_2, m_3\}$ can be defined as follows :
- With the signs abstraction :
   - $M^{\sharp}$ : $\quad \mathrm{x} \mapsto$ <hide>$[\geq 0]$</hide> $\quad \mathrm{y} \mapsto$ <hide>$\top\enspace\enspace\enspace$</hide> $\quad \mathrm{z} \mapsto$ <hide>$[\leq 0]$</hide>
- With the intervals abstraction :
   - $M^{\sharp}$ : $\quad \mathrm{x} \mapsto$ <hide>$[25, 35]$</hide> $\quad \mathrm{y} \mapsto$ <hide>$[-7, 8]$</hide> $\quad \mathrm{z} \mapsto$ <hide>$[-12, -9]$</hide>


---

# Overview

- Semantics (3.1)
- Abstraction (3.2)
   - <gray>The concept of abstraction</gray>
   - <gray>Non-relational abstraction</gray>
   - Relational abstraction
      - linear equalities
      - convex polyhedra
      - octagons
- Computable Abstract Semantics (3.3)
- Interpreter (3.4)

---

# Relational Abstraction (1/4)

Such as *convex polyhedra*.

<def>
<h4>
Definition 3.8 (Linear equalities)
</h4>

- The elements of abstract domain of linear equalities :
   - $\bot$ : empty set
   - conjunctions of linear equality constraints : constrain sets of memory states.
      - such as $\mathrm{y} = a \mathrm{x}$

</def>

In the geometrical point of view :
- abstract elements are in the affine space $\mathbb{V}^{N}$
   - $N$ : dimension (number of variables)

This abstraction features :
- best abstraction function
- concretization

The best abstraction:
- the smallest affine space that contains all the memories in $M$

---

# Relational Abstraction (2/4)

<def>
<h4>
Definition 3.8 (Convex polyhedra)
</h4>

- elements of abstract domain of linear inequalities :
   - $\bot$ : empty set
   - conjunctions of linear **in**equality constraints : constrain sets of memory states.

</def>

In the geometrical point of view :
- abstract elements : convex polyhedra of all dimension in $\mathbb{V}^{N}$
   - $N$ : dimension (number of variables)

This abstraction features :
- concretization
- but no best abstraction function
   - certain concrete sets do have a best abstraction though

The cost :
- the number of inequality is not bounded.

---

# Relational Abstraction (3/4)


<def>
<h4>
Definition 3.9 (Octagons)
</h4>

- element of abstract domain of octagons :
   - $\bot$ : empty set
   - conjunctions of linear inequality constraints of the form below:
      - $\pm\mathrm{x} \pm \mathrm{y} \leq c$
      - $\pm x = c$

</def>

In the geometrical point of view :
- abstract elements : "octagonal" shape

This abstraction features:
- best abstraction function
- concretization function


---

# Relational Abstraction (4/4)

- It is difficult to decide which abstract domain describes relational constraints efficiently.
   - We will not discuss this topic any further.

---

# Overview

- <gray>Semantics (3.1)</gray>
- <gray>Abstraction (3.2)</gray>
- Computable Abstract Semantics (3.3)
   - introduction
   - semantics of each commands
   - soundness
- <gray>Interpreter (3.4)</gray>

---

# Computable Abstract Semantics (1/n)

- we **use non-relational** abstract domain
   - we also discuss the modifications which is required to use relational abstract domain.

The form of analysis is :
- mathematical function
   - input : a program and an abstract pre-condition
   - output : an abstract post-condition

---

# Computable Abstract Semantics (2/n)

Some preparations :
- $\mathbb{A}$ : the state abstract domain
- $\gamma$ : associated concretization function
   - $\mathbb{A}_{\mathscr{V}}$ : underlying value abstraction.
   - $\gamma_{\mathscr{V}}$ : concretization function

The design of the analysis aims at:
- the soundness in the sense of definition 2.6
   - See figure 3.7
      - $\llbracket \mathrm{p} \rrbracket_{\mathscr{P}}^{\sharp}$ : the static analysis function (or *abstract semantics*)


---

# Computable Abstract Semantics (3/n)

$$
\begin{CD}
   a_{\mathrm{pre}} @>\mathrm{analyze}\enspace\mathtt{p}>> \llbracket \mathtt{p} \rrbracket^{\sharp}_{\mathscr{P}} (a_{\mathrm{pre}}) \\
@A\in \gamma (\cdot)AA @. \\
   m @>\mathrm{run}\enspace\mathtt{p}>> m\rq
\end{CD}
$$

$$
\begin{CD}
   a_{\mathrm{pre}} @. a_{\mathrm{post}}\\
@A\in \gamma (\cdot)AA @AA\in \gamma (\cdot)A \\
   m @>\mathrm{run}\enspace\mathtt{p}>> m\rq
\end{CD}
$$

- $\llbracket \mathtt{p} \rrbracket^{\sharp}_{\mathscr{P}}$ : analysis function, or *abstract semantics*

---

# Abstract Semantics of Each Commands

We're going to define the semantics of $\llbracket \cdot \rrbracket^{\sharp}_{\mathscr{P}}$ by induction.

- Definition of the semantics : very similar to that of concrete semantics.
- Soundness : ensured
   - soundness is ensured in a inductive manner
- Abstract semantics of a command : defined by that of its sub-commands.

---

# That's all 🙂

- $\llbracket n \rrbracket^{\sharp} (M^{\sharp}) = \phi_{\mathscr{V}} (n)$
- $\llbracket \mathrm{x} \rrbracket^{\sharp} (M^{\sharp}) = M^{\sharp}(\mathrm{x})$
- $\llbracket \mathtt{E_0} \odot \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}) = f_{\odot}^{\sharp} (\llbracket \mathtt{E_0} \rrbracket^{\sharp} (M^{\sharp}), \llbracket \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}))$
- $\llbracket \mathtt{C} \rrbracket_{\mathscr{P}}^{\sharp} (\bot) = \bot$
- $\llbracket \mathtt{skip} \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = M^{\sharp}$
- $\llbracket \mathtt{C}_0 ; \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = \llbracket \mathtt{C}_0 \rrbracket_{\mathscr{P}}^{\sharp} (\llbracket \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp}(M^{\sharp}))$
- $\llbracket \mathrm{x} \colonequals \mathtt{E} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp} [\mathrm{x} \mapsto \llbracket \mathtt{E} \rrbracket^{\sharp} (M^{\sharp})]$
- $\llbracket \mathtt{input(\mathrm{x})} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto \top_{\mathscr{V}}]$
- $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket^{\sharp}_{\mathscr{P}}(M^{\sharp}) = \llbracket C_0 \rrbracket_{\mathscr{P}}^{\sharp}(\mathscr{F}^{\sharp}_{B}(M^{\sharp})) \sqcup^{\sharp} \llbracket C_1 \rrbracket^{\sharp}_{\mathscr{P}}(\mathscr{F}^{\sharp}_{\neg B}(M^{\sharp}))$
- $\llbracket \mathtt{while} (B) \{ C \} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = \mathscr{F}_{\neg B}^{\sharp} (\mathrm{abs\_iter} ( \llbracket C \rrbracket^{\sharp}_{\mathscr{P}} \circ \mathscr{F}_B^{\sharp}, M^{\sharp} ))$

---

# Bottom Element, Skip Commands

<def>
<h4>
Bottom Element
</h4>

- $\llbracket \mathtt{C} \rrbracket_{\mathscr{P}}^{\sharp} (\bot) = \bot$
   - intuitive explanation : running a program from empty set of states is empty.
   - soundness : ensured

</def>

<def>
<h4>
Skip Commands
</h4>

- $\llbracket \mathtt{skip} \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = M^{\sharp}$
   - input is not modified
   - soundness : ensured

</def>

---

# Sequences of Commands

- Soundness property of figure 3.7 is stable is under composition.
   - $\llbracket \mathtt{p}_0 ; \mathtt{p}_1 \rrbracket_{\mathscr{P}} (M) = \llbracket \mathtt{p}_0 \rrbracket_{\mathscr{P}} (\llbracket \mathtt{p}_1 \rrbracket_{\mathscr{P}} (M))$


<def>
<h4>
Sequences of Commands
</h4>

   - $\llbracket \mathtt{C}_0 ; \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = \llbracket \mathtt{C}_0 \rrbracket_{\mathscr{P}}^{\sharp} (\llbracket \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp}(M^{\sharp}))$
   - this equation ensures that we can prove soundness by induction.
</def>

---

# Approximation of Composition (1/n)

<theorem>
<h4>
Theorem 3.1 (Approximation of composition)
</h4>

- $F_0$, $F_1$ : $\wp (\mathbb{M}) \rightarrow \wp (\mathbb{M})$
   - two monotone functions
- $F_0^{\sharp}$, $F_1^{\sharp}$ : $\mathbb{A} \rightarrow \mathbb{A}$
   - these two functions over-approximate the two function above.
   - such that
      - $F_0 \circ \gamma \subseteq \gamma \circ F_0^{\sharp}$ and $F_1 \circ \gamma \subseteq \gamma \circ F_1^{\sharp}$
- then, $F_0 \circ F_1$ can be over-approximated by $F_0^{\sharp} \circ F_1^{\sharp}$

</theorem>

---

# Approximation of Composition (2/n)

<h4>
Proof
</h4>

- Assumption : $M^{\sharp} \in \mathbb{A}$
- $F_1 \circ \gamma (M^{\sharp}) \subseteq \gamma \circ F_1^{\sharp} (M^{\sharp})$ ( by the soundness of $F_1$ )
- $F_0 \circ F_1 \circ \gamma (M^{\sharp}) \subseteq F_0 \circ \gamma \circ F_1^{\sharp} (M^{\sharp})$ ( applied $F_0$, since $F_0$ is monotone)
   - $\subseteq \gamma \circ F_0^{\sharp} \circ F_1^{\sharp} (M^{\sharp})$ ( by the soundness of $F_0$ )
- then,
   - $F_0 \circ F_1 \circ \gamma (M^{\sharp}) \subseteq \gamma \circ F_0^{\sharp} \circ F_1^{\sharp} (M^{\sharp})$
- so, $F_0 \circ F_1$ is over-approximated by $\circ F_0^{\sharp} \circ F_1^{\sharp}$

Note:
- concrete semantics heavily relies on this composition of function.

---

# Expressions (1/5)

<def>
<h4>
Abstract Interpretation of Expressions
</h4>

- $\llbracket \mathtt{E} \rrbracket^{\sharp}$ : abstract interpretation of expressions
- $\llbracket \mathtt{E} \rrbracket^{\sharp} : \mathbb{A} \rightarrow \mathbb{A}_{\mathscr{V}}$ 
- semantics of expressions
- $\llbracket n \rrbracket^{\sharp} (M^{\sharp}) = \phi_{\mathscr{V}} (n)$
- $\llbracket \mathrm{x} \rrbracket^{\sharp} (M^{\sharp}) = M^{\sharp}(\mathrm{x})$
- $\llbracket \mathtt{E_0} \odot \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}) = f_{\odot}^{\sharp} (\llbracket \mathtt{E_0} \rrbracket^{\sharp} (M^{\sharp}), \llbracket \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}))$
- soundness : ensured
- we will not see the proof though.
</def>


---

# Expressions (2/5)

- $\llbracket n \rrbracket^{\sharp} (M^{\sharp}) = \phi_{\mathscr{V}} (n)$
   - This shoud return any abstract element that over-approximate $n$
   - If the value abstraction has a best abstraction $\alpha_{\mathscr{V}}$, $\alpha_{\mathscr{V}} (\{n\})$ is enough.
   - $\phi_{\mathscr{V}} : \mathbb{V} \rightarrow \mathbb{A}_{\mathscr{V}}$
      - This function may not return the most precise abstraction.
      - This function is such that $n \in \gamma_{\mathscr{V}} (\phi_{\mathscr{V}}(n))$

---

# Expressions (3/5)

- $\llbracket \mathrm{x} \rrbracket^{\sharp} (M^{\sharp}) = M^{\sharp}(\mathrm{x})$
   - simply return a abstraction that is associated to the variable.

> - set of abstract elements $\mathbb{A}_{\mathscr{N}}$ = $\mathbb{X} \rightarrow \mathbb{A}_{\mathscr{V}}$

---

# Expressions (4/5)

- $\llbracket \mathtt{E_0} \odot \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}) = f_{\odot}^{\sharp} (\llbracket \mathtt{E_0} \rrbracket^{\sharp} (M^{\sharp}), \llbracket \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}))$
   - we need to apply the conservative abstraction of $f_\odot$ in the non-relational lattice.
   - we need an operator $f_{\odot}^{\sharp}$ such that:
      - for all $n_{0}^{\sharp}, n_{1}^{\sharp} \in \mathbb{A}_{\mathscr{V}}$
         - $\{ f_{\odot} (n_0, n_1) \enspace | \enspace n_0 \in \gamma_{\mathscr{V}} (n_{0}^{\sharp})$ and $n_1 \in \gamma_{\mathscr{V}} (n_{1}^{\sharp}) \} \subseteq \gamma_{\mathscr{V}} (f_{\odot}^{\sharp} (n_{0}^{\sharp}, n_{1}^{\sharp}))$
   - $f_{\odot}^{\sharp}$ shoud over-approximate the effect of operation of $f_{\odot}$ on concrete value.

---

# Expressions (5/5)

#### Example 3.10 (Abstract semantics of expressions)

- we use interval abstraction
- $M^{\sharp}$ is defined by $M^{\sharp} (\mathrm{x}) = [10, 20]$ and $M^{\sharp}(\mathrm{y}) = [8, 9]$

Interpretation of $\mathrm{x} + 2 * \mathrm{y} - 6$ : ($f^{\sharp}_{-}$ and $f^{\sharp}_{+}$ can be used)
- $\llbracket \mathrm{x} + 2 * \mathrm{y} - 6 \rrbracket^{\sharp} (M^{\sharp})$
   - $= f_{-}^{\sharp} (\llbracket \mathrm{x} + 2 * \mathrm{y} \rrbracket^{\sharp}(M^{\sharp}), \llbracket 6 \rrbracket^{\sharp}(M^{\sharp}))$
   - <hide>$= f_{+}^{\sharp} (\llbracket \mathrm{x} \rrbracket^{\sharp}(M^{\sharp}), \llbracket 2 * \mathrm{y} \rrbracket^{\sharp}(M^{\sharp})) - [6,6]$</hide>
   - <hide>$= M^{\sharp}(\mathrm{x}) + f_{*}^{\sharp}(\llbracket 2 \rrbracket^{\sharp}(M^{\sharp}), \llbracket \mathrm{y} \rrbracket^{\sharp}(M^{\sharp})) - [6,6]$</hide>
   - <hide>$= [10, 20] + [2,2] * [8,9] - [6,6]$</hide>
   - <hide>$= [20, 32]$</hide>

---

# Assignments (1/n)

> $\llbracket \mathrm{x} \colonequals E \rrbracket_{\mathscr{P}}(M) = \{m[\mathrm{x} \mapsto \llbracket E \rrbracket(m)] \enspace | \enspace m \in M\}$

Recall that assignment is the composition of 
1. Evaluation of the expression $\mathtt{E}$ to $n$
2. Update of the variable $\mathrm{x}$ with $n$

This composition can be over-approximated piece by piece (Theorem 3.1).

---

# Assignments (, Input) (2/n)


<def>

<h4>
Assignments
</h4>

- target : $\mathrm{x} \colonequals \mathtt{E}$
- $\llbracket \mathrm{x} \colonequals \mathtt{E} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp} [\mathrm{x} \mapsto \llbracket \mathtt{E} \rrbracket^{\sharp} (M^{\sharp})]$

</def>

<def>
<h4>
input
</h4>

- $\llbracket \mathtt{input(\mathrm{x})} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto \top_{\mathscr{V}}]$
   - repaleced the value with $\top_{\mathscr{V}}$
</def>

---

# Assignments (3/n)

#### Example 3.11 (Analysis of an assignment command)

> - $M^{\sharp} (\mathrm{x}) = [10, 20]$ and $M^{\sharp}(\mathrm{y}) = [8, 9]$
> - $\llbracket \mathrm{x} + 2 * \mathrm{y} - 6 \rrbracket^{\sharp} (M^{\sharp}) = [20, 32]$


- $\llbracket \mathrm{x} \colonequals \mathrm{x} + 2 * \mathrm{y} - 6 \rrbracket^{\sharp} (M^{\sharp}) =$ <hide>$\{ \mathrm{x} \mapsto [20,32], \mathrm{y} \mapsto [8,9] \}$</hide>

---

# Assignments (with Relational Abstract Domain) (1/n)

### Analysis of Assignments Using a Relational Abstract Domain

> 1. Add temporary dimension $\mathrm{x}\rq$ that is meant to describe the value of the expression
> 1. Represent as precisely as possible the constraint $\mathrm{x}\rq = \mathtt{E}$
> 1. Project out dimension $\mathrm{x}$, and rename $\mathrm{x}\rq$ to $\mathrm{x}$

---

# Assignments (with Relational Abstract Domain) (2/n)

#### Example 3.12

Assumption:
- abstract domain : convex polyhedra
- abstract pre-condition : $2 \leq \mathrm{x} \leq 3 \land 1 - \mathrm{x} \leq \mathrm{y}$
- assignment : $\mathrm{x} \colonequals \mathrm{y} + \mathrm{x} + 2$

We introduce the variable $\mathrm{x}\rq$ and write the constraint as below:
- $2 \leq \mathrm{x} \leq 3 \land 1 - \mathrm{x} \leq \mathrm{y} \land \mathrm{x}\rq = \mathrm{y} + \mathrm{x} + 2$

From the last term, we get $\mathrm{x} = \mathrm{x}\rq - \mathrm{y} - 2$. Then, apply this formula and we get
- $2 \leq \mathrm{x}\rq - \mathrm{y} - 2 \leq 3 \land 3 - \mathrm{x}\rq + \mathrm{y} \leq \mathrm{y}$
- $\iff 4 \leq \mathrm{x}\rq - \mathrm{y} \leq 5 \land 3 \leq \mathrm{x}\rq$ (rename $\mathrm{x}\rq$ to $\mathrm{x}$ if you want)

---

# Conditional Branching

- An in the last paragraph, we over-approximate the definition of concrete semantics step-by-step.

> $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket_{\mathscr{P}}(M) = \llbracket C_0 \rrbracket_{\mathscr{P}}(\mathscr{F}_{B}(M)) \cup \llbracket C_1 \rrbracket_{\mathscr{P}}(\mathscr{F}_{\neg B}(M))$

We will follow these steps:
1. design an operation to over-approximate $\mathscr{F}_{B}$ for any Boolean expression $B$.
1. use the abstract semantics of both branches
1. apply the over-approximation of the union of concrete sets.

---

# Analysis of Condition (1/4)

#### Analysis of Conditions

- abstraction of filtering function $\mathscr{F}_B$, which we denote by $F_{B}^{\sharp}$
- $\mathscr{F}_B$
   - input : memory states
   - output : memory states such that $B$ evaluates to *true*.
- $\mathscr{F}_B^{\sharp}$
   - input : an abstract state
   - output : an abstract state refined by the condition $B$

$\mathscr{F}_B^{\sharp}$ should satisfies the following soundness condition (ref. figure 3.7):
- for all conditions $B$ and all abstract states $M^{\sharp}$ 
   - $\mathscr{F}_{B}(\gamma (M^{\sharp})) \subseteq \gamma (\mathscr{F}_B^{\sharp} (M^{\sharp}))$

---

# Analysis of Condition (2/4)

We will see some examples.

- Sign abstract domain $\{ \bot, \top, [=0], [\geq 0], [\leq 0] \}$
   - $\mathscr{F}_{\mathrm{x} \lt 0}^{\sharp} (M^{\sharp})=$
      - $(\mathrm{y} \in \mathbb{X}) \mapsto \bot \enspace$ if $M^{\sharp} (\mathrm{x}) = [\geq 0]$ or $[= 0]$ or $\bot$
      - $M^{\sharp} [\mathrm{x} \mapsto [\leq]]\enspace$ if $M^{\sharp} (\mathrm{x}) = [\leq 0]$ or $\top$

- Interval abstract domain $M^{\sharp}(\mathrm{x}) = [a, b]$
   - $\mathscr{F}_{\mathrm{x} \leq n}^{\sharp}(M^{\sharp}) =$
      - $(\mathrm{y} \in \mathbb{X}) \mapsto \bot$ if $a > n$
      - $M^{\sharp} [\mathrm{x} \mapsto [a, n]]\enspace$ if $a \leq n \leq b$
      - $M^{\sharp}$ if $b \leq n$


---

# Analysis of Condition (3/4)

#### Example 3.13 (Analysis of a condition)

We consider the code fragment below that computes the absolute value of $\mathrm{x} - 7$.

```c
if(x > 7){
   y := x - 7
}else{
   y := 7 - x
}
```

Assumption:
- pre-condition $M^{\sharp}$ : $\mathrm{x} \mapsto \top$, $\mathrm{y} \mapsto \top$

Then, by the rule above,
- $\mathscr{F}_{\mathrm{x} \gt 7} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto [8, + \infty)]$
- $\mathscr{F}_{\mathrm{x} \leq 7} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto (-\infty, 7]]$

---

# Analysis of Condition (4/4)


<theorem>
<h4>
Theorem 3.3 (Soundness of the abstract interpretation conditions)
</h4>

- for all...
   - expressions $B$
   - non-relational abstract elements $M^{\sharp}$
   - memory states $m$ such that $m \in \gamma(M^{\sharp})$
- if $\llbracket B \rrbracket (m) = \mathbf{true},\quad$ then $\quad m \in \gamma (\mathscr{F}_B^{\sharp} (M^{\sharp}))$

</theorem>

---

# Analysis of Flow Joins (1/3)

> $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket_{\mathscr{P}}(M) = \llbracket C_0 \rrbracket_{\mathscr{P}}(\mathscr{F}_{B}(M)) \cup \llbracket C_1 \rrbracket_{\mathscr{P}}(\mathscr{F}_{\neg B}(M))$

Next, we want to abstract the union operator $\cup$.
Let $\sqcup^{\sharp}$ be the abstract union (join) operator.

$\sqcup^{\sharp}$ should satisfy the following soundness property:

<theorem>
   <h4>
Theorem 3.4 (Soundness of abstract join)
   </h4>

   Let $M_0^{\sharp}$ and $M_1^{\sharp}$ be the two abstract states.

   - $\gamma(M_0^{\sharp}) \cup \gamma(M_1^{\sharp}) \subseteq \gamma(M_0^{\sharp} \sqcup^{\sharp} M_1^{\sharp})$
   </theorem>

---

# Analysis of Flow Joins (2/3)

To define $\sqcup^{\sharp}$, we can simply
- define a join operator $\sqcup^{\sharp}_{\mathscr{V}}$ in the value abstract domain.
- apply operator $\sqcup^{\sharp}_{\mathscr{V}}$ in a point-wise manner:
   - for all variable $\mathrm{x}$, $(M_0^{\sharp} \sqcup^{\sharp} M_1^{\sharp}) (\mathrm{x}) = M_0^{\sharp}(\mathrm{x}) \sqcup^{\sharp}_{\mathscr{V}} M_1^{\sharp}(\mathrm{x})$

The definition of $\sqcup^{\sharp}_{\mathscr{V}}$ depends on the abstract domain.
For instance, for the interval domain:
- $[a_0, b_0] \sqcup^{\sharp}_{\mathscr{V}} [a_1, b_1] = [\mathtt{ min }(a_0, b_0), \mathtt{ max }(a_1, b_1)]$
- $[a_0, b_0] \sqcup^{\sharp}_{\mathscr{V}} [a_1, +\infty) = [\mathtt{ min }(a_0, b_0), +\infty)$

---

# Analysis of Flow Joins (3/3)

#### Example 3.14 (Analysis of flow joins)

- $M_0^{\sharp} = \{ \mathrm{x} \mapsto [0,3], \mathrm{y} \mapsto [6,7], \mathrm{z} \mapsto [4,8] \}$
- $M_1^{\sharp} = \{ \mathrm{x} \mapsto [5,6], \mathrm{y} \mapsto [0,2], \mathrm{z} \mapsto [6,9] \}$

Then,

- $M_0^{\sharp} \cup^{\sharp} M_1^{\sharp} = \{ \mathrm{x} \mapsto [$<hide>$0, 6$</hide>$], \mathrm{y} \mapsto [$<hide>$0, 7$</hide>$], \mathrm{z} \mapsto [$<hide>$6, 9$</hide>$]\}$

---

# Analysis of Conditional Commands (1/3)

Now, we have defined
- condition
- flow joins

and we can use those to define the semantics of conditional commands.

Semantics of conditional commands:
- $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket^{\sharp}_{\mathscr{P}}(M^{\sharp}) = \llbracket C_0 \rrbracket_{\mathscr{P}}^{\sharp}(\mathscr{F}^{\sharp}_{B}(M^{\sharp})) \sqcup^{\sharp} \llbracket C_1 \rrbracket^{\sharp}_{\mathscr{P}}(\mathscr{F}^{\sharp}_{\neg B}(M^{\sharp}))$

This definition is very similar to that of concrete one.

---

# Analysis of Conditional Commands (2/3)


We use this program from example 3.13 here.

```c
if(x > 3){
   y := x - 3
}else{
   y := 3 - x
}
```

---

# Analysis of Conditional Commands (3/3)

#### Example 3.15 (Analysis of a conditional command)

- abstract pre-condition : $M^{\sharp} = \{ \mathrm{x} \mapsto \top, \mathrm{y} \mapsto \top \}$

Analysis proceeds as follows :

1. the analysis of **true** branch
   1. filters pre-condition
   2. computes the post-condition for the assignment of $\mathrm{y} \colonequals \mathrm{x} - 3$
   3. we get : $\{ \mathrm{x} \mapsto [4, + \infty), \mathrm{y} \mapsto [1, +\infty) \}$
2. the analysis of **false** branch
   - we get : $\{ \mathrm{x} \mapsto (- \infty, 7], \mathrm{y} \mapsto [0, +\infty) \}$
3. abstract join of these two abstract states
   - we get : $\{ \mathrm{x} \mapsto \top, \mathrm{y} \mapsto [0, +\infty) \}$

---

# Conditional Commands with a Relational Abstract Domain (1/1)

We have to use different algorithm:
- for the analysis of condition tests
- for the computation of abstract join

Analysis of conditional test with a relational domain :
- add several constraints to the abstract states

In general, it is more precise. Condition test that involve several variables are more precise. (more likely to be presented exactly)
- Consider the case of $\mathrm{x} \leq \mathrm{y}$

---

# Abstract Interpretation of Loops (1/n)

### Concrete Semantics of Loop


> $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \Big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \Big)$

Note:
- Over-approximation of $\llbracket C \rrbracket_{\mathscr{P}}$ can be computed.
- Over-approximation of sequences of commands can be obtained by the over-approximation of each commands.

That is,
- Over-approximation of $\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B$ can be computed

---

# Abstract Interpretation of Loops (2/n)

### Concrete Semantics of Loop


> $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \Big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \Big)$

- $F = \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B$
- $F^{\sharp}$ : over-approximation of $F$

Goal:
- Over-approximation of the infinite union $\cup_{i \geq 0}F^{i}(M)$ with $F^{\sharp}$

---

# Abstract Interpretation of Loops (3/n)

#### Example 3.16 (Analysis of programs with loops)

We will use these programs as a example.
<div class="twocols">
   <p>

Figure 3.9(a)
```c
x := 0;
while (x >= 0) {
   x := x + 1;
}
```
   </p>

   <p class="break">

Figure 3.9(b)
```c
x := 0;
while (x <= 100) {
   if (x >= 50) {
      x := 10
   } else {
      x := x + 1
   }
}
```

   </p>
</div>

---

# Sequences of Concrete and Abstract Iterates (1/n)

**Situation** : a loop iterates at most $n$ times. ($n$ is a fixed integer value)

Then, the states they may generate at the loop head are :

- $M_n = \bigcup^{n}_{i=0} F^{i}(M)$

The sequences $(M_k)_{k \in \mathbb{N}}$ can be defined recursively as follows :
- $M_0 = M$
- $M_{k + 1} = M_k \cup F(M_k)$

Then,
- over-approximation of $M_n$ : can be easily done using $\sqcup^{\sharp}$ (, which is used in the previous chapter)

---

# Sequences of Concrete and Abstract Iterates (2/n)

Indeed, let us assume:
- $M^{\sharp}$ : an abstract element of the abstract domain
   - $M \subseteq \gamma( M^{\sharp} )$

We define the abstract iterates $( M_k^{\sharp} )_{k \in \mathbb{N}}$ as follows
- $M_0^{\sharp} = M^{\sharp}$
- $M_{k + 1}^{\sharp} = M_{k}^{\sharp} \sqcup^{\sharp} F^{\sharp}(M_{k}^{\sharp})$

Then we can prove by induction that
- for all integers $n$, $M_n \subseteq \gamma(M_n^{\sharp})$

---

# Proof of $\forall n$, $M_n \subseteq \gamma(M_n^{\sharp})$

1. $n = 0$
   - It is obvious from assumption that $M_0 \subseteq \gamma( M_0^{\sharp} )$
1. $n = k$
   - we assume that $M_k \subseteq \gamma( M_k^{\sharp} )$
   - $M_{k+1}$
      - $= M_k \cup F(M_k)$
      - $\subseteq \gamma(M_k^{\sharp}) \cup F(\gamma(M_k^{\sharp}))$ ( $\because M_k \subseteq \gamma( M_k^{\sharp} )$)
      - $\subseteq \gamma(M_k^{\sharp}) \cup \gamma(F^{\sharp}(M_k^{\sharp}))$ ( $\because$ soundness of $F^{\sharp}$)
      - $\subseteq \gamma(M_k^{\sharp} \sqcup^{\sharp} F^{\sharp}(M_k^{\sharp}))$ ( $\because$ soundness of $\sqcup^{\sharp}$)
      - $= \gamma(M_{k + 1}^{\sharp})$
   - $\therefore M_{k + 1} \subseteq \gamma(M_{k + 1}^{\sharp})$

---

# Sequences of Concrete and Abstract Iterates (3/n)

### Example 3.17 (Abstract iterates)

<div class="twocols">
   <p>

Figure 3.9(a)
```c
x := 0;
while (x >= 0) {
   x := x + 1;
}
```
   </p>

   <p class="break">

Figure 3.9(b)
```c
x := 0;
while (x <= 100) {
   if (x >= 50) {
      x := 10
   } else {
      x := x + 1
   }
}
```

   </p>
</div>

---

# Sequences of Concrete and Abstract Iterates (4/n)

### Example 3.17 (Abstract iterates)

<div class="twocols">
   <p>

In the case of program (a):
- $M_0^{\sharp} = \{\mathrm{x} \mapsto [0, 0]\}$
- $M_1^{\sharp} = \{\mathrm{x} \mapsto [0, 1]\}$
- $M_2^{\sharp} = \{\mathrm{x} \mapsto [0, 2]\}$
- ...
- $M_n^{\sharp} = \{\mathrm{x} \mapsto [0, n]\}$
- ...
   </p>

   <p class="break">

In the case of program (b):
- ...
- $M_{49}^{\sharp} = \{\mathrm{x} \mapsto [0, 49]\}$
- $M_{51}^{\sharp} = \{\mathrm{x} \mapsto [0, 50]\}$
- $M_{52}^{\sharp} = \{\mathrm{x} \mapsto [0, 50]\}$
- $M_{53}^{\sharp} = \{\mathrm{x} \mapsto [0, 50]\}$
- ...

   </p>
</div>

---

# Convergence of Iterates (1/3)

> $M_{k + 1}^{\sharp} = M_{k}^{\sharp} \sqcup^{\sharp} F^{\sharp}(M_{k}^{\sharp})$

We consider :
- the case of unbounded iteration
- the termination problem

Let us assume that :
- the abstract iteration stabilize at some rank $n$

Then,
- for all $k \geq n$, $\quad M_k^{\sharp} = M_n^{\sharp}\enspace$ and $\enspace M_k \subseteq \gamma( M_n^{\sharp} )$

Also,
- $M_{\mathrm{loop}} \subseteq \gamma(M_n^{\sharp})\quad$ where $\quad M_{\mathrm{loop}} = \bigcup_{i \geq 0} M_i$

---

# Convergence of Iterates (2/3)

Another interesting observation is that :
- $M_{\mathrm{loop}} = \bigcup_{i \geq 0} F^i(M) = \bigcup_{i \geq 0}M_i \subseteq \gamma(M_n^{\sharp})$

**If the sequences of abstract iterates converges** :
- its final value over-approximate *all* the concrete behaviors of $\mathtt{while} (B) (C)$.

> If the sequences of abstract iterates converges

This can be observed by checking two consecutive iterates.

---

# Convergence of Iterates (3/3)

#### Example 3.18 (Convergence of abstract iterates)

- In the case of program (a) :
   - the sequences of abstract iterates does not converge.
- In the case of program (b) :
   - the ranges of $\mathrm{x}$ stabilize but only after 51 iterations.

Neither of these are satisfactory.
- lack of termination
- hight number required to stabilize

We have to formalize the condition that ensures that
- the sequences of abstract iterates converges.

---

# Convergence in Finite Height Lattices (1/4)

Assumption:
- $\sqsubseteq$ is such that
   - $M_a^{\sharp} \sqsubseteq M_b^{\sharp}\quad$ if and only if $\quad\gamma(M_a^{\sharp}) \subseteq \gamma(=M_b^{\sharp})$ for all abstract states $M_a^{\sharp}, M_b^{\sharp}$

First case where convergence is ensured is when:

- $M_a^{\sharp} \sqsubset M_b^{\sharp}$

cannot hold infinitely many times.

This condition is realized when
- the abstract domain has ***finite height***, or
- the length of the chain below is bounded by some fixed value $h$ (*height of the abstract domain*).
   - $M_0^{\sharp} \sqsubset M_1^{\sharp} \sqsubset  \cdots \sqsubset M_k^{\sharp}$

---

# Convergence in Finite Height Lattices (2/4)

For example, if the abstract domain has finite height $h$, the sequences
- $M_0^{\sharp} , M_1^{\sharp} ,  \cdots , M_h^{\sharp} , M_{h+1}^{\sharp}$
 
is increasing for $\sqsubseteq$, but cannot be strictly increasing.

So there exists a number $n (\leq h)$
- at which it becomes stable.
- which is bounded by the height of lattice.
 
---

# Convergence in Finite Height Lattices (3/4)

- $M_{\mathrm{ lim }}^{\sharp}$ : over-approximation of $M_{\mathrm{loop}}$
- $M_{\mathrm{ lim }}^{\sharp}$ can be computed by the algorithm below :

<theorem>

<h4>
Figure 3.10 (a)
</h4>

- $\mathrm{abs\_iter}(F^{\sharp}, M^{\sharp})$
   - $R \longleftarrow M^{\sharp};$
   - $\mathrm{ repeat }$
      - $T \longleftarrow R;$
      - $R \longleftarrow R \sqcup^{\sharp}F^{\sharp}(R);$
   - $\mathrm{ until }$ $R = T$
   - $\mathrm{ return }$ $M^{\sharp}_{\mathrm{lim}} = T;$
</theorem>

---

# Convergence in Finite Height Lattices (4/4)

#### Example 3.19 (Convergence of abstract iterates in the signs abstract domain)

- domain : signs abstract domain
- program : same as example 3.16 and 3.17

- In the case of the program of (a), we obtain :
   - $M^{\sharp}_0 = \{\mathrm{x} \mapsto [= 0]\}$
   - $M^{\sharp}_1 = \{\mathrm{x} \mapsto [\geq 0]\}$
   - $M^{\sharp}_2 = \{\mathrm{x} \mapsto [\leq 0]\}$
      - this analysis terminates after only two iterations
- In the case of the program of (b), we obtain the same result.

---

# Widening Operators (1/7)

- We will use ***widening*** technique for iterates to converge quickly.
- Essentially, widening operator do:
   - over-approximate concrete unions
   - enforces termination of all sequences of iteration

---

# Widening Operators (2/7)

<def>
<h4>
Definition 3.11 (Widening operator)
</h4>

   - widening operator : $\triangledown$ such that
      1. for all abstract elements $a_0$ and $a_1,\quad$ $\gamma (a_0) \cup \gamma (a_1) \subseteq \gamma (a_0 \triangledown a_1)$
      1. for all sequences $(a_n)_{n \in \mathbb{N}}$ of abstract elements, the sequences of $(a_n \rq)_{n \in \mathbb{N}}$ defined below is ultimately stationary (= eventually converge).
         - $a_0\rq = a_0$
         - $a_{n + 1}\rq = a_n\rq \triangledown a_n$

</def>

Then we can turn the sequence of abstract iterates into a terminating sequence.

---

# Widening Operators (3/7)

<theorem>
<h4>
Theorem 3.5 (Abstract iterates with widening)
</h4>

Let we assume:
- $\triangledown$ : widening operator over non-relational abstract domain $\mathbb{A}$
- $F^{\sharp}$ : $\mathbb{A} \rightarrow \mathbb{A}$

Then, the algorithm shown in the next page terminates and returns $M^{\sharp}_{\mathrm{lim}}$.

</theorem>

---

# Widening Operators (4/7)

<theorem>
<h4>
Figure 3.10 (b)
</h4>

- $\mathrm{abs\_iter}(F^{\sharp}, M^{\sharp})$
   - $R \longleftarrow M^{\sharp};$
   - $\mathrm{ repeat }$
      - $T \longleftarrow R;$
      - $R \longleftarrow R \triangledown F^{\sharp}(R);$
   - $\mathrm{ until }$ $R = T$
   - $\mathrm{ return }$ $M^{\sharp}_{\mathrm{lim}} = T;$
</theorem>

---

# Widening Operators (5/7)

<theorem>
<h4>
Theorem 3.5 (Abstract iterates with widening) (continued)
</h4>

Let we assume:
- $F : \mathbb{M} \rightarrow \mathbb{M}$
   - continuous
   - $F \circ \gamma \subseteq \gamma \circ F^{\sharp}$ (in the sense of point-wise)

Then,
- $\bigcup_{i \geq 0 } F^{i}(\gamma(M^{\sharp})) \subseteq \gamma(M^{\sharp}_{\mathrm{lim}})$
   - $M^{\sharp}_{\mathrm{lim}}$ over-approximates the concrete semantics of the loop.

</theorem>

This theorem guarantees
- the termination of the loop analysis
- soundness

---

# Widening Operators (6/7)

Widening operator for the intervals domain would be like this:
   - $[n p] \triangledown_{\mathscr{V}} [n, q] =$
      - $[n, p]$ if $p \geq q$
      - $[n, + \infty)$ if $p < q$

---

# Widening Operators (7/7)

#### Example 3.20 (Widening operator for he abstract domain of intervals)

- program : same as example 3.16 and 3.17
- In both case, we obtain the following iteration sequence:
   - $M_0^{\sharp} = \{ \mathrm{x} \mapsto [0, 0] \}$
   - $M_1^{\sharp} = \{ \mathrm{x} \mapsto [0, + \infty) \}$
   - $M_2^{\sharp} = \{ \mathrm{x} \mapsto [0, + \infty) \}$
- The convergence is now very fast, however
   - the result is coarse in the case of program (b),
      - this analysis doesn't converge.
- Some common techniques to obtain more precise result is in section 5.2

---

# Analysis of Loops (1/1)

- semantics of the analysis of loop
   - $\llbracket \mathtt{while} (B) \{ C \} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = \mathscr{F}_{\neg B}^{\sharp} (\mathrm{abs\_iter} ( \llbracket C \rrbracket^{\sharp}_{\mathscr{P}} \circ \mathscr{F}_B^{\sharp}, M^{\sharp} ))$

---

# Analysis of Loops with a Relational Abstract Domain

- Almost same as with a non-relational domain
- Requires only an abstract join or widening operator specific to the abstraction being used

That is,
- In the case of linear equalities
   - widening is not necessary because its height of lattice is finite
- In the case of convex polyhedra and octagons
   - widening operator is required because its height of lattice is infinite.


---

# Another View on the Analysis of Loops (1/n)

- concrete semantics of a loop statement
   - $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \Big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \Big)$
      - = $\mathscr{F}_{\neg B} (M_{\mathrm{loop}})$

Let us consider the following equation:
- $M_{\mathrm{loop}} = \cup_{i \geq 0}(\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^{i}(M)$
   - $= M \cup \Big(\bigcup_{i \gt 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^{i}(M)\Big)$
   - $= M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B \Big(\bigcup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^{i}(M)\Big)$
   - $= M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B (M_{\mathrm{loop}})$

---

# Another View on the Analysis of Loops (2/n)

Observation:
- $M_{\mathrm{loop}}$ is a ***fixpoint*** of a function $G : X \mapsto M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B (X)$
- $M_{\mathrm{loop}}$ is a smallest set of states. $M_{\mathrm{loop}}$ is a ***least fixpoint*** of $G$

We let $\mathbf{lfp}\medspace G$ denote the least fixpoint of $G$.

Then, **concrete semantics** of a loop can be expressed like this
- $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} (\mathbf{lfp}\medspace G) \enspace$
   - where $\enspace G : X \mapsto M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B (X)$

---

# Another View on the Analysis of Loops (2/n)

- **abstract semantics** of a loop relies on the over-approximation of a concrete least fixpoint.

- When the abstract lattice has
   - *finite* height
      - we use abstract union
   - *infinite* height
      - we use widening operator
- We will see several improvements in section 5.2.

---

# That's all 🙂

- $\llbracket n \rrbracket^{\sharp} (M^{\sharp}) = \phi_{\mathscr{V}} (n)$
- $\llbracket \mathrm{x} \rrbracket^{\sharp} (M^{\sharp}) = M^{\sharp}(\mathrm{x})$
- $\llbracket \mathtt{E_0} \odot \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}) = f_{\odot}^{\sharp} (\llbracket \mathtt{E_0} \rrbracket^{\sharp} (M^{\sharp}), \llbracket \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}))$
- $\llbracket \mathtt{C} \rrbracket_{\mathscr{P}}^{\sharp} (\bot) = \bot$
- $\llbracket \mathtt{skip} \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = M^{\sharp}$
- $\llbracket \mathtt{C}_0 ; \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = \llbracket \mathtt{C}_0 \rrbracket_{\mathscr{P}}^{\sharp} (\llbracket \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp}(M^{\sharp}))$
- $\llbracket \mathrm{x} \colonequals \mathtt{E} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp} [\mathrm{x} \mapsto \llbracket \mathtt{E} \rrbracket^{\sharp} (M^{\sharp})]$
- $\llbracket \mathtt{input(\mathrm{x})} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto \top_{\mathscr{V}}]$
- $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket^{\sharp}_{\mathscr{P}}(M^{\sharp}) = \llbracket C_0 \rrbracket_{\mathscr{P}}^{\sharp}(\mathscr{F}^{\sharp}_{B}(M^{\sharp})) \sqcup^{\sharp} \llbracket C_1 \rrbracket^{\sharp}_{\mathscr{P}}(\mathscr{F}^{\sharp}_{\neg B}(M^{\sharp}))$
- $\llbracket \mathtt{while} (B) \{ C \} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = \mathscr{F}_{\neg B}^{\sharp} (\mathrm{abs\_iter} ( \llbracket C \rrbracket^{\sharp}_{\mathscr{P}} \circ \mathscr{F}_B^{\sharp}, M^{\sharp} ))$

---

# Overview

- <gray>Semantics (3.1)</gray>
- <gray>Abstraction (3.2)</gray>
- Computable Abstract Semantics (3.3)
   - <gray>introduction</gray>
   - <gray>semantics of each commands</gray>
   - soundness
- <gray>Interpreter (3.4)</gray>

---

# Soundness (1/2)

<theorem>
<h4>
Theorem 3.6 (Soundness)
</h4>

For all commands $C$ and all abstract states $M^{\sharp}$, the computation of $\gamma (\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}))$ terminates and:

- $\llbracket C \rrbracket_{\mathscr{P}} (\gamma (M^{\sharp})) \subseteq \gamma (\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}))$
   - Proof : by the induction over the syntax of commands.
      - For each kind of commands, we ensured that the definition of its semantics would lead to sound result.
</theorem>

---

# Soundness (2/2)

We can also use best abstraction function $\alpha$ instead of $\gamma$.

- $\alpha ( \llbracket C \rrbracket_{\mathscr{P}} (M)) \sqsubseteq \llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (\alpha (M))$

---

# Analysis of the whole program (1/n)

For instance,
<div class="twocols">
   <p>

- program : $C$
- initial state : $\gamma (M^{\sharp})$
   </p>
   <p class="break">

- output state : $\gamma(\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}))$
- property of interest : $M$
   </p>
</div>

Then, we check whether $\gamma(\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp})) \subseteq M$ holds or not.
- If it holds,
   - $\llbracket C \rrbracket_{\mathscr{P}} \gamma(M^{\sharp}) \subseteq M$ is guaranteed because the analysis is sound.
- If it doesn't hold,
   - no information can be derived from the analysis.
      - the program may violate the property
      - the analysis may be imprecise

---

# Analysis of the whole program (2/n)

In general, if the inclusion does not hold, ***alarms*** will be called.

- alarms : says that the analysis tools failed to prove the property of interest
- triage :
   1. inspect the result of the analysis
   1. decide whether the alarm is true or false

Note:
- The analysis function $\llbracket C \rrbracket^{\sharp}_{\mathscr{P}}$ is not monotone.
   - Therefore, replacing pre-condition $M^{\sharp}$ with more precise one does not ensure that the result is more precise.

---

# Different Abstraction

What if we want to use another abstraction.

The analysis of
- expression
- input

is essentially non-relational abstraction and it has to be modified.

However, in general, overall structure of the analysis doesn't need to be modified.

---

# Overview

- <gray>Semantics (3.1)</gray>
- <gray>Abstraction (3.2)</gray>
- <gray>Computable Abstract Semantics (3.3)</gray>
- Interpreter (3.4)

---

# Interpreter (1/5)

General three steps to construct a static analysis:
1. fix the reference concrete semantics
1. select the abstraction
1. derive analysis algorithm

---

# Interpreter (2/5)

### 1. Concrete Semantics

- $\llbracket C \rrbracket_{\mathscr{P}} : \wp (\mathbb{M}) \longrightarrow \wp (\mathbb{M})$
   - $\mathbb{M}$ : set of memory states
   - $f_{\odot}$ : operations for each operator in the language
   - $\mathscr{F}_{\mathtt{B}}$ : filter functions
   - $\cup$ : union
   - infinite set union, least fixpoint

---

# Interpreter (3/5)

### 2. Abstraction

- $\mathbb{A} = (\mathbb{X} \longrightarrow \mathbb{A}_{\mathscr{V}})$
- $\gamma : \mathbb{A} \longrightarrow \wp (\mathbb{M})$

Note:
- Actual definition relies on
   - the value abstraction $\mathbb{A}_{\mathscr{V}}$
   - the concretization function $\gamma_{\mathscr{V}}$

---

# Interpreter (4/5)

### 3. Abstract Semantics

- $\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} : \mathbb{A} \longrightarrow \mathbb{A}$

Note:
- Actual definition relies on
   - $f^{\sharp}_{\odot}$ : sound over-approximation of $f_{\odot}$
   - $\mathscr{F}^{\sharp}_{\mathtt{B}}$ : abstract filter function (which is sound with respect to $\mathscr{F}_{\mathtt{B}}$)
   - $\sqcup^{\sharp}$ : sound over-approximation of $\cup$
   - over-approximation of concrete fixpoint
      - based on a widening operator

---

# Interpreter (5/5)

This division of the analysis design into independent steps is important
- for the construction of a static analysis
- when a static analysis needs to be improved ( a static analysis is imprecise )

Common case a static analysis is imprecise:
- abstraction is coarse (step 2)
- algorithm return overly approximated result  (step 3)
- concrete semantics is too coarse to express the properties of interest (step 1)

