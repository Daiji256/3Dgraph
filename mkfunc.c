#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void mkfile(const char *fname, const char *equation)
{
	FILE *fp;
	char fnamec[32];

	sprintf(fnamec, "%s.c", fname);

	fp = fopen(fnamec, "w");
	if (fp == NULL)
	{
		printf("%s file not open.\n", fnamec);
		exit(EXIT_FAILURE);
	}

	fprintf(
		fp,
		"#include <stdio.h>\n"
		"#include <stdlib.h>\n"
		"#include <math.h>\n"
		"\n"
		"int main(int argc, char **argv)\n"
		"{\n"
		"\tFILE *fp;\n"
		"\tchar fname[32];\n"
		"\tint i, j, xnum, ynum;\n"
		"\tdouble x, y, z, xmin, xmax, xint, ymin, ymax, yint;\n"
		"\tdouble plot[100][100][3];\n"
		"\n"
		"\tsprintf(fname, \"%s\", argv[1]);\n"
		"\tfp = fopen(fname, \"w\");\n"
		"\n"
		"\txmin = atof(argv[2]); xmax = atof(argv[3]); xint = atof(argv[4]);\n"
		"\tymin = atof(argv[5]); ymax = atof(argv[6]); yint = atof(argv[7]);\n"
		"\n"

		"\tx = xmin;\n"
		"\tfor (i = 0; i < 1024; i++)\n"
		"\t{\n"
		"\t\ty = ymin;\n"
		"\t\tfor (j = 0; j < 1024; j++)\n"
		"\t\t{\n"
		"\t\t\tz = %s;\n"
		"\t\t\tplot[i][j][0] = x;\n"
		"\t\t\tplot[i][j][1] = y;\n"
		"\t\t\tplot[i][j][2] = z;\n"
		"\t\t\tif (y > ymax) break;\n"
		"\t\t\ty += yint;\n"
		"\t\t}\n"
		"\t\tif (x > xmax) break;\n"
		"\t\tx += xint;\n"
		"\t}\n"
		"\txnum = i; ynum = j;\n"
		"\tfprintf(fp, \"%s\", xnum, ynum);\n"
		"\tfor (i = 0; i < xnum; i++)\n"
		"\t{\n"
		"\t	for (j = 0; j < ynum; j++)\n"
		"\t	{\n"
		"\t		fprintf(fp, \"%s\", plot[i][j][0], plot[i][j][1], plot[i][j][2]);\n"
		"\t	}\n"
		"\t}\n"
		"\n"
		"\tfclose(fp);\n"
		"\n"
		"\treturn 0;\n"
		"}\n",
		"%s.csv",
		equation,
		"# %d, %d\\n",
		"%f, %f, %f\\n"
	);

	fclose(fp);
}

void compile(const char *fname)
{
	char command[128];

	sprintf(command, "gcc %s.c -lm -o %s\n", fname, fname);
	system(command);
}

void mkcsv(const char *fname, double xmin, double xmax, double xint, double ymin, double ymax, double yint)
{
	char command[128];

	sprintf(command, "%s %s %f %f %f %f %f %f\n", fname, fname, xmin, xmax, xint, ymin, ymax, yint);
	system(command);
}

void mkfunc(const char *fname, const char *equation, double xmin, double xmax, double xint, double ymin, double ymax, double yint)
{
	mkfile(fname, equation);
	compile(fname);
	mkcsv(fname, xmin, xmax, xint, ymin, ymax, yint);
}
