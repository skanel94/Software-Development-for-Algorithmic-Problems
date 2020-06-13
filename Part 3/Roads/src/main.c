#include "functions.h"
#include "input_functions.h"
#include "clustering_init.h"
#include "clustering_assignment.h"
#include "clustering_update.h"
#include "silhouette.h"
#include "output_functions.h"

// execution command: ./roads -i sample.csv -function DTW

int main(int argc, char* argv[])
{	
	int i, j, k_cluster, best_k_cluster = 0, metric_function = 0, dataset_size;
    int *centroids, *centroids_old;
	double sil, best_sil = -INFINITY;
	double **distances_array;
	char input_file[PATH_LENGTH] = "NULL", user_input[16];
	time_t  clustering_time, start, end;
	
	segment **seg_array;
    cluster **clusters, **best_clusters;

	
    // Parse arguments from command line
    if (argc >= 1 && argc <= 5)									
    {
        for (i = 1 ; i <= (argc-1) ; i++)
        {
            if (!strcmp(argv[i], "-i")) // If flag is -i ..
            {
                stpcpy(input_file, argv[++i]); // Copy of argument's value to var
                continue;
            }
			if (!strcmp(argv[i], "-function")) // If flag is -function ..
            {
                if (!strcmp(argv[++i], "Frechet")) // Check if metric is "DFT"
                {
                    metric_function = FRECHET; 
                }
                else if (!strcmp(argv[i], "DTW")) // or if it's "DTW"
                {
                    metric_function = DTW; 
                }
                else	//Otherwise .. the input is wrong!
                {
                    printf("Wrong metric function argument! \n");
                    return -1;
                }

                continue;
            }
            // If flag of any argument is wrong .. exit!
            printf("Wrong input arguments! \n");
            return -2;
        }
    }
    else	// If number of arguments is wrong .. exit!
    {
        printf("Wrong number of arguments! \n");
        return -3;	
    }

    // Parse input from user
    if (!strcmp(input_file, "NULL"))
    {
        printf("Please insert the path to input file: ");
        scanf("%s", input_file);
    } 
	
	if (!metric_function)
    {
        printf("Please insert the metric function: ");
        scanf("%s", user_input);

        if (!strcmp(user_input, "Frechet")) // Check if metric is "DFT"
        {
            metric_function = FRECHET; 
        }
        else if (!strcmp(user_input, "DTW")) // or if it's "DTW"
        {
            metric_function = DTW; 
        }
        else	//Otherwise .. the input is wrong!
        {
            printf("Wrong metric function argument! \n");
            return -1;
        }      
    } 
	
	

	// Seed random number generator 
	srand(time(NULL));

	// Parse data from input file		
	dataset_size = get_dataset_size(input_file);

	seg_array = parse_dataset(input_file, dataset_size);

	printf("\tCreating distances's array\n\n");
	distances_array = create_distances_array(seg_array, dataset_size, metric_function);

	// Clustering ..	
	for (k_cluster = dataset_size/4 ; k_cluster > 2 ; k_cluster/=2)
	{
		start = time(NULL);
		
		printf("---Perform clustering with %d clusters---\n", k_cluster);
		// Clustering
		centroids_old = (int*) malloc(k_cluster * sizeof(int));
		
		if (centroids_old == NULL)
		{
			printf("Malloc: memory allocation error/n");
			exit(3);
		}
		
		
		// Initialize centroids
		centroids = Init_1(seg_array, k_cluster, dataset_size, distances_array);	

		// Assign conformations to nearest cluster
		clusters = Assignment_1(seg_array, k_cluster, centroids, dataset_size, distances_array);

		// Update centroids
		PAM(seg_array, centroids, clusters, k_cluster, distances_array);

		do
		{
			for (i = 0 ; i < k_cluster ; i++)			
				centroids_old[i] = centroids[i];

			// Assign conformations to nearest cluster
			clusters = Assignment_1(seg_array, k_cluster, centroids, dataset_size, distances_array);

			// Update centroids
			PAM(seg_array, centroids, clusters, k_cluster, distances_array);
			
		}while(centroids_transposition(seg_array, centroids, centroids_old, k_cluster, distances_array) != k_cluster);
		
		end = time(NULL);
		
		// Calculate silhouette for current cluster
		sil = silhouette(seg_array, centroids, clusters, k_cluster, distances_array);
		
		printf("\tSilhouette: %f\n\n", sil);
		
		// Keep cluster with the best silhouette
		if (sil > best_sil)
		{
			// Free previous
			for(i = 0 ; i < best_k_cluster ; i++)
			{
				free(best_clusters[i]->segs_in_cluster);
				free(best_clusters[i]);
			}
			free(best_clusters);
			
			
			best_sil = sil;
			best_k_cluster = k_cluster;
			clustering_time = end - start;
			
			best_clusters = (cluster**) malloc(best_k_cluster * sizeof(cluster*));
			
			if (best_clusters == NULL)
			{
				printf("Malloc: memory allocation error/n");
				exit(3);
			}
			
			for (i = 0 ; i < best_k_cluster ; i++)
			{
				best_clusters[i] = (cluster*) malloc(sizeof(cluster));
				
				if (best_clusters[i] == NULL)
				{
					printf("Malloc: memory allocation error/n");
					exit(3);
				}
				
				best_clusters[i]->cluster_size = clusters[i]->cluster_size;
				
				best_clusters[i]->segs_in_cluster = (int*) malloc(best_clusters[i]->cluster_size *sizeof(int));
				
				if (best_clusters[i]->segs_in_cluster == NULL)
				{
					printf("Malloc: memory allocation error/n");
					exit(3);
				}
				
				for (j = 0 ; j < best_clusters[i]->cluster_size ; j++)
					best_clusters[i]->segs_in_cluster[j] = clusters[i]->segs_in_cluster[j];
			}
		}
		
		for(i = 0 ; i < k_cluster ; i++)
		{
			free(clusters[i]->segs_in_cluster);
			free(clusters[i]);
		}
		free(clusters);
		
		free(centroids);
		free(centroids_old);	
	}
	
	
	// Write results to output file
	if (metric_function == FRECHET)
		output("kmeans_ways_frechet.dat", best_clusters, best_k_cluster, best_sil, clustering_time);
	else
		output("kmeans_ways_dtw.dat", best_clusters, best_k_cluster, best_sil, clustering_time);
	
	
	// Free allocated memory for distances array
	for (i = 0 ; i < dataset_size ; i++)
		free(distances_array[i]);
	free(distances_array);
		
	// Free allocated memory for conformations array
	for (i = 0; i < dataset_size; i++ )
	{
		for ( j = 0; j < seg_array[i]->num_of_nodes; j++)
			free(seg_array[i]->coordinates[j]);
			
		free(seg_array[i]->coordinates);
		free(seg_array[i]);
	}
	free(seg_array);	
	
	return 0; 
}
