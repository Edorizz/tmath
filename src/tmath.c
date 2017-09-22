/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* tmath */
#include "tmath.h"

#define BUF_SIZ	256

void
usage(void)
{
	puts("usage: tmath <expression>");
}

int 
main(int argc, char **argv)
{
	node_op *ast;
	char input[BUF_SIZ], *nl;

	while (fgets(input, BUF_SIZ + 1, stdin)) {
		if ((nl = strchr(input, '\n')) == NULL) {
			fprintf(stderr, "error: input too large\n");
		}

		*nl = '\0';

		ast = (node_op *) parse(input);
		printf("%g\n", solve(ast));

		free_tree(ast);
	}

	return 0;
}
