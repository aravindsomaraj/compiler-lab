/**
 * REFER TO the README file for how to RUN THIS CODE!!!
*/

#include "../include/ExpressionTree.hpp"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <string>
using namespace std;

map<string,value*> symTable;
map<string,aboutFunc*> funcTable;
struct Program* code_start;
bool syntax_flag=true,semantic_flag=false;

void SymTableInsertion(char* name, Datatype type, map<string,value*>& symtable);
void SymTableInsertion(char* name, Datatype type, indexList* index, map<string,value*>& symtable);

evaluation* SymLookUp(char* name, map<string,value*>& symtable );
evaluation* SymLookUp(char* name, indexList* index, map<string,value*>& symtable);

void updateVar(char* name, evaluation* res, map<string,value*>& symtable );
void updateVar(char* name, indexList* index , evaluation* res, map<string,value*>& symtable);

void execStmtList(statement* stmt_list,map<string,value*>& symtable);
void printExpression(expr * tree_root);

evaluation* Calculate(expr* tree_root, map<string,value*> symtableLocal);
evaluation* executeFunctionCall(FunctionCall* functionCall, map<string,value*>& symtable_caller);

void printBool(bool b);
void printStrExprList(strExp* list);
void printType(Datatype type);
/*---------------------------------------------------------------------------------------------------------------------------*/

/*** -------      Symbol Table Functions ---------------*/
int Changeto1Dimension (indexList* declIndex, indexList* arrayIndex);
int dimensions (indexList* index);
int size_of_array (indexList* index);
bool range_check(indexList* declIndex , indexList* index);

void SymTableInsertion(char* name, Datatype type,map<string,value*>& symtable)
{
    if(symtable.count(name)>0){
        cerr << "Multiple declaration of the variable "<< name <<endl;syntax_flag = false;
        if(!semantic_flag)
            exit(1);
    }
    else{
        value* curr = DeclareNewVal(name,type);
        curr->type = type;
        symtable[name] = curr;
    }
}


void SymTableInsertion(char* name,Datatype type, indexList* index, map<string,value*>& symtable)
{
    if(symtable.count(name)>0){
        cerr << "Multiple Declare of the variable "<< name <<endl;syntax_flag = false;
        if(!semantic_flag)
            exit(1);
    }
    else{
        value* curr = DeclareNewVal(name,type,index);
        curr->type = type;
        symtable[name] = curr;
    }
}

evaluation* SymLookUp(char* name, map<string,value*>& symtableLocal)
{
    map<string,value*>* symtable;
    if(symtableLocal.count(name)>0)
        symtable = & symtableLocal;
    else if (symTable.count(name)>0)
        symtable = & symTable;
    else{
        cerr << "Undeclared Var - "<< name<<endl;syntax_flag = false;
        if(!semantic_flag)
            exit(1);
        return NULL;
    }
    
    value* curr = (*symtable)[name];
    evaluation* res = new evaluation;
    switch(curr->type)
    {
        case INT:
                if(curr->isArray)
                {
                    cerr<< name <<" : Array found. Index required*" << endl;
                    syntax_flag = false;
                    if(!semantic_flag)
                        exit(1);
                }
                else{
                    res->type = INT;
                    res->value = (int *)(curr->val);
                    return res;
                }
                break;
        case BOOL:
                if(curr->isArray)
                {
                    cerr<< name <<" : Array found. Index required*" << endl;
                    syntax_flag = false;
                    if(!semantic_flag)
                        exit(1);
                }
                else 
                {
                    res->type = BOOL;
                    res->value = (bool *)(curr->val);
                    return res;
                }
                break;
        default:
                cerr << "Type invalid*" << endl;
                syntax_flag = false;
                if(!semantic_flag)
                    exit(1);
    }
    return NULL;
}

evaluation* SymLookUp(char* name, indexList* index, map<string,value*>& symtableLocal)
{
    map<string,value*>* symtable;
    if(symtableLocal.count(name)>0)
        symtable = & symtableLocal;
    else if (symTable.count(name)>0)
        symtable = & symTable;
    else{
        cerr << "Undeclared Var - " << name << endl;syntax_flag = false;
        if(!semantic_flag)
            exit(1);
        return NULL;
    }
    
    value * curr = (*symtable)[name];
    evaluation * res = new evaluation;
    switch(curr->type)
    {
        case INT:
                if(!(curr->isArray))
                {
                    cerr<<name<<" : not an array, therefore not indexable." << endl;
                    syntax_flag = false;
                    if(!semantic_flag)
                        exit(1);
                    return NULL;
                }
                else{
                    Array * arr = (Array *)(curr->val);
                    int newindex = Changeto1Dimension(arr->indexLst,index);
                    

                    res->type = curr->type;
                    res->value = &(((int *)(arr->arr))[newindex]) ;
                    return res;
                }
                break;
        case BOOL:
                if(!(curr->isArray))
                {
                    cerr<<name<<" : not an array, therefore not indexable." << endl;
                    syntax_flag = false;
                    if(!semantic_flag)
                        exit(1);
                    return NULL;
                }
                else{
                    Array * arr = (Array *)(curr->val);
                    int newindex = Changeto1Dimension(arr->indexLst,index);

                    res->type = curr->type;
                    res->value = &(((bool *)(arr->arr))[newindex]) ;
                    return res;
                }
                break;
        default:
                cerr<< "Type invalid*" << endl;
                syntax_flag = false;
                if(!semantic_flag)
                    exit(1);
    }
    return NULL;
}

void updateVar(char* name, evaluation* result, map<string,value*>& symtableLocal )
{
    map<string,value*>* symtable;
    if(symtableLocal.count(name)>0)
        symtable = & symtableLocal;
    else if (symTable.count(name)>0)
        symtable = & symTable;
    else{
        cerr << "Undeclared Var - "<< name << endl;syntax_flag = false;
        if(!semantic_flag)
            exit(1);
        return;
    }
    value* curr = (*symtable)[name];
    switch(curr->type)
    {
        case INT:
                if(result->type == INT){
                    if(curr->isArray){
                        cerr<<name<<" - Array found. Index required. *" << endl;
                        syntax_flag = false;
                        if(!semantic_flag)
                            exit(1);
                    }
                    else{
                        *((int *)(curr->val)) = *(int*)(result->value);
                    }
                }
                else
                {
                    cerr << "Variable \""<< name <<"\" is integer type. Datatype error found*" << endl;syntax_flag = false;
                    if(!semantic_flag)
                        exit(1);
                }
                break;
        case BOOL:
                if(result->type == BOOL)
                {
                    if(curr->isArray)
                    {
                        cerr<< name <<" - Array found. Index required. *"<<endl;
                        syntax_flag = false;
                        if(!semantic_flag)
                            exit(1);
                    }
                    else{
                        *((bool *)(curr->val)) = *(bool*)(result->value);
                    }
                }
                else
                {
                    cerr << "Variable \""<< name <<"\" is boolean type. Datatype error found*" << endl;syntax_flag = false;
                    if(!semantic_flag)
                        exit(1);
                }
                break;
        default:
                cerr << "Invalid type "<<endl;
                syntax_flag = false;
                if(!semantic_flag)
                    exit(1);
    }
}

void updateVar(char* name, indexList* index , evaluation* res, map<string,value* >& symtableLocal)
{
    map<string,value*>* symtable;
    if(symtableLocal.count(name) > 0)
        symtable = & symtableLocal;
    else if (symTable.count(name) > 0)
        symtable = & symTable;
    else{
        cerr << "Undeclared Var - "<< name << endl;syntax_flag = false;
        if(!semantic_flag)
            exit(1);
        return;
    }
    
    value * curr = (*symtable)[name];
    if(curr->type == INT)
    {
        if(res->type == INT){
            if(!(curr->isArray))
            {
                cerr<<name<<" : not an array, therefore not indexable."<<endl;
                syntax_flag = false;
                if(!semantic_flag)
                    exit(1);
            }
            else{
                Array * arr = (Array *)(curr->val);
                int newIndex = Changeto1Dimension(arr->indexLst,index);    
                ((int *)(arr->arr))[newIndex] = *(int *)(res->value);
            }
        }
        else{   
            cerr << "Type mismatch* "<<endl;
            syntax_flag = false;
            if(!semantic_flag)
                exit(1);
        }
    }
    else if(res->type == BOOL)
    {
        if(res->type == BOOL){
            if(!(curr->isArray))
            {
                cerr<<name<<" : not an array, therefore not indexable."<<endl;
                syntax_flag = false;
                if(!semantic_flag)
                    exit(1);
            }
            else{
                Array * arr = (Array *)(curr->val);
                int newIndex = Changeto1Dimension(arr->indexLst,index);
                ((bool *)(arr->arr))[newIndex] = *(bool *)(res->value);
            }
        }
        else{   
            cerr << "Type mismatch* "<< endl;
            syntax_flag = false;
            if(!semantic_flag)
                exit(1);
        }
    }
    else{
        cerr << "Invalid type* "<<endl;
        syntax_flag = false;
        if(!semantic_flag)
            exit(1);
    }
}

/*-------------- Symbol table dimension change helper functions  -----------------*/

int dimensions(indexList* index)    // return dimensions of the array 1d
{
    int dimension = 0;
    while(index!= NULL)
    {
        dimension++;
        index = index->next;
    };

    return dimension;
}

int size_of_array(indexList* index) // returns size of the array obtained
{
    int size = 1;
    indexList* start = index;
    if(index == NULL)
        return 0;
    else{
        while(start!= NULL){
            if(start->i<=0){
                cerr<<"Invalid array index* " << endl;
                syntax_flag = false;
                if(!semantic_flag)
                    exit(1);
            }
            size *= start->i;
            start = start->next;
        };
    }
    return size;
}


