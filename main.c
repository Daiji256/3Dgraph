#include "eval.h"
#include "wbmp.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>

/********************************
	マクロ定義
********************************/

#define PI 3.14159265359
#define DATANUM 512
#define MOUSE_LEFT_BUTTON 0
#define MOUSE_MIDDLE_BUTTON 1
#define MOUSE_RIGHT_BUTTON 2

/********************************
	関数のプロトタイプ宣言
********************************/

void display(void);
void zRGB(GLdouble z, GLdouble rgb[3]);
void makeTics(void);
void wire3Dgraph(void);
void solid3Dgraph(void);
void plotGraph(void);
void saveImage(void);
void resize(int w, int h);
void keyin(unsigned char key, int x, int y);
void motionActive(int x, int y);
void motionPassive(int x, int y);
void mouse(int button, int state, int x, int y);
void init(void);

/********************************
	グローバル変数
********************************/

/* グラフのパラメータ */
char g_func[STR_LENGTH];
GLdouble g_xmin, g_xmax, g_xint, g_ymin, g_ymax, g_yint, g_zmin, g_zmax;
int g_xnum = 41, g_ynum = 41;
GLdouble g_plot[DATANUM][DATANUM][3], g_plotRGB[DATANUM][DATANUM][3];

/* カメラの視点e，注視点a，カメラの上方向u */
GLdouble g_ex, g_ey, g_ez;
GLdouble g_ax = 0.0, g_ay = 0.0, g_az = 0.0;
GLdouble g_ux = 0.0, g_uy = 0.0, g_uz = 1.0;

/* グラフの描画サイズ */
GLdouble g_size = 700.0;
GLdouble g_xscale = 1.0, g_yscale = 1.0, g_zscale = 1.0;

/* グラフや目盛りの有無 */
unsigned char g_wire = 1, g_solid = 1, g_tics = 1;

/* マウスの角度 */
int g_mx, g_my;
double g_mt_s, g_mp_s, g_mt = 3 * PI / 4, g_mp = PI / 4;

/********************************
	関数の定義
********************************/

/* 描画コールバック関数 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		g_ex + (g_xscale * g_ax), g_ey + (g_yscale * g_ay), g_ez + (g_zscale * g_az),
		g_xscale * g_ax, g_yscale * g_ay, g_zscale * g_az,
		g_ux, g_uy, g_uz
	);
	glScaled(g_xscale, g_yscale, g_zscale);

	if (g_solid == 1) solid3Dgraph();
	if (g_wire == 1) wire3Dgraph();
	if (g_tics == 1) makeTics();

	glutSwapBuffers();
}

/* zの値を色で表す関数 */
void zRGB(GLdouble z, GLdouble rgb[3])
{
	GLdouble zsize, tmp0, tmp1;

	zsize = g_zmax - g_zmin;
	tmp0 = (z - g_zmin) / zsize;
	tmp1 = tmp0 - 0.8;
	rgb[0] = exp(- tmp1 * tmp1 * 6);
	tmp1 = tmp0 - 0.4;
	rgb[1] = exp(- tmp1 * tmp1 * 6);
	tmp1 = tmp0;
	rgb[2] = exp(- tmp1 * tmp1 * 6);
}

