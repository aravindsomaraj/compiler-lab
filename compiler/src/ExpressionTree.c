#include "../include/ExpressionTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern SymbolTable SymbolTablesym;
extern Instruct *codeStart;
extern Instruct *codeEnd;
struct expr *AddNonTerminalExp(struct expr *left, char op, struct expr *right)
{
    struct expr *newnode = (struct expr *)malloc(sizeof(struct expr));
    // if malloc worked fine
    if (newnode != NULL)
    {
        // initializing values
        newnode->left = left;
        newnode->op = op;
        newnode->right = right;
    }
    return newnode;
}

// Create a new expression node with a float value
struct expr *TerminalExp(float val)
{
    // Allocate memory for the new node using malloc
    struct expr *newnode = (struct expr *)malloc(sizeof(struct expr));

    // Check if malloc was successful
    if (newnode != NULL)
    {
        // Initialize the values of the new node
        newnode->val = val;
        newnode->isTerminal = 1;
        newnode->isVar = 0;

        // Return the new node
        return newnode;
    }
    // If malloc failed
    else
    {
        printf("Malloc failed.\n");
        exit(1);
        return NULL;
    }
}

// Create a new expression node with a boolean value
struct expr *TerminalBool(int bool)
{
    // Allocate memory for the new node using malloc
    struct expr *newnode = (struct expr *)malloc(sizeof(struct expr));

    // Check if malloc was successful
    if (newnode != NULL)
    {
        // Initialize the values of the new node
        newnode->boolean = bool;
        newnode->isTerminal = 1;
        newnode->isVar = 0;

        // Return the new node
        return newnode;
    }
    // If malloc failed
    else
    {
        printf("Malloc failed.\n");
        exit(1);
        return NULL;
    }
}

// Create a new expression node with a variable name
struct expr *TerminalVar(char *name)
{
    // Allocate memory for the new node using malloc
    struct expr *newnode = (struct expr *)malloc(sizeof(struct expr));

    // Check if malloc was successful
    if (newnode != NULL)
    {
        // Initialize the values of the new node
        newnode->isTerminal = 1;
        newnode->varName = strndup(name, strlen(name));
        newnode->isVar = 1;

        // Return the new node
        return newnode;
    }
    // If malloc failed
    else
    {
        exit(1);
        return NULL;
    }
}

struct expr* AddRelationalOp(struct expr *left, RelationOps rop, struct expr *right)
{
    struct expr* newnode = (struct expr*)malloc(sizeof(struct expr));
    if(newnode!=NULL)
    {
        newnode->left=left;
        newnode->rop=rop;
        newnode->right=right;
    }
    return newnode;
}
struct expr* AddLogicalOp(struct expr *left, LogicalOps lop, struct expr *right)
{
    struct expr* newnode = (struct expr*)malloc(sizeof(struct expr));
    if(newnode!=NULL)
    {
        newnode->left=left;
        newnode->lop=lop;
        newnode->right=right;
    }
    return newnode;
}
// This function calculates the value of an expression tree.
// Input: a pointer to the expression tree
// Output: the calculated value of the expression tree

float Calculate(struct expr *tree)
{
    // If the tree is empty, return -1
    if (tree == NULL)
    {
        return -1;
    }
    // If the tree is a terminal node, return its value or look up its variable value in the symbol table
    if (tree->isTerminal == 1)
    {
        if (tree->isVar == 0)
        {
            return tree->val;
        }
        else
        {
            return lookUp(tree->varName, &SymbolTablesym);
        }
    }
    // If the tree is an operator node, recursively calculate the value of its left and right subtrees based on the operator
    else if (tree->isTerminal == 0)
    {
        switch (tree->op)
        {
        case '+':
            return Calculate(tree->left) + Calculate(tree->right);
            break;
        case '-':
            return Calculate(tree->left) - Calculate(tree->right);
            break;
        case '*':
            return Calculate(tree->left) * Calculate(tree->right);
            break;
        case '/':
            return Calculate(tree->left) / Calculate(tree->right);
            break;
        case '%':
            return (int)Calculate(tree->left)%(int)Calculate(tree->right);
            break;
        default:
            return -1; // return -1 for unknown operator
        }
    }
    else
    {
        return -1; // return -1 for invalid expression tree
    }
}

// This function adds a new variable to the variable declaration list.
// Input: a pointer to the variable name and a pointer to the current symbol table
// Output: a pointer to the new symbol table

symbol *Symbol(char *name, symbol *currSym)
{
    // Allocate memory for the new symbol and initialize its fields
    symbol *newnode = (symbol *)malloc(sizeof(symbol));
    newnode->nextSymbol = currSym;
    newnode->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    newnode->isValSet = 0;
    newnode->val = 0;
    newnode->num = 0;
    strcpy(newnode->name, name);
    return newnode;
}

