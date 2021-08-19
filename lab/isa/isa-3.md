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

- Semantics (3.1)
- Abstraction (3.2)
- Computable Abstract Semantics (3.3)
   - a
- Interpreter (3.4)


