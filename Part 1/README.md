# Overview Part 1

## Hashing and Search for polygonal curves

Our program gets as input a file which contains two curves of 3 or 4 dimensions each. For each curve, we create K curve-grids,
which are concatenated to create the key for our hash function (classic or probabilistic). Next, we put the curve into the hashtable
according to the index which was returned from our hash function. The aforementioned procedure is repeated L-times. Therefore, we get
our final search structure.

Next, our program gets as input a file with curves as before, but now the aim is to apply search. The curves have to be converted into
a curve-grid, like before, and according to the indexes have been returned from our hash function, the program will search the corres-
ponding buckets of the hashtable. From those buckets, the program checks only the curves that are represented with the same curve-grid.
A distance will be calculated between them according a distance metric that the user has chosen at the beggining of the program.

In case that this specific curve cannot be found in this bucket, the algorithm is going to calculate the distance among the asked curve
and all the curves of this bucket to find out the closest one. In the end, the program finds out the actual closest curve to the asked one
applying exhaustive search.

The results are exported into a file that the user defines also at the begging of the program.


The code comprises an implementation of the paper "Locality-Sensitive Hashing of Curves” των A. Driemel and F.
Silvestri" in the context of our module.