// This function prints the names of all symbols in the symbol table.
// Input: a pointer to the current symbol table
// Output: none

void printSymbols(symbol *currSym)
{
    // Traverse the symbol table and print the name of each symbol
    while (currSym != NULL)
    {
        printf("%s ", currSym->name);
        currSym = currSym->nextSymbol;
        if (currSym->nextSymbol == NULL)
        {
            break;
        }
    }
    printf("%s ", currSym->name); // print the last symbol
}

// void addIns (void * attribute)

Instruct *Declaration(symbol *syms, Datatype Type)
{
    Instruct *newIns = (Instruct *)malloc(sizeof(Instruct));
    declareAttr *newAttr = (declareAttr *)malloc(sizeof(declareAttr));
    newAttr->Type = Type;
    newIns->nxtInst = NULL;
    newIns->lst = (void *)syms;
    newIns->attr = (void *)newAttr;
    newIns->Type = DECLARATION;
    return newIns;
}

void printExp(struct expr *tree)
{
    if (tree == NULL)
    {
        return;
    }
    if (tree->isTerminal == 1)
    {
        int value = tree->val;
        if (tree->isVar == 0)
        {
            printf("%d ", value);
        }
        else
        {
            printf("%s ", tree->varName);
        }
    }
    else if (tree->isTerminal == 0)
    {
        switch (tree->op)
        {
        case '+':
            printf("PLUS ");
            printExp(tree->left);
            printExp(tree->right);
            break;
        case '-':
            printf("SUB ");
            printExp(tree->left);
            printExp(tree->right);
            break;
        case '*':
            printf("MUL ");
            printExp(tree->left);
            printExp(tree->right);
            break;
        case '/':
            printf("DIV ");
            printExp(tree->left);
            printExp(tree->right);
            break;
        default:;
        }
    }
}

void printIns(Instruct *ins)
{

    if (ins->Type == DECLARATION)
    {
        printf("DECL ");
        printSymbols(ins->lst);
        printf(";\n");
    }
    else if (ins->Type == ASSIGNMENT)
    {
        InsertAttr *attr = ins->attr;
        printf("ASSIGN %s ", attr->varName);
        printExp(ins->lst);
        printf(";\n");
    }
    else if (ins->Type == FUNC)
    {
        funcAttribute *attr = ins->attr;
        if (attr->fName == PRINT)
        {
            printf("CALL print ");
            printListExps(ins->lst);
            printf(";\n");
        }
    }
}

void addIns(Instruct *ins)
{
    if (codeStart == NULL)
    {
        codeStart = ins;
        codeEnd = ins;
    }
    else
    {
        codeEnd->nxtInst = ins;
        codeEnd = ins;
    }
}

void printCode()
{
    printf("Printing AST.\n");
    Instruct *ins = codeStart;
    while (ins != NULL)
    {
        // printf("reached fing here\n");
        printIns(ins);
        ins = ins->nxtInst;
    }
}

Instruct *returnRead(char *name)
{
    Instruct *newIns = (Instruct *)malloc(sizeof(Instruct));
    // InsertAttr *newAttr = (InsertAttr *)malloc(sizeof(InsertAttr));
    // newAttr->varName = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    // strcpy(newAttr->varName, name);
    int newval;
    scanf("%d",&newval);
    
    if(Present(name,&SymbolTablesym))
        printf("Baby I love you!");
    AddVal(name,newval,&SymbolTablesym);
    printf("or NOT ! moo mele");
    newIns->nxtInst = NULL;
    // newIns->lst = (void *)exp; 
    // newIns->attr = (void *)newAttr;
    newIns->Type = READ_STMT;
    return newIns;
}

Instruct *returnAssignment(char *name, struct expr *exp)
{
    Instruct *newIns = (Instruct *)malloc(sizeof(Instruct));
    InsertAttr *newAttr = (InsertAttr *)malloc(sizeof(InsertAttr));
    newAttr->varName = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(newAttr->varName, name);

    newIns->nxtInst = NULL;
    newIns->lst = (void *)exp; 
    newIns->attr = (void *)newAttr;
    newIns->Type = ASSIGNMENT;
    return newIns;
}

Instruct *returnFunction(functions fname, exprList *syms)
{
    Instruct *newIns = (Instruct *)malloc(sizeof(Instruct));
    funcAttribute *newAttr = (funcAttribute *)malloc(sizeof(funcAttribute));
    newAttr->fName = fname;

    newIns->nxtInst = NULL;
    newIns->lst = (void *)syms;
    newIns->attr = (void *)newAttr;
    newIns->Type = FUNC;
    return newIns;
}

