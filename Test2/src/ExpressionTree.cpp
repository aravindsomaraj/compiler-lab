#include "../include/ExpressionTree.hpp"
#include <bits/stdc++.h>
using namespace std;

map<string,value*> symTable;
struct Program* code_start;


void printExpression (expr * tree_root);
void printStrExprList (strExp * list); 
void printBool (bool b);

void SymTableInsertion (char* name, Datatype type, map<string,value* > & symtable = symTable);
void SymTableInsertion (char* name,Datatype type, int a, map<string,value* > & symtable = symTable);

evaluation* SymLookUp (char* name, map<string,value*> & symtable = symTable);
evaluation* SymLookUp (char* name, int index, map<string,value*> & symtable = symTable);


void updateVar (char* name, int val, map<string,value*> & symtable = symTable);
void updateVar (char* name, int index , int val, map<string,value*> & symtable = symTable);

void executeStatements (statement * stmt_list);

/*---------------------------------------------------------------------------------------------------------------------------*/

/*** -------      Symbol Table Functions ---------------*/

void SymTableInsertion (char* name, Datatype type,map<string,value*>&symtable){
    if(symtable.count(name) > 0){
        cout << "Multiple Declare of the variable "<<name<<endl; std::exit(1);
    }
    value * curr = DeclareNewVal(name);
    curr->type = type;
    symtable[name] = curr;
}

void SymTableInsertion (char * name,Datatype type, int index, map<string,value*>& symtable){
    if(symtable.count(name)>0){
        cout << "Multiple Declare of the variable "<<name<<endl; std::exit(1);
    }
    value * curr = DeclareNewVal(name,index);
    curr->type = type;
    symtable[name] = curr;
}

evaluation* SymLookUp (char* name, map<string,value*> & symtable ){
    if(symtable.count(name) <= 0){
        cout << "Undeclared Var - "<< name << endl; std::exit(1);
    }
    value * curr = symtable[name];
    evaluation * result = new evaluation;
    if(curr->type == INT){
        if(curr->isArray){
            cout<< name <<" : Array found. Index required*" << endl;
            std::exit(1);
        }
        else{
            result->type = INT;
            result->value = (int *)(curr->val);
            return result;
        }
    }
    return NULL;
}

evaluation * SymLookUp (char * name, int index, map<string,value * > & symtable ){
    if(symtable.count(name) <= 0){
        cout << "Undeclared Var - "<< name << endl; std::exit(1);
    }
    value * curr = symtable[name];
    evaluation * result = new evaluation;
    if(curr->type == INT){
        if(curr->isArray==false){
            cout<< name <<" : not an array, therefore not indexable."<<endl;
            std::exit(1);
        }
        else{
            Array* arr = (Array*)(curr->val);
            if(index >= arr->size){
                cerr<< "Index out of range for array "<< name <<" "<<endl;
                std::exit(0);
            }
            else{
                result->type = INT;
                result->value = &(((int *)(arr->arr))[index]);
                return result;
            }
        }
    }
    return NULL;
}

void updateVar (char* name, int val, map<string,value*>& symtable ){
    if(symtable.count(name) <= 0){
        cout << "Undeclared Var - "<< name<<endl; std::exit(1);
    }
    value * curr = symtable[name];
    if(curr->type == INT){
        if(curr->isArray){
            cout<<name<<" - Array found. Index required. *"<<endl;
            std::exit(1);
        }
        else{
            *((int *)(curr->val)) = val;
        }
    }
}

void updateVar (char* name, int index , int val, map<string,value*>& symtable ){
    if(symtable.count(name) <= 0){
        cout << "Undeclared Var - "<< name <<endl; std::exit(1);
    }
    value * curr = symtable[name];
    if(curr->type == INT){
        if((curr->isArray)==false){
            cout<<name<<" : not an array, therefore not indexable."<<endl; std::exit(1);
        }
        else{
            Array* arr = (Array*)(curr->val);
            if(index >= arr->size){
                cerr<< "Index out of range for array "<< name <<" "<<endl; std::exit(0);
            }
            else{
                ((int* )(arr->arr))[index] = val;
            }
        }
    }
}

