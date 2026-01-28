/* utils.h */

#ifndef UTILS_H
#define UTILS_H

#include "libs.h"

enum DataType{
	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_INT64,

	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPE_UINT64,

	TYPE_FLOAT,
	TYPE_DOUBLE,
};

struct Matrix{
	int rows;
	int cols;

	enum DataType type;

	void *matrix;
};

static size_t get_type_size(enum DataType type);
void *type_range(enum DataType type);



struct Matrix *matrix(int rows, int cols, enum DataType type);
struct Matrix *matrix_from_2d(int rows, int cols, enum DataType type, const void *src);

enum DataType datatype_selection(struct Matrix *M1, struct Matrix *M2);

static int digit_ll(long long x);
static int digit_ull(unsigned long long x);
void print_matrix(struct Matrix *m);


struct Matrix *add_matrix(struct Matrix *M1, struct Matrix *M2);

#endif
