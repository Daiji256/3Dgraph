#if !defined(MKFUNC_H)
#define MKFUNC_H

void mkfile(const char *fname, const char *equation);
void compile(const char *fname);
void mkcsv(const char *fname, double xmin, double xmax, double xint, double ymin, double ymax, double yint);
void mkfunc(const char *fname, const char *equation, double xmin, double xmax, double xint, double ymin, double ymax, double yint);

#endif /* MKFUNC_H */
