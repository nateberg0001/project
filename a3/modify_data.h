#ifndef MODIFY_DATA_H
#define MODIFY_DATA_H

// Function declarations (prototypes)
double hann(double i, int size);
double normalize(double i, int bit_depth);
double *prepare_data(double* data, int bit_depth, int size);
void amplify(int * amounts, fftw_complex * data, int size);
#endif