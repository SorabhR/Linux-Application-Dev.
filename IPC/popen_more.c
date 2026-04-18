#include <stdio.h>
#include <unistd.h>

#define MAXLINE 100

int main(int argc, char *argv[])
{
    FILE* stream = popen("more","w");
    char line[MAXLINE];
    FILE* fp;
    int n;
    if ((fp = fopen(argv[1], "r")) == NULL)
	printf("can’t open %s", argv[1]);


    while (fgets(line, MAXLINE, fp) != NULL) 
    {
	n = strlen(line);
	if (fprintf(stream,line) != n)
		perror("write error to pipe");
    }
    return pclose(stream);
}
