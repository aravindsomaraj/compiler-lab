/***
 * Enums for Storing Datatypes, Nodetypes, Statements Datatype and Condition Types
*/ 

enum exprType {LEAF, InternalNODE, FuncCall};
enum Datatype {INT, BOOL, none, Var};
enum stmtTypes {Write,Read,assign,COND,FUNC_CALL};
enum condStmtType {If, IF_ELSE, While, For};

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

struct exprList {
    expr * exp;
    exprList * next;
};

struct varExpr  {

    char * varName;
    bool isArray ;
    exprList * index;
    exprList * endIndex;
};

/*****************************************/

struct FunctionCall {
    char * funName;
    exprList * exprlist;
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

struct evaluation_list{
    evaluation * res;
    evaluation_list * next;
};

struct value{
    char * name;
    void * val;         
    Datatype type;
    bool isArray;
};

struct indexList {
    int i;
    indexList * next;
};

/*****************************************/
/************
 * 
 *  Structure for array, variable Declare and Declare block
*/


struct VariableDeclaration{
    char * name;
    indexList * index;
    indexList * endIndex;
    bool isArray;
};

struct var_list{
    char * name;
    var_list * next;
};

struct arg
{
    Datatype type;
    var_list * varList;
};

struct arg_list{
    arg * Arg;
    arg_list * next;
};

struct func{
    char * name;
    arg_list * argList ;
};

struct Array {
    void * arr;            
    indexList * indexLst;
    int dimensions;
};

struct Glist {
    void * node; 
    bool isGid;
    Glist * next;
};

struct Declare {
    Datatype type;
    Glist * list;
    Declare * next;
};

struct statement{
    void * stmt;
    stmtTypes stmtType;
    statement * next;
};

struct FunctionDef {
    Datatype retType;
    char * funcName;
    arg_list * FargList;
    Declare * LdeclSec;
    statement * stmtList;
    expr * retStmt;
    FunctionDef * next;
};

struct aboutFunc {
    func * Func;
    FunctionDef * fdef;
    bool isDefSet;
    Datatype declType;
};

struct Main{
    Datatype return_type;
    Declare * local_declaration;
    statement * stmt_list;
    expr * return_stmt;
};

struct Program {
    Declare * global_declarations;
    FunctionDef * fdef_sec;
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

struct read_stmt {
    varExpr * varexp;
};

struct conditional_stmt{
    condStmtType type ;
    void * cond_stmt_attrs; 
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

struct for_loop{
    assignment_stmt * startAssign;
    expr * COND;
    assignment_stmt * updateAssign;
    statement * stmtList;
};

/**
 * List of functions used to create syntax tree nodes or expressions
*/

expr * TerminalExp (int val);
expr * TerminalExp (bool val);
expr * TerminalExp (varExpr * var);
expr * TerminalExp (FunctionCall * _func_call);
expr * addNonTerminalExpr (expr * left, char OperatorSym ,expr * right);
varExpr * createVal (char * varName);
varExpr * createVal (varExpr * varExp, expr * index);
strExp * StringExp (char * name);
strExp * StringExp (char * name, strExp * exp);

/**
 * Declaration Section
*/
value * DeclareNewVal (char * varName, Datatype type);
value * DeclareNewVal (char * varName,Datatype type, indexList * index);
VariableDeclaration * DeclareNewNode (char * var );
VariableDeclaration * DeclareNewNode (VariableDeclaration * node, int size ) ;

Declare * create_decl (Datatype type, Glist * top);
Declare * create_decl_list (Declare * node, Declare * list);

var_list * create_variable_list (char * name);
var_list * create_variable_list (char * name,var_list * varList);

arg * parameters (Datatype type, var_list * varList);

arg_list * create_func_parameters (arg * Arg);
arg_list * create_func_parameters (arg * Arg, arg_list * argList);

exprList * makeExprList (expr * exp);
exprList * makeExprList (expr * exp,exprList * exprlist);

func * createFunction (char * name, arg_list * argList);

void Declare_block (Declare * root);
void printGlobalDecl (Declare * root);

Glist * create_global_decl_list (VariableDeclaration * node);
Glist * create_global_decl_list (VariableDeclaration * node, Glist * glist);

Glist * create_global_decl_list (func * node);
Glist * create_global_decl_list (func * node, Glist * glist);

FunctionDef * FunctionDefinition (Datatype ret_type,char * name, arg_list * argList , Declare * local_decl, statement * stmt_list, expr * ret_stmt );
FunctionDef * createFdefSection (FunctionDef * fdef, FunctionDef * fdefSec);
void addFdef_sec(FunctionDef * fdefSec);

/**
 * Assignment section
*/


statement * createAssignment (varExpr * var, expr * exp);
void printAssignStmt (assignment_stmt * stmt);

/**
 * Write statement
 */

statement * createWrite (expr * expr);
statement * createWrite (strExp * strExpr);
void printWriteStmt (write_stmt * stmt);

/**
 * Read statement
 */

statement * createRead(varExpr * varexp);

/**
 * CONDITIONAL statement functions 
 */

statement * create_IF (expr * cond_expr, statement * stmt_list);
statement * create_IFELSE (expr * cond_expr, statement * if_stmt_list, statement * else_stmt_list);
statement * create_WHILE (expr * cond_expr, statement * stmt_list);
statement * create_FOR (statement * startAssign, expr * condition, statement * updateAssign, statement * stmtList);
void printCOND (conditional_stmt * stmt);

/**
 * Function Statement functions
 */
FunctionCall * makefunc_call(char * name, exprList * exprlist);
statement * makeFuncStmt (FunctionCall * funcCall);

/**
 * General statements function
 */
statement * createStatementList (statement * stmt, statement * stmtList);
void printStmtList (statement * stmt);
void printStmt (statement * stmt);

/**
 * Main block functions
 */
Main * makeMainBlock (Datatype ret_type, Declare * local_decl, statement * stmt_list, expr * ret_stmt );
void addMainBlock (Main * mainBlock);

/**
 * Program execution start functions
 */
void start ();
void execGlobalDeclaration ();
evaluation * main_block_exec();

void print_program();
void printGlobalSymbolTable ();


void printResult (evaluation * val);

/**
 * Function to print the input source code
 */
void print_new_code_code();

void PrgBeginMessage();
void PrgEndMessage();