bool range_check(indexList* declIndex ,indexList* index)    // checking for viability of ranges given for the array
{
    if(declIndex==NULL || index==NULL)
        return false;
    else if(dimensions(declIndex)!=dimensions(index)){
        cerr<<"Array dimension mismatch* " << endl;syntax_flag = false;
        if(!semantic_flag)
            exit(1);
        return false;
    }
    int size = dimensions(declIndex);
    if(!semantic_flag){
        for (int i=0;i<size;i++){
            if(declIndex->i <= index->i || index->i < 0){
                cerr << i+1 <<"th index out of range* " << endl;syntax_flag = false;
                if(!semantic_flag)
                    exit(1);
                return false;
            }

            declIndex = declIndex->next;
            index = index->next;
        }
    }
    return true;
}

int Changeto1Dimension(indexList* declIndex, indexList* arrayIndex)  // Dimension conversion
{
    int index = 0;
    int size = dimensions(declIndex);
    if(range_check(declIndex,arrayIndex)){  
        for(int i=0;i<size-1;i++){
            index += (arrayIndex->i)*(size_of_array(declIndex->next));
            arrayIndex = arrayIndex->next;
            declIndex = declIndex->next;
        }
        index += arrayIndex->i;
        return index;
    }
    else{
        syntax_flag = false;
        if(!semantic_flag)
            exit(1);
    }
    return 0;
}


/*** -------------------  End of Symbol Table Functions --------  ****/


///------------------------------------------------------------------------------///


/**** ------------------------     Syntax Tree Expression Functions  -------------------****/


struct expr* TerminalExp(int val){  // Create a new LEAF expression node with a int value   
    // Creating an integer expression with required value 
    intExp * curr = new intExp;
    curr->type = INT;
    curr->val = val;
    
    // Creating a tree node to store the above integer expression
    expr * newCurr = new expr;
    newCurr->attribute = curr;
    newCurr->left = NULL;
    newCurr->right = NULL;
    newCurr->type = LEAF;
    newCurr->valueType = INT;
    
    return newCurr;
}

struct expr* TerminalExp(bool val){    // Create a new LEAF expression node with a bool value 
    // Creating an bool expression with required value
    boolExpr * curr = new boolExpr;
    curr->type = BOOL;
    curr->val = val;
    
    // Creating a tree node to store the above integer expression
    expr * newCurr = new expr;
    newCurr->attribute = curr;
    newCurr->left = NULL;
    newCurr->right = NULL;
    newCurr->type = LEAF;
    newCurr->valueType = BOOL;

    return newCurr;
}

struct expr* TerminalExp(FunctionCall* fun_call){  // Create a new funCall expression 
    expr * newCurr = new expr;
    newCurr->attribute = fun_call;
    
    // Type is attributed as 'FuncCall' type
    newCurr->left = NULL;
    newCurr->right = NULL;
    newCurr->type = FuncCall;
    newCurr->valueType = none;

    return newCurr;
}

struct expr* TerminalExp(varExpr* var)  // Create a new variable expression node for syntax tree
{
    expr * curr = new expr;
    curr->attribute = var;
    curr->left = NULL;
    curr->right = NULL;
    curr->type = LEAF;
    curr->valueType = Var;

    return curr;
}

struct varExpr* createVal(char* name){  // Create a new variable expression
    varExpr * curr = new varExpr;
    curr->varName = strdup(name);
    curr->isArray = false;
    curr->index = NULL;
    curr->endIndex = NULL;

    return curr;
}

struct exprList* makeExprList(struct expr* exp)    // for creating a fresh list
{
    exprList * newnode = new exprList;
    newnode->exp = exp;
    newnode->next = NULL;
    return newnode;
}

struct exprList* makeExprList (struct expr* exp, struct exprList* exprlist){ // for adding to an already existing list
    exprList * newnode = new exprList;
    newnode->exp = exp;
    newnode->next = exprlist;
    return newnode;
}


struct varExpr* createVal(struct varExpr* varExp, struct expr* index){  // Create a new variable expression for arrays
    exprList * newCurr = new exprList;
    newCurr->exp = index;
    newCurr->next = NULL;

    if(varExp->index == NULL){
        varExp->index = newCurr;
        varExp->endIndex = newCurr;
    }
    else{
        varExp->endIndex->next = newCurr;
        varExp->endIndex = newCurr;
    }
    varExp->isArray = true;
    return varExp;
}

Datatype opType (char op)  // Used to find out the datatype of the operands used, via the operator
{
    switch (op)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '<':
        case '>':
        case 'g':
        case 'l':
        case 'n':
        case 'e':
                    return INT;  // Fall through for all operators that yield integer
        case '!':
        case '&':
        case '|':
                    return BOOL; // Fall through for all operators that yield boolean
        default:
            cerr << "Invalid Operator" << endl;syntax_flag = false;
            if(!semantic_flag)
                exit (0);
    }
    return none;
}

struct expr * addNonTerminalExpr (expr * left, char op ,expr * right)
{
    OperatorSym * curr = new OperatorSym;
    curr->type = opType(op);
    curr->op = op;

    expr * newCurr = new expr;
    newCurr->attribute = curr;
    newCurr->left = left;
    newCurr->right = right;
    newCurr->type = InternalNODE;
    newCurr->valueType = none;

    return newCurr;
}

int sizeOfExprList (exprList * start){  // Returns the size of the expression List
    int i=0;
    while(start!=NULL){
        i+=1;
        start=start->next;
    }
    return i;
}

indexList* exprListToindexList(exprList* expList, map<string,value*> symtableLocal)
{
    indexList *start=NULL,*end=NULL;
    evaluation * res;
    int size = sizeOfExprList(expList);

    for(int i=0;i<size;i++)
    {
        if(start==NULL){

            start = new indexList;
            start->next = NULL;
            end = start;
            res = Calculate(expList->exp, symtableLocal);
            switch(res->type)
            {
                case BOOL:
                            cerr<<"Invalid expression type in the "<< (i+1) <<"th index; integer type expected* " << endl;syntax_flag = false;
                            if(!semantic_flag)
                                std::exit(1);
                            break;
                case INT:   
                            start->i = *(int *)(res->value);
                            break;
            }
            delete res;
        }
        else{
            end->next = new indexList;
            end = end->next;
            end->next = NULL;
            res = Calculate(expList->exp,symtableLocal);

            switch(res->type)
            {
                case BOOL:
                        cerr<<"Invalid expression type in the "<< (i+1) <<"th index; integer type expected* " << endl;syntax_flag = false;
                        if(!semantic_flag)
                            std::exit(1);
                        delete ((bool *)(res->value));
                        break;
                case INT:   
                        end->i = *(int *)(res->value);
                        delete ((int *)(res->value));
                        break;
            }
            delete res;
        }
        expList=expList->next;        
    }
    return start;
}

struct evaluation* Calculate(expr* tree_root, map<string,value *> symtableLocal)  // Calculation works for function calls too, hence the inclusion of 
{                                                                          // the symbol table of the function that calls Calculate()
    evaluation * curr = new evaluation ;
    switch(tree_root->type)
    {
        case LEAF:
            switch(tree_root->valueType)
            {
                case INT:
                        curr->type = ((intExp*)(tree_root->attribute))->type;
                        curr->value = &((intExp*)(tree_root->attribute))->val;
                        break;
                case BOOL:
                        curr->type = ((boolExpr*)(tree_root->attribute))->type;
                        curr->value = &((boolExpr*)(tree_root->attribute))->val;
                        break;
                case Var:{
                        varExpr* varE = (varExpr*)(tree_root->attribute) ;
                        if(!(varE->isArray))
                        {
                            evaluation* val = SymLookUp(varE->varName,symtableLocal);
                            curr = val;
                        }
                        else{
                            indexList * indexlist = exprListToindexList (varE->index, symtableLocal);
                            
                            evaluation * val = SymLookUp(varE->varName,indexlist,symtableLocal);
                            curr = val;
                            
                        }}
                        break;
                default:
                        cerr <<"invalid Type for a LEAF";
                        if(!semantic_flag)
                            std::exit(1);
            }
            break;
        case FuncCall:
                if(!semantic_flag){
                    delete curr;
                    curr = executeFunctionCall((FunctionCall*)(tree_root->attribute),symtableLocal);
                }
                else
                {   
                    FunctionCall* functionCall = (FunctionCall*)(tree_root->attribute);
                    if( funcTable.count(functionCall->funName)>0){
                        aboutFunc* funcdetails = funcTable[functionCall->funName];
                        if(!(funcdetails->isDefSet && funcdetails != NULL)){
                            cerr<< "Function \""<<functionCall->funName<<"\" not defined* " << endl;syntax_flag = false;
                        }

                        curr->type = funcdetails->declType;
                        if(curr->type == INT)
                            curr->value = new int(1);
                        else
                            curr->value = new bool(false);
                    }
                    else{
                        cerr<< "Function \""<<functionCall->funName<<"\" declaration not found* " << endl;
                        syntax_flag = false;
                        curr->type = INT;
                        curr->value = new int(1);
                    }           
                }
                break;
        case InternalNODE: {
                struct OperatorSym* currOp = (OperatorSym*)(tree_root->attribute);
                Datatype currType = currOp->type;
                switch(currType)
                {
                    case BOOL: {
                            char OP = currOp->op;
                            if(OP =='&' || OP == '|' || OP == '!'){
                                bool Left = *(bool*)(Calculate(tree_root->left,symtableLocal)->value);
                                bool Right= *(bool*)(Calculate(tree_root->right,symtableLocal)->value);
                                bool newVal = false;
                                switch(OP)
                                {
                                    case '&': newVal = Left & Right; break;
                                    case '|': newVal = Left | Right; break;
                                    case '!': newVal = !(Right); break;
                                }
                                curr->value = new bool(newVal);
                                curr->type = BOOL;
                            }
                            else{
                                cerr << "Invalid operator. Operator - Operand : type mismatch* " << endl;
                                syntax_flag = false;
                                if(!semantic_flag)
                                    std::exit(1);
                            }
                        }
                        break;
                    case INT: {
                            char OP = currOp->op;
                            if(OP=='+' || OP =='-' || OP =='/' || OP =='*' || OP =='%')
                            {
                                int Left = *(int*)(Calculate(tree_root->left,symtableLocal)->value);
                                int Right = *(int*)(Calculate(tree_root->right,symtableLocal)->value);
                                int newVal = 0;
                                switch(OP)
                                {
                                    case '+': newVal = Left + Right; break;
                                    case '-': newVal = Left - Right; break;
                                    case '*': newVal = Left * Right; break;
                                    case '/': newVal = Left / Right; break;
                                    case '%': newVal = Left % Right; break;
                                }
                                curr->value = new int(newVal);
                                curr->type = INT;
                            }
                            else if(OP == '<' || OP == '>' || OP == 'g' || OP == 'l' || OP == 'n' || OP == 'e')
                            {
                                int Left = *(int*)(Calculate(tree_root->left,symtableLocal)->value);
                                int Right= *(int*)(Calculate(tree_root->right,symtableLocal)->value);
                                bool newVal = false;
                                switch(OP)
                                {
                                    case '<': newVal = Left < Right; break;
                                    case '>': newVal = Left > Right; break;
                                    case 'g': newVal = Left >= Right; break;
                                    case 'l': newVal = Left <= Right; break;
                                    case 'n': newVal = Left != Right; break;
                                    case 'e': newVal = Left == Right; break;
                                }
                                curr->value = new bool(newVal);
                                curr->type = BOOL;
                            }
                            else
                            {
                                cerr << "Invalid operator. Operator - Operand : type mismatch* ";
                                syntax_flag = false;
                                if(!semantic_flag)
                                    std::exit(1);
                            }
                        }
                        break;
                    default:
                        cerr << "Invalid expression node found* ";
                        syntax_flag = false;
                        if(!semantic_flag)
                            std::exit(1);
                }
                }break;
    }
    return curr;   
}
/*** -------------------  End of Syntax Tree Functions --------  ****/



