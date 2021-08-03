<script type="text/javascript" async src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.7/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
 MathJax.Hub.Config({
 tex2jax: {
 inlineMath: [['$', '$'] ],
 displayMath: [ ['$$','$$'], ["\\[","\\]"] ]
 }
 });
</script>

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
```math
\mathbb{M} = \mathbb{X} \longrightarrow \mathbb{V}
```

If we map `x` to 2 and `y` to 7:
```math
\{ ¥mathrm{x} ¥mapsto 2, ¥mathrm{y} ¥mapsto 7\}
```

### Semantics of Scalar Expressions

- Scalar Expressions --(evaluation)--> Scalar Value

e.g.:

- `n` : constant
- `x` : variable

> `n` ----> `n`
> `x` ----> value of `x` in the memory state

See figure 3.2
