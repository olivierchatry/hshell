#ifndef __ARRAY_H__
#define __ARRAY_H__
#include <stdio.h>

#define ARRAY_ADD(array, data, add_buffer_size) \
	if ( (array##_size) >= (array##_buffer_size) ) {\
		void *array_add_next;\
		unsigned int 	i;\
		array_add_next = malloc((array##_buffer_size + add_buffer_size) * sizeof(array[0]));\
		for (i = 0; i < array##_buffer_size * sizeof(array[0]); ++i) {\
			((char*)array_add_next)[i] = ((char*)array)[i];\
		}\
		if (array) {\
			free(array);\
		}\
		array = array_add_next;\
		array##_buffer_size += add_buffer_size;\
	}\
	array[array##_size++]=(data)

#define ARRAY(array) array;\
	int array##_size;\
	int array##_buffer_size

#define ARRAY_FREE_EACH(array) { \
	int count;\
	for (count = 0; count < array##_size; ++count) {\
		if (array[count]) {\
			free(array[count]);\
		}\
	}\
	free(array);\
	array=NULL;\
	array##_size=0;\
	array##_buffer_size=0;\
}

#define ARRAY_FREE(array) free(array);\
	array=NULL;\
	array##_size=0;\
	array##_buffer_size=0

#define ARRAY_REMOVE(array, index) (array[index] = array[--array##_size])

#define ARRAY_EACH(array, fct) { \
	int count;\
	for (count = 0; count < array##_size; ++count) {\
		if (array[count]) {\
			fct(array[count]);\
		}\
	}\
}

#define ARRAY_POP(array) \
if (array##_size > 0) { \
	array##_size--; \
}

#define ARRAY_INIT(array) array=NULL;\
	array##_size=0;\
	array##_buffer_size=0

#define ARRAY_LAST(array) array[array##_size - 1]

#define ARRAY_SET(a, b) \
a = b;\
a##_size = b##_size;\
a##_buffer_size = b##_buffer_size;

#define ARRAY_ALLOCATE(a, size) \
a = malloc(size * sizeof(a[0])); \
a##_size = 0;\
a##_buffer_size = size;

#define ARRAY_RESET(a) a##_size = 0

#define ARRAY_CAT(a, b, size, buffer_size) {\
	int _array_cat_counter;\
	for (_array_cat_counter = 0; _array_cat_counter < size; ++_array_cat_counter) {\
		ARRAY_ADD(a, b[_array_cat_counter], buffer_size); \
	}\
}

#endif
