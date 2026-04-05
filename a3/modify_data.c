#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include "modify_data.h"
#define PI 3.14159265358979323846

/*
 * Data preparation methods (pre-FFT)
*/
double normalize(double i, int bit_depth){
    return i / (pow(2, bit_depth - 1));
}

double hann(double i, int size){
    return pow(sin(PI * i / (size-1)), 2);
}

double *prepare_data(double *data, int bit_depth, int size){ 
    for(int i = 0; i < size; i++){
        data[i] = normalize(data[i], bit_depth);
    }
    return data;
}

/*
 * Data modification methods (post-FFT)
*/

void amplify(int *amounts, fftw_complex* data, int size){
    for (int i = 0; i < size; i++) {
        if (i <= 20) {
            data[i][0] *= amounts[0];
            data[i][1] *= amounts[0];
        }
        else if (i <= 75) {
            data[i][0] *= amounts[1];
            data[i][1] *= amounts[1];
        }
        else if (i <= 500) {
            data[i][0] *= amounts[2];
            data[i][1] *= amounts[2];
        }
        else {
            data[i][0] *= amounts[3];
            data[i][1] *= amounts[3];
        }
    }
}