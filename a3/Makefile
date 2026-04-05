.PHONY: all clean

FLAGS = -Wall -Wextra -g
LIBS = -lm -I/usr/include/mkl/fftw -lmkl_rt

all: eq

eq: main.o parse_file.o fftw_helper.o modify_data.o audio_presets.o
	gcc ${FLAGS} -o $@ $^ ${LIBS}

main.o: main.c
	gcc ${FLAGS} -c $< ${LIBS}

%.o: %.c %.h
	gcc ${FLAGS} -c $< ${LIBS}

clean:
	rm -f *.o eq