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
- $M_0 = \{m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x}) < m(\mathrm{y}) \leq 8\}$
- $M_1 = \{m \in \mathbb{M} \enspace | \enspace 0 \leq m(\mathrm{x})\}$

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
- the dual of concretization function
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
- but no best abstraction function (certain concrete sets do have a best abstraction)

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

## 3.3 Computable Abstract Semantics

- we use non-relational abstract domain
   - we also discuss the modifications which is required to use relational abstract domain.

The form of analysis is :
- mathematical function
   - input : a program and an abstract pre-condition
   - output : an abstract post-condition

Some preparations :
- $\mathbb{A}$ : the state abstract domain
- $\gamma$ : associated concretization function
   - $\mathbb{A}_{\mathscr{V}}$ : underlying value abstraction.
   - $\gamma_{\mathscr{V}}$ : concretization function

The design of the analysis aims at:
- the soundness in the sense of definition 2.6
   - See figure 3.7
      - $\llbracket \mathrm{p} \rrbracket_{\mathscr{P}}^{\sharp}$ : the static analysis function (or *abstract semantics*)

$$
\begin{CD}
   a_{\mathrm{pre}} @>\mathrm{analyze}\enspace\mathtt{p}>> a_{\mathrm{post}} = \llbracket \mathtt{p} \rrbracket^{\sharp}_{\mathscr{P}} (a_{\mathrm{pre}}) \\
@A\in \gamma (\cdot)AA @AA\in \gamma (\cdot)A \\
   m @>\mathrm{run}\enspace\mathtt{p}>>m\rq
\end{CD}
$$

The best abstraction function $\alpha$ can also be used if it exists to discuss the *soundness*.

### Abstract Semantics of Each Commands

#### introduction

As in chapter 2, we are going to construct the definition of $\llbracket \cdot \rrbracket_{\mathscr{P}}^{\sharp}$ by induction.

- Definition of the semantics are very similar to that of concrete semantics.
- Easy to see that soundness is ensured.
   - we will see that soundness is ensured in a inductive manner.
- Abstract semantics of a command is defined by that of its sub-commands.
   - we assume that the semantics of sub-commands has already defined.

#### Bottom Element

- $\llbracket \mathtt{C} \rrbracket_{\mathscr{P}}^{\sharp} (\bot) = \bot$
   - intuitive explanation : running a program from empty set of states is empty.
- soundness : ensured

#### Skip Commands

- $\llbracket \mathtt{skip} \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = M^{\sharp}$
- soundness : ensured
   - input is not modified

#### Sequences of Commands

- Soundness property of figure 3.7 is stable is under composition.
   - $\llbracket \mathtt{p}_0 ; \mathtt{p}_1 \rrbracket_{\mathscr{P}} (M) = \llbracket \mathtt{p}_0 \rrbracket_{\mathscr{P}} (\llbracket \mathtt{p}_1 \rrbracket_{\mathscr{P}} (M))$
- $\llbracket \mathtt{C}_0 ; \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp} (M^{\sharp}) = \llbracket \mathtt{C}_0 \rrbracket_{\mathscr{P}}^{\sharp} (\llbracket \mathtt{C}_1 \rrbracket_{\mathscr{P}}^{\sharp}(M^{\sharp}))$
   - this equation ensures that we can prove soundness by induction.

#### Theorem 3.1 (Approximation of composition)

Some preparations :
- $F_0$, $F_1$ : $\wp (\mathbb{M}) \rightarrow \wp (\mathbb{M})$
   - two monotone functions
- $F_0^{\sharp}$, $F_1^{\sharp}$ : $\mathbb{A} \rightarrow \mathbb{A}$
   - these two functions over-approximate the two function above.
   - such that
      - $F_0 \circ \gamma \subseteq \gamma \circ F_0^{\sharp}$ and $F_1 \circ \gamma \subseteq \gamma \circ F_1^{\sharp}$
- then, $F_0 \circ F_1$ can be over-approximated by $F_0^{\sharp} \circ F_1^{\sharp}$

