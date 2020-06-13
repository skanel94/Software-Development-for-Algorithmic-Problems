#include "functions.h"
#include "input_functions.h"

// Function that parses the data from input file and stores them to an array of structs
segment **parse_dataset(char *input_file, int dataset_size)
{
	int i, j, way_ID , counter = 0;
	char line[LINE_LENGTH];
	char *data;
	
	segment *new_seg;
	segment **seg_array;
	
	// Open input file
	FILE *input = fopen(input_file, "r");
	
	// Check if input file has opened successfully
	if (input == NULL)
	{
		printf("Fopen: error opening input file!\n");
		exit(1);
	}
	
	// Allocate memory for array that keeps all segments
	seg_array = (segment**) malloc(dataset_size * sizeof(segment*));
	
	if (seg_array == NULL)
	{
		printf("Malloc: memory allocation error!\n");
		exit(3);
	}


	// For every single line..
	while (fgets(line, sizeof(line), input) != NULL)
	{		
			// Allocate memory for new segment
			new_seg = (segment*) malloc(sizeof(segment));
					
			if (new_seg == NULL)
			{
				printf("Malloc: memory allocation error!\n");
				exit(3);
			}
			
			// Update segment structure
			new_seg->seg_ID = atoi(strtok(line,","));		
			new_seg->way_ID = atoi(strtok(NULL,","));		
			new_seg->num_of_nodes = atoi(strtok(NULL,","));	
			
			new_seg->coordinates = (double**) malloc ( new_seg->num_of_nodes * sizeof(double*));
			
			if (new_seg->coordinates == NULL)
			{
				printf("Malloc: memory allocation error!\n");
				exit(3);
			}
			
			// Parse every single point of curve
			for (i = 0 ; i < new_seg->num_of_nodes ; i++)
			{	
				// Allocate memory for array to store each coordinate ( lat and lon )
				new_seg->coordinates[i] = (double*) malloc(2 * sizeof(double));
				
				if (new_seg->coordinates[i] == NULL)
				{
					printf("Malloc: memory allocation error!\n");
					exit(3);
				}
				
				new_seg->coordinates[i][0] = atof(strtok(NULL,","));	
				new_seg->coordinates[i][1] = atof(strtok(NULL,",")); 								
			}
			
			seg_array[counter] = new_seg;
			
			counter++;		
		}
		
		// Close input file
		fclose(input);
		
		return seg_array;
}


// Auxiliary function to get the dataset size
int get_dataset_size(char *input_file)
{
	int dataset_size = 0;
	char line[LINE_LENGTH];
	
	// Open input file
	FILE *input = fopen(input_file, "r");
	
	// Check if input file has opened successfully
	if (input == NULL)
	{
		printf("Fopen: error opening input file!\n");
		exit(1);
	}
	
	while (fgets(line, sizeof(line), input) != NULL)
		dataset_size++;
	
	// Close input file
	fclose(input);
	
	return dataset_size;
}
