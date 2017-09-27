/* Header file */
#include "resiz_arr.h"
/* C library */
#include <string.h>

int
inc_siz(struct resiz_arr *ra)
{
	uint8_t *tmp;

	if ((tmp = malloc(ra->real_siz * ra->obj_siz * GROWTH_RATE)) == NULL) {
		return -1;
	}

	memcpy(tmp, ra->arr, ra->real_siz * ra->obj_siz);
	free(ra->arr);
	ra->arr = tmp;
	ra->real_siz *= GROWTH_RATE;

	return 1;
}

int
create_arr(struct resiz_arr *ra, size_t obj_siz)
{
	ra->curr_siz = 0;
	ra->real_siz = ARR_START_SIZ;
	ra->obj_siz = obj_siz;
	ra->arr = malloc(ra->real_siz * obj_siz);

	return 0;
}

void
free_arr(struct resiz_arr *ra)
{
	free(ra->arr);
}

int
append(struct resiz_arr *ra, void *data, size_t siz, void **ptr)
{
	int err;

	err = 0;
	if (ra->curr_siz + siz >= ra->real_siz) {
		err = inc_siz(ra);
	}

	memcpy(ra->arr + ra->curr_siz * ra->obj_siz, data, siz * ra->obj_siz);

	if (ptr) {
		*(uint8_t **) ptr = ra->arr + ra->curr_siz * ra->obj_siz;
	}

	ra->curr_siz += siz;

	return err;
}