I might make some graph. (I've already made a graph in my notebook)

- Assumption : $M^{\sharp} \in \mathbb{A}$
- $F_1 \circ \gamma (M^{\sharp}) \subseteq \gamma \circ F_1^{\sharp} (M^{\sharp})$ ( by the soundness of $F_1$ )
- $F_0 \circ F_1 \circ \gamma (M^{\sharp}) \subseteq F_0 \circ \gamma \circ F_1^{\sharp} (M^{\sharp})$ ( applied $F_0$, since $F_0$ is monotone)
   - $\subseteq \gamma \circ F_0^{\sharp} \circ F_1^{\sharp} (M^{\sharp})$ ( by the soundness of $F_0$ )
- then,
   - $F_0 \circ F_1 \circ \gamma (M^{\sharp}) \subseteq \gamma \circ F_0^{\sharp} \circ F_1^{\sharp} (M^{\sharp})$
- so, $F_0 \circ F_1$ is over-approximated by $\circ F_0^{\sharp} \circ F_1^{\sharp}$

Note:
- concrete semantics heavily relies on this composition of function.

## 3.3.1 Abstract Interpretation of Assignment

- update to memory states are performed by assignment commands.
- base idea is identical to that of chapter 2.

We first need to define the interpretation of expressions and then, interpretation of assignment.

### Abstract Interpretation of Expressions

- $\llbracket \mathtt{E} \rrbracket^{\sharp}$ : abstract interpretation of expressions
   - $\llbracket \mathtt{E} \rrbracket^{\sharp} : \mathbb{A} \rightarrow \mathbb{A}_{\mathscr{V}}$ 
- semantics of expressions
   - $\llbracket n \rrbracket^{\sharp} (M^{\sharp}) = \phi_{\mathscr{V}} (n)$
      - This shoud return any abstract element that over-approximate $n$
      - If the value abstraction has a best abstraction $\alpha_{\mathscr{V}}$, $\alpha_{\mathscr{V}} (\{n\})$ is enough.
      - $\phi_{\mathscr{V}} : \mathbb{V} \rightarrow \mathbb{A}_{\mathscr{V}}$
         - This function may not return the most precise abstraction.
         - This function is such that $n \in \gamma_{\mathscr{V}} (\phi_{\mathscr{V}}(n))$
   - $\llbracket \mathrm{x} \rrbracket^{\sharp} (M^{\sharp}) = M^{\sharp}(\mathrm{x})$
      - simply return a abstraction that is associated to the variable.
   - $\llbracket \mathtt{E_0} \odot \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}) = f_{\odot}^{\sharp} (\llbracket \mathtt{E_0} \rrbracket^{\sharp} (M^{\sharp}), \llbracket \mathtt{E_1} \rrbracket^{\sharp} (M^{\sharp}))$
      - we need to apply the conservative abstraction of $f_\odot$ in the non-relational lattice.
      - we need an operator $f_{\odot}^{\sharp}$ such that:
         - for all $n_{0}^{\sharp}, n_{1}^{\sharp} \in \mathbb{A}_{\mathscr{V}}$, $\{ f_{\odot} (n_0, n_1) \enspace | \enspace n_0 \in \gamma_{\mathscr{V}} (n_{0}^{\sharp})$ and $n_1 \in \gamma_{\mathscr{V}} (n_{1}^{\sharp}) \} \subseteq \gamma_{\mathscr{V}} (f_{\odot}^{\sharp} (n_{0}^{\sharp}, n_{1}^{\sharp}))$
      - $f_{\odot}^{\sharp}$ shoud over-approximate the effect of operation of $f_{\odot}$ on concrete value.

#### Example 3.10 (Abstract semantics of expressions)

Assumption:
- we use interval abstraction
- we consider $\mathrm{x} + 2 * \mathrm{y} - 6$
- $M^{\sharp}$ is defined by $M^{\sharp} (\mathrm{x}) = [10, 20]$ and $M^{\sharp} (\mathrm{y}) [8, 9]$

Interpretation of the expressions would be like this:
- $\llbracket \mathrm{x} + 2 * \mathrm{y} - 6 \rrbracket^{\sharp} (M^{\sharp})$
   - $= f_{-}^{\sharp} (\llbracket \mathrm{x} + 2 * \mathrm{y} \rrbracket^{\sharp}(M^{\sharp}), \llbracket 6 \rrbracket^{\sharp}(M^{\sharp}))$
   - $= f_{+}^{\sharp} (\llbracket \mathrm{x} \rrbracket^{\sharp}(M^{\sharp}), \llbracket 2 * \mathrm{y} \rrbracket^{\sharp}(M^{\sharp})) - [6,6]$
   - $= M^{\sharp}(\mathrm{x}) + f_{*}^{\sharp}(\llbracket 2 \rrbracket^{\sharp}(M^{\sharp}), \llbracket \mathrm{y} \rrbracket^{\sharp}(M^{\sharp})) - [6,6]$
   - $= [10, 20] + [2,2] * [8,9] - [6,6]$
   - $= [10,20] + [16, 18] - [6,6]$
   - $= [26, 38] - [6, 6]$
   - $= [20, 32]$

This semantics is sound (we will not see the proof though).

Theorem 3.2 (Soundness of the abstract interpretation of expressions)

- for
   - all expressions $\mathtt{E}$
   - all non-relational abstract elements $M^{\sharp}$
   - all memory states $m$ such that $m \in \gamma (M^{\sharp})$
