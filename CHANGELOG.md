# **CHANGE LOG**

## 8 May
<code>Final Project</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>
- Added remaining print functions to complete the cpp file. 
    ```
    void print_new_code_code();   // Calls all required functions to print generated C code
    void print_program();         // Prints the corresponding AST for the source code
    ```
- Fixed minor bug issues in the yacc and header files.
- Tested and executed with sample input cases. &#10004;
- Project finally completed as required.

## 7 May
<code>Final Project</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>
- Created functions in the *Function Call helper section* to implement functionality of function calling.
    ```
    FunctionCall* makefunc_call();
    statement* makeFuncStmt();
    ```
- In the *Execution Helper functions* section, a lot of new functions have to be added to implement the working of function calling, declaration, and execution. This includes insertion of function declaration into SymTable, as well as creating the function body.
- In the type of statements to be executed, *READ* statements are now allowed.
    ```
    void executeREAD();
    ```
- Main block start function execution implemented, down to the semantics of the function definition and running.
- A section *Code correctness checking* added with function to ensure semantic and syntactic correctness of the code that is to be executed.
    ```
    void full_program_check();      // accompanying other parts of semantic check are called within this function execution
    ```
    
## 6 May
<code>Final Project</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>
- Made a lot of changes in the *Declaration Helper Functions* section.
- In some of the functions, updates were made to allow for array functionalities with boolean datatype in addition.
- Implementation of variable declaration improved to allow for local & global declarations.
- Creation of arguments for function prototype implemented, as well as the creation of the function following the function definition in this section.
    ```
    Glist* create_global_decl_list();   // list for Global declarations of variables
    var_list* create_variable_list();   // creation of variable list
    arg* parameters();                  // parameter definition for function prototype
    FunctionDef* FunctionDefinition();  // Creation of the function, and function definition
    ```
- Listing of all global and local declaration entities implemented.

## 5 May
<code>Final Project</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>
- Made a lot of changes in the *Syntax Tree Expression Functions* section.
- Added a function: `struct exprList* makeExprList()` to accomodate the expression node as a list for further calculation, including a function to calculate the size of the list.
- Because function calls are to be implemented in this project, definition for `Calculate()` function has been redefined to include the symbol table of the function that has called it, so variables can be accessed from the scope it is defined and called in.
- Added a case in the switch statement in `Calculate()` to implement functionality for when the node in the expression list of the syntax tree is of ***Function Call*** type.
- Creation of syntax nodes, variable nodes, and calculate function are similar as in the *Test2*, with minor changes in the function calls in the code.

## 30 April
<code>Final Project</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>
- Updated code definition to accomodate function definition and function calling.
- In the symbol table functions section, added a few function definition to implement the aforementioned.
    ```
    int Changeto1Dimension();   // to express array in its equivalent 1d form
    int dimensions();           // to obtain dimensions
    int size_of_array();        // to return effective size of array
    bool range_check();         // check viability of range of indices
    ```
- Made updates in *lookup, insertion, update* functions of the symbol table to accomodate the functionality of the **boolean** data type.

## 29 April
<code>Final Project</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>
- Added proper directory for final assignment submission with all the last features.
- Source code and all include files follow from the Test2 submission as final assignment is built upon Test2.

## 28 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>
- Made final modifications to how the program displays the outputs, making it more readable.
- Added indicators for start and end of programs.
- Improved indendations for certain parts of the output.

## 20 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>
- Solved a few bugs with structs and memory allocation for certain pointers.
- Organized code to make it more readable.

## 16 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>
- Solved quite a few bugs.
- Switched header file type from **.h to .hpp to accomodate the requirement for overloading** of certain functions.
- Added a few functions in the *print* section of the code to output the **Abstract Syntax Tree (AST)**, calculations in the required (and readable format), and to display symbol tables for all declarations utilised in the program.
    ```
    void printMainBlock();      // prints all main block content (formatted)
    void printGlobalSymbolTable(); // displays required symbol tables utilised
    void printOverallPrg();    // Calls print functions of global scope and main scope
    ```

## 12 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>   
- **Created exec statements** for the whole of the compiler code.
- Modified all the print functions to be accomodated in a single block.
- Added execution functions for all **statements, global code and start code**.

## 10 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>
- Created Functions for Assignment and Write statements
    ```
    statement* createAssignment(varExp* var, expr* exp);
    void print_assignments(assign_stmt* stmt);
    statement* createWrite();
    ```
- Created a few functions to handle conditional statements {*if,if else,while*}
    ```
    statement* create_IF();
    statement* create_IFELSE();
    statement* create_WHILE();
    ```
    
