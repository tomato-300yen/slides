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

