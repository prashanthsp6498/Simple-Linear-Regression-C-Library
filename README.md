# Simple Linear Regression Library

This project is a lightweight C library designed for simple linear regression. It provides an efficient and flexible way to perform data analysis, enabling users to model relationships between two variables with ease. The library includes functions for data normalization, dataset splitting, and model training, ensuring optimal performance even on systems with limited resources.
Key Features:

    Efficiency: The library is optimized for high performance, delivering accurate results with minimal computational overhead.
    Flexibility: Easily integrates into larger C projects, supporting various dataset formats and workflows.

Perfect for developers seeking a foundational yet robust tool for regression tasks in C.

## Project Structre

## Simple_Linear_Regression_Library

.
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

## How to compile ?

Here, I've used bear for compilation.

```bash
sudo apt install bear
```

Next, Make a Directory name called 'build'. Then use bear, it will generates compile_commands.json file.

```bash
bear -- make
```

Then run the binary file.

```bash
./build/test
```