- $\llbracket \mathtt{E} \rrbracket (m) \in \gamma_{\mathscr{V}} (\llbracket \mathtt{E} \rrbracket^{\sharp}(M^{\sharp}))$

### Analysis of Assignments

Recall that
> $\llbracket \mathrm{x} \colonequals E \rrbracket_{\mathscr{P}}(M) = \{m[\mathrm{x} \mapsto \llbracket E \rrbracket(m)] \enspace | \enspace m \in M\}$

1. Evaluation of the expression $\mathtt{E}$ to $n$
2. Update the variable $\mathrm{x}$ with $n$

This composition can be over-approximated piece by piece (Theorem 3.1).

- target : $\mathrm{x} \colonequals \mathtt{E}$
- semantics of assignments
   - $\llbracket \mathrm{x} \colonequals \mathtt{E} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp} [\mathrm{x} \mapsto \llbracket \mathtt{E} \rrbracket^{\sharp} (M^{\sharp})]$
- semantics of input
   - $\llbracket \mathtt{input(\mathrm{x})} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto \top_{\mathscr{V}}]$
      - repaleced the value with $\top_{\mathscr{V}}$

#### Example 3.11 (Analysis of an assignment command)

- $\llbracket \mathrm{x} \colonequals \mathrm{x} + 2 * \mathrm{y} - 6 \rrbracket^{\sharp} (M^{\sharp}) = \{ \mathrm{x} \mapsto [20,32], \mathrm{y} \mapsto [8,9] \}$

### Analysis of Assignments Using a Relational Abstract Domain

> 1. Add temporary dimension $\mathrm{x}\rq$ that is meant to describe the value of the expression
> 1. Represent as precisely as possible the constraint $\mathrm{x}\rq = \mathtt{E}$
> 1. Project out dimension $\mathrm{x}$, and rename $\mathrm{x}\rq$ to $\mathrm{x}$

#### Example 3.12

Assumption:
- abstract domain : convex polyhedra
- abstract pre-condition : $2 \leq \mathrm{x} \leq 3 \land 1 - \mathrm{x} \leq \mathrm{y}$
- assignment : $\mathrm{x} \colonequals \mathrm{y} + \mathrm{x} + 2$

We introduce the variable $\mathrm{x}\rq$ and write the constraint as below:
- $2 \leq \mathrm{x} \leq 3 \land 1 - \mathrm{x} \leq \mathrm{y} \land \mathrm{x}\rq = \mathrm{y} + \mathrm{x} + 2$

From the last term, we get $\mathrm{x} = \mathrm{x}\rq - \mathrm{y} - 2$. Then, apply this formula and we get
- $2 \leq \mathrm{x}\rq - \mathrm{y} - 2 \leq 3 \land 3 - \mathrm{x}\rq + \mathrm{y} \leq \mathrm{y}$
- $\iff 4 \leq \mathrm{x}\rq - \mathrm{y} \leq 5 \land 3 \leq \mathrm{x}\rq$

### 3.3.2 Abstract Interpretation of Conditional Branching

- An in the last paragraph, we over-approximate the definition of concrete semantics step-by-step.

> $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket_{\mathscr{P}}(M) = \llbracket C_0 \rrbracket_{\mathscr{P}}(\mathscr{F}_{B}(M)) \cup \llbracket C_1 \rrbracket_{\mathscr{P}}(\mathscr{F}_{\neg B}(M))$

We will first design an operation to over-approximate $\mathscr{F}_{B}$ for any Boolean expression $B$.
Then, we will use the semantics of conditional branching,
and finally, we will apply the over-approximation of the union of concrete sets.

#### Analysis of Conditions

- abstraction of filtering function $\mathscr{F}_B$, which we denote by $F_{B}^{\sharp}$
- $\mathscr{F}_B$
   - input : memory states
   - output : kmemory states such that $B$ evaluates to *true*.
- $\mathscr{F}_B^{\sharp}$
   - input : an abstract state
   - output : an abstract state refined by the condition $B$

$\mathscr{F}_B^{\sharp}$ should satisfies the following soundness condition (ref. figure 3.7):
- for
   - all conditions $B$
   - all abstract states $M^{\sharp}$ 
- $\mathscr{F}_{B}(\gamma (M^{\sharp})) \subseteq \gamma (\mathscr{F}_B^{\sharp} (M^{\sharp}))$

We will see some representative case.

