#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze_generator.h"

int main(){
	srand(time(0));
	int fw, fh;
	printf("enter width height:\n");
	scanf("%d %d", &fw, &fh);
	int ** f = create_field(fw,fh);
	f[0][0] = 0;
	wander_randomly(0, 0, fw, fh, f);
	//print_field(fw,fh,f);
	draw_field("generated_maze.eps", fw, fh, f);
	printf("done\n");
	return 0;
}