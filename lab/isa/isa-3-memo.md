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

### Memory States (Control States can be ignored)

In general, __program state__ should capture the configuration of the computer.
Therefore it should include a __memory state__ and __control state__.

- memory state : contents of the memory
- control state : a value of "program counter" (or next command to be executed)

In this chapter, we define an input-output semantics, so control states can be safely ignored.
Because input(output) state is fully determined by the memory state, a state is defined as a memory state.

In out language, memory state $$M$$ is defined by:
$$
\mathbb{M} = \mathbb{X} \longrightarrow \mathbb{V}
$$

If we map `x` to 2 and `y` to 7:
$$
\{ ¥mathrm{x} ¥mapsto 2, ¥mathrm{y} ¥mapsto 7\}
$$

> ここで言うmemory stateとは変数と値のマッピングのこと

### Semantics of Scalar Expressions

- Scalar Expressions --(evaluation)--> Scalar Value

e.g.:

- `n` : constant
- `x` : variable

> `n` ----> `n`
> `x` ----> value of `x` in the memory state

See figure 3.2

$$
\llbracket E \rrbracket : \mathbb{M} \longrightarrow \mathbb{V} \\
\\
\llbracket n \rrbracket (m) = n \\
\llbracket \mathrm{ x } \rrbracket (m) = m(\mathrm{ x }) \\
\llbracket E_0 \odot E_1 \rrbracket (m) = f_{\odot}(\llbracket E_0 \rrbracket (m), \llbracket E_1 \rrbracket (m)) \\
$$

### Semantics of Scalar Expressions

See figure 3.3

$$
\llbracket B \rrbracket = \mathbb{M} \longrightarrow \mathbb{B} \\
\llbracket \mathrm{x} < n \rrbracket = f_{<}(m(\mathrm{x}), n)
$$

### Semantics of Commands

- $$\llbracket C \rrbracket_{\mathscr{P}}$$ : semantics of a command $$C$$
   - a set of input states to a set of output states( which is observed __after__ the command )
      - non-terminating executions are not observed
- $$\wp(\mathbb{ M })$$ : power set of memory states
   - $$\mathbb{M}$$ : an element of $$\wp(\mathbb{ M })$$.

Semantics of commands is:

- $$\llbracket \texttt{slip} \rrbracket_{\mathscr{P}}(M) = M$$
   - identity function
- $$\llbracket C_0 ; C_1 \rrbracket_{\mathscr{P}}(M) = \llbracket C_1 \rrbracket_{\mathscr{P}}(\llbracket C_0 \rrbracket_{\mathscr{P}}(M))$$
   - composition of the semantics of each commands
- $$\llbracket \mathrm{x} \colonequals E \rrbracket_{\mathscr{P}}(M) = \{m[\mathrm{x} \mapsto \llbracket E \rrbracket(m)]\}$$
   - the evaluation of assignment updates the value of $$\mathrm{x}$$ in the memory states with the result of the evaluation of $$E$$.
- $$\llbracket \texttt{input} (\mathrm{x}) \rrbracket_{\mathscr{P}}(M) = \{ m[ \mathrm{x} \mapsto n ] | m \in M, n \in \mathbb{V} \}$$
   - replace the value of $$x$$ with any possible scalar value.

Quite easy, isn't it ?
The semantics below is a little bit complicated.

First, we need to define a filtering function $$\mathscr{F}_B$$.
This function filter out memory states.

Def:
$$
\mathscr{F}_{B}(M) = \{m \in M | \llbracket B\rrbracket(m) = \mathbf{true}\}
$$

Intuitive explanation : this function filter out memory states $$m$$ in which $$B$$ doesn't hold or can't be defined.

- $$\llbracket \texttt{if} (B) \{C_0\} \texttt{else} \{C_1\} \rrbracket_{\mathscr{P}}(M) = \llbracket C_0 \rrbracket_{\mathscr{P}}(\mathscr{F}_{B}(M)) \cup \llbracket C_1 \rrbracket_{\mathscr{P}}(\mathscr{F}_{\neg B}(M))$$

- $$\llbracket \texttt(while) (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \big)$$
   - complicated...

Let $$M_i$$ be a state produced by program execution that repeated the loop $$i$$ times.

$$M_i$$ is defined as follows:

$$
M_i = \mathscr{F}_{\neg B} \big( ( \llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B )^i (M) \big)
$$

Intuitive explanation : $$B$$ evaluates to __true__ $$i$$ times and to __false__ for the last(i+1-th test).

- $$\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B$$ : filter memory states, then execute the command.

The set of output states would be $$M_0 \cup M_1 \cup M_2 \dots$$, that is :

$$
\cup_{i \geq 0} M_i = \cup_{i \geq 0} \mathscr{F}_{\neg B} \big( (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^i (M) \big)
$$

\mathscr{F}_B commutes with the union, this,

$$
\cup_{i \geq 0} M_i = \mathscr{F}_{\neg B} \Big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B)^i (M) \Big)
$$

Therefore,

- $$\llbracket \texttt(while) (B) \{ C \} \rrbracket_{\mathscr{P}}(M) = \mathscr{F}_{\neg B} \big( \cup_{i \geq 0} (\llbracket C \rrbracket_{\mathscr{P}} \circ \mathscr{F}_B) ^i (M) \big)$$
