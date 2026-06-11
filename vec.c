#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec.h"


int push_str(char ***vec, int size, const char *str)
{
	char **temp = NULL;

	if(*vec == NULL)
	{
		temp = (char **)malloc((size + 1) * sizeof(char *));
	}
	else
	{
		temp = (char**)realloc(*vec, (size + 1) * sizeof(char *));
	}

	if (temp == NULL) return 0;

	*vec = temp;
	(*vec)[size] = (char*)malloc((strlen(str) + 1) * sizeof(char));
	strcpy((*vec)[size], str);

	return size + 1;
}

int pop_str(char ***vec, int size)
{
	if (*vec == NULL) return 0;

	free((*vec)[size - 1]);
    if (size == 1) {
        free(*vec);
        *vec = NULL;
        return 0;
    }

	if(size < 1) return 0;

	char **temp = (char **)realloc(*vec, (size - 1) * sizeof(char *));
	if(temp == NULL) return 0;
	*vec = temp;
	return size - 1;
}