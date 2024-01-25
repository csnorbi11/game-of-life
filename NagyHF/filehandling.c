#include "filehandling.h"
#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "main.h"
#include <string.h>

void read_file(FILE* fp, char* filename, int** actual, int* size)
{
	printf("file reading\n");
	fp = fopen("bbbb.txt", "r");
	if (fp != NULL)
	{
		int i = 0;
		int j = 0;
		int num1;
		int num2;
		while (fscanf(fp, "%d,%d;", &num1, &num2)==2) {
			if(num1<size&&num2<size)
			actual[num1][num2] = 1;
		}
			
		fclose(fp);
		printf("end of file read\n");
	}
	else
	{
		perror("cannot open the file");
	}


	
	
}

void write_file(FILE* fp, char* filename, int** actual, int *size)
{
	fp = fopen("bbbb.txt", "w");
	if (fp != NULL)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (actual[i][j] == 1)
					fprintf(fp, "%d,%d;", i, j);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
	else
	{
		perror("cannot open the file");
	}
	
}