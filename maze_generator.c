#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "maze_generator.h"

int random_range(int l, int u){
	return rand() % (u-l) + l;
}

void fill_array(int * array, int len){
	for(len--; len >= 0; len--) array[len] = 1;
}

int ** create_field(int width, int height){
	int ** new_field = (int **) calloc(height, sizeof(int *));
	for(height--; height >= 0; height--){
		new_field[height] = (int *) calloc(width, sizeof(int));
		fill_array(new_field[height], width);
	}
	return new_field;
}

int check_proceedable(int x, int y, enum direction d, int fw, int fh, int ** field){
	int result = 1;
	int * x_range, * y_range;
	int yl, xl;
	int cy, cx;
	int i, j;

	if(x >= 0 && x < fw && y >= 0 && y < fh){
		switch(d){
			case UP:
				y_range = (int *) calloc(yl = 2, sizeof(int));
				x_range = (int *) calloc(xl = 3, sizeof(int));
				x_range[0] = -1; x_range[1] = 0; x_range[2] = 1;
				y_range[0] = 0; y_range[1] = 1;
				break;
			case DOWN:
				y_range = (int *) calloc(yl = 2, sizeof(int));
				x_range = (int *) calloc(xl = 3, sizeof(int));
				x_range[0] = -1; x_range[1] = 0; x_range[2] = 1;
				y_range[0] = 0; y_range[1] = -1;
				break;
			case RIGHT:
				y_range = (int *) calloc(yl = 3, sizeof(int));
				x_range = (int *) calloc(xl = 2, sizeof(int));
				y_range[0] = -1; y_range[1] = 0; y_range[2] = 1;
				x_range[0] = 0; x_range[1] = 1;
				break;
			case LEFT:
				y_range = (int *) calloc(yl = 3, sizeof(int));
				x_range = (int *) calloc(xl = 2, sizeof(int));
				y_range[0] = -1; y_range[1] = 0; y_range[2] = 1;
				x_range[0] = 0; x_range[1] = -1;
				break;
		}

		for(j = 0; j < yl && result; j++){
			for(i = 0; i < xl && result; i++){
				cy = y+y_range[j]; cx = x+x_range[i];
				if(cy >= 0 && cx >= 0 && cy < fh && cx < fw && field[cy][cx] == 0) result = 0;
			}
		}
	}
	else result = 0;

	free(x_range);
	free(y_range);

	return result;
}

void swap_directions(enum direction * da, int f, int s){
	enum direction temp = da[f];
	da[f] = da[s];
	da[s] = temp;
}

void shuffle_directions(enum direction * da, int len){
	int i;
	for(i = 0; i < len-1; i++) swap_directions(da, i, random_range(i+1, len));
}

enum direction * detect_possibilities(int x, int y, int fw, int fh, int ** field, int * possible_direction_count){
	int i = 0;
	enum direction * possible_directions = (enum direction *) calloc(4, sizeof(enum direction));
	if(check_proceedable(x, y+1, UP, fw, fh, field)) possible_directions[i++] = UP;
	if(check_proceedable(x, y-1, DOWN, fw, fh, field)) possible_directions[i++] = DOWN;
	if(check_proceedable(x+1, y, RIGHT, fw, fh, field)) possible_directions[i++] = RIGHT;
	if(check_proceedable(x-1, y, LEFT, fw, fh, field)) possible_directions[i++] = LEFT;
	*possible_direction_count = i;
	return possible_directions;
}

enum direction * get_direction_array_sample(enum direction * da, int len, int sample_len){
	
	int i;
	enum direction * sample_array = (enum direction *) calloc(sample_len, sizeof(enum direction));
	shuffle_directions(da, len);
	for(i = 0; i < sample_len; i++) sample_array[i] = da[i];
	return sample_array;
}

