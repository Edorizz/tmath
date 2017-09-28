#ifndef TMATH_H
#define TMATH_H

/* C library */
#include <stdint.h>
#include <stdlib.h>

#define STACK_SIZ	256
#define RIGHT		  0
#define LEFT		  1

#define NODE_EXPR	  0
#define NODE_OP		  1

#define ASSIGN		  0
#define EXPR		  1

#define OPERATOR_COUNT	(sizeof operators / sizeof operators[0])

struct op_info {
	char ch;
	int prc;
	int ass;
	double (*fnc)(double, double);
};

struct node_expr {
	uint8_t type;
	double value;
};

struct node_op {
	uint8_t type;
	void *left, *right;
	double (*fnc)(double, double);
};

struct var {
	const char *label;
	struct node_op *expr;
};

/* Basic operations */
double _sum(double a, double b);
double _sub(double a, double b);
double _mult(double a, double b);
double _div(double a, double b);
double _mod(double a, double b);
double _exp(double a, double b);

void   *parse(const char *expr, struct var *vars, size_t var_cnt);
double eval(struct node_op *expr_tree);
void   free_tree(struct node_op *expr_tree);

#endif /* TMATH_H */
