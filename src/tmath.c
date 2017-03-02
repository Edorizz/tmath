#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tmath.h"

void
usage(void)
{
	puts("usage: tmath <expression>");
}

int 
main(int argc, char **argv)
{
	char *rpn, expr[100];

	if (argc != 2) {
		usage();
		return 1;
	}

	strcpy(expr, argv[1]);

	rpn = to_rpn(expr);
	printf("infix: %s\nrpn: %s\n", expr, rpn);

	free(rpn);
	return 0;
}
