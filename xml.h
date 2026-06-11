#ifndef DEF
#define DEF

int getlines(FILE *file, char **buffer, int max_len, int count_lines);
int get_elements(const char *xml_file_name, const char *element, int element_count, char **buffer);
int get_element_property(char **element, int size);
int get_property_values(char **property, int p_size, char ***vec, int v_size)

#endif