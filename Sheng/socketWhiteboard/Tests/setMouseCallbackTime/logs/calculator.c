#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char s[100];
    double ans;
    int times = 0;
    double average = 0.0;
    double max, min;
    if (argc < 2) {
        printf("usage: ./a.out [filename]\n");
        exit(0);
    }

    file = fopen(argv[1], "r");
    max = -1.0;
    min = 1000.0;
    while (1) {
        if (fgets(s, 100, file) != NULL) {
            if (!strstr(s, "time")) {
                average += atof(s);
                if (atof(s) > max) {
                    max = atof(s);
                } 
                if (min > atof(s)){
                    min = atof(s);
                }
                times++;
            }
        } else {
            break;
        }
    }
    printf("%lf\n", average/(double) times);
    printf("Max: %lf, Min: %lf\n", max, min);
}
