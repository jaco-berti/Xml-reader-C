#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xml.h"

int main(int argc, char *argv[])
{
	char *buffer = NULL;

	int result = get_elements("sheet1.xml", "pageSetup", 1, &buffer);
	result = get_element_property(&buffer, result);

	if(result != -1)
	{
		printf("%s\n", buffer);
	}

	free(buffer);
}