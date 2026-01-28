/* utils.c */

#include "../includes/libs.h"
#include "../includes/utils.h"

static size_t get_type_size(enum DataType type){

	/* get size of the datatype*/

	switch(type){

	case TYPE_INT8:
		return sizeof(int8_t);

	case TYPE_INT16:
		return sizeof(int16_t);

	case TYPE_INT32:
		return sizeof(int32_t);

	case TYPE_INT64:
		return sizeof(int64_t);

	case TYPE_UINT8:
		return sizeof(uint8_t);

	case TYPE_UINT16:
		return sizeof(uint16_t);

	case TYPE_UINT32:
		return sizeof(uint32_t);

	case TYPE_UINT64:
		return sizeof(uint64_t);

	default:
		return 0;

	}
}

//rank based approch
static int type_rank(enum DataType t)
{
    switch (t) {

        case TYPE_DOUBLE: return 100;
        case TYPE_FLOAT:  return 90;

        case TYPE_UINT64:
        case TYPE_INT64:  return 80;

        case TYPE_UINT32:
        case TYPE_INT32:  return 70;

        case TYPE_UINT16:
        case TYPE_INT16:  return 60;

        case TYPE_UINT8:
        case TYPE_INT8:   return 50;

        default: return -1;
    }
}


struct Matrix *matrix(int rows, int cols, enum DataType type){
	if(rows <=0 || cols <= 0){
		return NULL;
	}

	size_t element_size = get_type_size(type);

	if(element_size == 0){
		return NULL;
	}

	/* Overflow protection */
    if ((size_t)rows > SIZE_MAX / (size_t)cols / element_size){
        return NULL;
    }

	struct Matrix *new_matrix = (malloc(sizeof(struct Matrix)));

	if(new_matrix == NULL){
		return NULL;
	}

	new_matrix->type = type;

	new_matrix->rows = rows;
	new_matrix->cols = cols;

	size_t total_bytes = (size_t)rows * cols * element_size;

	new_matrix->matrix = malloc(total_bytes);

	if(!new_matrix->matrix){
		free(new_matrix);
		return NULL;
	}
	return new_matrix;
}

// Destructor
void matrix_destroy(struct Matrix *m) {
    if (!m) return;
    free(m->matrix);
    free(m);
}

// create a matrix from 2D array

struct Matrix *matrix_from_2d(int rows, int cols, enum DataType type, const void *src){
    if (!src) return NULL;

    struct Matrix *m = matrix(rows, cols, type);
    if (!m) return NULL;

    size_t elem_size = get_type_size(type);
    size_t total_bytes = (size_t)rows * cols * elem_size;

    memcpy(m->matrix, src, total_bytes);

    return m;
}

//========================================================================
// Helper Function for printing the array in right format

static int digits_ll(long long x)
{
    int d = (x <= 0) ? 1 : 0;   // handle 0 and negative
    while (x) {
        x /= 10;
        d++;
    }
    return d;
}

static int digits_ull(unsigned long long x)
{
    int d = (x == 0) ? 1 : 0;
    while (x) {
        x /= 10;
        d++;
    }
    return d;
}

//====================================================================

