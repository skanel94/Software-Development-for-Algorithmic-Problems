#ifndef _HASHTABLE_
#define _HASHTABLE_

// Declare functions of metric_functions.c file
double max_2(double, double);

double min_3(double, double, double);

double euclidean(double *, double *, int);

double frechet_distance(double **, double **, int, int, int);

double DTW_distance(double **, double **, int, int, int);

#endif