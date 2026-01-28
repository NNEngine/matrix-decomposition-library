#include "includes/libs.h"
#include "includes/utils.h"

int main(void){
	bool is_true = true;
	printf("%d\n", is_true);

	bool is_false = false;
	printf("%\n", is_false);

	int8_t A[2][3] = {
				{1,100,3},
				{4,5,6}
				};

	struct Matrix *M = matrix_from_2d(2, 3, TYPE_INT8, A);

	printf("%d\n", M->rows);
	print_matrix(M);

	printf("==================================\n");

	int32_t B[3][3] = {
			{100, 200, 1},
			{20000, 2, 90},
			{100, 1, 90000}
	};

	struct Matrix *M1 = matrix_from_2d(3, 3, TYPE_INT32, B);
	print_matrix(M1);

	int32_t C[3][3] = {
			{100, 200, 1},
			{20000, 2, 90},
			{100, 1, 90000}
	};
	struct Matrix *M2 = matrix_from_2d(3, 3, TYPE_INT32, C);
	print_matrix(M2);

	return 0;
}