- Sign abstract domain $\{ \bot, \top, [=0], [\geq 0], [\leq 0] \}$
   - $\mathscr{F}_{\mathrm{x} \lt 0}^{\sharp} (M^{\sharp})= $
      - $(\mathrm{y} \in \mathbb{X}) \mapsto \bot \enspace$ if $M^{\sharp} (\mathrm{x}) = [\geq 0]$ or $[= 0]$ or $\bot$
      - $M^{\sharp} [\mathrm{x} \mapsto [\leq]]\enspace$ if $M^{\sharp} (\mathrm{x}) = [\leq 0]$ or $\top$


- Interval abstract domain $M^{\sharp}(\mathrm{x}) = [a, b]$
   - $\mathscr{F}_{\mathrm{x} \leq n}^{\sharp}(M^{\sharp}) = $
      - $(\mathrm{y} \in \mathbb{X}) \mapsto \bot$ if $a > n$
      - $M^{\sharp} [\mathrm{x} \mapsto [a, n]]\enspace$ if $a \leq n \leq b$
      - $M^{\sharp}$ if $b \leq n$


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
- pre-condition $M^{\sharp}$ is defined by:
   - $\mathrm{x} \mapsto \top$
   - $\mathrm{y} \mapsto \top$

Then, by the rule above,
- $\mathscr{F}_{\mathrm{x} \gt 7} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto [8, + \infty)]$
- $\mathscr{F}_{\mathrm{x} \leq 7} (M^{\sharp}) = M^{\sharp}[\mathrm{x} \mapsto (-\infty, 7]]$
   - refined

Although we will not see the full definition of the condition analysis, it should be sound.

#### Theorem 3.3 (Soundness of the abstract interpretation conditions)

- for all
   - expressions $B$
   - non-relational abstract elements $M^{\sharp}$
   - memory states $m$ such that $m \in \gamma(M^{\sharp})$
- if $\llbracket B \rrbracket (m) = \mathbf{true}$, then $m \in \gamma (\mathscr{F}_B^{\sharp} (M^{\sharp}))$

### Analysis of Flow Joins

The concrete semantics computes the union of the results of both branches.
Thus, the analysis should over-approximate unions of sets of concrete states.
Therefore, the abstract join operator $\sqcup^{\sharp}$ should satisfy the following soundness property:
 
- $\gamma(M_0^{\sharp}) \cup \gamma(M_1^{\sharp}) \subseteq \gamma(M_0^{\sharp} \sqcup^{\sharp} M_1^{\sharp})$

To define the abstract union operator we can simply
- define a join operator $\sqcup^{\sharp}_{\mathscr{V}}$ in the value abstract domain.
- apply operator $\sqcup^{\sharp}_{\mathscr{V}}$ in a point-wise manner:
   - for all variable $\mathrm{x}$, $(M_0^{\sharp} \sqcup^{\sharp} M_1^{\sharp}) (\mathrm{x}) = M_0^{\sharp}(\mathrm{x}) \sqcup^{\sharp}_{\mathscr{V}} M_1^{\sharp}(\mathrm{x})$

The definition of $\cup^{\sharp}_{\mathscr{V}}$ depends on the abstract domain.
For instance, for the interval domain:
- $[a_0, b_0] \cup^{\sharp}_{\mathscr{V}} [a_1, b_1] = [\mathtt{ min }(a_0, b_0), \mathtt{ max }(a_1, b_1)]$
- $[a_0, b_0] \cup^{\sharp}_{\mathscr{V}} [a_1, +\infty] = [\mathtt{ min }(a_0, b_0), +\infty]$

The union operator defined for the non-relational domain is sound in the following sense.

#### Theorem 3.4 (Soundness of abstract join)

Let $M_0^{\sharp}$ and $M_1^{\sharp}$ be the two abstract states.
 
- $\gamma(M_0^{\sharp}) \cup \gamma(M_1^{\sharp}) \subseteq \gamma(M_0^{\sharp} \cup^{\sharp} M_1^{\sharp})$

#### Example 3.14 (Analysis of flow joins)

- $M_0^{\sharp} = \{ \mathrm{x} \mapsto [0,3], \mathrm{y} \mapsto [6,7], \mathrm{z} \mapsto [4,8] \}$
- $M_1^{\sharp} = \{ \mathrm{x} \mapsto [5,6], \mathrm{y} \mapsto [0,2], \mathrm{z} \mapsto [6,9] \}$

Then,

- $M_0^{\sharp} \cup^{\sharp} M_1^{\sharp} = \{ \mathrm{x} \mapsto [0, 6], \mathrm{x} \mapsto [0, 7], \mathrm{x} \mapsto [6, 9]\}$


### Analysis of a Conditional Commands

Now, we have defined
- condition
- flow joins

and we can use those to define the semantics of conditional commands.

