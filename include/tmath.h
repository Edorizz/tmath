#ifndef TMATH_H
#define TMATH_H

#define DEBUG

#define BUFFER_SIZE	256
#define RIGHT		  0
#define LEFT		  1

#define NODE_EXPR	  0
#define NODE_OP		  1

#define OPERATOR_COUNT	(sizeof(operators) / sizeof(operators[0]))

typedef unsigned char BYTE;

typedef struct {
	char ch;
	int prc;
	int ass;
	double (*fnc)(double, double);
} op_info;

typedef struct {
	BYTE type;
	double value;
} node_expr;

typedef struct {
	BYTE type;
	void *left, *right;
	double (*fnc)(double, double);
} node_op;

/* Basic operations */
double _sum(double a, double b);
double _sub(double a, double b);
double _mult(double a, double b);
double _div(double a, double b);
double _mod(double a, double b);
double _exp(double a, double b);

char *to_rpn(const char *expr);
void *parse(const char *expr);
double solve(node_op *expr_tree);
void free_tree(node_op *expr_tree);

#endif /* TMATH_H */
