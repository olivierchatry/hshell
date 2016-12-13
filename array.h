#ifndef __ARRAY_H__
#define __ARRAY_H__

#define ARRAY_ADD(array, data, add_buffer_size) \
	if ( (array##_size) >= (array##_buffer_size) ) {\
		void* array_add_next;\
		unsigned int 	i;\
		array_add_next = malloc(array##_buffer_size + add_buffer_size * sizeof(array[0]));\
		for (i = 0; i < array##_buffer_size * sizeof(array[0]); ++i) {\
			((char*)array_add_next)[i] = ((char*)array)[i];\
		}\
		free(array);\
		array = array_add_next;\
		array##_buffer_size += add_buffer_size;\
	}\
	array[array##_size++]=data
	
#define ARRAY(array) array;\
	int array##_size;\
	int array##_buffer_size

#define ARRAY_FREE(array) free(array);\
	array##_size=0;\
	array##_buffer_size=0

#define ARRAY_REMOVE(array, index) array[index]=array[--array##_size]

#define ARRAY_EACH(array, fct) { \
	int count;\
	for (count = 0; count < array##_size; ++count) {\
		fct(array[count]);\
	}\
}
	
#define ARRAY_INIT(array) array=NULL;\
	array##_size=0;\
	array##_buffer_size=0

#define ARRAY_LAST(array) array[array##_size - 1]	
#endif