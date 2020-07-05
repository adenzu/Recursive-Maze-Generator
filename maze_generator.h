#ifndef MGPROTECTION
#define MGPROTECTION


#define PI 3.14159265359
#define DEG_RAD PI / 180


enum direction{UP, DOWN, RIGHT, LEFT};


int random_range(int l, int u);
void fill_array(int * array, int len);
int ** create_field(int width, int height);
int check_proceedable(int x, int y, enum direction d, int fw, int fh, int ** field);
void swap_directions(enum direction * da, int f, int s);
void shuffle_directions(enum direction * da, int len);
void draw_equilateral_shape_side_length(FILE * file, double x, double y, double side_length, int side_count);
void draw_field(char * field_name, int fw, int fh, int ** field);
int ** surround_field(int fw, int fh, int ** field);
enum direction * detect_possibilities(int x, int y, int fw, int fh, int ** field, int * possible_direction_count);
enum direction * get_direction_array_sample(enum direction * da, int len, int sample_len);
void wander_randomly(int x, int y, int fw, int fh, int ** field);
void print_field(int fw, int fh, int ** field);

#endif