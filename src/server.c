#include <stdio.h>

int check_args(int argc, char** argv);

int main(int argc, char* argv[]) 
{
    return 0;
}

// ./dataServer -p <port> -s <thread_pool_size> -q <queue_size> -b <block_size>
// Όπου:
// 1. port: Η θύρα στην οποία ο server θα ακούει για εξωτερικές συνδέσεις.
// 2. thread pool size: Ο αριθμός των worker threads στο thread pool.
// 3. queue size: Ο αριθμός θέσεων στην ουρά εκτέλεσης.
// 4. block size: Το μέγεθος των μπλοκ των αρχείων σε bytes που θα στέλνουν οι worker threads.