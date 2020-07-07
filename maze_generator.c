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
		y_range = (int *) calloc(3, sizeof(int));
		x_range = (int *) calloc(3, sizeof(int));
		switch(d){
			case UP:
				yl = 2; xl = 3;
				x_range[0] = -1; x_range[1] = 0; x_range[2] = 1;
				y_range[0] = 0; y_range[1] = 1;
				break;
			case DOWN:
				yl = 2; xl = 3;
				x_range[0] = -1; x_range[1] = 0; x_range[2] = 1;
				y_range[0] = 0; y_range[1] = -1;
				break;
			case RIGHT:
				yl = 3; xl = 2;
				y_range[0] = -1; y_range[1] = 0; y_range[2] = 1;
				x_range[0] = 0; x_range[1] = 1;
				break;
			case LEFT:
				yl = 3; xl = 2;
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
		free(x_range);
		free(y_range);
	}
	else result = 0;

	return result;
}

void fill_field_seperately(int fw, int fh, int ** field, int dx, int dy){

	int i, j, k;

	int times_w = fw / dx;
	int times_h = fh / dy;

	int upper_limit_y;
	int upper_limit_x;

	for(j = 0; j < times_h; j++){
		for(i = 0; i < times_w; i++){

			if((upper_limit_x = (i + 1) * dx) > fw) upper_limit_x = fw;
			if((upper_limit_y = (j + 1) * dy) > fh) upper_limit_y = fh;

			wander_randomly(dx/2 + i * dx, dy/2 + j * dy, upper_limit_x, upper_limit_y, field);
		}
	}
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

enum direction * get_directions(){
	enum direction * new_directions_p = (enum direction *) calloc(4, sizeof(enum direction));
	new_directions_p[0] = UP;
	new_directions_p[1] = DOWN;
	new_directions_p[2] = RIGHT;
	new_directions_p[3] = LEFT;
	return new_directions_p;
}

void wander_randomly(int x, int y, int fw, int fh, int ** field){

	int i, nx, ny;
	enum direction * pd_pointer = get_directions();
	shuffle_directions(pd_pointer, 4);

	field[y][x] = 0;

	for(i = 0; i < 4; i++){
		nx = x; ny = y;

		switch(*pd_pointer){
			case UP:	ny++;	break;
			case DOWN: 	ny--;	break;
			case RIGHT: nx++;	break;
			case LEFT: 	nx--;
		}

		if(check_proceedable(nx, ny, *pd_pointer++, fw, fh, field)) wander_randomly(nx, ny, fw, fh, field);
	}
	free(pd_pointer);
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

void draw_field(char * field_name, double side_length, int fw, int fh, int ** field){

	int i, j;

	int ** drawn_field = surround_field(fw, fh, field);
	fw += 2;
	fh += 2;

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