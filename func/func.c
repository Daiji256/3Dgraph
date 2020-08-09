#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
	FILE *fp;
	char fname[32];
	int i, j, xnum, ynum;
	double x, y, z, xmin, xmax, xint, ymin, ymax, yint;
	double plot[100][100][3];

	sprintf(fname, "%s.csv", argv[1]);
	fp = fopen(fname, "w");

	xmin = atof(argv[2]); xmax = atof(argv[3]); xint = atof(argv[4]);
	ymin = atof(argv[5]); ymax = atof(argv[6]); yint = atof(argv[7]);

	x = xmin;
	for (i = 0; i < 1024; i++)
	{
		y = ymin;
		for (j = 0; j < 1024; j++)
		{
			z = x*x - y*y;
			plot[i][j][0] = x;
			plot[i][j][1] = y;
			plot[i][j][2] = z;
			if (y > ymax) break;
			y += yint;
		}
		if (x > xmax) break;
		x += xint;
	}
	xnum = i; ynum = j;
	fprintf(fp, "# %d, %d\n", xnum, ynum);
	for (i = 0; i < xnum; i++)
	{
		for (j = 0; j < ynum; j++)
		{
			fprintf(fp, "%f, %f, %f\n", plot[i][j][0], plot[i][j][1], plot[i][j][2]);
		}
	}

	fclose(fp);

	return 0;
}
