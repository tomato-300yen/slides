# git-internals

## 10.1

- Content-addressable filesystem layer is cool
- Two types of commands
  - Plumbing commands
    - do low-level work
  - Porcelain commands
    - user-friendly

In the `.git` directory:
- `config`: project-specific configurations
- `info`: global exclude file for ignored patterns that you don't want to track in a `.gitignore` file.
- `hooks`: contains hook scripts.

Directories below are important.
- `objects`: stores all the content for your database.
- `refs`: stores pointers into commit objects in that data. (branches, tags, remotes, and more)
- `HEAD`: points to branch you currently have checked out.
- `index`: file is where Git stores staging area information.

## 10.2

- Git: content-addressable filesystem
  - key-value store
- `git hash-objects`
   1. takes some data
   1. stores in your `.git/objects`
   1. gives you back unique key that refers to that data objects
  - `-w`: write to `.git/objects`
  - '--stdin': takes data from standard input
- `git cat-file`
  - `-p`: pretty print the content
  - `-t`: show the objects type

### tree objects

All the contents is stored as `tree` and `blob` objects.
- `tree`: directory
- `blob`: file or content
