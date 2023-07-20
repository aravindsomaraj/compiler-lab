# CS3140 - Compiler Design Laboratory

- **Course Instructor: Unnikrishnan C** 

## Introduction

Welcome to the [Compiler Design Laboratory Course](https://lms.iitpkd.ac.in/course/view.php?id=269)! In this course, a compiler is built from scratch. There's different stages of the compilation process, including lexical analysis, syntax analysis, creation of Abstract Syntax Tree, code generation, and optimization which are all utilized in this course.

## Instructions
- To clone the repository to your local machine, open terminal in the desired directory and run the following:  
    ```
    git clone https://gitlab.com/112001007/112001007-cs3140.git
    ```
- Go the repo, and there's 3 directories present:
    - *calculator* is a simple compiler that evaluates arithmetic operations and prints the output. To do this compiler, run the following:
        ```
        cd calculator
        make run
        ```
    - *compiler* is a fully designed compiler that's capable of compiling and executing more elaborate code. Run the following to see:
        ```
        cd compiler
        make run
        ```
    - *Test2* is the directory containing the code for Lab Test 2 of CS3140. It supports local and global declarations, array declarations and evaluation of integer and boolean along with assign and write capabilities (for strings as well).
    To run the compiler:
        ```
        cd Test2
        make
        bin/compiler < [Your test input file here]
    
    - *compiler_final* is the directory that contains the code for the final project of CS3140. It supports dimensional arrays, function definition and calling, along with read capabilities, and finally the generated of the corresponding C code analogous to the source code.
    To run:
        ```
        cd compiler_final
        make
        bin/compiler < [Your .sil test input file here]
