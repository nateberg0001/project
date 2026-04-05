#include <fftw3.h>

static fftw_complex *in, *out;
static fftw_plan p_forward, p_backward;
static int N = 2048; // number of samples to take for FFT

void initialize(int size){
    N = size;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

    // Create a plan for FFT (optimizes the calculations)
    p_forward = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_MEASURE);
    p_backward = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_MEASURE);
}

fftw_complex* fft_execute(int starting_pos, double* frame){
    for (int i = 0; i < N; i++) {
        in[i][0]= frame[starting_pos+i];
        in[i][1]= (double) 0;
    }
    fftw_execute(p_forward); 
    return out;
}

fftw_complex* ifft_execute(fftw_complex* frame){
    for (int i = 0; i < N; i++) {
        in[i][0]= frame[i][0];
        in[i][1]= frame[i][1];
    }
    fftw_execute(p_backward); 
    return out;
}

void deinitialize(){
    fftw_destroy_plan(p_forward);
    fftw_destroy_plan(p_backward);
    fftw_free(in); 
    fftw_free(out);
}

fftw_complex* execute_fft_specific(int size, double* last_frame){
    fftw_complex* in_specific = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex* out_specific = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

    fftw_plan q_forward = fftw_plan_dft_1d(N, in_specific, out_specific, FFTW_FORWARD, FFTW_ESTIMATE);
        for (int i = 0; i < N; i++) {
        in_specific[i][0]= last_frame[i];
        in_specific[i][1]= (double) 0;
    }

    fftw_execute(q_forward); 
    fftw_destroy_plan(q_forward);
    fftw_free(in_specific); 
    return out_specific;
}

fftw_complex* execute_ifft_specific(int size, fftw_complex* last_frame){
    fftw_complex* in_specific = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    fftw_complex* out_specific = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

    fftw_plan q_back = fftw_plan_dft_1d(N, in_specific, out_specific, FFTW_BACKWARD, FFTW_ESTIMATE);
        for (int i = 0; i < N; i++) {
        in_specific[i][0] = last_frame[i][0];
        in_specific[i][1] = last_frame[i][1];
    }

    fftw_execute(q_back); 
    fftw_destroy_plan(q_back);
    fftw_free(in_specific); 
    return out_specific;
}