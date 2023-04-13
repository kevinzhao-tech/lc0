#include "sa_mm.h"

void matrix_multiply_4(const float *A, const float *B, float *C, const int lda, const int ldb, const int ldc, const int M, const int N, const int K, const int i, const int j, const int k) {

    if (state == NULL) {
        state = cnn_setup();
    }

    cnn_load_wts(state, B, 0);
    cnn_load_iacts(state, A, 0);
    cnn_compute(state);

    cnn_get_oacts(state, C, 0);

}