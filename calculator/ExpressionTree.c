#include <stdio.h>
#include <stdlib.h>
#include "ExpressionTree.h"

struct node *AddnonTerminal(struct node *left, char op, struct node *right)
{
    struct node *Node = (struct node *)malloc(sizeof(struct node));
    if (Node)
    {
        Node->op = op;
        Node->left = left;
        Node->right = right;
        return Node;
    }
}

struct node *AddLeaf(double value)
{
    struct node *Node = (struct node *)malloc(sizeof(struct node));
    Node->value = value;
    Node->left = NULL;
    Node->right = NULL;
    return Node;
}
double Solve(struct node *Node)
{
    if (Node->right == NULL && Node->left == NULL)
        return Node->value;
    double leftkey = Solve(Node->left);
    double rightkey = Solve(Node->right);

    if (Node->op == '+')
        return leftkey + rightkey;
    else if (Node->op == '-')
        return leftkey - rightkey;
    else if (Node->op == '*')
        return leftkey * rightkey;
    else if (Node->op == '/')
        return leftkey / rightkey;
}