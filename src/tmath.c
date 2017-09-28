/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* Custom */
#include "tmath.h"
#include "resiz_arr.h"

#define BUF_SIZ	256

int 
main(int argc, char **argv)
{
	struct resiz_arr var, var_nam;
	struct var input_expr;
	char input_buf[BUF_SIZ], *nl, *p;
	int i;

	create_arr(&var, sizeof (struct var));
	create_arr(&var_nam, sizeof (char));

	while (fgets(input_buf, BUF_SIZ + 1, stdin)) {
		if ((nl = strchr(input_buf, '\n')) == NULL) {
			fprintf(stderr, "error: input_buf too large\n");
		}

		*nl = '\0';

		if ((nl = strchr(input_buf, '=')) == NULL) {
			input_expr.expr = (struct node_op *) parse(input_buf, (struct var *) var.arr, var.curr_siz);
			printf("%g\n", eval(input_expr.expr));
			free_tree(input_expr.expr);

		} else {
			input_expr.expr = (struct node_op *) parse(nl + 1, (struct var *) var.arr, var.curr_siz);

			/*
			 * Skip to the first character of the first word, which
			 * SHOULD be the name of the variable.
			 */
			for (p = input_buf; !isalpha(*p); ++p)
				;

			/* Get lenght of the name */
			for (i = 0; isalpha(p[i]); ++i)
				;

			p[i] = '\0';
			append(&var_nam, p, i + 1, (void **) &input_expr.label);

			append(&var, &input_expr, 1, NULL);
			printf("%g\n", eval(input_expr.expr));
		}


		/* DEBUG
		printf("-==+ VARIABLES +==-\n");
		for (i = 0; i != var.curr_siz; ++i) {
			ptr = (struct var *) (var.arr + sizeof (struct var) * i);
			printf("%s = %g\n", ptr->label, eval(ptr->expr));
		}
		printf("-==+==-\n");
		*/
	}

	return 0;
}