/*** -------------------  End of Symbol Table Functions --------  ****/


///------------------------------------------------------------------------------///


/**** ------------------------     Syntax Tree Expression Functions  -------------------****/


struct expr* TerminalExp (int value){ // Create a new LEAF expression node with a int value   
    // Creating an integer expression with required value
    intExp * curr = new intExp;
    curr->type = INT;
    curr->val = value;
    
    // Creating a tree node to store the above integer expression
    expr * newCurr = new expr;
    newCurr->attribute = curr;
    newCurr->left = NULL;
    newCurr->right = NULL;
    newCurr->type = LEAF;
    newCurr->valueType = INT;
    
    return newCurr;
}

struct expr* TerminalExp (bool val){    // Create a new LEAF expression node with a bool value 
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

struct varExpr* createVal(char* name)   // Create a new variable expression
{
    varExpr* curr = new varExpr;
    curr->type = none;
    curr->varName = strdup(name);
    curr->isArray = false;
    curr->index = NULL;
    return curr;
}

struct varExpr* createVal (char* name, expr* index) // Create a new variable expression, with index for array
{
    varExpr * curr = new varExpr;
    curr->type = none;
    curr->varName = strdup(name);
    curr->isArray = true;
    curr->index = index;
    return curr;
}

struct expr* TerminalExp (varExpr* var)     // Create a new variable expression node for syntax tree
{
    expr* newCurr = new expr;
    newCurr->attribute = var;
    newCurr->left = NULL;
    newCurr->right = NULL;
    newCurr->type = LEAF;
    newCurr->valueType = Var;

    return newCurr;
}

Datatype opType (char op)       // Used to find out the datatype of the operands used, via the operator
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
            cout << "Invalid Operator";
            std::exit (0);
    }
    return none;
}

struct expr* addNonTerminalExpr (expr* left, char op ,expr* right)  // Create a new non terminal expression node for syntax tree
{
    OperatorSym* curr = new OperatorSym;
    curr->type = opType(op);
    curr->op = op;

    expr* newCurr = new expr;
    newCurr->attribute = curr;
    newCurr->left = left;
    newCurr->right = right;
    newCurr->type = InternalNODE;
    newCurr->valueType = none;

    return newCurr;
}