Semantics of conditional commands:
- $\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket^{\sharp}_{\mathscr{P}}(M^{\sharp}) = \llbracket C_0 \rrbracket_{\mathscr{P}}^{\sharp}(\mathscr{F}^{\sharp}_{B}(M^{\sharp})) \sqcup^{\sharp} \llbracket C_1 \rrbracket^{\sharp}_{\mathscr{P}}(\mathscr{F}^{\sharp}_{\neg B}(M^{\sharp}))$

This definition is very similar to that of concrete one.

#### Example 3.15 (Analysis of a conditional command)

We use this program from example 3.13 here.

```c
if(x > 7){
   y := x - 7
}else{
   y := 7 - x
}
```

- abstract pre-condition : $M^{\sharp} = \{ \mathrm{x} \mapsto \top, \mathrm{y} \mapsto \top \}$

Analysis proceeds as follows :

1. the analysis of **true** branch
   1. filters pre-condition
   2. computes the post-condition for the assignment of $\mathrm{y} \colonequals \mathrm{x} - 7$
   3. we get:
      - $\{ \mathrm{x} \mapsto [8, + \infty), \mathrm{y} \mapsto [1, +\infty) \}$
2. the analysis of **false** branch
   - we get:
      - $\{ \mathrm{x} \mapsto (- \infty, 7], \mathrm{y} \mapsto [0, +\infty) \}$
3. abstract join of these two abstract states
   - we get:
      - $\{ \mathrm{x} \mapsto \top, \mathrm{y} \mapsto [0, +\infty) \}$

### Analysis of Conditional Commands with a Relational Abstract Domain

We have to use different algorithm for the analysis of:
- for the analysis of condition tests
- for the computation of abstract join

Analysis of conditional test with a relational domain -> addition of several constraints to the abstract states

In general, it is more precise. Condition test that involve several variables are more precise. (more likely to be presented exactly)


## 3.3.3 Abstract Interpretation of Loops

Last one is loop.
As you know, we rely on the definition of concrete semantics.

Before we define the abstract semantics of loop, we will study its concrete semantics a bit more.

### Concrete Semantics of Loop


> $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \Big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \Big)$

We are defining the abstract semantics of loop by induction, we assume that we can compute the over-approximation of $\llbracket C \rrbracket_{\mathscr{P}}$.

Over-approximation of sequences of commands can be obtained by the over-approximation of each commands.

What we need to solve is to compute an over-approximation of the infinite union $\cup_{i \geq 0}F^{i}(M)$ with function $F^{\sharp}$ that over-approximate $F$, where $F = \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B$

#### Example 3.16 (Analysis of programs with loops)

We will use these programs as a example.

Figure 3.9(a)
```c
x := 0;
while (x >= 0) {
   x := x + 1;
}
```

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

### Sequences of Concrete and Abstract Iterates

We first consider a situation where a loop iterates at most $n$ times. ($n$ is a fixed integer value)

Then, the states they may generate at the loop head are:

- $M_n = \bigcup^{n}_{i=0} F^{i}(M)$
   - this set of states can be computed by induction over $n$.
      - HINT : $M_0 = M$, $M_{k+1} = M \cup F(M_k)$

Indeed, the sequences $(M_k)_{k \in \mathbb{N}}$ can be defined recursively as follows.
- $M_0 = M$
- $M_{k + 1} = M_k \cup F(M_k)$

It is very easy to compute an over-approximation of $M_n$ using the same techniques used in the previous section.

Assumption :
- $M^{\sharp}$ : an abstract element of the abstract domain
   - $M \subseteq \gamma( M^{\sharp} )$

We define the abstract iterates $( M_k^{\sharp} )_{k \in \mathbb{N}}$ as follows
- $M_0^{\sharp} = M^{\sharp}$
- $M_{k + 1}^{\sharp} = M_{k}^{\sharp} \cup^{\sharp} F^{\sharp}(M_{k}^{\sharp})$

Then we can prove by induction that
- for all integers $n$,
   - $M_n \subseteq \gamma(M_n^{\sharp})$

Proof of the statement above:
1. $n = 0$
   - It is obvious from assumption that $M_0 \subseteq \gamma( M_0^{\sharp} )$
1. $n = k$
   - we assume that $M_k \subseteq \gamma( M_k^{\sharp} )$
   - $M_{k+1}$
      - $= M_k \cup F(M_k)$
      - $\subseteq \gamma(M_k^{\sharp}) \cup F(\gamma(M_k^{\sharp}))$ ( $\because M_k \subseteq \gamma( M_k^{\sharp} )$)
      - $\subseteq \gamma(M_k^{\sharp}) \cup \gamma(F^{\sharp}(M_k^{\sharp}))$ ( $\because$ soundness of $F^{\sharp}$)
      - $\subseteq \gamma(M_k^{\sharp} \sqcup^{\sharp} F^{\sharp}(M_k^{\sharp}))$ ( $\because$ soundness of $\sqcup^{\sharp}$)
      - $= \gamma(M_{k + 1}^{\sharp}) $
   - $\therefore M_{k + 1} \subseteq \gamma(M_{k + 1}^{\sharp})$

