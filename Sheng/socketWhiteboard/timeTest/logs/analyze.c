#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long seeki (FILE *, int, int *); 
double ns2s (long);

int main(int argc, char *argv[]) {
    FILE *serverfp, *clientfp, *save;
    char Wbuffer[100];
    long ans;
    int status; 
    time_t t = time(NULL);
    struct tm tm = *(localtime(&t));

    serverfp = fopen("./serverLog.csv", "r");
    clientfp = fopen("./clientLog.csv", "r");
    sprintf(Wbuffer, "%d%02d%02d_%02d%02d%02dLog.csv", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    save = fopen(Wbuffer, "w");
    //save = fopen("./outputLog.csv", "w");
    sprintf(Wbuffer, "UID, Way, AvgDiff (unit: s)\n");
    fwrite(Wbuffer, strlen(Wbuffer), 1, save);
    bzero(Wbuffer, sizeof(Wbuffer));

    for (int i = 1; i < 11; i++) {
        fseek(clientfp, 0, SEEK_SET);
        fseek(serverfp, 0, SEEK_SET);

        int times = 0;
        int average = 0;
        int exit = 0;
        while (1) {
            if ((ans = seeki(serverfp, i, NULL)) == -1) {
                exit++;
            }
            if((ans -= seeki(clientfp, i, &status)) == -1) {
                exit++;
            } 
            if (ans > 0) {
                times++;
            }
            average += ans;
            if (exit == 2) {
                break;
            }
        }

        double s = ns2s(average)/(double) times;

        if (status == 0) {
            sprintf(Wbuffer, "%d, Line, %.6lf\n", i, s);
        } else {
            sprintf(Wbuffer, "%d, Dot, %.6lf\n", i, s);
        }
        fwrite(Wbuffer, strlen(Wbuffer), 1, save);
    }

    return 0;
}

long seeki (FILE *fp, int i, int *status) {
    char s[100];
    long ans;

    ans = -1;
    while (1) {
        if (fgets(s, 100, fp) == NULL) {
            break;
        } else if (atoi(s) == i) {
            if (strstr(s, "Send") || strstr(s, "Read")) {
                ans = atol(strstr(strstr(s, ",")+1, ",")+1);
                if (status != NULL) {
                    if (strstr(s, "DotSend") != NULL) {
                        *status = 1;
                    } else if (strstr(s, "LineSend") != NULL) {
                        *status = 0;
                    }
                }
            } else if (strstr(s, "Exit") || strstr(s, "exit")) {
                break;
            } else {
                continue;
            }
            break;
        }
    }

    return ans;
}

double ns2s (long time) {
    double ans;

    ans = time;
    for (int i = 0; i < 9; i++) {
        ans/= 10;
    }
    return ans;
}


