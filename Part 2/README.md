# Overview Part 2

## Implementation of K-means/ K-medoids for polygonal curves

Initially, our program constructs a search method for our hashtables and reads the required elements from the config file (e.g. number
of clusters). The main function clustering() is called. According to the metric function that the user chooses we have taked into account
in case of Dynamic Time Warping (DTW) not to use Mean Discrete Frechet curve. 

There are 3 groups of functions in order to implement the clustering behaviour.
* Functions (Init_1(), Init_2()) for initialising the centers of the clusters.
* Functions (Assignment_1 (), Assignment_2 ()) for assigning each curve to the closest cluster-center.
* Functions (PAM (), Mean_Discrete_Frechet ()) for updating the centers of the clusters.

The clustering procedure is terminated when the cluster-centers either converged (stop moving) or their translation was lower than
a given threshold. After the procedure is finished, the shilhoutte both of all elements and  the elements of each cluster is calculated.
The results are exported into a file which is defined by the user at he beggining of the program.

## Program Compilation
A makefile is included. Run the command "./make" in the directory of the code. The command "./makeclean" cleans the folder from the 
Object files that have been created by "./make" command. In order to compile the program run:
* ./cluster –i {input file} –c {configuration file} -o {output file} -d {metric}
