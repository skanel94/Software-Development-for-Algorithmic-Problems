#ifndef _HASHTABLE_
#define _HASHTABLE_

// Declare functions of hashtable.c file
hashtable **hashtable_init(int, int);

void hashtable_insert(hashtable *, int, node *);

void hashtable_destroy(hashtable **, int);

int **generate_array_r_classic(int, int);

int hash_function_LSH(double *, int, int, int *, int, double **);

double **generate_array_LSH(int, int);

node *hashtable_crossing(hashtable *, int);

void hashtable_print(hashtable *);

#endif