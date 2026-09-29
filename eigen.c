#include <math.h>
#include "linalg.c"

typedef struct eigen {
matrix* vector;
float value;
}eigen;


float Lf(matrix*x, matrix*A, float s){
    // matrix*m=matrix_prod(A,x);
    // m->name="m";
    // matrix_print(A->cols, A->rows,A->data, A->name);
    // matrix_print(x->cols, x->rows,x->data, x->name);
    // matrix_print(m->cols, m->rows, m->data, m->name);

    // exit(0);
    float L = matrix_norm(matrix_minus(
            matrix_prod(A,x),
            matrix_prod(
                matrix_scalar(A->cols, A->rows,s),
                A)
    ));
    for (int i = matrix_pool_size-1; i > matrix_pool_size - 4; i--) {
        // free(matrix_pool[i]);
    }

    matrix_pool_size -= 4;
    return L;

}

eigen* matrix_eigen(matrix*A){

    eigen *e = malloc(sizeof(eigen));
    e->vector = malloc(sizeof(float)*A->cols);

    matrix* x = matrix_random(1,A->cols);
    float s = (float)(rand())/(float)RAND_MAX;

    float L;
    float Lb;
    float La;

    int t = 0;
    float rate = 5.;
    printf("start: hill climbing\n");
    epoch:
    t++; if(t >= 250) goto end;
    //hill climb
    // printf("step %i\n",t);

    float ds = rate*( 1.-2.*(float)rand()/(float)RAND_MAX);
    for (int i = 0; i < x->cols*x->rows; i++) {
        float dx = rate*( 1.-2.*(float)rand()/(float)RAND_MAX);

        Lb = Lf(x, A, s);
        x->data[i] += dx;
        La = Lf(x, A, s);
        if(La > Lb) x->data[i] -= dx;
    }

    Lb = Lf(x, A, s);
    s += ds;
    La = Lf(x, A, s);
    if(La > Lb) s -= ds;

    printf("t(%i) -> L=%f\n",t,L);

    calc_L:

        L = matrix_norm(matrix_minus(
                matrix_prod(A,x),
                matrix_prod(
                    matrix_scalar(A->cols, A->rows,s),
                    A)
        ));
    goto epoch;

    end:
    printf("training end\n");
    e->vector = matrix_new(1, A->rows);
    e->vector->data = x->data;
    e->value = s;
    return e;
}

int main(void){
    matrix*A = matrix_scalar(2, 2, 1);
    eigen*e=matrix_eigen(A);

    matrix_print(1, 2, e->vector->data, "eigen vector:");
    matrix_print(1, 2, matrix_prod(A,e->vector)->data, "Ax");
    printf("eigen value: %f\n",e->value);


    // matrix_pool_destroy();
    for (int i = 0; i < matrix_pool_size; i++) {
        matrix*m = matrix_pool[i];
        // matrix_print(m->cols,m->rows,m->data,m->name);
        free(m);
    }
    printf("freed matrix_pool[%i]",matrix_pool_size);
}
