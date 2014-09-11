#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
char **p;
FILE *fp, *fpout;
int total, nums, num;
if (argc < 2) {
fprintf (stderr, "Usage: %s file...\n", argv[0]);
exit (1);
}
fpout = fopen("avgs.txt", "w");
if (!fpout) {
fprintf(stderr, "Error: cannot open output file\n");
exit(1);
}

p = argv;
while (*++p) {
fp = fopen(*p, "r");
if (!fp) {
fprintf(stderr, "Error: cannot open %s\n", *p);
exit(1);
}
total = 0;
nums = 0;
while (fscanf(fp, "%d", &num) == 1) {
  total += num;
  nums += 1;
  }
  fprintf(fpout, "%s: %.3f\n", *p, (float)total / nums);
  
}
}