### Example 3.17 (Abstract iterates)

Figure 3.9(a)
```c
x := 0;
while (x >= 0) {
   x := x + 1;
}
```

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

In the case of program (a):
- $M_0^{\sharp} = \{\mathrm{x} \mapsto [0, 0]\}$
- $M_1^{\sharp} = \{\mathrm{x} \mapsto [0, 1]\}$
- $M_2^{\sharp} = \{\mathrm{x} \mapsto [0, 2]\}$
- ...
- $M_n^{\sharp} = \{\mathrm{x} \mapsto [0, n]\}$
- ...

In the case of program (b):
- $M_0^{\sharp} = \{\mathrm{x} \mapsto [0, 0]\}$
- $M_1^{\sharp} = \{\mathrm{x} \mapsto [0, 1]\}$
- $M_2^{\sharp} = \{\mathrm{x} \mapsto [0, 2]\}$
- ...
- $M_49^{\sharp} = \{\mathrm{x} \mapsto [0, 49]\}$
- $M_50^{\sharp} = \{\mathrm{x} \mapsto [0, 50]\}$
- $M_50^{\sharp} = \{\mathrm{x} \mapsto [0, 50]\}$
- $M_50^{\sharp} = \{\mathrm{x} \mapsto [0, 50]\}$
- ...

### Convergence of Iterate

> $M_{k + 1}^{\sharp} = M_{k}^{\sharp} \cup^{\sharp} F^{\sharp}(M_{k}^{\sharp})$

We consider:
- the case of unbounded iteration
- the termination problem

Let us assume that the abstract iteration stabilize at some rank $n$, which means that $M_{n}^{\sharp} = M_{n + 1}^{\sharp}$.

Then,
- for all $k \geq n$
   - $M_k^{\sharp} = M_n^{\sharp}$
   - $M_k \subseteq \gamma( M_n^{\sharp} )$

Also,
- $M_{\mathrm{loop}} \subseteq \gamma(M_n^{\sharp})\quad$ where $\quad M_{\mathrm{loop}} = \bigcup_{i \geq 0} M_i$

Another interesting observation is that
- $M_{\mathrm{loop}} = \bigcup_{i \geq 0} F^i(M) = \bigcup_{i \geq 0}M_i \subseteq \gamma(M_n^{\sharp})$

Then, **If the sequences of abstract iterates converges**, then its final value over-approximate *all* the concrete behaviors.

#### Example 3.18 (Convergence of abstract iterates)

- In the case of program (a), the sequences of abstract iterates does not converge.
- In the case of program (b), the ranges of $\mathrm{x}$ stabilize but only after 51 iterations.

Neither of these are satisfactory.
- lack of termination
- hight number required to stabilize

Therefor, we have to formalize the condition the ensures that the sequences of abstract iterates converges.


### Convergence in Finite Height Lattices

Assumption:
- $\sqsubseteq$ is such that
   - $M_a^{\sharp} \sqsubseteq M_b^{\sharp}\quad$ if and only if $\quad\gamma(M_a^{\sharp}) \subseteq \gamma(=M_b^{\sharp})$ for all abstract states $M_a^{\sharp}, M_b^{\sharp}$

First case where convergence is ensured is when:

- $M_a^{\sharp} \sqsubset M_b^{\sharp}$

cannot hold infinitely many times.

This condition is realized when the abstract domain has *finite height*.
That is when the length of all the chains of the form
- $M_0^{\sharp} \sqsubset M_1^{\sharp} \sqsubset  \cdots \sqsubset M_k^{\sharp} $

is bounded by some fixed value $h$, *height of the abstract domain*.

For example, if the abstract domain has finite height $h$, the sequences
- $M_0^{\sharp} , M_1^{\sharp} ,  \cdots , M_h^{\sharp} , M_{h+1}^{\sharp}$
 
is increasing for $\sqsubseteq$, but cannot be strictly increasing.
So there exists a number lower than $h$ at which it becomes stable.
This number is bounded by the height of lattice.
 
We can now compute an abstract state $M_{\mathrm{ lim }}^{\sharp}$ that over-approximate $M_{\mathrm{loop}}$ by the algorithm in figure 3.10 (a).

