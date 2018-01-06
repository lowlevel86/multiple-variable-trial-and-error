#include <inttypes.h>

void iniCloserFurther(double *, double *, int);

void closerFurther(double *, double *, double *, double *,
                   int, double,
                   int *, int *, double *);

void iniGuessValues(double *, double *, int, double *, double *, double *);

void guessValues(double *, double *, int, double, uint32_t, double *, double *, double *);


void generateValues(int, uint32_t, double *);

void generateRangedValues(double *, double *, int, uint32_t, double *);

int64_t sumCurrentAndPriorLayers(int, int);
