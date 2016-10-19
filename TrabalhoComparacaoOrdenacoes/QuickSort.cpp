//
//  QuickSort.cpp
//  TrabalhoComparacaoOrdenacoes
//
//  Created by Yan Mendes on 30/09/16.
//  Copyright © 2016 Yan Mendes. All rights reserved.
//

#include "QuickSort.hpp"

string QuickSort::getName(){
    stringstream name;
    name << "Quick Sort - " << this->pivot;
    
    return name.str();
};

string QuickSort::getInferiorLimit(string instance_type){
    return "O(n log n)";
}

string QuickSort::getSuperiorLimit(string instance_type){
    return !strcmp(instance_type.c_str(), "ascending") ? "O(n^2)" : "O(n log n)";
}

int QuickSort::getSmallestConstant(string instance_type, int * A, int n){
    return !strcmp(instance_type.c_str(), "ascending") ?
        floor(this->getCount() / (n * n)) :
        floor(this->getCount() / (n * log2(n)));
}

/**
 * Class constructor that requires which version of
 * the partition algorithm will be executed.
 *
 * @param partitionMethod (int)
 */
QuickSort::QuickSort(int partitionMethod){
    switch (partitionMethod) {
        case 1:
            this->pivot = 1;
            this->current_pivot_choice_method = &QuickSort::biggestFirstTwo;
            break;
        case 2:
            this->pivot = 2;
            this->current_pivot_choice_method = &QuickSort::biggestFirstMiddle;
            break;
        default:
            this->pivot = 3;
            this->current_pivot_choice_method = &QuickSort::biggestRandom;
            break;
    }
}

int QuickSort::biggestFirstTwo(int *A, int start, int end){
    incrementCount(1);
    if(A[start] > A[end])
        return start;
    
    return end;
}

int QuickSort::biggestFirstMiddle(int *A, int start, int end){
    int middle = floor((start + end) / 2);
    
    incrementCount(1);
    if(A[start] > A[middle])
        return start;
    
    return middle;
}


int QuickSort::biggestRandom(int *A, int start, int end){
    int firstRandom = (start + rand()) % end;
    int secondRandom = (start + rand()) % end;
    
    incrementCount(1);
    if(A[firstRandom] > A[secondRandom])
        return firstRandom;
    
    return secondRandom;
}

/**
 * Particionamento
 *
 * @param A    (int*)  Shuffled array
 * @param start (int)  Relative first element of the array
 * @param end   (int)  Relative last element of the array
 *
 */
int QuickSort::partition(int * A, int start, int end){
    int i = start, j = end;
    int pivot = (this->*current_pivot_choice_method)(A, start, end);
    
    while (i <= j) {
        while (A[i] < pivot)
            i++;
        while (A[j] > pivot)
            j--;
        if (i <= j) {
            swap(A[i], A[j]);
            i++;
            j--;
        }
    };
    
    return i;
}

/**
 * Quick Sort
 *
 * @param A (int*) Shuffled array
 * @param n (int)  Number of elements of said array
 */
void QuickSort::sortAlg(int * A, int n){
    this->quickSort(A, 0, n - 1);
}

/**
 * Método recursivo auxiliar à construção do quicksort
 *
 * @param A    (int*)  Shuffled array
 * @param start (int)  Relative first element of the array
 * @param end   (int)  Relative last element of the array
 */
void QuickSort::quickSort(int * A, int start, int end){
    if(start < end){
        int index = partition(A, start, end);
    
        incrementCount(1);
        
        if (start < index - 1)
            quickSort(A, start, index - 1);
        if (index < end)
            quickSort(A, index, end);
    }
}
