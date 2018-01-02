#include <inttypes.h>

void iniCloserFurther(double *, double *, int);

void closerFurther(double *, double *, double *, double *,
                   int, double, int,
                   int *, int *, double *);

void iniGuessValues(double *, double *, int, double *, double *, double *);

void guessValues(double *, double *, int, double, uint32_t, double *, double *, double *);


void generateValuesA(int, uint32_t, double *);
void generateValuesB(int, uint32_t, double *);
void generateValuesC(double *, double *, int, uint32_t, double *);

int64_t getLayerValuesCnt(int, int);