exprList *addExp(struct expr *exp, exprList *lst)
{
    exprList *newExprListNode = (exprList *)malloc(sizeof(exprList));
    if (newExprListNode != NULL)
    {
        newExprListNode->exp = exp;
        newExprListNode->next = lst;
    }
    else
    {
        printf("malloc failed in addExp function \n");
    }
    return newExprListNode;
}

void printListExps(exprList *lst)
{
    exprList *ptr = lst;
    while (ptr != NULL)
    {
        printExp((ptr->exp));
        ptr = ptr->next;
    }
}

int Present(char *varName, SymbolTable *table)
{

    symbol *var = table->start;
    while (var != NULL)
    {
        if (strcmp(var->name, varName) == 0)
        {
            return 1;
        }
        else
        {
            var = var->nextSymbol;
        }
    }
    return 0;
}

int VariableAdd(SymbolTable *table, symbol *currSym, Datatype type)
{

    if (Present(currSym->name, table) == 1)
    {
        printf("Multiple delcaration of the variable '%s'\n", currSym->name);
        exit(1);
    }
    else
    {
        if (table->start == NULL)
        {
            table->start = currSym;
            table->end = currSym;
            table->maxIndex += 1;
        }
        else
        {
            table->end->nextSymbol = currSym;
            table->end = currSym;
            table->maxIndex += 1;
        }

        currSym->num = table->maxIndex;
        currSym->type = type;
        currSym->nextSymbol = NULL;
        return 1;
    }
}

symbol *retrieveSym(char *varName, SymbolTable *table)
{

    symbol *var = table->start;

    while (var != NULL)
    {
        if (strcmp(var->name, varName) == 0)
        {
            return var;
        }
        var = var->nextSymbol;
    }
    return NULL;
}

int AddVal(char *varName, int val, SymbolTable *table)
{
    symbol *var = retrieveSym(varName, table);

    if (var != NULL)
    {
        var->isValSet = 1;
        var->val = val;
        return 1;
    }
    else
    {
        if (Present(varName, table) == 0)
        {
            printf("Variable '%s' not declared.\n", varName);
            exit(1);
        }
        return 0;
    }
}

// This look up function returns zero value even if the values is not assigned to the variable ...
// ... if the variable is present in the table .
int lookUp(char *varName, SymbolTable *table)
{
    symbol *var = retrieveSym(varName, table);

    if (var != NULL)
    {

        return var->val;
    }
    else
    {
        if (Present(varName, table) == 0)
        {
            printf("Variable '%s' not declared.\n", varName);
            exit(1);
        }
        return 0;
    }
}

symbol *symDup(symbol *sym1)
{
    symbol *newnode = (symbol *)malloc(sizeof(symbol));

    if (newnode == NULL)
    {
        printf("Malloc failed.\n");
        exit(1);
    }

    newnode->isValSet = sym1->isValSet;
    newnode->name = sym1->name;
    newnode->nextSymbol = sym1->nextSymbol;
    newnode->num = sym1->num;
    newnode->type = sym1->type;
    newnode->val = sym1->val;
    return newnode;
}

int addVars(SymbolTable *table, symbol *lst, Datatype type)
{
    symbol *currSym = lst, *temp, *newnode;
    while (currSym != NULL)
    {
        newnode = symDup(currSym);
        newnode->nextSymbol = NULL;
        VariableAdd(table, newnode, type);
        currSym = currSym->nextSymbol;
    }
    return 1;
}

int callPrint(exprList *vals)
{
    exprList *expr = vals;
    int val;
    while (expr != NULL)
    {
        val = Calculate(expr->exp);
        printf("%d ", val);
        expr = expr->next;
    }
    printf("\n");
    return 1;
}

int InsExec(Instruct *ins)
{
    if (ins->Type == DECLARATION)
    {
        declareAttr *attr = ins->attr;
        addVars(&SymbolTablesym, ins->lst, attr->Type);
        return 1;
    }
    else if (ins->Type == ASSIGNMENT)
    {
        InsertAttr *attr = ins->attr;
        struct expr *val = ins->lst;

        AddVal(attr->varName, Calculate(val), &SymbolTablesym);
        return 1;
    }
    else if (ins->Type == FUNC)
    {
        funcAttribute *attr = ins->attr;
        if (attr->fName == PRINT)
        {
            callPrint(ins->lst);
        }
        return 1;
    }
}

int CodeExec(Instruct *codeStart)
{
    Instruct *ins = codeStart;

    while (ins != NULL)
    {
        InsExec(ins);
        ins = ins->nxtInst;
    }
}