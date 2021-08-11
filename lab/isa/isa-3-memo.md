# memo of isa 3

## 3.1.1

> To illustrate the concepts of static analysis, we do not require a language with a very large set of features.

- $\mathbb{V}$ : a set of scalar value
- $\mathbb{X}$ : a finite set of variable( which is fixed )
- $\mathbb{B} = \{ \mathbf{true}, \mathbf{false} \}$

For other syntax see figure 3.1.

## 3.1.2

There're several kind of semantics( also called concrete semantics in the rest of this chapter ). For instance, __trace semantics__, __denotational semantics__.
> __trace semantics__ : describes program execution as a sequence of program state
> __denotational semantics__ : describes only input-output relation

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

Focusing the post-condition is enough.

### An Input-Output Semantics

- Input-output semantics : characterize the effect of executing a program using a mathematical function that maps input states into corresponding output state.
   - program execution : input states --(mathematical function)--> output states

> program semantics : set of input states $\longmapsto$ set of output states

Note that `input` is non-deterministic execution( we may observe infinitely many output states, so output is a set of states).
Input is also a set of states for compositionality.

This is a very basic form of __denotational__ semantics. This behaves like __interpreter__.

Definition of interpreter would be:

> program and input state $\longmapsto$ output state

This definition is very close to that of input-output semantics.
The main difference is that interpreter inputs single state and output single state.
Besides this point, the interpreter essentially implements the input-output semantics.

**NOTE** : input-output semantics is **compositional**.

**compositional** : the semantics of a command composed of several sub-commands(e.g. sequence or conditional) can be defined simply by composing the semantics of the sub-commands.

e.g.:
> $C \colonequals C_1; C_2$

Semantics of C can be defined by that of C_1 and C_2.

### Memory States (Control States can be ignored)

In general, __program state__ should capture the configuration of the computer.
Therefore it should include a __memory state__ and __control state__.

- memory state : contents of the memory
- control state : a value of "program counter" (or next command to be executed)

In this chapter, we define an input-output semantics, so control states can be safely ignored.
Because input(output) state is fully determined by the memory state, a state is defined as a memory state.

In our language, memory state $M$ is defined by:
$$
\mathbb{M} = \mathbb{X} \longrightarrow \mathbb{V}
$$

If we map `x` to 2 and `y` to 7 in a memory state $m$:
$$
m = \{ \mathrm{x} \mapsto 2, \mathrm{y} \mapsto 7\}
$$

> ここで言うmemory stateとは変数からスカラー値のマッピングのこと

### Semantics of Scalar Expressions

- Scalar Expressions --(evaluation)--> Scalar Value

e.g.:

- $n$ : constant
- $\mathrm{x}$ : variable

> `n` ----> `n`
> `x` ----> value of `x` in the memory state

See figure 3.2

- $\llbracket E \rrbracket(m)$ : semantics of expression $E$, in the memory state $m$.
   - $\llbracket E \rrbracket : \mathbb{M} \longrightarrow \mathbb{V}$
      - This is a function from memory states to scalar values

Semantics of each scalar expression is as follows:
- $\llbracket n \rrbracket (m) = n$
- $\llbracket \mathrm{ x } \rrbracket (m) = m(\mathrm{ x })$
   - $m(\mathrm{x})$ : value of x in the memory state $m$
- $\llbracket E_0 \odot E_1 \rrbracket (m) = f_{\odot}(\llbracket E_0 \rrbracket (m), \llbracket E_1 \rrbracket (m))$
   - $f_{\odot}$ : mathematical function associated to the operator $\odot$

### Semantics of Boolean Expressions

See figure 3.3

- $\llbracket B \rrbracket = \mathbb{M} \longrightarrow \mathbb{B}$
   - This is a function from memory states to boolean values
