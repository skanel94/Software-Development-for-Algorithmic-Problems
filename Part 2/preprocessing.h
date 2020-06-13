#ifndef _PREPROCESSING_
#define _PREPROCESSING_

// Declare functions of preprocessing.c file
hashtable **preprocessing(char *, int, int);

dataset_info *get_dataset_information(char *);

config *get_config_information(char *);

int min_points_in_dataset(char *);

int max_points_in_dataset(char *);

int number_of_curves_in_dataset(char *);

double **generate_array_grid_t(int, int, double);

double rand_gaussian();

#endif