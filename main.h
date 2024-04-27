#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <chrono>
#include <time.h>

using namespace std;
using namespace chrono;

queue <int> buffer;
string outfile;
sem_t semEmpty;
sem_t semFull;
sem_t sem_mutex;
sem_t sem_outfile;
int askcount = 0;
int recievecount = 0;
int completecount = 0;
int sleepcount = 0;
int workcount = 0;
auto start = high_resolution_clock::now();
FILE *ptr;
int n = 0;
int TransSave = 0;
int *c;