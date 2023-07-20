struct node 
{
	double value;
    char op;
    struct node *left;
    struct node *right;
};

struct node* AddnonTerminal(struct node *left,char op,struct node *right) ;

struct node* AddLeaf(double value) ;

double Solve(struct node *node) ;