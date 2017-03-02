/* Include header */
#include "../include/tmath.h"
/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEBUG

const struct op_info operators[] = { { '+', 1, LEFT, _sum },  { '-', 1, LEFT, _sub },
				     { '*', 2, LEFT, _mult }, { '/', 2, LEFT, _div },
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
	rpn = malloc(sizeof(expr) * 2);

	/* shunting-yard algorithm */
	for (head = 0; *expr; ++expr) {
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

#ifdef DEBUG
			printf("Stack: [");
			for (int j = 0; j != head; ++j)
				printf(" %c", stack[j]);
			printf(" ]\n");
#endif
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

