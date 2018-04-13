#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>

void fc(const int m_in, const int n_in, const int k_in, 
	const float *W_in, const float *I_in, const float *B_in, float* O_in);
void matmul(const int m_in, const int n_in, const int k_in, 
	const float *W_in, const float *I_in, float *O_in);
void add(const int n_in, float *a_in, float *b_in);
void sig_act(float *a_in, const int len);
void tanh_act(float *a, const int len);


//Single Layer of PiNN
double pinn_layer(){
    #ifndef MODEL_SIZE
        #error MODEL_SIZE must be defined
        //#define MODEL_SIZE 16
    #endif
    static const float tanh_weights [MODEL_SIZE*MODEL_SIZE] = {0.3};
    static const float sig_weights [MODEL_SIZE*MODEL_SIZE] = {0.3};
    static const float inter_weights [MODEL_SIZE*MODEL_SIZE] = {0.3};

    static const float sig_bias[MODEL_SIZE] = {0.2};
    static const float inter_bias[MODEL_SIZE] = {0.2};

    float sig_temp0[MODEL_SIZE]  = {0};
    float sig_temp[MODEL_SIZE]  = {0};
    float tanh_temp[MODEL_SIZE] = {0};
    float tanh_temp0[MODEL_SIZE] = {0};
    float inter_temp[MODEL_SIZE] = {0};

    fc(MODEL_SIZE, 1, MODEL_SIZE, sig_weights, sig_temp0, sig_bias, sig_temp);
    matmul(MODEL_SIZE, 1, MODEL_SIZE, tanh_weights, tanh_temp0, tanh_temp);
    fc(MODEL_SIZE, 1, MODEL_SIZE, inter_weights, tanh_temp, inter_bias, inter_temp);
    add(MODEL_SIZE, inter_temp, sig_temp);
    sig_act(sig_temp, MODEL_SIZE);
    tanh_act(tanh_temp, MODEL_SIZE);

    return tanh_temp[0];
    

}

int main(int argc, char *argv[]){
    char *p;
    errno = 0;
    long N = strtol(argv[1], &p, 10);
    if (errno != 0 || *p != '\0' || N > LONG_MAX){
        printf("Invalid N: %d %s\n", errno, argv[1]);
        return 1;
    }

    long model_size = strtol(argv[2], &p, 10);
    if (errno != 0 || *p != '\0' || model_size > LONG_MAX){
        printf("Invalid model_size: %d %s\n", errno, argv[2]);
        return 1;
    }
    

    FILE *fp;
    fp = fopen(argv[3], "a");
    if (fp == NULL){
        printf("Error opening file %s\n", argv[2]);
        return 1;
    }
    struct timeval start_t, end_t;
    gettimeofday(&start_t, NULL);
    long i;
    for (i = 0; i < N; i++){
        pinn_layer();
    }
    gettimeofday(&end_t, NULL);
    double elapsed = (double) (end_t.tv_usec - start_t.tv_usec)/1000000 + 
        (double) (end_t.tv_sec - start_t.tv_sec);
    fprintf(fp, "%ld, %8g\n", model_size, elapsed);
    fclose(fp);

    return 0;
}

