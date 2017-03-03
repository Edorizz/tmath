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
	node_op *ast;

	if (argc != 2) {
		usage();
		return 1;
	}

	ast = (node_op*)parse(argv[1]);
	printf("%s = %g\n", argv[1], solve(ast));

	free_tree(ast);
	return 0;
}