- $\llbracket \mathrm{x} < n \rrbracket = f_{<}(m(\mathrm{x}), n)$
   - $ < $ : denote comparison operators (Couldn't find the symbol in the book)

### Semantics of Commands

- $\llbracket C \rrbracket_{\mathscr{P}}$ : semantics of a command $C$
   - a set of input states to a set of output states( which is observed __after__ the command )
      - non-terminating executions are not observed
- $\wp(\mathbb{ M })$ : power set of memory states
   - $\mathbb{M}$ : an element of $\wp(\mathbb{ M })$.

Semantics of commands is:

- $\llbracket \texttt{slip} \rrbracket_{\mathscr{P}}(M) = M$
   - identity function
- $\llbracket C_0 ; C_1 \rrbracket_{\mathscr{P}}(M) = \llbracket C_1 \rrbracket_{\mathscr{P}}(\llbracket C_0 \rrbracket_{\mathscr{P}}(M))$
   - composition of the semantics of each commands
- $\llbracket \mathrm{x} \colonequals E \rrbracket_{\mathscr{P}}(M) = \{m[\mathrm{x} \mapsto \llbracket E \rrbracket(m)] | m \in M\}$
   - the evaluation of assignment updates the value of $\mathrm{x}$ in the memory states with the result of the evaluation of $E$.
- $\llbracket \texttt{input} (\mathrm{x}) \rrbracket_{\mathscr{P}}(M) = \{ m[ \mathrm{x} \mapsto n ] | m \in M, n \in \mathbb{V} \}$
   - replace the value of $x$ with any possible scalar value.

Quite easy, isn't it ?
The semantics below is a little bit complicated.

First, we need to define a filtering function $\mathscr{F}_B$. This function filter out memory states.

Def:
$$
\mathscr{F}_{B}(M) = \{m \in M | \llbracket B\rrbracket(m) = \mathbf{true}\}
$$

Intuitive explanation : this function filter out memory states $m$ in which $B$ doesn't hold or can't be defined.

- $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket_{\mathscr{P}}(M) = \llbracket C_0 \rrbracket_{\mathscr{P}}(\mathscr{F}_{B}(M)) \cup \llbracket C_1 \rrbracket_{\mathscr{P}}(\mathscr{F}_{\neg B}(M))$
   - union of the results of each branch

- $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \big)$
   - complicated...

Let $M_i$ be a state produced by program execution that repeated the loop $i$ times.

$M_i$ is defined as follows:

$$
M_i = \mathscr{F}_{\neg B} \big( ( \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B )^i (M) \big)
$$

Intuitive explanation : $B$ evaluates to __true__ $i$ times and to __false__ for the last(i+1-th test).

- $\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B$ : filter memory states, then execute the command.

The set of output states would be $M_0 \cup M_1 \cup M_2 \dots$, that is :

$$
\cup_{i \geq 0} M_i = \cup_{i \geq 0} \mathscr{F}_{\neg B} \big( (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^i (M) \big)
$$

$\mathscr{F}_B$ commutes with the union, thus,

$$
\cup_{i \geq 0} M_i = \mathscr{F}_{\neg B} \Big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^i (M) \Big)
$$

Therefore,

> - $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \big)$

## 3.2.1

We will see definition of:
- abstract domains
- concretization function

### Intuition Gathered from the Previous Chapter

Abstractions based on signs, intervals, and convex polyhedra are meaningful for our language introduced in chapter 3.1.
The analysis we will see also behaves like the intuitive analysis in chapter 2.

### Concrete Elements, Abstract Elements and Abstraction Relation

We carefully distinguish between these:
- abstract domain that is used for the analysis of program ($\longrightarrow$ "___abstract___" qualifier for this)
- domain the program is defined ($\longrightarrow$ "___concrete___" qualifier for this)

#### Definition 3.1 (Concrete Domain) :

- concrete domain : a set $\mathbb{C}$ used to describe concrete behaviors
   - $\subseteq$ : order relation that compare program behaviors in the logical point of view
      - $x \subseteq y$ means that $x$ implies behavior y, that is:
         - $x$ expresses a stronger property than $y$.

#### Example 3.1 (Concrete Domain) :

- $\mathbb{C} = \wp (\mathbb{M})$
   - choose $\wp (\mathbb{M})$ as the concrete domain to study the sets of reachability states.

Some preparations:

- A way to interpret elements of an abstract domain with respect to the concrete level.
   - $c$ : concrete element
   - $a$ : abstract element
   - $c \vDash a$ : $c$ satisfies the logical properties expressed by $a$

#### Definition 3.2 (Abstract domain and abstract relation):

- abstract domain : a pair of a set $\mathbb{A}$ and an ordering relation $\sqsubseteq$ over that set.

Given a concrete domain $(\mathbb{C}, \subseteq)$, abstraction is defined by:
- $(\mathbb{A}, \sqsubseteq)$
- an abstract relation "$\vDash$" such that:
   - for all $c \in \mathbb{C}, a_0, a_1 \in \mathbb{A},$ if $c \vDash a_0$ and $a_0 \sqsubseteq a_1,$ then $c \vDash a_1;$ and
   - for all $c_0, c_1 \in \mathbb{C}, a \in \mathbb{A},$ if $c_0 \subseteq c_1$ and $c_1 \vDash a,$ then $c_0 \vDash a$.

<!-- Example of abstraction : -->
<!-- 1. for the first one : -->
<!--    - $c$ : $x = 1$ -->
<!--    - $a_0$ : $x > 1$ -->
<!--    - $a_1$ : $x > 0$ -->
<!-- 2. for the second one : -->
<!--    - $c_0$ : $x = 1, y = 1$ -->
<!--    - $c_1$ : $x = 1$ -->
<!--    - $a$ : $x > 0$ -->

