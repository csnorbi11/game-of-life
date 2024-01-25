#pragma once
#include <stdio.h>

#define FILEHANDLING_H


void read_file(FILE* fp, char* filename, int** actual, int *size);
void write_file(FILE* fp, char* filename, int** actual, int *size);