/* 目盛りを定義する関数 */
void makeTics(void)
{
	int i, j, num;
	char str[16];
	GLdouble x, y, z, dz, rgb[3];
	GLdouble s_mtt, c_mtt;
	GLdouble tmp_x, tmp_y, /* tmp_z, */tmp_zx, tmp_zy;
	GLdouble tmp_z1_x, tmp_z1_y, tmp_z2_x, tmp_z2_y;

	glLineWidth(2);

	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(g_xmin, g_ymin, g_zmin); glVertex3d(g_xmax, g_ymin, g_zmin);
	glVertex3d(g_xmin, g_ymax, g_zmin); glVertex3d(g_xmax, g_ymax, g_zmin);
	glEnd();

	glColor3d(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3d(g_xmin, g_ymin, g_zmin); glVertex3d(g_xmin, g_ymax, g_zmin);
	glVertex3d(g_xmax, g_ymin, g_zmin); glVertex3d(g_xmax, g_ymax, g_zmin);
	glEnd();

	glColor3d(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(g_xmin, g_ymin, g_zmin); glVertex3d(g_xmin, g_ymin, g_zmax);
	glVertex3d(g_xmax, g_ymin, g_zmin); glVertex3d(g_xmax, g_ymin, g_zmax);
	glVertex3d(g_xmax, g_ymax, g_zmin); glVertex3d(g_xmax, g_ymax, g_zmax);
	glVertex3d(g_xmin, g_ymax, g_zmin); glVertex3d(g_xmin, g_ymax, g_zmax);
	glEnd();

	s_mtt = sin(g_mt);
	c_mtt = cos(g_mt);
	if ((s_mtt > 0) && (c_mtt <= 0))
	{
		tmp_x = g_xmax + 0.05 / g_xscale;
		tmp_y = g_ymin - 0.10 / g_yscale;
		tmp_zx = g_xmax - 0.175 * c_mtt / g_xscale;
		tmp_zy = g_ymax + 0.175 * s_mtt / g_yscale;
		tmp_z1_x = g_xmax - 0.05 * c_mtt / g_xscale;
		tmp_z1_y = g_ymax + 0.05 * s_mtt / g_yscale;
		tmp_z2_x = g_xmax - 0.15 * c_mtt / g_xscale;
		tmp_z2_y = g_ymax + 0.15 * s_mtt / g_yscale;
	}
	else if ((s_mtt > 0) && (c_mtt > 0))
	{
		tmp_x = g_xmax + 0.10 / g_xscale;
		tmp_y = g_ymax + 0.05 / g_yscale;
		tmp_zx = g_xmin - 0.175 * c_mtt / g_xscale;
		tmp_zy = g_ymax + 0.175 * s_mtt / g_yscale;
		tmp_z1_x = g_xmin - 0.05 * c_mtt / g_xscale;
		tmp_z1_y = g_ymax + 0.05 * s_mtt / g_yscale;
		tmp_z2_x = g_xmin - 0.15 * c_mtt / g_xscale;
		tmp_z2_y = g_ymax + 0.15 * s_mtt / g_yscale;
	}
	else if ((s_mtt <= 0) && (c_mtt > 0))
	{
		tmp_x = g_xmin - 0.05 / g_xscale;
		tmp_y = g_ymax + 0.10 / g_yscale;
		tmp_zx = g_xmin - 0.175 * c_mtt / g_xscale;
		tmp_zy = g_ymin + 0.175 * s_mtt / g_yscale;
		tmp_z1_x = g_xmin - 0.05 * c_mtt / g_xscale;
		tmp_z1_y = g_ymin + 0.05 * s_mtt / g_yscale;
		tmp_z2_x = g_xmin - 0.15 * c_mtt / g_xscale;
		tmp_z2_y = g_ymin + 0.15 * s_mtt / g_yscale;
	}
	else if ((s_mtt <= 0) && (c_mtt <= 0))
	{
		tmp_x = g_xmin - 0.10 / g_xscale;
		tmp_y = g_ymin - 0.05 / g_yscale;
		tmp_zx = g_xmax - 0.175 * c_mtt / g_xscale;
		tmp_zy = g_ymin + 0.175 * s_mtt / g_yscale;
		tmp_z1_x = g_xmax - 0.05 * c_mtt / g_xscale;
		tmp_z1_y = g_ymin + 0.05 * s_mtt / g_yscale;
		tmp_z2_x = g_xmax - 0.15 * c_mtt / g_xscale;
		tmp_z2_y = g_ymin + 0.15 * s_mtt / g_yscale;
	}

	glColor3d(0, 0, 0);

	x = g_xmin;
	for (i = 0; i < 5; i++)
	{
		glRasterPos3d(x, tmp_y, g_zmin);
		num = sprintf(str, "%.2f", x);
		for (j = 0; j < num; j++) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[j]);
		x += (g_xmax - g_xmin) / 4;
	}

	y = g_ymin;
	for (i = 0; i < 5; i++)
	{
		glRasterPos3d(tmp_x, y, g_zmin);
		num = sprintf(str, "%.2f", y);
		for (j = 0; j < num; j++) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[j]);
		y += (g_ymax - g_ymin) / 4;
	}

	if (g_solid == 0)
	{
		tmp_zx = tmp_z1_x;
		tmp_zy = tmp_z1_y;
	}

	z = g_zmin;
	for (i = 0; i < 5; i++)
	{
		glRasterPos3d(tmp_zx, tmp_zy, z);
		num = sprintf(str, "%.2f", z);
		for (j = 0; j < num; j++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[j]);
		}
		z += (g_zmax - g_zmin) / 4;
	}

	if (g_solid == 0)
	{
		return;
	}

	dz = (g_zmax - g_zmin) / 16;
	glBegin(GL_QUADS);
	z = g_zmin;
	for (i = 0; i < 16; i++)
	{
		zRGB(z, rgb);
		glColor3dv(rgb);
		glVertex3d(tmp_z1_x, tmp_z1_y, z);
		glVertex3d(tmp_z2_x, tmp_z2_y, z);
		zRGB(z + dz, rgb);
		glColor3dv(rgb);
		glVertex3d(tmp_z2_x, tmp_z2_y, z + dz);
		glVertex3d(tmp_z1_x, tmp_z1_y, z + dz);
		z += dz;
	}
	glEnd();
}

