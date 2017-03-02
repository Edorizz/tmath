#ifndef TMATH_H
#define TMATH_H

#define DEBUG

#define BUFFER_SIZE	256
#define RIGHT		  0
#define LEFT		  1

#define OPERATOR_COUNT	(sizeof(operators) / sizeof(operators[0]))

/* Basic operations */
double _sum(double a, double b);
double _sub(double a, double b);
double _mult(double a, double b);
double _div(double a, double b);
double _exp(double a, double b);

struct op_info {
	char ch;
	int prc;
	int ass;
	double (*fnc)(double, double);
};

typedef struct op_info op_info;

char *to_rpn(const char *expression);

#endif /* TMATH_H */
