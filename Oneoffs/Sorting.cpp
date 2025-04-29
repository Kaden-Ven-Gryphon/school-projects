
#include <iostream>
#include <fstream>

#include <cstdlib>
#include <ctime>


bool LESS(double a, double b);
void Quicksort(double* A, int start, int end);
int Partion(double* A, int start, int end);
void swap(double* a, double* b);
void Mergesort(double* A, int end);
void Merge(double* B, int sizeB, double* C, int sizeC, double* A);
int Partion(double* A, int start, int end);


int comparison_count = 0;           //The goblal coutner for comparisons, used in merge() and Partion()

using namespace std;


int main()
{
    int numOfElements = 0;          //will be gotten from file

    double* quickInput;             //the pointer for the quick array
    double* mergeInput;             //The pointer for the merge array

    ifstream infile;                //input file pointer
    ofstream outfile;               //output file pointer


    /* Used to Create random test files

    
    outfile.open("input.txt");
    outfile << 100 << endl;
    srand((unsigned)time(0));

    for (int i = 0; i < 100; i++)
    {
        int randnum = (rand() % 100) + 1;
        outfile << randnum << endl;
    }
    outfile.close();

    */


    infile.open("input.txt");                   //open the input file
    infile >> numOfElements;                    //get the number of elemntes

    quickInput = new double[numOfElements];     //create arrays for the two sort methods
    mergeInput = new double[numOfElements];     //with a number of elements goten from the file

    for (int i = 0; i < numOfElements; i++)     //loop though the number of elements in the file
    {
        infile >> quickInput[i];                //get the entry from the file and add it to quick sort array
        mergeInput[i] = quickInput[i];          //get the entry from quick and add it to merge
    }

    infile.close();                             //close the file


    Mergesort(mergeInput, numOfElements);       //call merge sort


    outfile.open("kb0125_merge.txt");           //open the output file for merge sort


   
    for (int i = 0; i < numOfElements; i++)     //loop thorugh the number of elements
    {
        outfile << mergeInput[i] << endl;       //add the array to the file
    }
   

    outfile << endl << "Comparisons: " << comparison_count;     //add the number of comparesons
    outfile.close();                                            //close the file

    comparison_count = 0;                       //reset the golbal coutner

    Quicksort(quickInput, 0, numOfElements);    //call quick sort

    outfile.open("kb0125_quick.txt");           //open the quick sort output file

    for (int i = 0; i < numOfElements; i++)     //loop though the number of elements
    {
        outfile << quickInput[i] << endl;       //print the arrat to the file
    }


    outfile << endl << "Comparisons: " << comparison_count;     //add the number of comparesons
    outfile.close();                                            //close the file
    
}//END OF MAIN


//takes two doubles
//outputs a bool: true it a is less than b
//also adds one to the comparison_count
bool LESS(double a, double b)
{
    comparison_count++;
    if (a < b) return true;
    return false;
}

//Takes a pointer to an array and a strat and end index
void Quicksort(double* A, int start, int end)
{
    if (start < end-1)                          //if the array has more than one element
    {
        int split = Partion(A, start, end-1);   //partition the array
        Quicksort(A, start, split);             //call quick sort on the first half
        Quicksort(A, split + 1, end);           //call quick sort on the second half
    }
}

//used by quicksort
//take an array pointer and and start and end index
//returns the index of the pivot element
int Partion(double* A, int start, int end)
{
    int pivot = A[end];                 //get the value of the pivot, the last element ofthe array
    int i = (start - 1);                //set up the left index

    for (int j = start; j < end ; j++)  //set up the right index
    {
        if (LESS(A[j], pivot))          //if the element at J is less than the pivot
        {
            i++;                        //put the element at j to the left of i
            swap(&A[i], &A[j]);             
        }
    }                                   //other wise just increment j
    swap(&A[i + 1], &A[end]);           //swap the elemnt one past i with the pivot
    return i + 1;                       //return the index of the pivot
}

//takes two array pointers, can be he same array
//swaps the two elements
void swap(double* a, double* b)
{
    double t = *a;
    *a = *b;
    *b = t;
}

//takes a poitner to an array and the length of the array
void Mergesort(double* A, int end)
{
    if (end > 1)                                //if there is more than one element inthe array
    {
        int sizeB = end / 2;                    //size of the new left array
        int sizeC = ceil(double(end / 2.0));    //size of the new right array, take the ceil in case of odd numbers of elements
        double* B = new double[sizeB];          //create a new array for the left
        double* C = new double[sizeC];          //create an new array for the right

        int j = 0;                              //array index for the original array

        for (int i = 0; i < sizeB; i++)         //loop through the new left array
        {
            B[i] = A[j++];                      //copy the first half of the original array into the new left array
        }
        for (int i = 0; i < sizeC; i++)         //loop through the new right array
        {
            C[i] = A[j++];                      //copy the second half of the original array into the new right array
        }

        Mergesort(B, sizeB);                    //call mergesort on the new left side
        Mergesort(C, sizeC);                    //call mergesort on the new right side

        Merge(B, sizeB, C, sizeC, A);           //merger the two now sorted arrays

    }
}

//used by mergesort
//takes two array pointers, and the size of each array
//also takes an array pointer for the two arrays to merge into
//the two input arrays must already be sorted
void Merge(double* B, int sizeB, double* C, int sizeC, double* A)
{
    int i = 0;                      //pointer for hte left array
    int j = 0;                      //pointer for the right array
    int k = 0;                      //pointer for hte combined array

    while (i < sizeB && j < sizeC)  //while both poitners for the two input arrays are within their arrays
    {
        if (LESS(B[i], C[j]))       //if the element in the left array is less than the right array
        {
            A[k] = B[i++];          //add it to the combined array and incrment the left array pinter
        }
        else
        {
            A[k] = C[j++];          //else take the element from the right array and added to the combvined array
        }
        k++;                        //increment the combined array pointer
    }
    if (i == sizeB)                 //if there are no more elements in the left array
    {
        while (j < sizeC)           //copy the rest of the right array in to the combined array
        {
            A[k++] = C[j++];
        }
    }
    else                            //else the right array has run out of elements
    {
        while (i < sizeB)           //copy the rest of the left array inthe combined array
        {
            A[k++] = B[i++];
        }
    }

    delete B;                       //DELETE THE MEMORY USED FOR THE INTERMEDITE ARRAYS TO AVOID MEMORY LEAK
    delete C;                       //I only remebered that I needed to add this while adding comments right before I turned it in.
}