struct evaluation* Calculate (expr* tree_root)  // Function for syntax tree evaluation
{
    evaluation* curr = new evaluation;
    switch(tree_root->type)
    {
        case LEAF:
            switch(tree_root->valueType)
            {
                case INT:
                            curr->type = ((intExp*)(tree_root->attribute))->type;
                            curr->value = &((intExp*)(tree_root->attribute))->val;
                            break;
                case BOOL:  curr->type = ((boolExpr*)(tree_root->attribute))->type;
                            curr->value = &((boolExpr*)(tree_root->attribute))->val;
                            break;
                case Var:   {varExpr* varExpression = (varExpr*)(tree_root->attribute);
                            if(!(varExpression->isArray))
                            {
                                evaluation* val = SymLookUp(varExpression->varName);
                                curr = val;
                            }
                            else{
                                evaluation* i = Calculate (varExpression->index);
                                if(i->type==INT)
                                {
                                    evaluation* val = SymLookUp(varExpression->varName,*((int *)(i->value)));
                                    curr = val;
                                }
                                else{
                                    cout << "Index of array contains bool*"<<endl;
                                    std::exit(1);
                                }
                            }
                            }
                            break;
                default:    cout <<"Invalid Type for a LEAF*" << endl;
                            std::exit(0);
            }
            break;
        case InternalNODE: {
                struct OperatorSym* currOp = (OperatorSym*)(tree_root->attribute);
                Datatype currType = currOp->type;
                switch(currType)
                {
                    case BOOL: {
                        char OP = currOp->op;
                        if(OP =='&' || OP == '|' || OP == '!')
                        {
                            bool Left = *(bool*)(Calculate(tree_root->left)->value);
                            bool Right= *(bool*)(Calculate(tree_root->right)->value);
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
                        else
                        {
                            cout << "Invalid operator";
                            std::exit(1);
                        }
                    } 
                    case INT: {
                        char OP = currOp->op;
                        if(OP=='+' || OP =='-' || OP =='/' || OP =='*' || OP =='%')
                        {
                            int Left = *(int*)(Calculate(tree_root->left)->value);
                            int Right = *(int*)(Calculate(tree_root->right)->value);
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
                            int Left = *(int*)(Calculate(tree_root->left)->value);
                            int Right= *(int*)(Calculate(tree_root->right)->value);
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
                            cout << "Invalid operator";
                            std::exit(1);
                        }
                        break;
                    }
                    default:
                        cout << "Invalid Expr Node detected";
                        std::exit(0);
                }
            }
    }
    return curr;
}


/*** -------------------  End of Syntax Tree Functions --------  ****/



/*---------------------------------------*/


/***   ---------------  Declaration Functions  -------------   ****/



value* DeclareNewVal (char* name)       // Declaring a new variable 
{
    value* curr = new value;
    curr->type = INT;
    curr->val = new int(0);
    curr->isArray = false;
    curr->name = strdup(name);
    return curr;
}
value* DeclareNewVal (char* name, int maxSize)  // Declaring a new variable array
{
    value * curr = new value;
    curr->type = INT;
    
    Array * arr = new Array;
    arr->arr = new int[maxSize]();
    arr->size = maxSize;
    
    curr->val = arr;
    curr->isArray = true;
    curr->name = strdup(name);

    return curr;   
}

VariableDeclaration * DeclareNewNode (char * name ) // Creation of declaration node for the variable declared
{
    value * var = DeclareNewVal (name);
    
    VariableDeclaration * newCurr = new VariableDeclaration;
    newCurr -> var = var;
    newCurr -> next = NULL;
    return newCurr;
} 

VariableDeclaration * DeclareNewNode (char * name, int size ) // Creation of declaration node for the variable array declared
{
    value * var = DeclareNewVal (name,size);

    VariableDeclaration * newCurr = new VariableDeclaration;
    newCurr -> var = var;
    newCurr -> next = NULL;
    return newCurr;
}

VariableDeclaration * makeDeclList (VariableDeclaration * tree_root, VariableDeclaration * lst )  // Creation of Declaration List
{
    tree_root -> next = lst;
    return tree_root;
}

Declare * makeDeclaration (Datatype type,VariableDeclaration * top)
{
    Declare * newCurr = new Declare;
    
    newCurr -> type = type;
    newCurr -> list = top;
    newCurr -> next = NULL;
    
    return newCurr;
}

Declare * makeDeclarationList (Declare * tree_root, Declare * list)
{
    tree_root->next = list;
    return tree_root;
}

void Declare_block (Declare * root)
{
    code_start = (struct Program*)(malloc(sizeof(struct Program)));
    code_start->global_declarations = root;
}




/*** -------------------  End of Declaration Functions --------  ****/


/*---------------------------------------*/


/*** -------------------  Statement List Functions --------  ****/


// -------------      Assignment functions -----------------------//

statement * createAssignment (varExpr * var, expr * exp)
{
    assignment_stmt * newCurr = new assignment_stmt;
    newCurr->var = var;
    newCurr->exp = exp;

    statement * newStmt = new statement;
    newStmt->stmt = newCurr;
    newStmt->stmtType = assign; 
    newStmt->next = NULL;
    return newStmt;
}



///------------ Making string expressions ----------------------------------///


strExp * StringExp (char * name)
{
    strExp * newCurr = new strExp;
    newCurr->var = strdup(name);
    newCurr->next = NULL;
    return newCurr;
}


strExp * StringExp (char * name, strExp * exp)
{
    strExp * newCurr = new strExp;
    newCurr->var = strdup(name);
    newCurr->next = exp;
    return newCurr;
}



//---------------------              write functions    ------------------------//


statement * createWrite (expr * expr)
{
    write_stmt * newCurr = new write_stmt;
    newCurr->val = expr;
    newCurr->isExpr = true;
    
    statement * newStmt = new statement;
    newStmt->stmt = newCurr;
    newStmt->stmtType = Write; 
    newStmt->next = NULL;
    return newStmt;
}

statement * createWrite (strExp * strExpr)
{    
    write_stmt * newCurr = new write_stmt;
    newCurr->val = strExpr;
    newCurr->isExpr = false;
    
    statement * newStmt = new statement;
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

statement * create_IF (expr * cond_expr, statement * stmt_list)
{
    if_stmt* newIf = new if_stmt;
    newIf->COND = cond_expr;
    newIf->stmtList = stmt_list;

    conditional_stmt * newCondStmt = new conditional_stmt;
    newCondStmt->attributes = newIf;
    newCondStmt->type = If;

    statement * newStmt = new statement;
    newStmt->stmt = newCondStmt;
    newStmt->stmtType = COND; 
    newStmt->next = NULL;
    return newStmt;    

}

statement * create_IFELSE (expr * exp, statement * if_stmt_list, statement * else_stmt_list)
{
    ifelse_stmt * newIfElse = new ifelse_stmt;
    newIfElse->COND = exp;
    newIfElse->if_stmtList = if_stmt_list;
    newIfElse->else_stmtList = else_stmt_list;

    conditional_stmt * newCondStmt = new conditional_stmt;
    newCondStmt->attributes = newIfElse;
    newCondStmt->type = IF_ELSE;

    statement * newStmt = new statement;
    newStmt->stmt = newCondStmt;
    newStmt->stmtType = COND; 
    newStmt->next = NULL;
    return newStmt;    

}

statement * create_WHILE (expr * exp, statement * stmt_list)
{
    while_stmt * newWhile = new while_stmt;
    newWhile->COND = exp;
    newWhile->stmtList = stmt_list;

    conditional_stmt * newCondStmt = new conditional_stmt;
    newCondStmt->attributes = newWhile;
    newCondStmt->type = While;

    statement * newStmt = new statement;
    newStmt->stmt = newCondStmt;
    newStmt->stmtType = COND; 
    newStmt->next = NULL;
    return newStmt; 
}

//--------------------------------------------------//


/***------------------    End of Statement Functions   -----------------***/


/***-------------------     Main Block Functions  ---------------***/


Main * makeMainBlock (Datatype ret_type, Declare * local_decl, statement * stmt_list, expr * ret_stmt )
{
    Main* mainBlock = (struct Main*)(malloc(sizeof(struct Main)));
    mainBlock->local_declaration = local_decl;
    mainBlock->return_stmt = ret_stmt;
    mainBlock->return_type = ret_type;
    mainBlock->stmt_list = stmt_list;
    return mainBlock;
}

void addMainBlock (Main * mainBlock)
{
    code_start->mainBlock = mainBlock;
}

//----------------------------------------//




/***-----------------          Execution Functions    ------------***/

void start()
{

    cout << "\n**********-----------------  PRINTING OUTPUT  -----------------**********\n"<<endl;
    start_global_exec ();
    start_main_exec();
    cout<< "\n***********-----------------------------------------------------------***********"<<endl<<endl;

}

void addDeclaration (map<string,value * > & symtable, Declare * decl)
{
    VariableDeclaration * tree_root = decl->list;
    while(tree_root !=NULL)
    {
        if(!(tree_root->var->isArray)){
            SymTableInsertion(tree_root->var->name,decl->type);
        }
        else{
            SymTableInsertion(tree_root->var->name,decl->type,((Array*)(tree_root->var->val))->size);
        }
        tree_root = tree_root->next;
    }
}

void addDeclarationList (map<string,value * > & symtable, Declare * decl_list)
{
    while(decl_list!=NULL)
    {
        addDeclaration(symtable, decl_list);
        decl_list = decl_list->next;
    }
}

void start_global_exec ()
{
    addDeclarationList(symTable, code_start->global_declarations );
}

//----------------------------------------//




/***-----------------      Statement specific Execution Functions    ------------***/

void executeWRITE (write_stmt * writeStmt){
    
    if(writeStmt->isExpr)
    {
        evaluation * val = Calculate((expr *)(writeStmt->val));
        if(val->type == INT)
        {
            cout << "\t\t\t\t" << *(int*)(val->value) << endl;
        }
        else if (val->type == BOOL)
        {
            cout << "\t\t\t\t";
            printBool( *(int*)(val->value) );
            cout << endl;
        }
    }
    else
    {
        strExp * list = (strExp *) (writeStmt->val);
        printStrExprList(list);
    }
}

void executeASSIGN (assignment_stmt * assignStmt)
{
    evaluation * result = Calculate(assignStmt->exp);
    varExpr * var = assignStmt->var;

    if(var->isArray){
        evaluation * index = Calculate(var->index);
        if(index->type == INT)
            updateVar(var->varName,*((int *)(index->value)),*((int *)(result->value)),symTable);
        else
        {
            cout << "Index should be INT type. BOOL type found"<<endl;
            std::exit(1);
        }
    }
    else{
        updateVar(var->varName,*((int *)(result->value)),symTable);
    }

}

void execIF_ELSE (ifelse_stmt * stmt)
{
    evaluation * COND = Calculate(stmt->COND);
    if(COND->type == BOOL)
    {
        if(*(bool*)(COND->value))
        {
            executeStatements(stmt->if_stmtList);
        }
        else
        {
            executeStatements(stmt->else_stmtList);
        }
    }
    else
    {
        cout<<"Condition should of type bool. INT type condition not supported"<<endl;
        std::exit(1);
    }
}

void execWHILE (while_stmt * stmt)
{
    evaluation * COND = Calculate(stmt->COND);
    if(COND->type == BOOL)
    {
        while(*(bool*)(COND->value))
        {
            executeStatements(stmt->stmtList);
            delete COND;
            COND = Calculate(stmt->COND);
            
            if(COND->type != BOOL)
            {
                cout<<"Condition should of type bool. INT type condition not supported"<<endl;
                std::exit(1);
            }
        }
    }
    else
    {
        cout<<"Condition should of type bool. INT type condition not supported"<<endl;
        std::exit(1);
    }
    delete COND;
    
}

void execCOND (conditional_stmt * condStmt)
{
    if(condStmt->type == IF_ELSE)
    {
        execIF_ELSE ((ifelse_stmt * )(condStmt->attributes));
    }
    else if (condStmt->type == While)
    {
        execWHILE ((while_stmt*)(condStmt->attributes));
    }
    else
    {
        cout << "Invalid conditional statement type."<<endl;
        std::exit(1);
    }
}

void execStatement (statement * stmt)
{
    if(stmt->stmtType == Write)
    {
        executeWRITE((write_stmt * )(stmt->stmt));

    }
    else if(stmt->stmtType == assign)
    {
        executeASSIGN((assignment_stmt * )(stmt->stmt));
    }
    else if(stmt->stmtType == COND)
    {
        execCOND((conditional_stmt * )(stmt->stmt));
    }
    else
    {
        cout<< "Invalid statement type";
        std::exit(1);
    }
}

void executeStatements (statement * stmt_list)
{
    while (stmt_list != NULL)
    {
        execStatement (stmt_list);
        stmt_list = stmt_list->next;
    }

}

void start_main_exec()
{
    executeStatements(code_start->mainBlock->stmt_list);
}


//------------------------//


/***--------------------     All Print Functions    -----------------***/

/**
 *   (1)   To print all the expressions that come in the input code as part of AST
*/

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
                            cout <<"ARRAY_REF VAR ";
                            printExpression(varExpression->index);
                        }
                        }
                        break;
                    default: cout << "Invalid type for LEAF*" << endl;
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
 *  (2) Printing Declared expression nodes (type and datatype and (array or variable)), 
