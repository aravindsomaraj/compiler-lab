typedef enum stmtType
{
    READ_STMT,
    DECLARATION,
    ASSIGNMENT,
    FUNC
} stmtType;
typedef enum Datatype
{
    INT = 0,
    BOOL = 1
} Datatype;
typedef enum functions
{
    PRINT = 0
} functions;

typedef enum RelationOps
{
    GREATER=0,
    LESSER=1
} RelationOps;

typedef enum LogicalOps
{
    AND=0,
    OR=1,
    NOT=2
} LogicalOps;

struct expr
{
    struct expr *left;
    struct expr *right;
    int boolean;
    float val;
    char op;
    RelationOps rop;
    LogicalOps lop;
    int isTerminal;
    int isVar;
    char *varName;
};

typedef struct SymbolTable
{
    int maxIndex;
    struct symbol *start;
    struct symbol *end;
} SymbolTable;

/* This is a struct for the attribute of a declaration statement. It contains the data type of the variable being declared */
typedef struct declareAttr
{
    Datatype Type;
} declareAttr;  

/* This is a struct for the attribute of an assignment statement. It contains the name of the variable being assigned a value */
typedef struct InsertAttr
{
    char *varName;
} InsertAttr;

/* This is a struct for the attribute of a function call statement. It contains the name of the function being called */
typedef struct funcAttribute
{
    functions fName;
} funcAttribute;

typedef struct Instruct
{
    stmtType Type;
    void *lst;
    void *attr;
    struct Instruct *nxtInst;
} Instruct;

typedef struct symbol
{
    int num; // to store the position of the symbol in the symbol table
    Datatype type;
    char *name;
    int val;      // to store the value of the integer
    int isValSet; // takes value 1 when value is set for the variable  o/w 0
    struct symbol *nextSymbol;
} symbol;

// currently only used for making print statement list
typedef struct exprList
{
    struct expr *exp;
    struct exprList *next;
} exprList;

struct expr *AddNonTerminalExp(struct expr *left, char op, struct expr *right);

struct expr *TerminalExp(float val);

struct expr *TerminalBool(int bool);

struct expr *TerminalVar(char *name);

float Calculate(struct expr *tree);

// A function to add variables to variable declaration list
symbol *Symbol(char *name, symbol *sym);

void printSymbols(symbol *sym);

struct expr* AddRelationalOp(struct expr *left, RelationOps rop, struct expr *right);

struct expr* AddLogicalOp(struct expr *left, LogicalOps lop, struct expr *right);

Instruct *Declaration(symbol *syms, Datatype Type);

void printIns(Instruct *decl);

void addIns(Instruct *ins);

void printCode();

Instruct *returnRead(char *name); 

Instruct *returnAssignment(char *name, struct expr *exp);

Instruct *returnFunction(functions fname, exprList *syms);

exprList *addExp(struct expr *exp, exprList *lst);

void printListExps(exprList *lst);

int Present(char *varName, SymbolTable *table);

int VariableAdd(SymbolTable *table, symbol *sym, Datatype type);

symbol *retrieveSym(char *varName, SymbolTable *table);

int AddVal(char *varName, int val, SymbolTable *table);

int lookUp(char *varName, SymbolTable *table);

int addVars(SymbolTable *table, symbol *lst, Datatype type);

int callPrint(exprList *vals);

int InsExec(Instruct *ins);

int CodeExec(Instruct *codeStart);