/*---------------------------------------*/


/***   ---------------  Declaration Functions  -------------   ****/

value* DeclareNewVal(char* name, Datatype type) // Declaring a new variable 
{
    value* curr = new value;
    curr->type = type;
    if(type == INT)
        curr->val = new int(0);
    else if(type == BOOL)
        curr->val = new bool(false);

    curr->isArray = false;
    curr->name = strdup(name);
    return curr;
}

value* DeclareNewVal(char* name, Datatype type, indexList* index) // Declaring a new variable array
{
    value* curr = new value;
    curr->name = strdup(name);
    curr->type = type;
    curr->isArray = true;

    Array* arr = new Array;
    if(type == INT)
        arr->arr = new int[size_of_array(index)]();
    else if (type == BOOL)
        arr->arr = new bool[size_of_array(index)]();
        
    arr->indexLst = index;
    arr->dimensions = dimensions(index);
    curr->val = arr;

    return curr;
}

VariableDeclaration* DeclareNewNode(char* name) // Creation of declaration node for the variable declared
{    
    VariableDeclaration* newCurr = new VariableDeclaration;
    newCurr->name = strdup(name);
    newCurr->index = NULL;
    newCurr->endIndex = NULL;
    newCurr->isArray = false;
    return newCurr;
} 


VariableDeclaration* DeclareNewNode(VariableDeclaration* node, int size) // Creation of declaration node for the variable array declared
{
    indexList* var = new indexList;
    var->i = size;
    var->next = NULL;
    if(node->index==NULL){
        node->index = var;
        node->endIndex = var;
    }
    else{
        node->endIndex->next = var;
        node->endIndex = var;
    }
    node->isArray = true;

    return node; 
}

Glist* create_global_decl_list (VariableDeclaration* node)     // Fresh list for Global declarations of variables
{                                                        
    Glist * curr = new Glist;
    curr->next = NULL;
    curr->isGid = true;
    curr->node = node;
    return curr;
}

Glist* create_global_decl_list (VariableDeclaration* node, Glist* glist) // Adding to already existing list of global declarations
{
    Glist* curr = new Glist;
    curr->next = glist;
    curr->isGid = true;
    curr->node = node;
    return curr;
}

Glist* create_global_decl_list(func* node) // Fresh list for Global declarations for function definitions
{
    Glist* curr = new Glist;
    curr->next = NULL;
    curr->isGid = false;
    curr->node = node;
    return curr;
}

Glist* create_global_decl_list(func* node, Glist* glist)  // Adding to already existing list of global declarations for function definitions
{
    Glist* curr = new Glist;
    curr->next = glist;
    curr->isGid = false;
    curr->node = node;
    return curr;
}


var_list* create_variable_list (char* name){
    var_list* list = new var_list;
    list->name = strdup(name);
    list->next = NULL;
    return list;
}

var_list* create_variable_list(char* name, var_list* varList){
    var_list* list = new var_list;
    list->name = strdup(name);
    list->next = varList;
    return list;
}

arg* parameters(Datatype type, var_list* varList)
{
    arg* curr = new arg;
    curr->type = type;
    curr->varList = varList;
    return curr;
}

arg_list* create_func_parameters(arg* Arg)
{
    arg_list* list = new arg_list;
    list->Arg = Arg;
    list->next = NULL;
    return list;
}

arg_list* create_func_parameters(arg* Arg, arg_list* argList)
{
    arg_list* newList = new arg_list;
    newList->Arg = Arg;
    newList->next = argList;
    return newList;
}

func* createFunction(char* name, arg_list* argList)
{
    func* fun = new func;
    fun->name = strdup(name);
    fun->argList = argList;
    return fun;
}

FunctionDef* FunctionDefinition(Datatype ret_type, char* name, arg_list* argList , Declare* local_decl, statement* stmt_list, expr* ret_stmt){
    FunctionDef* fun = new FunctionDef;
    fun->retType = ret_type;
    fun->funcName = strdup(name);
    fun->FargList = argList;
    fun->LdeclSec = local_decl;
    fun->stmtList = stmt_list;
    fun->retStmt = ret_stmt;
    fun->next = NULL;
    return fun;
}

FunctionDef* createFdefSection(FunctionDef* fdef, FunctionDef* fdefSec){
    fdef->next = fdefSec;
    return fdef;
}


Declare* create_decl(Datatype type, Glist* top){
    Declare* curr = new Declare;
    curr->type = type;
    curr->list = top;
    curr->next = NULL;
    return curr;
}

Declare* create_decl_list(Declare* node, Declare* list){
    node->next = list;
    return node;
}

void Declare_block (Declare* root){
    code_start = (struct Program*)malloc(sizeof(struct Program));
    code_start->global_declarations = root;
}


/*** -------------------  End of Declaration Functions --------  ****/


/*---------------------------------------*/


/*** -------------------  Statement List Functions --------  ****/


// -------------      Assignment functions -----------------------//

statement* createAssignment(varExpr* var, expr* exp){
    assignment_stmt* curr = new assignment_stmt;
    curr->var = var;
    curr->exp = exp;
    
    statement* stmt = new statement;
    stmt->stmt = curr;
    stmt->stmtType = assign; 
    stmt->next = NULL;
    return stmt;
}



///------------ Making string expressions ----------------------------------///

strExp* StringExp (char* name)
{
    strExp* newCurr = new strExp ;
    newCurr->var = strdup(name);
    newCurr->next = NULL;
    return newCurr;
}


strExp* StringExp (char* name, strExp* exp)
{
    strExp* newCurr = new strExp ;
    newCurr->var = strdup(name);
    newCurr->next = exp;
    return newCurr;
}

//---------------------              write functions    ------------------------//

statement* createWrite (expr* exp)          // Write function for normal expressions (variables, array values, etc.)
{
    write_stmt* newCurr = new write_stmt;
    newCurr->val = exp;
    newCurr->isExpr = true;
    
    statement* newStmt = new statement;
    newStmt->stmt = newCurr;
    newStmt->stmtType = Write; 
    newStmt->next = NULL;
    return newStmt;
}

statement* createRead(varExpr* exp)         // Read function for variables
{
    read_stmt* newCurr = new read_stmt;
    newCurr->varexp = exp;

    struct statement* curr = (struct statement*)malloc(sizeof(struct statement));
    curr->stmt = newCurr;
    curr->stmtType = Read;
    curr->next = NULL;
    return curr;
}

statement* createWrite (strExp* exp)    // Write function for string expressions
{    
    write_stmt* newCurr = new write_stmt;
    newCurr->val = exp;
    newCurr->isExpr = false;
    
    statement* newStmt = new statement;
    newStmt->stmt = newCurr;
    newStmt->stmtType = Write;
    newStmt->next = NULL;
    return newStmt;
}


//----------------------------------------------//

//-----------------    General statement functions   --------//

statement * createStatementList (statement * stmt, statement * stmtList)
{
    stmt->next = stmtList;
    return stmt;
}



//----------------------------------------------//


//------------------    Conditional Statement functions        ----------------------------//
statement* create_IF(expr* exp, statement* stmt_list)
{
    if_stmt* newIf = new if_stmt;
    newIf->COND = exp;
    newIf->stmtList = stmt_list;

    conditional_stmt* newCondStmt = new conditional_stmt;
    newCondStmt->cond_stmt_attrs = newIf;
    newCondStmt->type = If;

    statement* newStmt = new statement;
    newStmt->stmt = newCondStmt;
    newStmt->stmtType = COND; 
    newStmt->next = NULL;
    return newStmt;    

}

statement* create_IFELSE(expr* exp, statement* if_stmt_list, statement* else_stmt_list)
{
    ifelse_stmt* newIfElse = new ifelse_stmt;
    newIfElse->COND = exp;
    newIfElse->if_stmtList = if_stmt_list;
    newIfElse->else_stmtList = else_stmt_list;

    conditional_stmt* newCondStmt = new conditional_stmt;
    newCondStmt->cond_stmt_attrs = newIfElse;
    newCondStmt->type = IF_ELSE;

    statement* newStmt = new statement;
    newStmt->stmt = newCondStmt;
    newStmt->stmtType = COND; 
    newStmt->next = NULL;
    return newStmt;    

}