*/

void printDeclVarNodes (VariableDeclaration * list)
{
    VariableDeclaration * curr = list;
    while (curr != NULL)
    {
        if(curr->var->isArray)
        {
            int i = ((Array *)(curr->var->val))->size;
            cout << "ARR VAR "<<i;
        }
        else{
            cout<<"VAR";
        }
        curr = curr->next;
        if(curr != NULL )
            cout<<", ";
    }
}

void printDeclstmt(Declare * root)
{
    cout << "DECL ";
    if (root-> type == INT)
    {
        cout << "INT ";
    }
    else if (root->type == BOOL)
    {
        cout << "BOOL ";
    }
    else 
    {
        cout <<"invalid type ";
    }
    printDeclVarNodes (root->list);
    cout <<endl;
}

void printGlobalDecl (Declare * root)
{
    Declare * curr = root;
    while(curr != NULL)
    {
        printDeclstmt(curr);
        curr = curr->next;
    }
}

void printMainBlock()       // Superceding function to call the functions to print Statement List and Expressions
{                           // That is, all content that is inside the main function of the input
    // cout << "\t\t";
    Main * mainBlock = code_start->mainBlock;

    cout << "FUN ";
    if(mainBlock->return_type==INT)
    {
        cout << "INT ";
    }
    else{
        cout<< "Unsupported variable type* ";
        std::exit(1);
    }

    cout<< "MAIN "<<endl;
    printStmtList(mainBlock->stmt_list);

    cout<< "\tRETURN ";
    
    printExpression(mainBlock->return_stmt);
    cout<<endl;

    cout<< "END MAIN"<<endl;

}


