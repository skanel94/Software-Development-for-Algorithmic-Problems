typedef struct _segment_
{
	int seg_ID;
	int way_ID;		// The way that this segment belongs to
	int num_of_nodes;	
	double **coordinates;
	
}segment;

typedef struct _cluster_
{
    int cluster_size;
    int *segs_in_cluster;
}cluster;
