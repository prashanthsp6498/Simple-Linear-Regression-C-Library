# Simple Linear Regression Library

This project is a lightweight C library designed for simple linear regression. It provides an efficient and flexible way to perform data analysis, enabling users to model relationships between two variables with ease. The library includes functions for data normalization, dataset splitting, and model training, ensuring optimal performance even on systems with limited resources.
Key Features:

    Efficiency: The library is optimized for high performance, delivering accurate results with minimal computational overhead.
    Flexibility: Easily integrates into larger C projects, supporting various dataset formats and workflows.

Perfect for developers seeking a foundational yet robust tool for regression tasks in C.

## Project Structre

## Simple_Linear_Regression_Library

```bash
Project Root Directory
|-- build
| `-- test
|-- EDA
|   |-- DataAnalysis.c
|   |-- DataAnalysis.h
|-- Regression
| |-- Linear.c
| |-- Linear.h
|-- Test
| |-- test.c
|-- compile_commands.json
|-- License
|-- makefile
|-- README.md
|-- winequality.names
|-- winequality-red.csv
`-- winequality-white.csv
```

## How to compile ?

Here, I've used bear for compilation.

Step 1: If bear is not Installed, Install it or you can jump to Step 2 if its Installed,

```bash
sudo apt install bear
```

Step 2: Use Bear to Generate .json file.

```bash
bear -- make
```

Step 3: use "make" command to compile .

```bash
make
```

Step 4: Run the generated Binary file.

```bash
./build/test
```

## How to Debug or trace Memory allocation Error ?

Use GDB for Debuging and Valgrind for Memory issues.

```bash
gdb ./build/test
```

Refer official documentation of GDB and Valgrind for more info.

```bash
valgrind --leak-check=full --track-origins=yes -s ./build/test
```
