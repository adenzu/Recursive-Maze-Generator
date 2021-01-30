#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze_generator.h"

int main(){
	srand(time(0));

	int fw, fh;
	int dx, dy;
	double side_len;

	printf("enter width, height:\n");
	scanf("%d %d", &fw, &fh);

	printf("enter dx, dy:\n");
	scanf("%d %d", &dx, &dy);

	printf("enter side length for the square:\n");
	scanf("%lf", &side_len);

	int ** f = create_field(fw, fh);
	//wander_randomly(fh/2, fw/2, fw, fh, f); // <--- much better at creating mazes, but recursive thus limited
	fill_field_seperately(fw, fh, f, dx, dy); // <--- worse at creating mazes but practically able to create infinite size

	draw_field("generated_maze_samples/generated_maze.eps", side_len, fw, fh, f);
	printf("done\n");


	return 0;
}
