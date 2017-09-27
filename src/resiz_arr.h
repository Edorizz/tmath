#ifndef RESIZ_ARR_H
#define RESIZ_ARR_H

/* C library */
#include <stdlib.h>
#include <stdint.h>

#define GROWTH_RATE	2
#define ARR_START_SIZ	64

struct resiz_arr {
	uint8_t *arr;
	size_t real_siz;
	size_t curr_siz;
	size_t obj_siz;
};

int  inc_siz(struct resiz_arr *ra);
int  create_arr(struct resiz_arr *ra, size_t obj_siz);
void free_arr(struct resiz_arr *ra);
int  append(struct resiz_arr *ra, void *data, size_t siz, void **ptr);

#endif /* RESIZ_ARR_H */

