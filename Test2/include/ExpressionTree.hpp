#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
/***
 * Enums for Storing Datatypes, Nodetypes, Statements Datatype and Condition Types
*/ 

enum exprType {LEAF, InternalNODE};
enum Datatype {INT, BOOL, none, Var};
enum stmtTypes {Write,assign,COND};
enum condStmtType {If, IF_ELSE, While};

/**********************************/

/**
 * structures for storing expression Datatype
*/
struct expr {
    expr * left;
    expr * right;
    
    void * attribute;
    exprType type;
    Datatype valueType;
};

struct intExp {
    int val;
    Datatype type;
};

struct boolExpr {
    bool val;
    Datatype type;
};

struct varExpr  {
    char * varName;
    bool isArray ;
    expr * index;
    Datatype type;
};


/*****************************************/


struct OperatorSym {
    char op;
    Datatype type;
};

struct evaluation{
    Datatype type;
    void * value;      
};

struct value{
    char * name;
    void * val;         
    Datatype type;
    bool isArray;
};

/*****************************************/

/************
 * 
 *  Structure for array, variable Declare and Declare block
*/
struct VariableDeclaration{
    value * var;
    VariableDeclaration * next;
};

struct Array {
    void * arr;
    int size;
};

struct Declare {
    Datatype type;
    VariableDeclaration * list;
    Declare * next;
};

/*****************************************/


/**
 * Structures for storing statement Datatype, block statement and program IC
*/
struct statement{
    void * stmt;
    stmtTypes stmtType;
    statement * next;
};

struct Main{
    Datatype return_type;
    Declare * local_declaration;
    statement * stmt_list;
    expr * return_stmt;
};

struct Program {
    Declare * global_declarations;
    Main * mainBlock;
};

struct strExp {
    char * var;
    strExp * next;
};

/*****************************************/


/**
 * Structure for storing data for different Datatype of statements
*/
struct assignment_stmt {
    varExpr * var;
    expr * exp;
};

struct write_stmt {
    bool isExpr;
    void * val;  
};

struct conditional_stmt{
    condStmtType type ;
    void * attributes; 
};

struct if_stmt {
    expr * COND;
    statement * stmtList; 
};

struct ifelse_stmt{
    expr * COND;
    statement * if_stmtList;
    statement * else_stmtList;
};

struct while_stmt{
    expr * COND;
    statement * stmtList;   
};


/**
 * List of functions used to create syntax tree nodes or expressions
*/
expr * TerminalExp (int val);
expr * TerminalExp (bool val);
expr * TerminalExp (varExpr * var);
expr * addNonTerminalExpr (expr * left, char OperatorSym ,expr * right);
evaluation * Calculate (expr * root);
varExpr * createVal (char * name);
varExpr * createVal (char * name, expr * index);
strExp * StringExp (char * name);
strExp * StringExp (char * name, strExp * exp);


/**
 * Declaration Section
*/
value * DeclareNewVal (char * varName);
value * DeclareNewVal (char * varName, int maxSize);
VariableDeclaration * DeclareNewNode (char * var ) ;
VariableDeclaration * DeclareNewNode (char * var, int size ) ;

VariableDeclaration * makeDeclList (VariableDeclaration * node, VariableDeclaration * lst ) ;

Declare * makeDeclaration (Datatype type,VariableDeclaration * top);

Declare * makeDeclarationList (Declare * node, Declare * list);

void Declare_block (Declare * root);
void printGlobalDecl (Declare * root);

/**
 * Assignment section
*/

statement * createAssignment (varExpr * var, expr * exp);
void print_assignments (assignment_stmt * stmt);

/**
 * Write statement
 */

statement * createWrite (expr * expr);
statement * createWrite (strExp * strExpr);
void print_write (write_stmt * stmt);

/**
 * CONDITIONAL statement functions 
 */

statement * create_IF (expr * cond_expr, statement * stmt_list);
statement * create_IFELSE (expr * cond_expr, statement * if_stmt_list, statement * else_stmt_list);
statement * create_WHILE (expr * cond_expr, statement * stmt_list);
void printCOND (conditional_stmt * stmt);

/**
 * General statements function
 */
statement * createStatementList (statement * stmt, statement * stmtList);
void printStmtList (statement * stmt);
void printStmt (statement * stmt);

//main block helper functions
Main * makeMainBlock (Datatype ret_type, Declare * local_decl, statement * stmt_list, expr * ret_stmt );
void addMainBlock (Main * mainBlock);

//function for execution of code
void start ();
void start_global_exec ();
void start_main_exec();

void printOverallPrg();
void printGlobalSymbolTable();
void PrgBeginMessage();
void PrgEndMessage();
// Some printing and testing functions
void printResult (evaluation * val);