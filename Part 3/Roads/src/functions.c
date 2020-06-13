#include "functions.h"
#include "cRMSD.h"
#include "metric_functions.h"

// Function that returns True if a specific conformation is centroid
int is_centroid(int *centroids, int index, int size)
{
    int i;
    
    for (i = 0 ; i < size ; i++)
        if (centroids[i] == index)
            return 1;
        
    return 0;
}


// Function that generates a random number following Gaussian Distribution
double rand_gaussian()
{
	double r, y1, y2;

	do
	{
		y1 = (rand() / (RAND_MAX + 1.0)) * 2.0;
		y2 = (rand() / (RAND_MAX + 1.0)) * 2.0;  
		r = y1*y1 + y2*y2;	
	}
	while (r >= 1);

	return y1 * pow(((pow(r,-2) - 1)/r) , 1/2);
}


// Function that calculates the number of centroids that haven't been transposed ( or remotely transposed ) after an update 
int centroids_transposition(segment **seg_array, int *centroids, int *centroids_old, int k_cluster, double **distances_array)
{
	int i, counter = 0;

	// for every single cluster
	for(i = 0 ; i < k_cluster ; i++)
		if (distances_array[centroids[i]][centroids_old[i]] < THRESHOLD)
			counter++;
	
			
	return counter;
}


// Auxiliary function that creates an array to store distances among every conformation
double **create_distances_array(segment **seg_array, int dataset_size, int metric_function)
{
	int i, j, z;
	double distance;
	double **array, **tmp_coordinates;
	
	array = (double**) malloc(dataset_size * sizeof(double*));
	
	if (array == NULL)
	{	
		printf("Malloc: memory allocation error!\n");
		exit(3);
	}
	
	for (i = 0 ; i < dataset_size ; i++)
	{
		array[i] = (double*) malloc(dataset_size * sizeof(double));
			
		if (array[i] == NULL)
		{	
			printf("Malloc: memory allocation error!\n");
			exit(3);
		}
	}
	
	// Calculate distances among conformations
	for (i = 0 ; i < dataset_size ; i++)
	{
		// Set main diagonial of distance array to 0
		array[i][i] = 0.0;
			
		// Fill in the lower left triangle part of array
		for (j = 0 ; j < i ; j++)
		{
			if (seg_array[i]->num_of_nodes == seg_array[j]->num_of_nodes)
				distance = cRMSD(seg_array[i]->coordinates, seg_array[j]->coordinates, seg_array[i]->num_of_nodes, metric_function);
				
			else if (seg_array[i]->num_of_nodes > seg_array[j]->num_of_nodes)
			{
				tmp_coordinates = (double**) malloc (seg_array[j]->num_of_nodes * sizeof(double*));
				
				for(z = 0; z < seg_array[j]->num_of_nodes; z++)
					tmp_coordinates[z] = seg_array[i]->coordinates[z];
					
				distance = cRMSD(tmp_coordinates, seg_array[j]->coordinates, seg_array[j]->num_of_nodes, metric_function);
				
			}
			
			else
			{
				tmp_coordinates = (double**) malloc (seg_array[i]->num_of_nodes * sizeof(double*));
				
				for(z = 0; z < seg_array[i]->num_of_nodes; z++)
					tmp_coordinates[z] = seg_array[j]->coordinates[z];
					
				distance = cRMSD(seg_array[i]->coordinates, tmp_coordinates, seg_array[i]->num_of_nodes, metric_function);
				
			}
			
			// Fill in the rest array ( upper right )
			array[i][j] = distance;
			array[j][i] = distance;
		}
	}

	return array;
}