- $\mathrm{abs\_iter}(F^{\sharp}, M^{\sharp})$
   - $R \longleftarrow M^{\sharp};$
   - $\mathrm{ repeat }$
      - $T \longleftarrow R;$
      - $R \longleftarrow R \cup^{\sharp}F^{\sharp}(R);$
   - $\mathrm{ until }$ $R = T$
   - $\mathrm{ return }$ $M^{\sharp}_{\mathrm{lim}} = T;$

This iteration technique is actually general and can be useful when analysing programs that contains iterative constructions.

#### Example 3.19 (Convergence of abstract iterates in the signs abstract domain)

The analysis using the signs abstract domain in the case of the two programs of example 3.16

- In the case of the program of figure 3.9(a), we obtain the following iteration sequence:
   - $M^{\sharp}_0 = \{\mathrm{x} \mapsto [= 0]\}$
   - $M^{\sharp}_1 = \{\mathrm{x} \mapsto [\geq 0]\}$
   - $M^{\sharp}_2 = \{\mathrm{x} \mapsto [\leq 0]\}$
      - this analysis terminates after only two iterations
- In the case of the program of figure 3.9(b), we obtain the same iteration sequence, same result.

### Widening Operators

- We will use *widening* technique for iterates to converge quickly
- Essentially, widening operator over-approximate concrete unions
   - and also enforces termination of all sequences of iteration

#### Definition 3.11 (Widening operator)

- widening operator : $\triangledown$ such that
   1. for all abstract elements $a_0, a_1$
      - $\gamma (a_0) \cup \gamma (a_1) \subseteq \gamma (a_0 \triangledown a_1)$
   1. for all sequences $(a_n)_{n \in \mathbb{N}}$ of abstract elements, the sequences of $(a_n \rq)_{n \in \mathbb{N}}$ defined below is ultimately stationary (= eventually converge).
      - $a_0\rq = a_0$
      - $a_{n + 1}\rq = a_n\rq \triangledown a_n$

Then we can turn the sequence of abstract iterates into a terminating sequence.

#### Theorem 3.5 (Abstract iterates with widening)

Assumption:
- $\triangledown$ : widening operator over non-relational abstract domain $\mathbb{A}$
- $F^{\sharp}$ : $\mathbb{A} \rightarrow \mathbb{A}$

Then, the algorithm shown below terminates and returns $M^{\sharp}_{\mathrm{lim}}$.

- $\mathrm{abs\_iter}(F^{\sharp}, M^{\sharp})$
   - $R \longleftarrow M^{\sharp};$
   - $\mathrm{ repeat }$
      - $T \longleftarrow R;$
      - $R \longleftarrow R \triangledown F^{\sharp}(R);$
   - $\mathrm{ until }$ $R = T$
   - $\mathrm{ return }$ $M^{\sharp}_{\mathrm{lim}} = T;$

Assumption:
- $F : \mathbb{M} \rightarrow \mathbb{M}$
   - continuous
   - $F \circ \gamma \subseteq \gamma \circ F^{\sharp}$ (in the sense of point-wise)

Then,
- $\bigcup_{i \geq 0 } F^{i}(\gamma(M^{\sharp})) \subseteq \gamma(M^{\sharp}_{\mathrm{lim}})$
   - $M^{\sharp}_{\mathrm{lim}}$ over-approximates the concrete semantics of the loop.

This theorem guarantees
- the termination of the loop analysis
- soundness

#### Example 3.20 (Widening operator for he abstract domain of intervals)

- $[n p] \triangledown_{\mathscr{V}} [n, q]$
   - $[n, p]$ if $p \geq q$
   - $[n, + \infty)$ if $p < q$

Then, we consider the program in the figure 3.9.

- In both case, we obtain the following iteration sequence:
   - $M_0^{\sharp} = \{ \mathrm{x} \mapsto [0, 0] \}$
   - $M_1^{\sharp} = \{ \mathrm{x} \mapsto [0, + \infty) \}$
   - $M_2^{\sharp} = \{ \mathrm{x} \mapsto [0, + \infty) \}$
- The convergence is now very fast, however
   - the result is coarse in the case of program (b),
      - this analysis doesn't converge.
- We will see some common techniques to obtain more precise result in section 5.2


### Analysis of Loops

- semantics of the analysis of loop
   - $\llbracket \mathtt{while} (B) \{ C \} \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}) = \mathscr{F}_{\neg B}^{\sharp} (\mathrm{abs\_iter} ( \llbracket C \rrbracket^{\sharp}_{\mathscr{P}} \circ \mathscr{F}_B^{\sharp}, M^{\sharp} ))$

### Analysis of Loops with a Relational Abstract Domain

- Almost same as with a non-relational domain
- Requires only an abstract join or widening operator specific to the abstraction being used

That is,
- In the case of linear equalities
   - widening is not necessary because its height of lattice is finite
- In the case of convex polyhedra and octagons
   - widening operator is required because its height of lattice is infinite.

