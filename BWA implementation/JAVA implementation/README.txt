THE JAVA IMPLEMENTATION

old implementation:
this implementation is not fully functional because we abandoned it in the middle of the work due to the fact that we understand the BWA algorithm and we wanted to move forward with the project and focus on the pairing part.
this implementation is working without gaps and misses.

new implementation:
the new java implement is working well, calculating the index and alignments and converting the hits into its actual places on the DNA string.
the java implementation take a dna file path and a reads file path (both are entered in the main) and print the results to a given hits file path (also entered in main).
the number of allowed misses can be changed (second parameter in the call to inexact match from the main)