/* ワイヤフレームのグラフを定義する関数 */
void wire3Dgraph(void)
{
	int i, j;

	glLineWidth(2);
	glColor3d(0, 0, 0);

	for (i = 0; i < g_xnum; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < g_ynum; j++) glVertex3dv(g_plot[i][j]);
		glEnd();
	}
	for (i = 0; i < g_ynum; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < g_xnum; j++) glVertex3dv(g_plot[j][i]);
		glEnd();
	}
}

/* サーフェイスモデルのグラフを定義する関数 */
void solid3Dgraph(void)
{
	int i, j;

	glBegin(GL_QUADS);
	for (i = 0; i < g_xnum - 1; i++)
	{
		for (j = 0; j < g_ynum - 1; j++)
		{
			glColor3dv(g_plotRGB[i][j]);
			glVertex3dv(g_plot[i][j]);
			glColor3dv(g_plotRGB[i+1][j]);
			glVertex3dv(g_plot[i+1][j]);
			glColor3dv(g_plotRGB[i+1][j+1]);
			glVertex3dv(g_plot[i+1][j+1]);
			glColor3dv(g_plotRGB[i][j+1]);
			glVertex3dv(g_plot[i][j+1]);
		}
	}
	glEnd();
}

/* グラフの頂点座標を求める関数 */
void plotGraph(void)
{
	int i, j, k, l;
	char func[STR_LENGTH], tmp[STR_LENGTH];
	GLdouble x, y, z;

	g_zmin = DBL_MAX;
	g_zmax = -DBL_MAX;

	for (i = 0, x = g_xmin; i < g_xnum; i++, x += g_xint)
	{
		for (j = 0, y = g_ymin; j < g_ynum; j++, y += g_yint)
		{
			for (k = 0; k < STR_LENGTH; k++) func[k] = '\0';
			for (k = 0, l = 0; g_func[k] != '\0'; k++, l++)
			{
				if ((g_func[k] == 'x' || g_func[k] == 'y') && \
					(k == 0 || \
					g_func[k - 1] == '+' || g_func[k - 1] == '-' || g_func[k - 1] == '*' || g_func[k - 1] == '/' || \
					g_func[k - 1] == ' ' || g_func[k - 1] == ',' || g_func[k - 1] == '(' || g_func[k - 1] == ')') && \
					(g_func[k + 1] == '\0' || \
					g_func[k + 1] == '+' || g_func[k + 1] == '-' || g_func[k + 1] == '*' || g_func[k + 1] == '/' || \
					g_func[k + 1] == ' ' || g_func[k + 1] == ',' || g_func[k + 1] == '(' || g_func[k + 1] == ')'))
				{
					if (g_func[k] == 'x') sprintf(tmp, "(%f)", x);
					else if (g_func[k] == 'y') sprintf(tmp, "(%f)", y);
					strcat(func, tmp);
					l = strlen(func) - 1;
				}
				else func[l] = g_func[k];
			}
			z = eval(func);
			g_plot[i][j][0] = x;
			g_plot[i][j][1] = y;
			g_plot[i][j][2] = z;
			if (z < g_zmin) g_zmin = z;
			if (z > g_zmax) g_zmax = z;
		}
	}
	for (i = 0; i < g_xnum; i++)
	{
		for (j = 0; j < g_ynum; j++)
		{
			zRGB(g_plot[i][j][2], g_plotRGB[i][j]);
		}
	}

	g_ax = (g_xmax + g_xmin) / 2;
	g_ay = (g_ymax + g_ymin) / 2;
	g_az = (g_zmax + g_zmin) / 2;
	g_xscale = 1 / (g_xmax - g_xmin);
	g_yscale = 1 / (g_ymax - g_ymin);
	g_zscale = 1 / (g_zmax - g_zmin);
}