<!-- I might make a graph. -->

#### Example 3.2 (Abstraction) :

I might make some graph.

- concrete domain : $\wp (\mathbb{M})$
- variable : $\mathrm{x}, \mathrm{y}$

Elements of concrete domain :
- $M_0 = {m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x}) < m(\mathrm{y}) \leq 8}$
- $M_1 = {m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x})}$

An element of abstract domain :
- $M^{\sharp}$ : over-approximates each value
   - $\mathrm{x}$ : [0, 10]
   - $\mathrm{y}$ : [0, 100]

Then,
- $M_0 \vDash M^{\sharp}$ : any memory state in $M_0$ is included in $M^{\sharp}$.
- $M_1 \nvDash M^{\sharp}$ : $(11,0)$ is an element of $M_1$, but doesn't satisfy $M^{\sharp}$

#### Concretization Function

Sometimes, $"\vDash"$ is not useful. Thus, we define concretization function.

#### Definition 3.3 (Concretization function)

A concretization function (or, for short, concretization) :
- $\gamma : \mathbb{A} \rightarrow \mathbb{C}$
   - for any abstract element $a$, $\gamma (a)$ satisfies $a$. ($\gamma (a) \vDash a$)
   - $\gamma (a)$ is the maximum element of $\mathbb{C}$ that satisfies $a$

******

A concretization function fully describe the abstraction relation, because of the following equivalence:

- $\forall c \in \mathbb{C}, a \in \mathbb{A}$
   - $\enspace c \vDash a \iff c \subseteq \gamma (a)$

Concretization function is also monotone.

#### Example 3.3 (Concretization function)

- same notion as example 3.2. ($M^{\sharp}, M_0, M_1$)
- There are memory states in $\gamma (M^{\sharp})$ that are not in $M_1$

#### Abstraction Function and Galois Connection

#### Definition 3.4 (Abstraction function)

Let $c$ be a concrete element. $c$ has a **best abstraction** if and only if there exists an abstract element such that:
- $a$ is an abstraction of $c$
- any other abstraction of $c$ is greater than $c$.

Abstraction function is a function $\alpha : \mathbb{C} \rightarrow \mathbb{A}$ that:
- maps each concrete element to its best abstraction

An abstraction function is:
- dual of concretization function
- monotone

#### Example 3.4 (Abstraction function)
- same notion as example 3.2 and 3.3
- $M^{\sharp}$ is not a best abstraction of $M_0$
   - Best abstraction of $M_0$ is smaller than $M^{\sharp}$

******

Note:
- The existence of a best abstraction is not guaranteed in general
- Abstract relations such that no concretization function can be defined will not arise in this book.

When an abstraction relation defines both a concretization function and abstraction function, they are tightly related to each other (which we call *Galois connection*).

Definition 3.5 (Galois connection):

Galois connection is a pair made of a concretization function $\gamma $and an abstraction function $\alpha$ such that:
- $\forall c \in \mathbb{C}, \forall a \in \mathbb{A}$
   - $\enspace \alpha (c) \sqsubseteq a \iff c \subseteq \gamma (a)$

We write such a pair as follows:
- $(\mathbb{C}, \subseteq) \xtofrom[\alpha]{\gamma}(\mathbb{A}, \sqsubseteq)$

Some interesting properties (proof is in B.1):
- $\alpha$ and $\gamma$ are monotone function.
- $\forall c \in \mathbb{C}$
   - $c \subseteq \gamma (\alpha (c))$
   - applying the abstraction function and concretizing the result back yield a less precise result
- $\forall a \in \mathbb{A}$
   - $\alpha (\gamma (a)) \sqsubseteq a$
   - concretizing an abstract element and abstracting the result back refines the information available in the initial abstract element (which is known as *reduction*)

## 3.2.2 Non-Relational Abstraction

non-relational abstraction : does not treat relations among program variable.
This abstraction proceeds in two step :
1. For each variable, it collects the values that the variable may take.
2. Then, over-approximates each of these set of values with one abstract element per variable.


#### Definition 3.6 (Value abstraction)

A value abstraction is an abstraction of $(\wp (\mathbb{V}), \subseteq)$

As we saw in chapter 2, interval and sign constraints define value abstractions.

#### Example 3.5 (Signs)

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

I need to make the diagram.
- node : abstract elements
- edge : link neighbors in the ordering relation

In the convention:
- "smaller" abstract elements are lower than "bigger" abstract element.

#### Example 3.6 (A variation on the lattice of sign, with no abstraction function)

- If we remove $[= 0]$ from the abstract domain above, it doesn't have best abstract function.
- concrete set $\{ 0 \}$
   - we can't define abstraction function of this
   - $[\leq]$ and $[\geq]$ are incomparable

