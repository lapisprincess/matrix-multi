# Multithreaded Matrix Multiplication
### Assignment for CSCI 240, Operating Systems, at the University of Puget Sound, Tacoma, WA

My implementation of a multithreaded matrix multiplicaton algorithm.

My initial attempt had me trying to write a binary space partitioning algorithm to give each thread its own sub-matrix.
However, due to time limitations, I decided to stick with seperating the thread work by rows instead of partitions.