/* 画像保存関数 */
void saveImage(void)
{
	GLubyte *buf;
	Image *img;
	char filename[64];
	int width, height, i, j;

	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	buf = (GLubyte*)malloc(width * height * 3);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width , height, GL_RGB, GL_UNSIGNED_BYTE, buf);

	if ((img = createImage(width, height)) == NULL)
	{
		fprintf(stdout, "cannot create image\n");
		return;
	};
	for (i = 0; i < img->height; i++)
	{
		for (j = 0; j < img->width; j++)
		{
			img->data[(img->height - i - 1) * img->width + j].r = buf[3 * (width * i + j)];
			img->data[(img->height - i - 1) * img->width + j].g = buf[3 * (width * i + j) + 1];
			img->data[(img->height - i - 1) * img->width + j].b = buf[3 * (width * i + j) + 2];
		}
	}
	sprintf(filename, "%ld.bmp", time(NULL));
	if (writeBmp(img, filename))
	{
		fprintf(stdout, "cannot write image\n");
		freeImage(img);
		return;
	};
	freeImage(img);
	fprintf(stdout, "saved %s\n", filename);
}

/* リサイズコールバック関数 */
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / g_size, w / g_size, -h / g_size, h / g_size, -1024, 1024);
}

/* キーボードコールバック関数 */
void keyin(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'x': g_xscale *= 1.25; glutPostRedisplay(); break;
		case 'X': g_xscale /= 1.25; glutPostRedisplay(); break;
		case 'y': g_yscale *= 1.25; glutPostRedisplay(); break;
		case 'Y': g_yscale /= 1.25; glutPostRedisplay(); break;
		case 'z': g_zscale *= 1.25; glutPostRedisplay(); break;
		case 'Z': g_zscale /= 1.25; glutPostRedisplay(); break;
		case 'w': g_wire ^= 1; glutPostRedisplay(); break;
		case 's': g_solid ^= 1; glutPostRedisplay(); break;
		case 't': g_tics ^= 1; glutPostRedisplay(); break;
		case 'u': g_size *= 1.125; resize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); glutPostRedisplay(); break;
		case 'U': g_size /= 1.125; resize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); glutPostRedisplay(); break;
		case 'S': saveImage(); break;
		case '\033': exit(0);
		default: break;
	}
}

/* マウスのボタンが押されているときの処理 */
void motionActive(int x, int y)
{
	int dx, dy;

	dx = x - g_mx;
	dy = y - g_my;

	g_mt = g_mt_s + dx / 1024.0;
	g_mp = g_mp_s + dy / 1024.0;

	if (g_mp > PI * 0.4375) g_mp = PI * 0.4375;
	else if (g_mp < -PI * 0.4375) g_mp = -PI * 0.4375;

	g_ex = sin(g_mt) * cos(g_mp);
	g_ey = cos(g_mt) * cos(g_mp);
	g_ez = sin(g_mp);

	glutPostRedisplay();
}

/* マウスのボタンが押されていないときの処理 */
void motionPassive(int x, int y)
{
	g_mx = x;
	g_my = y;
	g_mt_s = g_mt;
	g_mp_s = g_mp;
}

/* マウスのホイールの処理 */
void mouse(int button, int state, int x, int y)
{
	if (button == 3)
	{
		g_size *= 1.125;
		resize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		glutPostRedisplay();
	}
	else if (button == 4)
	{
		g_size /= 1.125;
		resize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		glutPostRedisplay();
	}
}

/* 初期化する関数 */
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	g_ex = sin(g_mt) * cos(g_mp);
	g_ey = cos(g_mt) * cos(g_mp);
	g_ez = sin(g_mp);
}

/* メイン関数 */
int main(int argc, char **argv)
{
	if (argc != 6 && argc != 8)
	{
		fprintf(stderr, "Usage: %s func x_min x_max (x_num) y_min y_max (y_num)\n", argv[0]);
		return EXIT_FAILURE;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutCreateWindow(argv[1]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyin);
	glutMotionFunc(motionActive);
	glutPassiveMotionFunc(motionPassive);
	glutMouseFunc(mouse);

	strcpy(g_func, argv[1]);
	if (argc == 6)
	{
		g_xmin = atof(argv[2]);
		g_xmax = atof(argv[3]);
		g_ymin = atof(argv[4]);
		g_ymax = atof(argv[5]);
	}
	else if (argc == 8)
	{
		g_xmin = atof(argv[2]);
		g_xmax = atof(argv[3]);
		g_xnum = atof(argv[4]);
		g_ymin = atof(argv[5]);
		g_ymax = atof(argv[6]);
		g_ynum = atof(argv[7]);
	}
	g_xint = (g_xmax - g_xmin) / (g_xnum - 1);
	g_yint = (g_ymax - g_ymin) / (g_ynum - 1);

	plotGraph();
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}