/**
 * (3) Printing all assignments
*/

void print_assignments (assignment_stmt * stmt)
{
    cout <<"ASSIGN ";
    if (stmt->var->isArray)
    {
        cout << "ARRAY_REF VAR ";
        printExpression(stmt->var->index);
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

void printStrExprList (strExp * list)
{
    cout << "\t\t\t\t";
    if(list == NULL)
    {
        cout<<"List was NULL\n";
    }
    while (list!=NULL)
    {
        if(list->var== NULL){
            cout << "Empty String*";
        }
        cout << list->var << " ";
        list = list->next;
    }
    cout<<endl;
}

/**
 * (5) Printing of Write statements
*/

void print_write (write_stmt * stmt)
{
    cout << "FUNC ";
    if(stmt->isExpr){
        printExpression((expr *)(stmt->val));
    }
    else 
    {
        printStrExprList((strExp *)(stmt->val));
    }
}

/**
 * (6) Printing statement list and statements
*/


void printStmtList (statement * stmt){
    
    while (stmt!=NULL)
    {
        cout << "\t\t";
        printStmt(stmt);
        stmt=stmt->next;
        cout<<endl;
    }
}

void printStmt (statement * stmt){
    if(stmt->stmtType == Write)
    {
        print_write((write_stmt*)(stmt->stmt));
    }
    else if (stmt->stmtType == assign)
    {
        print_assignments((assignment_stmt * )(stmt->stmt));
    }
    else if (stmt->stmtType == COND)
    {
        printCOND((conditional_stmt * )(stmt->stmt));
    }
}

/**
 * (7) Condition and Loop statements
*/

void printIF (if_stmt * stmt)
{
    cout << "\nIF "; 
    printResult(Calculate( stmt->COND));
    cout << endl << " THEN" << endl; 
    printStmtList(stmt->stmtList);
    cout << "ENDIF"<<endl;

}

void printIFELSE (ifelse_stmt * stmt)
{
    cout << "\n\tIF "; 
    printExpression(stmt->COND);
    cout <<" THEN "<<endl; 
    printStmtList(stmt->if_stmtList);
    cout << "\tELSE"<<endl;
    printStmtList(stmt->else_stmtList);
    cout << "\tENDIF"<<endl;
}

void printWHILE (while_stmt * stmt)
{
    cout << "\n\tWHILE "; 
    printExpression( stmt->COND);
    cout << " DO " << endl; 
    printStmtList(stmt->stmtList);
    cout << "\tENDWHILE"<<endl;

}

void printCOND (conditional_stmt * stmt)
{
    if(stmt->type == If)
    {
        printIF((if_stmt *)(stmt->attributes));
    }
    else if (stmt->type == IF_ELSE)
    {
        printIFELSE((ifelse_stmt *)(stmt->attributes));
    }
    else if (stmt->type == While)
    {
        printWHILE((while_stmt *)(stmt->attributes));
    }
}

/**
 * Value types, and global symbol table
*/

void printType (Datatype type)
{
    if(type == INT)
    {
        cout<<"int";
    }
    else if  (type == BOOL)
    {
        cout<<"bool";
    }
}

void printBool (bool b)
{
    if(b)
    {
        cout << "true ";
    }
    else
    {
        cout << "false ";
    }
}

void printBoolArray (bool * lst,int size)
{
    for (int i =0;i<size;i++)
    {
        printBool(lst[i]);
    }
}

void printIntArray (int * lst,int size)
{
    for (int i=0;i<size;i++)
    {
        cout << lst[i] << " ";
    }
}

void printSymbolTable (const map<string,value * > & symtable)
{
    cout << "**********-------------------- SYMBOL TABLE --------------------**********\n"<<endl;
    
    for (auto it = symtable.begin(); it != symtable.end(); ++it) 
    {    
        cout << "\t\t";    
        if(it->second->isArray)
        {
            Array * arr = (Array *)(it->second->val);
            cout << it->first << "\t: " ;
            if(it->second->type == INT)
            {
                printIntArray ((int *)(arr->arr),arr->size);
            }
            else
            {
                printBoolArray ((bool *)(arr->arr),arr->size);
            }
            cout << "(";
            printType(it->second->type);
            cout << ")";
            cout<<"[" << ((Array *)(it->second->val))->size << "]" << endl;
        }
        else
        {
            cout << it->first << "\t: "; 
            if(it->second->type == INT)
            {
                cout << *(int *)(it->second->val)<< "\t(";
            }
            else 
            {
                printBool(*(bool *)(it->second->val));

                cout << "\t(";
            }
            printType(it->second->type);
            cout<<") "<< endl;
        }
    }
    cout<< "______________________________________________________________________________" << endl << endl;

}

void printGlobalSymbolTable ()
{
    printSymbolTable(symTable);
}


void printOverallPrg()
{
    cout << "\n***********-------------- PRINTING AST --------------***********\n"<<endl;
    printGlobalDecl(code_start->global_declarations);
    printMainBlock();
    cout<< "____________________________________________________________________________\n" << endl;
}

void PrgBeginMessage()
{
    cout << "\n\t\t  ------ START OF PROGRAM -----\n" << endl;
}

void PrgEndMessage()
{
    cout << "\n\t\t  ------ END OF PROGRAM -----\n" << endl;
}
// some printing and test functions
void printResult (evaluation * val)
{
    if(val -> type == INT)
    {
        cout << *(int *)(val->value) ;
    }
    else if (val -> type == BOOL)
    {
        cout << *(bool *)(val->value) ;   
    }
}