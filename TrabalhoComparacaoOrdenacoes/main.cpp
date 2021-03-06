//
//  main.cpp
//  TrabalhoComparacaoOrdenacoes
//
//  Created by Yan Mendes on 30/09/16.
//  Copyright © 2016 Yan Mendes. All rights reserved.
//

#include "Reader.hpp"

//Algorithms
#include "CountingSort.hpp"
#include "RadixSort.hpp"
#include "HeapSort.hpp"
#include "BubbleSort.hpp"
#include "MergeSort.hpp"
#include "InsertionSort.hpp"
#include "SelectionSort.hpp"
#include "QuickSort.hpp" ///ERRO

bool debug            = false;
string inputFolder    = "";
int partitionMethod   = 1;
int mergeMethod       = 1;
Reader * r            = new Reader();
list<string> inputFiles;
int * A;
SortAlgorithm * algorithm;
string instance_type;

string getCurrentTimeStamp(){
    stringstream ss;

    time_t t = time(0);
    struct tm * now = localtime( & t );
    ss << now->tm_mday << '/' << now->tm_mon + 1 << '/' <<  now->tm_year + 1900 << ' '
    << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << endl;

    return ss.str();
}

static void usage(){
    cout << "Usage:\n" <<
    "    ./SortTest [options] [algorithm] \n\n" <<
    "Options:\n" <<
    "    -h                     Show this help\n" <<
    "    --debug                Print Shuffled Vector before the sorted one\n" <<
    "    --input-folder=value   Path to the input folder\n" <<
    "    --merge-type=value     When using merge sort, choose the merge algorithm [1: Classic (dynamically allocating an array in the merge method), 2: Alocating beforehand] \n" <<
    "    --partition=value      When using quick sort, choose the partition method [1: First and second, 2: First and middle, 3: Random] \n" <<
    "Algorithm:\n" <<
    "    bubble          Bubble Sort\n" <<
    "    counting        Counting Sort\n" <<
    "    heap            Heap Sort\n" <<
    "    insertion       Insertion Sort\n" <<
    "    merge           Merge Sort\n" <<
    "    quick           Quick Sort\n" <<
    "    radix           Radix Sort\n" <<
    "    selection       Selection Sort\n";
}

int processArgs(int argc, const char * argv[]){

    if(argc < 2){
        usage();
        return 2;
    }

    int argInd;

    for(argInd = 1; argInd < argc; argInd++) {

        if (argv[argInd][0] != '-')
            break;

        if (!strcmp(argv[argInd], "-h")) {
            usage();
            return 2;
        } else if (!strncmp(argv[argInd], "--input-folder=", 15)) {
            inputFolder = &argv[argInd][15];
        } else if (!strcmp(argv[argInd], "--debug")) {
            debug = true;
        } else if (!strncmp(argv[argInd], "--partition=", 12)) {
            partitionMethod = atoi(&argv[argInd][12]);
        } else if (!strncmp(argv[argInd], "--merge-type=", 13)) {
            mergeMethod = partitionMethod = atoi(&argv[argInd][13]);
        }
    }

    if (argInd != argc - 1) {
        usage();
        return 1;
    }

    Helper *h = new Helper();
    inputFiles = h->getFilesInDirectory(inputFolder);
    if(inputFolder.empty() || inputFiles.empty()){
        cout << "Invalid folder name or empty directory" <<endl;
        return 3;
    }

    if(!strcmp (argv[argInd], "bubble")) {
        algorithm = new BubbleSort();
    } else if(!strcmp (argv[argInd], "counting")) {
        algorithm = new CountingSort();
    } else if(!strcmp (argv[argInd], "heap")) {
        algorithm = new HeapSort();
    } else if(!strcmp (argv[argInd], "insertion")) {
        algorithm = new InsertionSort();
    } else if(!strcmp (argv[argInd], "merge")) {
        algorithm = new MergeSort(mergeMethod);
    } else if(!strcmp (argv[argInd], "quick")) {
        algorithm = new QuickSort(partitionMethod);
    } else if(!strcmp (argv[argInd], "radix")) {
        algorithm = new RadixSort();
    } else if(!strcmp (argv[argInd], "selection")) {
        algorithm = new SelectionSort();
    } else {
        cout << "Invalid Algorithm!!" << endl;
        usage();
        return 4;
    }

    return 0;
}

static void set_instance_type(string file)
{
    if (file.find("O") != string::npos)
        instance_type = "ascending";
    else if (file.find("R") != string::npos)
        instance_type = "descending";
    else if (file.find("UD") != string::npos)
        instance_type = "shuffled with duplicates";
    else if (file.find("U") != string::npos)
        instance_type = "shuffled";
}

void printArray(int * A, int n, fstream& outputFile){
    for(int i = 0; i < n; ++i){
        outputFile << A[i] << endl;
    }
}

int main(int argc, const char * argv[]) {
    int errors;

    if((errors = processArgs(argc, argv)))
        return errors;

    Helper h;

    cout << "Selected Algorithm: " << algorithm->getName() << endl;
    while(!inputFiles.empty()){
        string file = inputFiles.back();
        inputFiles.pop_back();
        cout << file << endl;
        int n = r->read(file, &A);

        set_instance_type(file);

        file.replace(file.end() - 3, file.end(), "_" + algorithm->getName() + "_output.txt");

        fstream outputFile;

        outputFile.open(file, fstream::out);

        outputFile << "Running test" << " @ " << getCurrentTimeStamp() << endl;
        outputFile << "########TEST PARAMS########" << endl;
        outputFile << "Instance identifier: " <<
            h.explode(file.replace(file.end() - 11, file.end(), ""), '/').back() << endl;
        outputFile << "Array Size: " << n << endl;
        outputFile << "Instance type: " << instance_type << endl;
        outputFile << "Algorithm: " << algorithm->getName() << endl;
        outputFile << "Inferior theorical limit: " << algorithm->getInferiorLimit(instance_type) << endl;
        outputFile << "Superior theorical limit: " << algorithm->getSuperiorLimit(instance_type) << endl;

        if(debug){
            outputFile << "########BEGINING DEBUG########" << endl;
            outputFile << "########SHUFFLED ARRAY########" << endl;
            printArray(A, n, outputFile);
        }

        clock_t before_sort = clock();
        algorithm->sort(A, n);
        clock_t after_sort  = clock();

        if(debug){
            outputFile << "########SORTED ARRAY########" << endl;
            printArray(A, n, outputFile);
            outputFile << "########ENDING DEBUG########" << endl;
        }

        outputFile << "Operations Count: " << algorithm->getCount() << endl;
        outputFile << "Smallest constant: " << algorithm->getSmallestConstant(instance_type, A, n) << endl;
        outputFile << "Time elapsed: " << after_sort - before_sort << " ms" << endl;
        //outputFile << "Time elapsed: " << ((double) (after_sort - before_sort)) / CLOCKS_PER_SECOND << " s" << endl;

        outputFile.close();

        delete [] A;
    }

    return 0;
}
