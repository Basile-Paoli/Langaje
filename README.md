# Golmon

Golmon is an interpreted language written in C.

It allows you to modify the syntax of the language in order to create the most cursed language you can think of.

## How to run

- Ensure you have [CMake](https://cmake.org) installed
- Run CMake to generate build files `cmake .`
- Compile with the generated makefile `make`
- Profit `./golmon main.gol`

## Usage

You can run the interpreter with a file as an argument. The file should contain the code you want to run.

```sh
./golmon main.gol
```

You can also run the interpreter without a file. This will let you execute code directly in the interpreter.

```sh
./golmon
```

If you want to exit the interpreter, you can use the `@exit` command.

## Features

You can find the syntax and features of the language in
the [documentation](https://www.notion.so/Documentation-LANGAGE-13dfd7dfb4fe80d59eded7137ddf4451)

## Custom syntax

You can create your own syntax by writing a file in the `lang` directory. The file should contain definitions for all
the tokens you want to use. Take a look at lang/classic.lang for an example.

Once you have created your file, you can add the instruction `#lang "your_file_without_extension"` at the beginning of
your source code.