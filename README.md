# PA3 

## Program 1 
Currently doesn't work. Concept was that it would use an optimistic linked list and pop from the beginning
which eliminates the issue of contains performance being somewhat bad.Main thread shuffles the letter numbers
and the index of the array is tracked with an atomic integer using fetch_and_add() which linearizes 
taking the value from the array. The head is an empty node that gets passed to each thread (if it were to work). 
The rest is a standard Optimistic Linked List implementation in C++ 

If you want to attempt to run this I'll make a makefile in the morning

## Program 2 
Doesn't quite work. Idea was to have a shared time variable only being incremented by the main thread. 
The other threads would be synchronized to this thread, and store their temperatures accordingly. This
program however does not store the recorded values on a minute by minute basis since the spec's 
minimum required time for the calculations is 10 minutes. We can cut down on the search space y keeping
two tables of temperatures, one for min and max temp, which allows us to compute the greatest temperature 
difference during the 10 minute period, while reducing the search space for the 1 hr report for the 
highest/lowest temperature by a magnitude of 10. 
