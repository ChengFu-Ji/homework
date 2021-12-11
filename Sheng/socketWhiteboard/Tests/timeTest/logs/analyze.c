#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long seeki (FILE *, int, int *); 
double ns2s (long);
void writeFile (char *, int, int, double, double, double, FILE *);

int main(int argc, char *argv[]) {
    FILE *serverfp, *clientfp, *save;
    char Wbuffer[100];
    long ans, tmp[2];
    int status; 
    time_t t = time(NULL);
    struct tm tm = *(localtime(&t));
    char *Tstatus[3] = {"start2get", "end2get", "start2end"};

    serverfp = fopen("./serverLog.csv", "r");
    clientfp = fopen("./clientLog.csv", "r");
    sprintf(Wbuffer, "%d%02d%02d_%02d%02d%02dLog.csv", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    save = fopen(Wbuffer, "w");
    //save = fopen("./outputLog.csv", "w");
    sprintf(Wbuffer, "UID, Way");
    for (int i = 0; i < 3; i++) {
        strcat(Wbuffer, ", ");
        strcat(Wbuffer, Tstatus[i]);
        strcat(Wbuffer, "-AvgDiff (unit: s)");
    }
    strcat(Wbuffer, "\n");
    //printf("%s\n", Wbuffer);
    fwrite(Wbuffer, strlen(Wbuffer), 1, save);
    bzero(Wbuffer, sizeof(Wbuffer));

    for (int i = 1; i < 11; i++) {
        fseek(clientfp, 0, SEEK_SET);
        fseek(serverfp, 0, SEEK_SET);

        int times = 0;
        long average[3] = {0};
        int exit = 0;
        while (1) {
            if ((ans = seeki(serverfp, i, NULL)) == -1) {
                exit++;
            }
            if((tmp[0] = seeki(clientfp, i, &status)) == -1) {
                exit++;
            }
            
            if (status == 0 || status == 2) {
                if((tmp[1] = seeki(clientfp, i, &status)) == -1) {
                    exit++;
                }
            }
            if (ans-tmp[0] > 0 && ans-tmp[1] > 0 && tmp[1]-tmp[0] > 0) {
                times++;
                average[0] += (ans-tmp[0]);
                average[1] += (ans-tmp[1]);
                average[2] += (tmp[1]-tmp[0]);
            }
            if (exit == 2) {
                break;
            }
        }

        double start2get = ns2s(average[0])/(double) times;
        double end2get = ns2s(average[1])/(double) times;
        double start2end = ns2s(average[2])/(double) times;


        writeFile(Wbuffer, status, i, start2get, end2get, start2end, save);
    }
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
                    if (strstr(s, "D") != NULL) {
                        *status = 2;
                    } else if (strstr(s, "L") != NULL) {
                        *status = 0;
                    }
                    if (strstr(s, "ES") != NULL) {
                        (*status)++;
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


void writeFile (char *buffer, int type, int id, double avg, double avg1, double avg2, FILE *save) {
    char *Ttype[2] = {"Line", "Dot"};
    sprintf(buffer, "%d, %s, %.6lf, %.6lf, %.6lf\n", id, Ttype[type/2], avg, avg1, avg2);
    fwrite(buffer, strlen(buffer), 1, save);
}