void wander_randomly(int x, int y, int fw, int fh, int ** field){

	int possible_direction_count;
	enum direction * possible_directions = detect_possibilities(x, y, fw, fh, field, &possible_direction_count);

	if(possible_direction_count){
		int chosen_direction_count = random_range(1, possible_direction_count+1), i;
		enum direction * chosen_directions;

		if(chosen_direction_count == possible_direction_count) chosen_directions = possible_directions;
		else chosen_directions = get_direction_array_sample(possible_directions, possible_direction_count, chosen_direction_count);
		

		for(i = 0; i < chosen_direction_count; i++){
			switch(chosen_directions[i]){
				case UP:	field[y+1][x] = 0; break;
				case DOWN:	field[y-1][x] = 0; break;
				case RIGHT:	field[y][x+1] = 0; break;
				case LEFT:	field[y][x-1] = 0; break;
			}
		}

		for(i = 0; i < chosen_direction_count; i++){
			switch(chosen_directions[i]){
				case UP:	wander_randomly(x, y+1, fw, fh, field); break;
				case DOWN:	wander_randomly(x, y-1, fw, fh, field); break;
				case RIGHT:	wander_randomly(x+1, y, fw, fh, field); break;
				case LEFT:	wander_randomly(x-1, y, fw, fh, field); break;
			}
		}

		free(chosen_directions);
	}
	free(possible_directions);
}

int ** surround_field(int fw, int fh, int ** field){

	int i,j;

	int nfw = fw + 2;
	int nfh = fh + 2;

	int ** new_field = (int **) calloc(nfh, sizeof(int *));

	for(i = 0; i < nfh; i++){
		new_field[i] = (int *) calloc(nfw, sizeof(int));
		for(j = 0; j < nfw; j++){
			if(!(i*j) || i == fh + 1 || j == fw + 1) new_field[i][j] = 1;
			else new_field[i][j] = field[i-1][j-1];
		}
	}

	return new_field;
}

void print_field(int fw, int fh, int ** field){

	int i;
	for(i = 0; i < fw+2; i++) printf("#");
	printf("\n");
	for(fh--; fh >= 0; fh--){
		printf("#");
		for(i = 0; i < fw; i++){
			if(field[fh][i]) printf("#");
			else printf(" ");
		}
		printf("#\n");
	}
	for(i = 0; i < fw+2; i++) printf("#");
	printf("\n");
	printf("\n");
}

void draw_equilateral_shape_side_length(FILE * file, double x, double y, double side_length, int side_count){

	int i;

	double curr_angle;
	double angle_per_side = 360.0f / side_count;
	double radius = side_length / (2 * sin(angle_per_side / 2 * DEG_RAD));

	fprintf(file, "%lf %lf moveto\n", x + radius * cos(DEG_RAD * (angle_per_side / 2 - 90)), y + radius * sin(DEG_RAD * (angle_per_side / 2 - 90)));

	for(i = 1; i < side_count; i++){
		curr_angle = angle_per_side * i + angle_per_side / 2 - 90;
		curr_angle *= DEG_RAD;
		fprintf(file, "%lf %lf lineto\n", x + radius * cos(curr_angle), y + radius * sin(curr_angle));
	}

	fprintf(file, "closepath\n");
	fprintf(file, "fill\n");
	fprintf(file, "stroke\n");
}

void draw_field(char * field_name, int fw, int fh, int ** field){

	int i, j;

	int ** drawn_field = surround_field(fw, fh, field);
	fw += 2;
	fh += 2;

	double side_length = 5;
	double half_side_len = side_length / 2;
	double width = fw * side_length;
	double height = fh * side_length;

	FILE * file = fopen(field_name, "w");
	fprintf(file, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(file, "%%%%BoundingBox: 0 0 %lf %lf\n", width, height);

	for(j = 0; j < fh; j++){
		for(i = 0; i < fw; i++){
			if(drawn_field[j][i]) draw_equilateral_shape_side_length(file, half_side_len + i * side_length, half_side_len + j * side_length, side_length, 4);
		}
	}

	fclose(file);

}