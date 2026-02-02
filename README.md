# HSE AMI SD 2026 project

[![CI](https://github.com/SPbZOVal/better-than-fluffy-tribble/actions/workflows/build.yml/badge.svg?branch=release)](https://github.com/SPbZOVal/better-than-fluffy-tribble/actions/workflows/build.yml)

Minimal command-line interpreter (mini shell) written in C++.

## Examples

```sh
echo "Hello, world!"
>>>Hello, world!
```

```sh
FILE=example.txt
cat $FILE
>>>Some example text
```

```sh
cat example.txt | wc
>>>1 3 18
```

```sh
echo 123 | wc
>>>1 1 3
```

```sh
x=ex y=it
$x$y
>>>exit
```

## Expected behavior

### Built-in commands

- `cat <file>`
  Prints the file contents to stdout.

- `echo <args...>`
  Prints all arguments to stdout (separated by spaces).

- `wc <file>`
  Prints three numbers: lines, words, bytes.

- `pwd`
  Prints the current working directory.

- `exit`
  Terminates the interpreter.

### Quoting rules

- Single quotes '...' (full quoting):
  - Everything inside is treated literally.
  - Variable expansion ($NAME) must not be performed.
  - Spaces inside stay part of the same argument.

- Double quotes "..." (weak quoting):
  - Spaces inside stay part of the same argument.
  - Variable expansion ($NAME) must be performed.

### Environment and variable expansion

- Assignments of the form NAME=value set/update variables in the interpreter environment.
- $NAME expands to the current value of NAME.
- Concatenated expansions must work, e.g.:
  x=ex y=it
  $x$y
  produces:
  exit

### External commands

If the command name is not one of the built-ins, the interpreter should attempt to execute it as an external program.

### Pipelines

A pipeline cmd1 | cmd2 | ... | cmdN must:
- connect stdout of each command to stdin of the next command
- work for both built-ins and external commands
- preserve the usual left-to-right data flow semantics

### Documentation

Documentation in russian language can be found in [documentation directory](https://github.com/SPbZOVal/better-than-fluffy-tribble/tree/main/docs).

### Build

After clone in project directory:
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Launch:
```sh
./btft
```

### Team

- Andrey Gladkikh
- Ilya Vinogradov
- Pavel Kort