statement* create_WHILE(expr* exp, statement* stmt_list)
{
    while_stmt* newWhile = new while_stmt;
    newWhile->COND = exp;
    newWhile->stmtList = stmt_list;

    conditional_stmt* newCondStmt = new conditional_stmt;
    newCondStmt->cond_stmt_attrs = newWhile;
    newCondStmt->type = While;

    statement* newStmt = new statement;
    newStmt->stmt = newCondStmt;
    newStmt->stmtType = COND; 
    newStmt->next = NULL;
    return newStmt; 
}

statement* create_FOR (statement * startAssign, expr * condition, statement * updateAssign, statement * stmtList)
{
    struct for_loop* new_loop = (struct for_loop*)malloc(sizeof(struct for_loop));
    new_loop->COND = condition;
    new_loop->startAssign = (assignment_stmt *)(startAssign->stmt);
    new_loop->stmtList = stmtList;
    new_loop->updateAssign = (assignment_stmt *)(updateAssign->stmt);

    struct conditional_stmt* new_cond_stmt = (struct conditional_stmt*)malloc(sizeof(struct conditional_stmt));
    new_cond_stmt->type = For;
    new_cond_stmt->cond_stmt_attrs = new_loop;

    struct statement* new_stmt = (struct statement*)malloc(sizeof(struct statement));
    new_stmt->next = NULL;
    new_stmt->stmt=new_cond_stmt;
    new_stmt->stmtType = COND;
    return new_stmt;
}

//--------------------------------------------------//


/***------------------    End of Statement Functions   -----------------***/


/***-------------------     Source code 'Function Call' functions  ---------------***/

FunctionCall* makefunc_call(char* name, exprList* exprlist)
{
    struct FunctionCall* new_func = (struct FunctionCall*)malloc(sizeof(struct FunctionCall));
    new_func->exprlist= exprlist;
    new_func->funName = name;
    return new_func;
}


statement* makeFuncStmt(FunctionCall* funcCall)
{
    struct statement* new_stmt = (struct statement*)malloc(sizeof(struct statement));
    new_stmt->next=NULL;
    new_stmt->stmt=funcCall;
    new_stmt->stmtType=FUNC_CALL;
    return new_stmt;
}

//--------------------------------------------------//


/***------------------    End of 'Function Call' functions'   -----------------***/


/***-------------------     Main Block Functions  ---------------***/


Main* makeMainBlock(Datatype ret_type, Declare* local_decl, statement* stmt_list, expr* ret_stmt )
{
    Main* mainBlock = (struct Main*)(malloc(sizeof(struct Main)));
    mainBlock->local_declaration = local_decl;
    mainBlock->return_stmt = ret_stmt;
    mainBlock->return_type = ret_type;
    mainBlock->stmt_list = stmt_list;
    return mainBlock;
}

void addMainBlock(Main* mainBlock){
    code_start->mainBlock = mainBlock;
}

void addFdef_sec(FunctionDef * fdefSec){        // for function definitions in the global scope
    code_start->fdef_sec = fdefSec;
}

//----------------------------------------//




/***-----------------          Execution Functions    ------------***/

bool arguments_list_comp(arg_list* argList1, arg_list* argList2);