As a consequence:
- in general, it is impossible to identify one element as a most precise (sound) one.

> Provided the analysis designer and user are aware of this fact, it is not a serious limitation, however.

#### Example 3.7 (Intervals)

- intervals value abstract domain $\mathbb{A}_{\mathscr{S}}$ :
   - $\bot$ : the empty set of values
   - $(n_0, n_1)$ : 
      - $n_0$ : either $- \infty$ or a value
      - $n_1$ : either $\infty$ or a value
      - $n_0 \leq n_1$
- concretization function :
   - $\gamma_{\mathscr{S}}$ :
      - $\bot \longmapsto \empty$
      - $[n_0, n_1]\enspace \longmapsto \enspace \{n \in \mathbb{V} \enspace | \enspace n_0 \leq n \leq n_1\}$
      - $[n_0, + \infty]\enspace \longmapsto \enspace \{n \in \mathbb{V} \enspace | \enspace n_0 \leq n\}$
      - $[- \infty, n_1]\enspace \longmapsto \enspace \{n \in \mathbb{V} \enspace | \enspace n \leq n_1\}$
      - $(- \infty, + \infty) \longmapsto \mathbb{V}$

The order relation is defined by the inclusion of concrete interval.

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

******
ここでのお気持ちをまとめておく。
Value Abstractionは一つの変数に関するabstraction。(Non-)relational abstractionは複数の変数をまとめた時のabstraction。

複数の変数を扱えるようになっている。
******

#### Definition 3.7 (Non-relational abstraction)

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

End of definition.

Intuitive explanation:
- treats each variable independently
- applies the value abstraction to each variable separately from the other
- order relation is point-wise

The least element of the non-relational abstract domain is
- the function that maps each variable to the least element $\bot_{\mathscr{V}}$ :
   - $\forall \mathrm{x} \in \mathbb{X}, \bot_{\mathscr{N}} (\mathrm{x}) = \bot_{\mathscr{V}}$

The greatest element $\top_{\mathscr{N}}$ can be defined similarly.

When the value abstraction has an abstraction function $\alpha_{\mathscr{V}}$, the non-relational abstraction also has one.

It is defined as follows:
- $\alpha_{\mathscr{N}}$ : $M \longmapsto \Big( (\mathrm{x} \in \mathbb{X}) \longmapsto \alpha_{\mathscr{V}} (\{ m(\mathrm{x}) \enspace | \enspace m \in M\}) \Big)$

Note:
- $\bot_{\mathscr{N}}$ is the best abstraction of $\empty$

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
   - $M^{\sharp}$ : $\quad \mathrm{x} \mapsto [\geq 0] \quad \mathrm{y} \mapsto \top \quad \mathrm{z} \mapsto [\leq 0]$
- With the intervals abstraction :
   - $M^{\sharp}$ : $\quad \mathrm{x} \mapsto [25, 35] \quad \mathrm{y} \mapsto [-7, 8] \quad \mathrm{z} \mapsto [-12, -9]$

### 3.2.3 Relational Abstraction

Such as *convex polyhedra*.

#### Definition 3.8 (Linear equalities)

- The elements of abstract domain of linear equalities :
   - $\bot$ : empty set
   - conjunctions of linear equality constraints over the program variables : to constrain sets of memory states.
      - I don't know what this means
      - $\mathrm{x} = a \mathrm{y}$

I might make some graph of linear equalities.

In the geometrical point of view :
- abstract elements are in the affine space $\mathbb{V}^{N}$
   - $N$ : dimension (number of variables)

This abstraction features :
- best abstraction function
- concretization

The best abstraction of any set $M$ of memory states :
- the smallest affine space that contains all the memories in $M$

### Definition 3.8 (Convex polyhedra)

- elements of abstract domain of linear inequalities :
   - $\bot$ : empty set
   - conjunctions of linear inequality constraints over the program variables : to constrain sets of memory states.

In the geometrical point of view :
- abstract elements : convex polyhedra of all dimension in $\mathbb{V}^{N}$
   - $N$ : dimension (number of variables)

This abstraction features :
- concretization
- but no no best abstraction function (certain concrete sets do have a best abstraction)

I might make some graph of convex polyhedra.

The cost :
- the number of inequality is not bounded.

### Definition 3.9 (Octagons)

- element of abstract domain of octagons :
   - $\bot$ : empty set
   - conjunctions of linear inequality constraints of the form below:
      - $\pm\mathrm{x} \pm \mathrm{y} \leq c$
      - $\pm x = c$

In the geometrical point of view :
- abstract elements : "octagonal" shape

This abstraction has both :
- best abstraction function
- concretization function

In general, choice of an efficient computer representation for abstract domains that describe relational constraints is more difficult than in the case of non-relational domains. Therefore, we do not discuss this topic any further.