### Another View on the Analysis of Loops

- concrete semantics of a loop statement
   - $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \Big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \Big)$
      - = $\mathscr{F}_{\neg B} (M_{\mathrm{loop}})$

Let us consider the following equation:
- $M_{\mathrm{loop}} = \cup_{i \geq 0}(\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^{i}(M)$
   - $= M \cup \Big(\bigcup_{i \gt 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^{i}(M)\Big)$
   - $= M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B \Big(\bigcup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^{i}(M)\Big)$
   - $= M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B (M_{\mathrm{loop}})$

Observation:
- $M_{\mathrm{loop}}$ is a *fixpoint* of a function $G : X \mapsto M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B (X)$
- $M_{\mathrm{loop}}$ is a smallest set of states. $M_{\mathrm{loop}}$ is a *least fixpoint* of $G$

We let $\mathbf{lfp} G$ denote the least fixpoint of $G$.

Then,
- concrete semantics of a loop can be expressed like this
   - $\llbracket \texttt{while} (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} (\mathbf{lfp}\medspace G) \enspace$ where $\enspace G : X \mapsto M \cup \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B (X)$
- abstract semantics of a loop relies on the over-approximation of a concrete least fixpoint.
   - we can use many techniques to compute over-approximation of least fixpoint such as:
      - abstract union
      - widening operator
   - we will see several improvements in section 5.2

## 3.3.4 Putting Everything Together

See figure 3.11.
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

We have defined several elements:
- $\phi_{\mathscr{V}}$ : abstraction of constants
- $f_{\odot}$ : mathematical function for abstraction of binary operator $\odot$, specific to the underlying value abstraction
- $\mathscr{F}_B^{\sharp}$ : specific to the underlying value abstraction
- $\sqcup^{\sharp}$ : abstract union operation, specific to the underlying value abstraction
- $\mathrm{abs\_iter}$ : abstract iteration function, relies on the widening operator $\triangledown$

We have defined these semantics so that each of these is sound.

### Theorem 3.6 (Soundness)
 
For all commands $C$ and all abstract states $M^{\sharp}$, the computation of $\gamma (\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}))$ terminates and:

- $\llbracket C \rrbracket_{\mathscr{P}} (\gamma (M^{\sharp})) \subseteq \gamma (\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}))$
   - The proof of this theorem proceeds by induction over the syntax of commands.
      - For each kind of commands, we ensured that the definition of its semantics would lead to sound result.


### Computing an Over-Approximation of All Reachable States

We can use the analysis of 3.11 to analyze a whole program.

### Using the Result of the Analysis to Prove the Properties of Interest

For instance,
- program : $C$
- initial state : $\gamma (M^{\sharp})$
- output state : $\gamma(\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp}))$
- property of interest : $M$

Then, we check whether $\gamma(\llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (M^{\sharp})) \subseteq M$ holds or not.
- If it holds,
   - $\llbracket C \rrbracket_{\mathscr{P}} \gamma(M^{\sharp}) \subseteq M$ is guaranteed because the analysis is sound.
- If it doesn't hold,
   - no information can be derived from the analysis.
      - the program may violate the property
      - the analysis may be imprecise

In general, if the inclusion does not hold, *alarms* will be called.
And users need to further inspect the result of the analysis to decide whether the alarms is true or false.

- alarms : says that the analysis tools failed to prove the property of interest
- triage : the process above

The analysis function $\llbracket C \rrbracket^{\sharp}_{\mathscr{P}}$ is not monotone.
Therefore, replacing pre-condition $M^{\sharp}$ with more precise one does not ensure that the result is more precise.

### Adapting the Analysis to Use a Different Abstraction

What if we want to use another abstraction.

The analysis of
- expression
- input

is essentially non-relational abstraction and it has to be modified.

However, in general, overall structure of the analysis doesn't need to be modified.


### Soundness theorem using best abstraction

We can also use best abstraction function $\alpha$ instead of $\gamma$.

- $\alpha ( \llbracket C \rrbracket_{\mathscr{P}} (M)) \sqsubseteq \llbracket C \rrbracket^{\sharp}_{\mathscr{P}} (\alpha (M))$

## 3.4 The Design of an Abstract Interpreter

We will summarize the process we have followed in this chapter.

General three steps to construct a static analysis:
- fix the reference concrete semantics
- select the abstraction
- derive analysis algorithm

This division of the analysis design into independent steps is important
- for the construction of a static analysis
- when a static analysis needs to be improved ( a static analysis is imprecise )

Common case a static analysis is imprecise:
- abstraction is coarse
- algorithm return overly approximated result for the sake of cost
- concrete semantics is too coarse to express the properties of interest

