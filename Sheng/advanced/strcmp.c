#include <stdio.h>
#include <string.h>

int mystrcmp (char *s1, char *s2);	/*實作 strcmp function*/
int mystrncmp (char *s1, char *s2, int n);/*實作 strncmp function*/
int lenof(char *s);			/*計算長度的 function*/

/*實作時發現 strncmp 及 strcmp 是
 * s1>s2回傳大於0 s2>s1則回傳小於0*/
int main (int argc, char *argv[]) {
	char *s1="whaa";
	char *s2="what";
	int ans=strcmp(s1, s2);
	int ans2=strncmp(s1, s2, 3);

	printf("return : strcmp %d, strncmp %d \n", ans, ans2);
	
	ans=mystrcmp(s1, s2);
	ans2=mystrncmp(s1, s2, 3);
	printf("return : mystrcmp %d, mystrncmp %d \n", ans, ans2);
	
	return 0;
}	

int mystrcmp (char *s1, char *s2) {
	int i, len=lenof(s1), cmp=0;

	if (lenof(s2)>lenof(s1)) {
		len=lenof(s2);
	}

	for (i=0; i<len && cmp==0; i++) {
		cmp=*(s1+i)-*(s2+i);	
	}
	return cmp;
}

int mystrncmp (char *s1, char *s2, int n) {
	int i, cmp=0;

	for (i=0; i<n && cmp==0; i++) {
		cmp=*(s1+i)-*(s2+i);	
	}
	return cmp;
}

int lenof(char *s) {
	int i;

	for (i=0; *(s+i)!='\0'; i++);

	return i;
}