// Printing the array
void print_matrix(struct Matrix *m)
{
    if (!m || !m->matrix) {
        printf("[]\n");
        return;
    }

    int rows = m->rows;
    int cols = m->cols;

    int col_width[256];   // assuming max 256 columns
    for (int j = 0; j < cols; ++j)
        col_width[j] = 0;

    /* -------- pass 1: compute max width per column -------- */
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {

            size_t idx = (size_t)i * cols + j;
            int w = 1;

            switch (m->type) {

                case TYPE_INT8:
                case TYPE_INT16:
                case TYPE_INT32:
                case TYPE_INT64: {
                    long long v = 0;
                    if (m->type == TYPE_INT8)   v = ((int8_t*)m->matrix)[idx];
                    if (m->type == TYPE_INT16)  v = ((int16_t*)m->matrix)[idx];
                    if (m->type == TYPE_INT32)  v = ((int32_t*)m->matrix)[idx];
                    if (m->type == TYPE_INT64)  v = ((int64_t*)m->matrix)[idx];
                    w = digits_ll(v);
                    break;
                }

                case TYPE_UINT8:
                case TYPE_UINT16:
                case TYPE_UINT32:
                case TYPE_UINT64: {
                    unsigned long long v = 0;
                    if (m->type == TYPE_UINT8)   v = ((uint8_t*)m->matrix)[idx];
                    if (m->type == TYPE_UINT16)  v = ((uint16_t*)m->matrix)[idx];
                    if (m->type == TYPE_UINT32)  v = ((uint32_t*)m->matrix)[idx];
                    if (m->type == TYPE_UINT64)  v = ((uint64_t*)m->matrix)[idx];
                    w = digits_ull(v);
                    break;
                }

                case TYPE_FLOAT:
                case TYPE_DOUBLE:
                    w = 8;   // fixed width for floats
                    break;

                default:
                    w = 1;
            }

            if (w > col_width[j])
                col_width[j] = w;
        }
    }

    /* -------- pass 2: print aligned -------- */
    printf("[");

    for (int i = 0; i < rows; ++i) {

        if (i > 0)
            printf(" ");

        printf("[");

        for (int j = 0; j < cols; ++j) {

            size_t idx = (size_t)i * cols + j;
            int w = col_width[j];

            switch (m->type) {

                case TYPE_INT8:
                    printf("%*d", w, (int)((int8_t*)m->matrix)[idx]);
                    break;

                case TYPE_INT16:
                    printf("%*d", w, (int)((int16_t*)m->matrix)[idx]);
                    break;

                case TYPE_INT32:
                    printf("%*d", w, ((int32_t*)m->matrix)[idx]);
                    break;

                case TYPE_INT64:
                    printf("%*lld", w, (long long)((int64_t*)m->matrix)[idx]);
                    break;

                case TYPE_UINT8:
                    printf("%*u", w, (unsigned)((uint8_t*)m->matrix)[idx]);
                    break;

                case TYPE_UINT16:
                    printf("%*u", w, (unsigned)((uint16_t*)m->matrix)[idx]);
                    break;

                case TYPE_UINT32:
                    printf("%*u", w, ((uint32_t*)m->matrix)[idx]);
                    break;

                case TYPE_UINT64:
                    printf("%*llu", w, (unsigned long long)((uint64_t*)m->matrix)[idx]);
                    break;

                case TYPE_FLOAT:
                    printf("%*.*f", w, 3, ((float*)m->matrix)[idx]);
                    break;

                case TYPE_DOUBLE:
                    printf("%*.*f", w, 6, ((double*)m->matrix)[idx]);
                    break;

                default:
                    printf("%*s", w, "?");
                    break;
            }

            if (j < cols - 1)
                printf(" ");
        }

        printf("]");

        if (i < rows - 1)
            printf("\n");
    }

    printf("]\n");
}

// Datatype Selection

enum DataType datatype_selection(struct Matrix *M1, struct Matrix *M2)
{
    if (!M1 || !M2) return TYPE_INVALID;

    int r1 = type_rank(M1->type);
    int r2 = type_rank(M2->type);

    if (r1 < 0 || r2 < 0) return TYPE_INVALID;

    type = (r1 >= r2) ? M1->type : M2->type;

    return type;
}


// Adding two Matrix

struct Matrix *add_matrix(struct Matrix *M1, struct Matrix *M2)
{
    if (!M1 || !M2 || !M1->matrix || !M2->matrix)
        return NULL;

    if (M1->rows != M2->rows || M1->cols != M2->cols)
        return NULL;

    enum DataType out_type = datatype_selection(M1, M2);
    if (out_type == TYPE_INVALID)
        return NULL;

    int rows = M1->rows;
    int cols = M1->cols;
    size_t total = (size_t)rows * cols;

    struct Matrix *R = matrix(rows, cols, out_type);
    if (!R) return NULL;

    for (size_t i = 0; i < total; ++i) {

        double a = 0.0;
        double b = 0.0;

        /* ---- load M1 value into double ---- */
        switch (M1->type) {
            case TYPE_INT32:  a = ((int32_t*)M1->matrix)[i]; break;
            case TYPE_FLOAT:  a = ((float*)M1->matrix)[i];   break;
            case TYPE_DOUBLE: a = ((double*)M1->matrix)[i];  break;
            default: return NULL;
        }

        /* ---- load M2 value into double ---- */
        switch (M2->type) {
            case TYPE_INT32:  b = ((int32_t*)M2->matrix)[i]; break;
            case TYPE_FLOAT:  b = ((float*)M2->matrix)[i];   break;
            case TYPE_DOUBLE: b = ((double*)M2->matrix)[i];  break;
            default: return NULL;
        }

        double sum = a + b;

        /* ---- store into result ---- */
        switch (out_type) {
            case TYPE_INT32:
                ((int32_t*)R->matrix)[i] = (int32_t)sum;
                break;

            case TYPE_FLOAT:
                ((float*)R->matrix)[i] = (float)sum;
                break;

            case TYPE_DOUBLE:
                ((double*)R->matrix)[i] = sum;
                break;

            default:
                return NULL;
        }
    }

    return R;
}
