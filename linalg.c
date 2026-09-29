typedef enum bool {true=1, false = !true}bool;

#include <stdio.h>
#include <stdlib.h>

// typedef struct vector {
//     float* data;
//     void (*append)(float);
//     int length;
// }vector;

// typedef struct vector {
//     float* data;
//     void (*append)(float);
//     int rows;
//     int columns;
// }vector;





float matrix_get(int i, int j, int cols, float*data, bool transposed){
    // if (transposed) return (data[i + j*rows]);
    return (data[j + i*cols]);
}
void matrix_set(int i, int j, int cols, float*data, float value){
    data[j + i*cols] = value;
}


void matrix_info(int cols, int rows, char*name){
    printf("%s(%ix%i)\n",name,rows,cols);
}

void matrix_print(int cols, int rows, float*data, char* name){
    printf("%s(%ix%i)\n",name,rows,cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2f ", matrix_get(i,j,cols,data,false));
        }
            printf("\n");

    }
    printf("\n");

}

typedef struct matrix {
    int cols;
    int rows;
    float*data;
    float (*get)(int, int, int, float*,bool);
    void (*set)(int, int, int, float*, float);
    void (*print)(int, int, float*, char*);
    void(*info)(int,int,char*);
    //
    int serial;
    char*name;
    bool transposed;

} matrix;

matrix* matrix_pool[1000]; int matrix_pool_size = 0;

matrix* matrix_new(int cols, int rows){
    matrix* m = malloc(sizeof(matrix));

    m->cols = cols;
    m->rows = rows;
    m->data = malloc(sizeof(float)*rows*cols);

    matrix_pool[matrix_pool_size] = m;
    matrix_pool_size++;

    m->get = matrix_get;
    m->set = matrix_set;
    m->print = matrix_print;
    m->info = matrix_info;
    return m;

};


matrix* matrix_random(int cols, int rows){
    matrix*m = matrix_new(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j<cols ; j++) {
            m->set(i,j,cols,m->data,(float)rand()/(float)RAND_MAX);
        }
    }
    return m;

}

matrix* matrix_prod(matrix*A, matrix*B){
    if( A->cols != B->rows) {

        printf("dimension missmatch in matrix_prod(%s %s)\n",A->name,B->name);
        A->info(A->cols, A->rows,A->name);
        B->info(B->cols,B->rows,B->name);
        exit(1);
    }

    matrix*R = matrix_new(A->rows, B->cols);
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {

        float s = 0;

        for (int k = 0; k < A->cols; k++) {
            s += A->get(i,k,A->cols, A->data,true) * B->get(k,j,B->cols,B->data,true);
        }
        R->set(i,j,B->cols,R->data,s);

        }
    }
    return R;
    }

matrix* matrix_from_array(int cols, int rows, float** array){
    matrix*m=matrix_new(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->set(i,j,cols, m->data, array[i][j]);
        }
    }
}

matrix* matrix_identity(int d){

}

matrix* matrix_scalar(int cols, int rows, float s){
    matrix*m = matrix_new(cols,rows);
    for(int i = 0; i < rows; i ++){
        for(int j = 0; j < cols; j ++)
        {
            m->set(i,j,cols,m->data, s);
        }
    }
    return m;
}



matrix* matrix_minus(matrix*A,matrix*B){
    matrix* R = matrix_new(A->cols, B->rows);
    for (int i = 0; i < A->cols*A->rows; i++) {
        R->data[i] = A->data[i] - B->data[i];
    }
    return R;
}


matrix* matrix_plus(matrix*A,matrix*B){
    matrix* R = matrix_new(A->cols, B->rows);
    for (int i = 0; i < A->cols*A->rows; i++) {
        R->data[i] = A->data[i] + B->data[i];
    }
    return R;
}


float matrix_norm(matrix*A){
    float r = 0.;
    for (int i = 0; i < A->cols*A->rows; i++) {
        r += A->data[i]*A->data[i];
    }
    return sqrtf(r);
}
//test
// int main(void){

//     // printf("what");
//     float arr[3][3] = {
//         {1, 0, 0},
//         {0, 1, 0},
//         {0, 0, 1},
//     };

//     matrix*A = matrix_random(2, 2);
//     matrix*R = matrix_prod(A, A);
//     A->print(2,2,A->data,A->name);
//     A->print(2,2,R->data,R->name);

//     return 0;
// }
