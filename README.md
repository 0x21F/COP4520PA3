# PA3 

## Program 1 
Currently doesn't work. Concept was that it would use an optimistic linked list and pop from the beginning
which eliminates the issue of contains performance being somewhat bad.Main thread shuffles the letter numbers
and the index of the array is tracked with an atomic integer using fetch_and_add() which linearizes 
taking the value from the array. The head is an empty node that gets passed to each thread (if it were to work). 
The rest is a standard Optimistic Linked List implementation in C++ 

If you want to attempt to run this I'll make a makefile in the morning

## Program 2 
DNE