## 8 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>
- Completed all the required Symbol Tables functions, including **symbol insertion, lookup, and updating of values** in the symbol table.
    ```
    void SymTableInsertion (char* name, Datatype type, map<string, struct value*>& symtable);
    void SymTableInsertion (char* name,Datatype type, int a, map<string, struct value*>& symtable);

    evaluation* SymLookUp (char* name, map<string, struct value*>& symtable);
    evaluation* SymLookUp (char* name, int index, map<string, struct value*>& symtable);

    void updateVar (char* name, int value, map<string, struct value*>& symtable);
    void updateVar (char* name, int index , int value, map<string, struct value*>& symtable);
    ```
- Made good progress in the list of the required Declaration functions - namely, declaration of variables, nodes and creation of declaration list.
    ```
    value* DeclareNewVal();
    VariableDeclaration* DeclareNewNode();
    VariableDeclaration* Declaraction_List();
    declaration* makeDeclaration();
    void Declare_block(declaration* root);
    ```
- Removed some syntax errors with yacc file, header file and some of the functions associated with the symbol table in .cpp file.

## 6 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>   
- Modified **Calculate()** function as a series of switch statements to handle relational expression evaluation.
- Created a function to obtain enum of Datatype using switch on the **operator** used.
- Created function to create internals of syntax tree

## 4 April
<code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>
- Made Symbol table as a STL map function mapping **symbol name** to **key pointer** in the table.
- Added separate functions for creation of syntax tree nodes for boolean and integer values.
- Created functions of creating **variable expressions**, and further creation of **tree nodes** with those variable expressions.

## 31 March
- ### 12:40 PM
    <code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>

    - Updated lex file to return more tokens as required in the test cases.
    - Created function in C file to handle declaration of one-dimensional arrays
        ```
        struct varExp* makeVar(char * varName);
        ```
- ### 12:10 PM
    <code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>  <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark> 

    - Made modifications in the grammar in yacc file to implement global declaration and 1D array declaration.
    - Created function and structure in header and code files to accomodate one-dimensional arrays.  
  
- ### 11:25 AM
    <code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>

    - Created required src and header files with Lab3 as basis.
    - Modified the grammar in the yacc file as required.

- ### 10:40 AM
    <code>Test2</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>

    - Created directory as required for Test2.
    - Created empty Makefile for later use.  
    
## 30 March
<code>Lab3</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>   
- Fixed bug issues with **newline** and **whitespaces** in the lex file.
- Updated and improved the grammar in the yacc file.
- Created function in ExpressionTree.c to handle *READ* statements:
    ```
    Instruct *returnRead(char *name); 
    ```

## 24 March
<code>Lab3</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> 
- Created functions in ExpressionTree.c to add expression node for **terminal booleans** and also **logical operations**.
    ```
    Exp AddLogicalOp(Exp *left, LogicalOps lop, Exp *right);
    Exp *TerminalBool(int bool);
    ```
- Updated compiler.y to as per the above change (function calls).
- Created function calls for assignment statement in compiler.y as well as for adding the assignment statement into list of instructions.

## 15 March
<code>Lab3</code>&ensp;&#8594;&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> 
- Worked on the Lex and Yacc files to implement the language with more functionality.
- Created more functions to handle more operators and for use of compilation on a wider basis.
## 10 March
<mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>  
- Fixed bug issues in Makefile for /compiler.
- <font style="color:#ff2b3d;">Deleted</font> redundant directories.
## 3 March

<code>Lab2</code>&ensp;[&#8594;](https://github.com/unnikrishnan-c/CS-3140-Compiler-Design-Laboratory-Jan-2023/blob/main/README.md#cs-3140-compiler-design-laboratory-jan-2023)&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark> <mark style="background-color: grey; color: white">&ensp;**UPDATED**&ensp;</mark>  
- Updated C program to extend the language to support **global variable declaration** with variables of type "integer", **assignment statements**, and **function calls** (specifically <font style="color:#e8ed7e;">PRINT</font> ).
- Reorganized Compiler Design repository as required. <a title="New structure of repository" target="_blank" href="https://lms.iitpkd.ac.in/pluginfile.php/19764/mod_resource/content/1/Assignment%20Instructions.pdf#page=5">&#8599;</a>
- Created Makefiles for both /compiler and /calculator directories.

Assignment 2 completed. <a title="Assignment 2" target="_blank" href="https://gitlab.com/112001007/112001007-cs3140/-/tree/assignment-2">&#8599;</a>


## 9 February  
<code>Lab1</code>&ensp;<span title="Assignment 1 Question">[&#8594;](https://github.com/unnikrishnan-c/CS-3140-Compiler-Design-Laboratory-Jan-2023/blob/main/README.md#cs-3140-compiler-design-laboratory-jan-2023)</span>&ensp;<mark style="background-color: green; color: white">&ensp;**ADDED**&ensp;</mark>  
- Created a C program and and a lex file to implement a **syntax tree** for calculator and **evaluation** of the Syntax Tree.  

Assignment 1 completed. <span title="Assignment 1 Solved">[&#8599;](https://gitlab.com/112001007/112001007-cs3140/-/tree/dbfc698ef5c8acf9550d4bf36f95ca7cafea07a3/lab2)</span>

