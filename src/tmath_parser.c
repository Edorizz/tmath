/* Include header */
#include "tmath.h"
/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

const op_info operators[] = { { '+', 1, LEFT, _sum },  { '-', 1, LEFT, _sub },
			      { '*', 2, LEFT, _mult }, { '/', 2, LEFT, _div }, { '%', 2, LEFT, _mod },
			      { '^', 3, RIGHT, _exp } };

double
_sum(double a, double b)
{ return a + b; }

double
_sub(double a, double b)
{ return a - b; }

double
_mult(double a, double b)
{ return a * b; }

double
_div(double a, double b)
{ return a / b; }

double
_mod(double a, double b)
{ return fmod(a, b); }

double
_exp(double a, double b)
{ return pow(a, b); }

int
op_find(char c)
{
	for (int i = 0; i != OPERATOR_COUNT; ++i)
		if (operators[i].ch == c)
			return i;
	
	return -1;
}

node_op *
new_op(void *left, void *right, double(*fnc)(double, double))
{
	node_op *op_node;

	op_node = (node_op*)malloc(sizeof(*op_node));

	op_node->type = NODE_OP;
	op_node->left = left;
	op_node->right = right;
	op_node->fnc = fnc;

	return op_node;
}

void *
parse(const char *expr)
{
	char op_stack[BUFFER_SIZE];
	void *expr_stack[BUFFER_SIZE];
	int op_head, expr_head, op_index;
	op_info op_curr, op_top;
	node_expr *tmp_expr;
	node_op *tmp_op;

	/* Compiler won't shut up */
	tmp_expr = NULL;
	tmp_op = NULL;

	/* Shunting yard */
	for (op_head = expr_head = 0; *expr; ++expr) {
		/* If digit, push it to the expression stack */
		if (isdigit(*expr)) {
			tmp_expr = (node_expr*)malloc(sizeof(node_expr));

			tmp_expr->type = NODE_EXPR; 
			sscanf(expr, "%lf", &tmp_expr->value);

			while (isdigit(*(expr + 1)) || *(expr + 1) == '.') {
				++expr;
			}

			expr_stack[expr_head++] = tmp_expr;
			tmp_expr = NULL;
		}
		
		/* If operator */
		else if ((op_index = op_find(*expr)) != -1) {
			op_curr = operators[op_index];

			/* Pop operator stack, link it to the two newest expressions in the expression stack
			 * (left = oldest, right = newest) and push it where the left expression previously 
			 * resided (which SHOULD be the top). Do this while the current operator has lower
			 * precedence than the operator at the top of the stack, or is equal and the current
			 * operator is left assosiative.
			 */
			while ((op_head && op_stack[op_head - 1] != '(') &&
			       ((op_curr.prc < op_top.prc) ||
				(op_curr.prc == op_top.prc && op_curr.ass == LEFT))) {
				tmp_op = new_op(expr_stack[expr_head - 2], expr_stack[expr_head - 1], op_top.fnc);

				expr_stack[--expr_head - 1] = tmp_op;

				if (--op_head) {
					op_top = operators[op_find(op_stack[op_head - 1])];
				}
			}

			/* Push the operator */
			op_stack[op_head++] = op_curr.ch;
			op_top = op_curr;
		}
		
		/* Flush the operator stack until an opening parethesis is encountered */
		else if (*expr == ')') {
			while (op_stack[--op_head] != '(') {
				tmp_op = new_op(expr_stack[expr_head - 2], expr_stack[expr_head - 1], op_top.fnc);

				expr_stack[--expr_head - 1] = tmp_op;

				if (op_stack[op_head - 1] != '(') {
					op_top = operators[op_find(op_stack[op_head - 1])];
				}
			}

			if (op_head) {
				op_top = operators[op_find(op_stack[op_head - 1])];
			}
		}
		
		else if (*expr == '(') {
			op_stack[op_head++] = *expr;
		}
	}

	/* Flush the operator stack */
	while (op_head--) {
		tmp_op = new_op(expr_stack[expr_head - 2], expr_stack[expr_head - 1], op_top.fnc);

		expr_stack[--expr_head - 1] = tmp_op;

		if (op_head) {
			op_top = operators[op_find(op_stack[op_head - 1])];
		}
	}

	return expr_stack[0];
}

double solve(node_op *expr_tree)
{
	if (expr_tree->type == NODE_EXPR) {
		return ((node_expr*)expr_tree)->value;
	}

	return expr_tree->fnc(solve(expr_tree->left), solve(expr_tree->right));
}

void free_tree(node_op *expr_tree)
{
	if (expr_tree->type == NODE_EXPR) {
		free(expr_tree);
		return;
	}

	free_tree(expr_tree->left);
	free_tree(expr_tree->right);
}

