those implementation are not fully functional because we abandoned them in the middle of the work due to the fact that we understand the BWA algorithm and we wanted to move forward with the project and focus on the pairing part.
the java implementation is working without gaps and misses.
the new java implement is working well, calculating the index and alignments and converting the hits into its actual places on the DNA string.
the java implementation take a dna file path and a reads file path (both are entered in the main) and print the results to a given hits file path (also entered in main).
the number of allowed misses can be changed (second parameter in the call to inexact match from the main)