void addFunctionDef(FunctionDef* fdef)
{
    if(funcTable.count(fdef->funcName)<=0){
        cerr << "Function "<< fdef->funcName << " not declared* " << endl; syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
    else{
        aboutFunc * newFunDetails = funcTable[fdef->funcName];
        if(newFunDetails->isDefSet)
        {
            cerr<<"Multiple declarations of function \""<<newFunDetails->Func->name<<"\" found* "<<endl; syntax_flag = false;
            if(!semantic_flag)
                std::exit(1);
        }
        else{
            if(!arguments_list_comp(fdef->FargList, newFunDetails->Func->argList))
            {
                syntax_flag = false;
                if(!semantic_flag)
                    std::exit(1);
            }


            if(fdef->retType != newFunDetails->declType){
                cerr<<"Function \""<<fdef->funcName<<"\" has conflicting return types at definition and declaration."<<endl;
                if(!semantic_flag)
                    std::exit(1);
            }   
            
            newFunDetails->fdef = fdef;
            newFunDetails->isDefSet = true;
        }
    }
}

void addFunctionDefSec()
{
    FunctionDef* fdef = code_start->fdef_sec;
    while(fdef != NULL)
    {
        addFunctionDef(fdef);
        fdef = fdef->next;
    }
}

void start()
{
    cout << "\n**********-----------------  PRINTING OUTPUT  -----------------**********\n"<<endl;
    execGlobalDeclaration ();
    addFunctionDefSec();
    main_block_exec();
    cout<< "\n***********-----------------------------------------------------------***********"<<endl<<endl;
}

void insertFunctionDecl(func* f, Datatype type)
{
    if( funcTable.count(f->name)>0){
        cerr << "Multiple declarations of the function " << f->name <<endl; syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
    else{
        aboutFunc* newFunDetails = new aboutFunc;
        newFunDetails->declType = type;
        newFunDetails->Func = f;
        newFunDetails->fdef = NULL;
        newFunDetails->isDefSet = false;
        funcTable[f->name] = newFunDetails;
    }
}

void addDeclaration(map<string,value*>& symtable, Declare* decl)
{
    Glist* glist = decl->list;
    while(glist!=NULL)
    {
        if(glist->isGid)
        {
            VariableDeclaration* node = (VariableDeclaration*)(glist->node);
            if(!(node->isArray)){
                SymTableInsertion(node->name,decl->type,symtable);
            }
            else{
                SymTableInsertion(node->name,decl->type,node->index,symtable);
            }
        }
        else{
            func* node = (func*)(glist->node);
            insertFunctionDecl(node,decl->type);
        }
        glist = glist->next;
    }
}

void addDeclarationList (map<string,value*>& symtable, Declare* decl_list)
{
    while(decl_list!=NULL)
    {
        addDeclaration(symtable, decl_list);
        decl_list = decl_list->next;
    }
}

void execGlobalDeclaration ()
{
    addDeclarationList(symTable, code_start->global_declarations);
}

void executeWRITE (write_stmt * writeStmt,map<string,value *>& symtableLocal){
    
    // write_stmt * writeStmt = (write_stmt *)(stmt->stmt);
    if(writeStmt->isExpr)
    {
        evaluation* val = Calculate((expr*)(writeStmt->val),symtableLocal);
        if(!semantic_flag)
        {
            if(val->type == INT)
            {
                cout << "\t\t\t\t" << *(int*)(val->value) << endl;
            }
            else if (val->type == BOOL)
            {
                cout << "\t\t\t\t";
                printBool(*(bool*)(val->value));
                cout << endl;
            }
        }
    }
    else
    {
        strExp* list = (strExp*)(writeStmt->val);
        if(!semantic_flag)
            printStrExprList(list);
    }
}

void executeREAD(read_stmt* readStmt, map<string,value *>& symtableLocal)
{
    varExpr* var = readStmt->varexp;

    if(var->isArray)
    {
        indexList* newIndex = exprListToindexList(var->index,symtableLocal);
        evaluation* res = SymLookUp(var->varName,newIndex,symtableLocal);
        if(!semantic_flag)
        {
            switch(res->type)
            {
                case INT:   {
                        cin>>*(int *)(res->value);
                        updateVar(var->varName,newIndex,res,symtableLocal);
                }
                break;
                case BOOL:  {
                        cin >> boolalpha >>*(bool *)(res->value);
                        updateVar(var->varName,newIndex,res,symtableLocal);
                }
                break;
            }
        }
    }
    else{
        evaluation* res = SymLookUp(var->varName,symtableLocal);
        if(!semantic_flag)
        {
            switch(res->type)
            {
                case INT:   {
                        cin>>*(int *)(res->value);
                        updateVar(var->varName,res,symtableLocal);
                }
                break;
                case BOOL:  {
                        cin >> boolalpha >>*(bool *)(res->value);
                        updateVar(var->varName,res,symtableLocal);
                }
                break;
            }
        }
    }
}

void executeASSIGN (assignment_stmt* assignStmt, map<string,value*>& symtableLocal)
{
    evaluation* res = Calculate(assignStmt->exp, symtableLocal );
    varExpr* var = assignStmt->var;

    if(res == NULL)
    {
        cerr << "Invalid Expression on the left hand side of a assignment statement."<<endl;
        syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }

    if(var->isArray){

        updateVar(var->varName,exprListToindexList(var->index,symtableLocal),res,symtableLocal);
    }
    else{
        updateVar(var->varName,res,symtableLocal);
    }
}

void exec_IF(if_stmt* stmt,map<string,value*>& symtableLocal)
{
    evaluation* COND = Calculate(stmt->COND,symtableLocal);
    if(COND->type == BOOL)
    {
        if(*(bool*)(COND->value))
        {
            execStmtList(stmt->stmtList,symtableLocal);
        }
        delete COND;
    }
    else
    {
        // condtion should be of interger type as of now 
        cerr<<"If statement: Condition should of type bool. INT type condition not supported"<<endl;
        syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
}


void execIF_ELSE(ifelse_stmt* stmt,map<string,value*>& symtableLocal)
{
    evaluation* COND = Calculate(stmt->COND,symtableLocal);
    if(COND->type == BOOL)
    {
        if(*(bool*)(COND->value))
        {
            execStmtList(stmt->if_stmtList,symtableLocal);
        }
        else
        {
            execStmtList(stmt->else_stmtList,symtableLocal);
        }
        delete COND;
    }
    else
    {
        // condtion should be of interger type as of now 
        cerr<<"If Else statement: Condition should of type bool. INT type condition not supported"<<endl;
        syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
}

void execWHILE(while_stmt* stmt, map<string,value*>& symtableLocal)
{
    evaluation* COND = Calculate(stmt->COND,symtableLocal);
    if(COND->type == BOOL)
    {
        while(*(bool*)(COND->value))
        {
            execStmtList(stmt->stmtList,symtableLocal);
            delete COND;
            COND = Calculate(stmt->COND,symtableLocal);
            
            if(COND->type != BOOL)
            {
                cerr<<"While Stmt: Condition should of type bool. INT type condition not supported"<<endl;
                syntax_flag = false;
                if(!semantic_flag)
                    std::exit(1);
            }
            if(semantic_flag)
                break;
        }
    }
    else
    {
        // condtion should be of interger type as of now 
        cerr<<"While Stmt: Condition should of type bool. INT type condition not supported"<<endl;
        syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
    delete COND;
}

void execFOR (for_loop* stmt, map<string,value*>& symtableLocal)
{
    evaluation* res = Calculate(stmt->COND,symtableLocal);
    if(res->type!=BOOL)
    {
        cerr << "For Loop: Condition for a for loop should evaluate to bool. It is of type ";
        printType(res->type);
        cout<<endl;
        syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
    executeASSIGN(stmt->startAssign,symtableLocal);
    while(*(bool*)(res->value))
    {
       execStmtList(stmt->stmtList,symtableLocal);
       executeASSIGN(stmt->updateAssign,symtableLocal);

       delete res;
       res = Calculate(stmt->COND,symtableLocal);
       if(res->type!=BOOL)
       {
            cerr << "For Loop: Condition for a for loop should evaluate to bool. It is of type ";
            printType(res->type);
            cout<<endl;
            syntax_flag = false;
            if(!semantic_flag)
                std::exit(1);
       }
       if(semantic_flag)
            break;
    }
    delete res;
}

void execCOND (conditional_stmt* condStmt,map<string,value*>& symtableLocal)
{
    switch(condStmt->type)
    {
        case If: exec_IF ((if_stmt*)(condStmt->cond_stmt_attrs),symtableLocal); break;
        case IF_ELSE: execIF_ELSE ((ifelse_stmt*)(condStmt->cond_stmt_attrs),symtableLocal); break;
        case While: execWHILE ((while_stmt*)(condStmt->cond_stmt_attrs),symtableLocal); break;
        case For: execFOR((for_loop*)(condStmt->cond_stmt_attrs),symtableLocal); break;
        default:
                cerr << "Invalid conditional statement type." << endl;
                syntax_flag = false;
                if(!semantic_flag)
                    std::exit(1);
    }
}

int size_variable_list (var_list* list1)
{
    int i=0;
    for (var_list* list = list1; list!=NULL; list=list->next)
    {
        i++;
    }
    return i;
}

bool compare_arg(arg* arg1, arg* arg2)
{
    bool flag = true;
    if(arg1->type!=arg2->type)
    {
        cerr << "Argument type mismatch* "<<endl;
        flag = false;
    }
    if(size_variable_list(arg1->varList)!=size_variable_list(arg2->varList))
    {
        cerr<< "Arguments count mismatch* "<<endl;
        return false;
    }

    // argument name should be same, 

    for (var_list* l1 = arg1->varList,*l2 = arg2->varList; l1!=NULL && l2!=NULL;l1 = l1->next,l2 = l2->next)
    {
        if(strcmp(l1->name,l2->name)!=0)
        {
            cerr<<"Argument mismatch: " << l1->name << " and " << l2->name << "* " << endl;
            flag = false;
        }
    }

    return flag;
}

int size_argument_list(arg_list* argList1)
{
    int i=0;
    for (arg_list* list = argList1; list!=NULL; list = list->next)
    {
        i++;
    }
    return i;
}


bool arguments_list_comp (arg_list* argList1, arg_list* argList2)  // function that compares the argument list in the function definition and Declare
{
    bool flag = true;
    if(size_argument_list(argList1)!=size_argument_list(argList2) )
    {
        cerr<<"Argument list count mismatch* "<<endl;
        return false;
    }

    for (arg_list* arL1 = argList1,*arL2 = argList2; arL1!=NULL && arL2!=NULL ;arL1 = arL1 ->next,arL2 = arL2 ->next)
    {
        if(!compare_arg(arL1->Arg,arL2->Arg))
            flag = false;
    }
    return flag;
}

bool isPresent(char* name, map<string,value*> symtable)
{
    if(symtable.count(name)>0)
        return true;
    else
        return false;
}

void print_result_list(evaluation_list* reslist)
{
    for (evaluation_list* list = reslist;list!=NULL;list = list->next)
    {
        printResult(list->res);
        cout << ", ";
    }
    cout <<endl;
}

bool insertArg (arg* Arg, map<string,value*> & symtable)
{
    bool flag= true; 
    for (var_list* list = Arg->varList; list!=NULL;list = list->next)    
    {
        if(isPresent(list->name,symtable))
        {
            cerr<<"Duplicate arguments '"<<list->name<<"' in function definition* "<<endl ;
            flag = false;
        }
        else
            SymTableInsertion(list->name,Arg->type,symtable);
    }
    return flag;
}

bool insertArgList (arg_list* argList, map<string,value*> & symtable)
{
    for (arg_list* list = argList; list!=NULL; list = list->next)
    {
        if(!insertArg(list->Arg,symtable))
            return false;
    }
    return true;
}


evaluation_list* exprList_to_resultList (exprList* explist, map<string,value*> symtableLocal)
{
    evaluation_list* start=NULL, *end=NULL;
    for (exprList* list = explist; list !=NULL; list = list->next)
    {
        if(start==NULL)
        {
            start = new evaluation_list{Calculate(list->exp,symtableLocal),NULL};
            end = start;
        }
        else
        {
            end->next = new evaluation_list {Calculate(list->exp,symtableLocal),NULL};
            end = end->next;
        }
    }
    return start;
}

int list_len(evaluation_list* resultList)
{
    int i = 0;
    for (evaluation_list* list = resultList; list!=NULL; list = list->next)
    {
        i++;
    }
    return i;
}

int arg_nums (arg_list* arglist )
{
    int i=0;
    for (arg_list* list = arglist; list!=NULL; list = list->next)
    {
        i+= size_variable_list(list->Arg->varList);
    }
    return i;
}

bool function_args_comp(evaluation_list* actualParams, arg_list* arglist, map<string,value*> symtableLocal)
{
    int len = list_len(actualParams);
    if(len != arg_nums(arglist)){
        cout << "Formal and actual parameter count mismatch"<<endl;
        return false;
    }

    evaluation_list* start = actualParams;
    for (arg_list* arglst = arglist; arglst!=NULL; arglst = arglst->next)
    {
        int varListlen = size_variable_list(arglst->Arg->varList);
        for(int i =0; i<varListlen; i++)
        {
            if(start->res->type != arglst->Arg->type)
            {
                cout << "Formal and actual parameter type mismatch"<<endl;
                return false;
            }
            start = start->next;
        }
    }
    return true;
}


evaluation* executeFunctionCall (FunctionCall* functionCall,map<string,value*>& symtable_caller)
{
    if(funcTable.count(functionCall->funName)>0)
    {
        map<string,value*> local_symbol_table;
        aboutFunc* funcdetails = funcTable[functionCall->funName];
        if(funcdetails->isDefSet && funcdetails != NULL)
        {
            if(arguments_list_comp(funcdetails->fdef->FargList,funcdetails->Func->argList))
            {
                evaluation_list* actualParams = exprList_to_resultList(functionCall->exprlist, symtable_caller);
                if(!function_args_comp (actualParams, funcdetails->fdef->FargList, symtable_caller))
                {
                    cout<<"Actual and formal parameter for function '"<<functionCall->funName<<"' does not match* "<<endl;
                    std::exit(0);
                }
                if(insertArgList(funcdetails->Func->argList,local_symbol_table))
                {
                    evaluation_list* start = actualParams;
                    for (arg_list* _arg_list = funcdetails->Func->argList; _arg_list!=NULL ;_arg_list = _arg_list->next)
                    {
                        for (var_list*  _var_list = _arg_list->Arg->varList; _var_list!=NULL; _var_list = _var_list->next)
                        {
                            updateVar(_var_list->name,start->res,local_symbol_table);
                            start = start->next;
                        }
                    }

                    FunctionDef* fdef = funcdetails->fdef;
                    addDeclarationList(local_symbol_table,fdef->LdeclSec);
                    execStmtList(fdef->stmtList,local_symbol_table);
                    evaluation* res = Calculate(fdef->retStmt,local_symbol_table);
                    if(res->type == fdef->retType)
                        return res;
                    else{
                        cout<< "Type of return value and Declare return type mismatch for function '"<<fdef->funcName<<"'*" <<endl;
                        std::exit(0);
                        return NULL;
                    }
                }
                else{
                    cout<<"Argument insertion to local symbol table failed* "<<endl;
                    std::exit(0);
                }
            }
            else
            {
                cout << "Arguments are bad* "<<endl;
                std::exit(0);
            }
        }
        else
        {
            cout << "Func definition not found* "<<endl;
            std::exit(0);
        }
    }
    else
    {
        cerr <<"Function '"<< functionCall->funName <<"' not declared* "<<endl;
        std::exit(0);
    }
}
evaluation* function_call_check(FunctionCall* functionCall,map<string,value*>& symtable_caller);

void execStmt(statement* stmt,map<string,value*>& symtableLocal)
{
    switch(stmt->stmtType)
    {
        case Write: executeWRITE((write_stmt*)(stmt->stmt),symtableLocal); break;
        case Read:  executeREAD((read_stmt*)(stmt->stmt),symtableLocal); break;
        case assign: executeASSIGN((assignment_stmt*)(stmt->stmt),symtableLocal); break;
        case COND: execCOND((conditional_stmt*)(stmt->stmt),symtableLocal); break;
        case FUNC_CALL: 
                if(!semantic_flag)
                    executeFunctionCall((FunctionCall*)(stmt->stmt),symtableLocal);
                else
                    function_call_check((FunctionCall*)(stmt->stmt),symtableLocal);
                break;
        default: 
                cout<< "Invalid statement type";
                syntax_flag = false;
                if(!semantic_flag)
                    std::exit(1);
    }
}

void execStmtList(statement* stmt_list, map<string,value*>& symtableLocal)
{
    while(stmt_list!= NULL)
    {
        execStmt (stmt_list,symtableLocal);
        stmt_list=stmt_list->next;
    }
}

evaluation* main_block_exec()
{
    map<string,value*> symtableLocal;
    addDeclarationList(symtableLocal,code_start->mainBlock->local_declaration);
    evaluation* return_value = Calculate(code_start->mainBlock->return_stmt,symtableLocal);
    if(return_value->type != code_start->mainBlock->return_type)
    {
        cerr << "Return value and return type mismatch* "<<endl;
        syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
    delete return_value;

    execStmtList( code_start->mainBlock->stmt_list, symtableLocal);
    return_value = Calculate(code_start->mainBlock->return_stmt,symtableLocal);
    return return_value;
}


//------------------------//

/***--------------------     End of Execution Helper Functions    -----------------***/

/***--------------------     Source Code correctness checking    -----------------***/

evaluation* function_call_check(FunctionCall* functionCall,map<string,value*>& symtable_caller)  // Semantic checking of function call
{
    if(funcTable.count(functionCall->funName)>0)
    {
        map<string,value*> local_symbol_table;
        aboutFunc* funcdetails = funcTable[functionCall->funName];
        if(funcdetails->isDefSet && funcdetails != NULL)
        {
            evaluation_list* actualParams = exprList_to_resultList(functionCall->exprlist, symtable_caller);
            if(!function_args_comp (actualParams, funcdetails->fdef->FargList, symtable_caller))
            {
                cerr<<"Parameters conflict for function '" << functionCall->funName<<"' * "<<endl;
                syntax_flag =false;
            }
            if(insertArgList(funcdetails->Func->argList,local_symbol_table))
            {
                evaluation_list* start = actualParams;
                for (arg_list* _arg_list = funcdetails->Func->argList; _arg_list!=NULL ;_arg_list = _arg_list->next)
                {
                    for (var_list*  _var_list = _arg_list->Arg->varList; _var_list!=NULL; _var_list = _var_list->next)
                    {
                        updateVar(_var_list->name,start->res,local_symbol_table);
                        start = start->next;
                    }
                }
            }
            else{
                cerr <<"Multiple arguments in definition of function \""<<functionCall->funName<<"\"* "<<endl;
                syntax_flag = false;
            }
        }
        else
        {
            cerr << "Function definition of \""<<functionCall->funName<<"\" not found* "<<endl;
            syntax_flag = false;
        }
    }
    else
    {
        cerr <<"Function '"<< functionCall->funName <<"' not declared* "<<endl; syntax_flag = false;
    }
    return NULL;
}

void function_definition_check(char* funName)   // checks function arguments and definition
{
    if(funcTable.count(funName)>0)
    {
        map<string,value *> local_symbol_table;
        aboutFunc * funcdetails = funcTable[funName];
        if(funcdetails->isDefSet && funcdetails != NULL)
        {
            if(!arguments_list_comp(funcdetails->fdef->FargList,funcdetails->Func->argList))
            {
                cerr << "Arguments of the function \""<<funName<<"\" are not of same size or shape* "<<endl;syntax_flag = false;
            }

            if(!insertArgList(funcdetails->Func->argList,local_symbol_table))
            {
                cerr <<"Multiple argument in definition of function \""<<funName<<"\"* "<<endl;syntax_flag = false;
            }

            FunctionDef * fdef = funcdetails->fdef;
            addDeclarationList(local_symbol_table,fdef->LdeclSec);
            execStmtList(fdef->stmtList,local_symbol_table);
            evaluation * res = Calculate(fdef->retStmt,local_symbol_table);
            
            if(res->type != fdef->retType)
            {
                cerr << "Type of return value and Declare return type mismatch for function '"<<fdef->funcName<<"'* " <<endl;
                syntax_flag = false;
            }
        }
        else
        {
            cout << "Func definition not found. "<<endl;
            syntax_flag = false;
        }
    }
    else
    {
        cerr <<"Function '"<<funName <<"' not declared. "<<endl;
        syntax_flag = false;
    }
}

void function_decl_check(FunctionDef* fdef) // checks state of function def
{
    if(funcTable.count(fdef->funcName)<=0)
    {
        cerr << "No Declare of the function "<< fdef->funcName << " * " << endl; 
        syntax_flag = false;
        if(!semantic_flag)
            std::exit(1);
    }
    else
    {
        aboutFunc* newFunDetails = funcTable[fdef->funcName];
        if(newFunDetails->isDefSet)
        {
            cerr<<"Multiple definition of function \""<<newFunDetails->Func->name<<"\"* "<<endl;
            syntax_flag = false;
            if(!semantic_flag)
                std::exit(1);
        }
        else{
            newFunDetails->fdef = fdef;
            newFunDetails->isDefSet = true;
            function_definition_check(fdef->funcName);
        }
    }
}

void function_body_check()      // checks semantic correctness of function body
{
    FunctionDef * fdef = code_start->fdef_sec;
    while(fdef != NULL)
    {
        function_decl_check(fdef);
        fdef = fdef->next;
    }
}

void full_program_check()       // overall program check, including global content
{
    semantic_flag = true;
    execGlobalDeclaration ();
    function_body_check();
    main_block_exec();
    semantic_flag = false;
}

/***--------------------     All Print Functions    -----------------***/

/**
 *   (1)   To print all the expressions that come in the input code as part of AST
*/
void printExprList(exprList* start)
{
    while(start !=NULL)
    {
        cout<<"[";
        printExpression(start->exp);
        cout<<"]";
        start = start->next;
    }
}

void printIndexList(indexList* start)
{
    while (start != NULL)
    {
        cout<<"["<<start->i<<"]";
        start = start->next;
    }
}
void printLeftAndRight (struct expr* tree_root)
{
    printExpression(tree_root->left);
    printExpression(tree_root->right);
}
void printExpression (expr * tree_root){
    evaluation * curr = new evaluation ;
    
    exprType node = tree_root->type;
    Datatype nodeVal = tree_root->valueType;
    
    switch(node)
    {
        case LEAF:
                switch(nodeVal)
                {
                    case INT: cout<<"NUM "; break;
                    case BOOL:cout<<"BOOL ";break;
                    case Var: {
                        varExpr * varExpression = (varExpr * )(tree_root->attribute) ;
                        if(!(varExpression->isArray))
                        {
                            cout <<"VAR ";
                        }
                        else{
                            cout <<"ARRAY_ref VAR ";
                            printExprList(varExpression->index); 
                        }
                        }
                        break;
                    default: cout << "Invalid type for LEAF*" << endl; std::exit(0);
                }
                break;
        case InternalNODE: {
            Datatype op = ((OperatorSym*)(tree_root->attribute))->type;
            switch(op)
            {
                case BOOL:
                        switch (((OperatorSym*)(tree_root->attribute))->op)
                        {
                            case '&':
                                cout << "LOGICAL_AND ";
                                printLeftAndRight(tree_root);
                                break;
                            case '|':
                                cout << "LOGICAL_OR ";
                                printLeftAndRight(tree_root);
                                break;
                            case '!':
                                cout << "LOGICAL_NOT ";
                                printExpression(tree_root->right);
                                break;
                            default:
                                cout << "Invalid operator";
                                std::exit (1);
                        }
                        break;
                case INT: 
                        switch (((OperatorSym*)(tree_root->attribute))->op)
                        {
                            //operations that yield int itself
                            case '+':
                                cout << "PLUS ";
                                printLeftAndRight(tree_root);
                                break;
                            case '-':
                                cout << "SUB ";
                                printLeftAndRight(tree_root);
                                break;
                            case '/':
                                cout << "DIV ";
                                printLeftAndRight(tree_root);
                                break;
                            case '*':
                                cout << "MUL ";
                                printLeftAndRight(tree_root);
                                break;
                            case '%':
                                cout << "MOD ";
                                printLeftAndRight(tree_root);
                                break;
                            
                            //operation that yeild bool
                            case '>':
                                cout << "MORE ";
                                printLeftAndRight(tree_root);
                                break;
                            case '<':
                                cout << "LESS ";
                                printLeftAndRight(tree_root);
                                break;
                            case 'g':
                                cout << "GREATERTHANOREQUAL ";
                                printLeftAndRight(tree_root);
                                break;
                            case 'l':
                                cout << "LESSTHANOREQUAL ";
                                printLeftAndRight(tree_root);
                                break;
                            case 'n':
                                cout << "NOTEQUAL ";
                                printLeftAndRight(tree_root);
                                break;
                            case 'e':
                                cout << "EQUALEQUAL ";
                                printLeftAndRight(tree_root);
                                break;
                            default:
                                cout << "Invalid operator";
                                std::exit (1);
                        }
                        break;
                default:
                    cout << "Invalid Expr Node detected";
                    std::exit(0);
            }
            break;
        }
    }
}

// ------------------------------- //


/**
 *  (2) Printing Declared expression nodes (type and datatype and (array or variable)), function arguments and such
*/

void printDeclVarNodes (VariableDeclaration* list)
{
    VariableDeclaration* curr = list;
    if(curr == NULL)
    {
        cerr << "Ambigious Case. Error in code itself." << endl;
        std::exit(0);
    }
    if(curr->isArray)
    {
        cout << "ARR VAR";
        printIndexList(curr->index);
    }
    else{
        cout << "VAR";
    }
}

void printVarList(var_list* curr)
{
    while(curr!= NULL)
    {
        cout << "VAR";
        curr = curr->next;
        if(curr!= NULL)
            cout << ", ";
    }
}

void printType(Datatype type)
{
    switch(type)
    {
        case INT: cout << "integer"; break;
        case BOOL: cout << "boolean"; break;
    }
}

void printArg(arg* Arg)
{
    if(Arg->type == INT)
        cout << "INT";
    else if (Arg->type ==BOOL)
        cout << "BOOL";
    cout << " ";
    printVarList(Arg->varList);
}

void printArgList(arg_list* argList)
{
    while (argList!=NULL)
    {
        printArg(argList->Arg);
        argList = argList->next;
        if(argList != NULL)
            cout<< "; ";
    }
}

void printFunc(func* Func)
{
    cout << "FUNCVAR (";
    printArgList(Func->argList);
    cout << ")";
}

void printGlist(Glist* glist)
{
    while(glist != NULL)
    {
        if(glist->isGid)
            printDeclVarNodes((VariableDeclaration *)(glist->node));
        else
            printFunc((func*)(glist->node));
        glist = glist->next;
        if(glist != NULL)
            cout << ", ";
    }
}

void printDeclstmt(Declare* root)
{
    cout << "DECL ";
    switch(root->type)
    {
        case INT: cout << "INT "; break;
        case BOOL: cout << "BOOL "; break;
        default: cout <<"invalid type "; 
    }

    if(root->list->isGid)
    {
        printGlist(root->list);
    }
    else
    {
        cout << "*";
    }
    cout << endl;
}

void printGlobalDecl(Declare* root)
{
    Declare * ptr = root;
    while(ptr != NULL)
    {
        printDeclstmt(ptr);
        ptr = ptr->next;
    }
    if(root!=NULL)
        cout<<endl;
}


/**
 * (3) Printing all assignments
*/
void printAssignStmt(assignment_stmt* stmt)
{
    cout <<"ASSIGN ";
    if (stmt->var->isArray)
    {
        cout << "ARRAY_ref VAR ";
        
        printExprList(stmt->var->index);
    }
    else
    {
        cout <<"VAR ";
    }

    printExpression(stmt->exp);

}
/**
 * (4) Printing of String expressions
*/

void printStrExprList(strExp* list)
{
    if(list == NULL)
        cerr<<"List was NULL\n";
    while (list!=NULL)
    {
        if(list->var==NULL){
            cerr << "String is null";
        }
        cout<<list->var<<" ";
        list = list->next;
    }
    cout<<endl;
}

/**
 * (5) Printing of Write statements, arguments, function call and read statements
*/

void printWriteStmt(write_stmt* stmt)
{
    cout << "FUNC ";
    if(stmt->isExpr){
        printExpression((expr*)(stmt->val));
    }
    else 
    {
        printStrExprList((strExp*)(stmt->val));
    }
}

void printArgExprList(exprList* exprlist)
{
    for (exprList * list = exprlist; list!=NULL; list = list->next)
    {
        cout << "VAR ";
        if(list->next!= NULL)
            cout << ", ";
    }
}

void printFunCall(FunctionCall* stmt)
{
    cout << "FUNCVAR (";
    printArgExprList(stmt->exprlist);
    cout <<")";
}

void printReadStmt(read_stmt* stmt)
{
    cout <<"READ ";
    if (stmt->varexp->isArray)
    {
        cout << "ARRREF VAR ";
        
        printExprList(stmt->varexp->index);
    }
    else
    {
        cout <<"VAR ";
    }
}
/**
 * (6) Printing statement list and statements
*/

void printStmt(statement* stmt){
    switch(stmt->stmtType)
    {
        case Write: printWriteStmt((write_stmt*)(stmt->stmt)); break;
        case assign: printAssignStmt((assignment_stmt*)(stmt->stmt)); break;
        case COND: printCOND((conditional_stmt*)(stmt->stmt)); break;
        case FUNC_CALL: printFunCall((FunctionCall*)(stmt->stmt)); break;
        case Read: printReadStmt ((read_stmt*)(stmt->stmt)); break;
        default: cout << "invalid statement type* "<<endl;
    }
}
void printStmtList (statement * stmt){
    while (stmt!=NULL)
    {
        printStmt(stmt);
        stmt=stmt->next;
        cout<<endl;
    }
}
/**
 * (7) Condition and Loop statements
*/
void printIF(if_stmt* stmt)
{
    cout << "\nIF "; 
    printExpression(stmt->COND);
    cout << endl << " THEN" << endl; 
    printStmtList(stmt->stmtList);
    cout << "ENDIF" << endl;
}

void printIFELSE(ifelse_stmt* stmt)
{
    cout << "\nIF "; 
    printExpression(stmt->COND);
    cout <<" THEN " << endl; 
    printStmtList(stmt->if_stmtList);
    cout << "ELSE" << endl;
    printStmtList(stmt->else_stmtList);
    cout << "ENDIF" << endl;
}

void printWHILE(while_stmt* stmt)
{
    cout << "\nWHILE "; 
    printExpression(stmt->COND);
    cout << " DO " << endl; 
    printStmtList(stmt->stmtList);
    cout << "ENDWHILE" << endl;
}

void printFOR(for_loop* stmt)
{
    cout << "FOR ";
    printAssignStmt(stmt->startAssign);
    cout << ";";
    printExpression(stmt->COND);
    cout << ";";
    printAssignStmt(stmt->updateAssign);
    cout << endl;

    printStmtList(stmt->stmtList);
    cout << "END FOR"<<endl;
}


void printCOND(conditional_stmt* stmt)
{
    switch(stmt->type)
    {
        case If: printIF((if_stmt *)(stmt->cond_stmt_attrs)); break;
        case IF_ELSE: printIFELSE((ifelse_stmt *)(stmt->cond_stmt_attrs)); break;
        case While: printWHILE((while_stmt *)(stmt->cond_stmt_attrs)); break;
        default: printFOR((for_loop *)(stmt->cond_stmt_attrs));
    }
}
/**
 * Value types, and global symbol table
*/

void printBool(bool b)
{
    if(b)
        cout << "true ";
    else
        cout << "false ";
}

void printTypeForAST (Datatype type)
{
    switch(type)
    {
        case INT: cout<<"INT"; break;
        case BOOL: cout<<"BOOL"; break;
        default: cout<<"Invalid Type* " << endl; std::exit(0);
    }
}

void printMainBlock()
{
    Main* mainBlock = code_start->mainBlock;
    cout << "FUN ";
    printTypeForAST(mainBlock->return_type);
    cout << "MAIN "<<endl;
    printGlobalDecl(mainBlock->local_declaration);
    printStmtList(mainBlock->stmt_list);
    cout << "RET ";
    printExpression(mainBlock->return_stmt);
    cout << endl;
    cout << "END MAIN"<<endl;
}
void print_function_definition(FunctionDef* fdef)
{
    cout << "FUN ";
    printTypeForAST(fdef->retType);
    cout << " ";
    cout << fdef->funcName << " " << "(";
    printArgList(fdef->FargList);
    cout << ")" << endl;
    printGlobalDecl(fdef->LdeclSec);
    printStmtList(fdef->stmtList);
    cout << "RET ";
    printExpression(fdef->retStmt);
    cout << endl;
    cout << "END "<< fdef->funcName << endl << endl;
}

void print_function_body()
{
    FunctionDef* fdef = code_start->fdef_sec;
    while(fdef!=NULL)
    {
        print_function_definition(fdef);
        fdef = fdef->next;
    }
}

void print_program()
{   
    cout << "\n***********-------------- PRINTING AST --------------***********\n"<<endl;
    printGlobalDecl(code_start->global_declarations);
    print_function_body();
    printMainBlock();
    cout<< "____________________________________________________________________________\n" << endl;
}

void printResult(evaluation* val)
{
    switch(val->type)
    {
        case INT:   cout << *(int*)(val->value); break;
        case BOOL:  cout << *(bool*)(val->value);  break;
    }
}

/**
 * To print the generated C code from source code
*/
void print_new_code_type(Datatype type)
{
    if(type == INT)
        cout<<"int ";
    else if (type == BOOL)
        cout<<"bool ";
}

void print_new_code_variableL(var_list* varList, Datatype type)
{
    while(varList != NULL)
    {
        print_new_code_type(type);

        cout<< varList->name;
        varList = varList->next;
        if(varList != NULL)
        {
            cout<< ", ";
        }
    }
}

void print_new_code_arguments(arg* Arg)
{
    print_new_code_variableL(Arg->varList,Arg->type);
}


void print_new_code_argumentsList(arg_list* argList)
{
    while (argList != NULL)
    {
        print_new_code_arguments(argList->Arg);
        argList = argList->next;
        if(argList != NULL)
        {
            cout<< ", ";
        }
    }
}

void print_new_code_indexL(indexList* start)
{
    while (start != NULL)
    {
        cout<<"["<<start->i<<"]";
        start = start->next;
    }
}

void print_new_code_declarations_nodes(VariableDeclaration* list)
{
    VariableDeclaration* ptr = list;
    if(ptr == NULL)
    {
        cout<< "From printDeclNodes, we shouldn't be here"<<endl;
        std::exit(0);
    }
    if(ptr->isArray)
    {
        cout << ptr->name;
        print_new_code_indexL(ptr->index);
    }
    else
        cout << ptr->name;
}

void print_new_code_funcs(func* Func)
{
    cout<<Func->name<<" (";
    print_new_code_argumentsList(Func->argList);
    cout <<")";
}

void print_new_code_global_ents(Glist* glist)
{
    while(glist!=NULL)
    {
        if(glist->isGid)
        {
            print_new_code_declarations_nodes((VariableDeclaration*)(glist->node));
        }
        else
        {
            print_new_code_funcs((func * )(glist->node));
        }
        glist = glist->next;
        if(glist != NULL)
            cout << ", ";
    }
}

void print_new_code_decls(Declare* decl)
{
    print_new_code_type (decl->type);
    print_new_code_global_ents(decl->list);
    cout<<";";
    cout<<endl;
}


void print_new_code_declList(Declare* decl_list)
{
    for (Declare * start = decl_list; start!=NULL; start = start->next)
    {
        print_new_code_decls (start);
    }
}

void print_new_code_Expr(expr* tree_root);

void print_new_code_ExprIndexList(exprList* start)
{
    while(start !=NULL)
    {
        cout<<"[";
        print_new_code_Expr(start->exp);
        cout<<"]";
        start = start->next;
    }
}

void print_new_code_FunCall(FunctionCall* stmt, int flag = 1);

void print_new_code_Expr(expr* tree_root)
{
    switch(tree_root->type)
    {
        case LEAF:
                switch(tree_root->valueType)
                {
                    case INT:   cout << ((intExp*)(tree_root->attribute))->val; break;
                    case BOOL: cout << boolalpha << ((boolExpr*)(tree_root->attribute))->val; break;
                    case Var: {
                            varExpr* varE = (varExpr*)(tree_root->attribute) ;
                            if(!(varE->isArray))
                            {
                                cout<<varE->varName;
                            }
                            else{
                                cout<<varE->varName;
                                print_new_code_ExprIndexList(varE->index);
                            }
                    } 
                    break;
                    default: cout <<"invalid Type for a LEAF* "; std::exit(0);
                }
                break;
        case FuncCall: print_new_code_FunCall((FunctionCall*)(tree_root->attribute),0); break;
        case InternalNODE: 
                    switch(((OperatorSym*)(tree_root->attribute))->type)
                    {
                        case BOOL: 
                            switch (((OperatorSym*)(tree_root->attribute))->op)
                            {
                                case '&':
                                    print_new_code_Expr(tree_root->left);
                                    cout << " && ";
                                    print_new_code_Expr(tree_root->right);
                                    break;
                                case '|':
                                    print_new_code_Expr(tree_root->left);
                                    cout << " || ";
                                    print_new_code_Expr(tree_root->right);
                                    break;
                                case '!':
                                    cout << "! ";
                                    print_new_code_Expr(tree_root->right);
                                    break;
                                default:
                                    cout << "Invalid operator";
                                    exit (1);
                            }
                            break;
                        case INT:
                                switch (((OperatorSym*)(tree_root->attribute))->op)
                                {
                                    case '+':
                                        print_new_code_Expr(tree_root->left);
                                        cout << "+";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case '-':
                                        print_new_code_Expr(tree_root->left);
                                        cout << "-";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case '/':
                                        print_new_code_Expr(tree_root->left);
                                        cout << "/";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case '*':
                                        print_new_code_Expr(tree_root->left);
                                        cout << "*";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case '%':
                                        print_new_code_Expr(tree_root->left);
                                        cout << " % ";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    
                                    case '>':
                                        print_new_code_Expr(tree_root->left);
                                        cout << " > ";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case '<':
                                        print_new_code_Expr(tree_root->left);
                                        cout << " < ";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case 'g':
                                        print_new_code_Expr(tree_root->left);
                                        cout << " >= ";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case 'l':
                                        print_new_code_Expr(tree_root->left);
                                        cout << " <= ";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case 'n':
                                        print_new_code_Expr(tree_root->left);
                                        cout << " != ";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    case 'e':
                                        print_new_code_Expr(tree_root->left);
                                        cout << " == ";
                                        print_new_code_Expr(tree_root->right);
                                        break;
                                    default:
                                        cout << "Invalid operator";
                                        exit (1);
                                }
                                break;
                        default: cout << "Invalid Expr Node detected* "; std::exit(0);
                    }
                    break;
    }
}

void print_new_code_StrExprList(strExp* list)
{
    if(list == NULL)
        cout<<"List was NULL\n";
    while (list!=NULL)
    {
        if(list->var== NULL){
            cout << "string is null";
        }
        cout<<list->var<<" ";
        list=list->next;
    }
}



void print_new_code_WriteStmt(write_stmt* stmt)
{
    if(stmt->isExpr){
    cout << "printf(\"\%d \", ";
        print_new_code_Expr((expr *)(stmt->val));
    }
    else 
    {
        cout<<"printf(\"";
        print_new_code_StrExprList((strExp*)(stmt->val));
        cout <<"\"";
    }
    cout << ");";
}

void print_new_code_AssignStmt(assignment_stmt* stmt, int flag = 1)
{
    if(stmt->var->isArray)
    {
        cout << stmt->var->varName;        
        print_new_code_ExprIndexList(stmt->var->index);
    }
    else
        cout << stmt->var->varName;

    cout << " = ";
    print_new_code_Expr(stmt->exp);
    if(flag==1)
        cout << ";";
}

void print_new_code_StmtList(statement* stmt);

void print_new_code_IfStmt(if_stmt* stmt)
{
    cout << "if("; 
    print_new_code_Expr(stmt->COND);
    cout << ") "<<endl;

    cout << "{" <<endl;
    print_new_code_StmtList(stmt->stmtList);
    cout << "}" <<endl;
}

void print_new_code_IfElseStmt(ifelse_stmt* stmt)
{
    cout << "if("; 
    print_new_code_Expr(stmt->COND);
    cout << ") "<<endl;

    cout << "{" <<endl;
    print_new_code_StmtList(stmt->if_stmtList);
    cout << "}" <<endl;
    
    cout << "else "<<endl;
    cout << "{" <<endl;
    print_new_code_StmtList(stmt->else_stmtList);
    cout << "}" <<endl;
}

void print_new_code_WhileStmt(while_stmt* stmt)
{
    cout << "while("; 
    print_new_code_Expr( stmt->COND);
    cout << ")";

    cout << "{ " << endl; 
    print_new_code_StmtList(stmt->stmtList);
    cout << "}"<<endl;

}

void print_new_code_For_loop(for_loop* stmt)
{
    cout << "for(";
    print_new_code_AssignStmt (stmt->startAssign,0);
    cout << ";";
    print_new_code_Expr(stmt->COND);
    cout << ";";
    print_new_code_AssignStmt (stmt->updateAssign,0);
    cout << ")" <<endl;

    cout << "{ " << endl; 
    print_new_code_StmtList(stmt->stmtList);
    cout << "}"<<endl;
}

void print_new_code_CondStmt(conditional_stmt* stmt)
{
    switch(stmt->type)
    {
        case If: print_new_code_IfStmt((if_stmt*)(stmt->cond_stmt_attrs)); break;
        case IF_ELSE: print_new_code_IfElseStmt((ifelse_stmt*)(stmt->cond_stmt_attrs)); break;
        case While: print_new_code_WhileStmt((while_stmt*)(stmt->cond_stmt_attrs)); break;
        case For: print_new_code_For_loop((for_loop*)(stmt->cond_stmt_attrs)); break;
        default: cout << "Invalid type of conditional statement* " << endl;
    }
}

void print_new_code_ExprList(exprList* exprlist)
{
    for (exprList* list = exprlist;list!=NULL;list=list->next)
    {
        print_new_code_Expr(list->exp);
        if(list->next!= NULL)
            cout << ", ";
    }
}
 
void print_new_code_FunCall(FunctionCall* stmt, int flag)
{
    cout<<stmt->funName<<"(";
    print_new_code_ExprList (stmt->exprlist);
    if(flag==1)
        cout << ");";
    else
        cout << ") ";
}

void print_new_code_ReadStmt(read_stmt* stmt)
{
    cout << "scanf(\"\%d\",&";
    if (stmt->varexp->isArray)
    {
        cout << stmt->varexp->varName;        
        print_new_code_ExprIndexList(stmt->varexp->index);
    }
    else
        cout << stmt->varexp->varName;

    cout<<");";

}

void print_new_code_Stmt (statement* stmt){
    switch(stmt->stmtType)
    {
        case Write: print_new_code_WriteStmt((write_stmt*)(stmt->stmt)); break;
        case assign: print_new_code_AssignStmt((assignment_stmt*)(stmt->stmt)); break;
        case COND: print_new_code_CondStmt((conditional_stmt*)(stmt->stmt)); break;
        case FUNC_CALL: print_new_code_FunCall((FunctionCall*)(stmt->stmt)); break;
        case Read: print_new_code_ReadStmt ((read_stmt*)(stmt->stmt)); break;
        default: cout << "invalid statement type* " << endl;
    }
}

void print_new_code_StmtList (statement * stmt){
    
    while (stmt!=NULL)
    {
        print_new_code_Stmt(stmt);
        stmt=stmt->next;
        cout<<endl;
    }
}

void print_new_code_Fdef(FunctionDef * fdef)
{
    print_new_code_type(fdef->retType);
    cout<<fdef->funcName<<" ";
    cout<<"(";
    print_new_code_argumentsList(fdef->FargList);
    cout<<")"<<endl;

    cout << "{"<<endl;
    print_new_code_declList (fdef->LdeclSec);
    print_new_code_StmtList(fdef->stmtList);
    
    cout<< "return ";
    print_new_code_Expr(fdef->retStmt);
    cout<<"; "<< endl;

    cout<< "} "<<endl;
}

void print_new_code_FDefSec()
{
    FunctionDef * fdef = code_start->fdef_sec;
    while(fdef !=NULL)
    {
        print_new_code_Fdef(fdef);
        fdef = fdef->next;
    }
}

void print_new_code_MainBlock()
{
    Main * mainBlock = code_start->mainBlock;
    print_new_code_type(mainBlock->return_type);
    cout<< "main ()"<<endl;   
    cout<< "{"<<endl;
    print_new_code_declList(mainBlock->local_declaration);
    print_new_code_StmtList(mainBlock->stmt_list);
    cout<< "return ";
    print_new_code_Expr(mainBlock->return_stmt);
    cout<<"; "<< endl;
    cout<< "} "<<endl;
}


void print_new_code_code()
{
    syntax_flag = true;
    full_program_check();
    
    if(syntax_flag)
    {
        cout << "***********-------------- PRINTING GENERATED C CODE --------------***********" << endl;
        cout <<"#include <stdio.h>"<<endl;
        cout <<"#include <stdbool.h>"<<endl;
        print_new_code_declList(code_start->global_declarations);
        print_new_code_FDefSec();
        print_new_code_MainBlock ();
        cout << "____________________________________________________________________________" << endl;
    }
}

void PrgBeginMessage()
{
    cout << "\n\t\t  ------ START OF PROGRAM -----\n" << endl;
}

void PrgEndMessage()
{
    cout << "\n\t\t  ------ END OF PROGRAM -----\n" << endl;
}
