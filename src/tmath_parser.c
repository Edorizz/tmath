/* Include header */
#include "../include/tmath.h"
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
{ return 0; }

int
op_find(char c)
{
	for (int i = 0; i != OPERATOR_COUNT; ++i)
		if (operators[i].ch == c)
			return i;
	
	return -1;
}

char *
to_rpn(const char *expr)
{
	char *rpn, stack[BUFFER_SIZE];
	op_info top_op, curr_op;
	int head, op_index, i;

	/* allocate enough memory */
	rpn = malloc(strlen(expr) * 2 + 1);

	/* shunting-yard algorithm */
	for (head = i = 0; *expr; ++expr) {
		if (isdigit(*expr)) {
			do {
				rpn[i++] = *expr;
			} while (isdigit(*(expr + 1)) && ++expr);

			rpn[i++] = ' ';
		} else if ((op_index = op_find(*expr)) != -1) {
			curr_op = operators[op_index];

			while ((head && stack[head - 1] != '(') &&
			      ((curr_op.prc < top_op.prc) ||
			      (curr_op.prc == top_op.prc && curr_op.ass == LEFT))) {
				rpn[i++] = top_op.ch;
				rpn[i++] = ' ';

				if (--head)
					top_op = operators[op_find(stack[head - 1])];
			}

			stack[head++] = curr_op.ch;
			top_op = curr_op;
		} else if (*expr == ')') {
			while (stack[head - 1] != '(') {
				rpn[i++] = stack[--head];
				rpn[i++] = ' ';
			}

			--head;
		} else if (*expr == '(') {
			stack[head++] = *expr;
		}
	}

	/* flush the operator stack */
	while (head) {
		rpn[i++] = stack[--head];
		rpn[i++] = ' ';
	}

	rpn[i] = '\0';

	return rpn;
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

	tmp_expr = NULL;
	tmp_op = NULL;

	for (op_head = expr_head = 0; *expr; ++expr) {
		if (isdigit(*expr)) {
			tmp_expr = (node_expr*)malloc(sizeof(node_expr));

			tmp_expr->type = NODE_EXPR; 
			sscanf(expr, "%lf", &tmp_expr->value);

			while (isdigit(*(expr + 1)) || *(expr + 1) == '.')
				++expr;

			expr_stack[expr_head++] = tmp_expr;
			tmp_expr = NULL;
		} else if ((op_index = op_find(*expr)) != -1) {
			op_curr = operators[op_index];

			while ((op_head && op_stack[op_head - 1] != '(') &&
			       ((op_curr.prc < op_top.prc) ||
				(op_curr.prc == op_top.prc && op_curr.ass == LEFT))) {
				tmp_op = (node_op*)malloc(sizeof(node_op));

				tmp_op->type = NODE_OP;
				tmp_op->left = expr_stack[expr_head - 2];
				tmp_op->right = expr_stack[expr_head - 1];
				tmp_op->fnc = op_top.fnc;

				expr_stack[--expr_head - 1] = tmp_op;

				if (--op_head)
					op_top = operators[op_find(op_stack[op_head - 1])];
			}

			op_stack[op_head++] = op_curr.ch;
			op_top = op_curr;
		} else if (*expr == ')') {
			while (op_stack[--op_head] != '(') {
				tmp_op = (node_op*)malloc(sizeof(node_op));

				tmp_op->type = NODE_OP;
				tmp_op->left = expr_stack[expr_head - 2];
				tmp_op->right = expr_stack[expr_head - 1];
				tmp_op->fnc = op_top.fnc;

				expr_stack[--expr_head - 1] = tmp_op;

				if (op_stack[op_head - 1] != '(')
					op_top = operators[op_find(op_stack[op_head - 1])];
			}

			if (op_head)
				op_top = operators[op_find(op_stack[op_head - 1])];
		} else if (*expr == '(') {
			op_stack[op_head++] = *expr;
		}
	}

	while (op_stack[--op_head] != '(') {
		tmp_op = (node_op*)malloc(sizeof(node_op));

		tmp_op->type = NODE_OP;
		tmp_op->left = expr_stack[expr_head - 2];
		tmp_op->right = expr_stack[expr_head - 1];
		tmp_op->fnc = op_top.fnc;

		expr_stack[--expr_head - 1] = tmp_op;

		if (op_stack[op_head - 1] != '(')
			op_top = operators[op_find(op_stack[op_head - 1])];
	}

	return expr_stack[0];
}

double solve(node_op *expr_tree)
{
	if (expr_tree->type == NODE_EXPR)
		return ((node_expr*)expr_tree)->value;

	return expr_tree->fnc(solve(expr_tree->left), solve(expr_tree->right));
}

