#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xml.h"
#include "vec.h"

void clear_buffer(char **buffer, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		(*buffer)[i] = '\0';
	}
}

int is_isolate_symbol(char *array, int pos)
{
	if(
		(array[pos-1] == '"' || array[pos-1] == '\'') ||
		(array[pos+1] == '"' || array[pos+1] == '\'')
	)
		return 0;
	else
		return 1;
}

int is_right_isolate_symbol(char *array, int pos)
{
	if((array[pos+1] == '"' || array[pos+1] == '\''))
		return 0;
	else
		return 1;
}

int strflip(char **str)
{
    if (str == NULL || *str == NULL) return 1; 

    int i, j = 0;
    int len = strlen(*str);

    char *flipped = (char*) malloc((len + 1) * sizeof(char));
    if (flipped == NULL) return 2;

    for(i = len - 1; i >= 0; i--, j++)
    {
        flipped[j] = (*str)[i];
    }

    flipped[j] = '\0';

    free(*str);

    *str = flipped;

    return 0;
}

int get_elements(const char *xml_file_name, const char *element, int element_count, char **buffer)
{
    FILE *file = fopen(xml_file_name, "r");
    if (file == NULL) return -1;
    char *lines = (char*) malloc(1024 * sizeof(char));
    int chars_len = getlines(file, &lines, 1024, -1);
    if (chars_len == 0)
    {
    	free(lines);
    	fclose(file);
    	return -1;
    }
    fclose(file);

    int ret_len = 0;
    int i, tlc, bc = 0, j = 0, start, grab = 0, count_minors;
    int element_chars_count = strlen(element);
    char *temp_line = (char*) malloc(element_chars_count * sizeof(char));
    *buffer = (char*) realloc(*buffer, chars_len + 1);
    clear_buffer(buffer, chars_len + 1);

    for (i = 0; i < chars_len; i++)
    {
    	if(element_count != -1)
    	{
    		if(element_count == 0) break;
    	}

    	if(grab)
    	{
    		if(lines[i] == '<' && is_isolate_symbol(lines, i))
    		{
    			count_minors++;
    		}	
    		else if (lines[i] == '>' && is_right_isolate_symbol(lines, i))
    		{
    			if(lines[i-1] == '/')
    			{
    				if(count_minors == 1)
    				{
    					element_count--;
    					grab = 0;
    				}
    			}
    			else if(i != (element_chars_count + 1))
    			{
    				tlc = 0;
		    		for(j = i - 1; j >= i - element_chars_count; j--, tlc++)
					{
						temp_line[tlc] = lines[j];
					}

					if(strflip(&temp_line) == 0)
					{
						if(strcmp(temp_line, element) == 0)
						{
							if(lines[j] == '/' && lines[j - 1] == '<')
							{
								element_count--;
								grab = 0;
							}
						}
					}
    			}
    		}

    		(*buffer)[bc] = lines[i];
    		// printf("%c", (*buffer)[bc]);
    		bc++;
    		ret_len++;
    	}
    	else if(lines[i] == '<' && is_isolate_symbol(lines, i))
    	{
    		tlc = 0;
    		for(j = i + 1; j < (i + element_chars_count) + 1; j++, tlc++)
			{
				temp_line[tlc] = lines[j];
			}

			temp_line[tlc] = '\0';

			if(strcmp(temp_line, element) == 0)
			{
				if(lines[j] == ' ' || lines[j] == '>')
				{
					grab = 1;
					i--;
					count_minors = 0;
				}
			}
    	}
    }
    
    (*buffer)[chars_len] = '\0';
    free(lines);
    return ret_len;
}

int getlines(FILE *file, char **buffer, int max_len, int count_lines)
{
	if(count_lines != -1)
	{
		if (count_lines < 0) return 0;
	}

	int len = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF && (count_lines > 0 || count_lines == -1))
    {
    	if(count_lines != -1)
    	{
    		if(ch == '\n') count_lines--;
    	}

        if (len >= max_len)
        {
            max_len += 100;
            char *n_ptr = realloc(*buffer, max_len);
            if (n_ptr == NULL) {
                fclose(file);
                return 2;
            }
            *buffer = n_ptr;
        }

        (*buffer)[len] = (char)ch;
        len++;
    }

    (*buffer)[len] = '\0';

    return len;
}

int get_element_property(char **element, int size)
{
	int i = 0, can_count = 0, count = 0;

	if(size < 1) return -1;

	for(i = 0; i < size; i++)
	{
		if(can_count) count++;

		if((*element)[i] == '<' && is_isolate_symbol(*element, i))
		{
			can_count = 1;
		}
		if((*element)[i] == '>' && is_right_isolate_symbol(*element, i))
		{
			i = size;
		}
	}

	char *temp = (char*) realloc(*element, (count * sizeof(char) + 1));
	if(temp != NULL) *element = temp;

	(*element)[count + 1] = '\0';

	return count;
}

// int get_property_values(char **property, int p_size, char ***vec, int v_size)
// {
// 	int i;
// 	for(i = 0; i < p_size; i++)
// 	{
		
// 	}
// }