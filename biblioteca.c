#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

char **split(char *buf){
		
	int i = 0, n = 0;
	char **result;
	result = malloc(10*sizeof(char*));
	result[n++] = buf;

	while(buf[i]){
		if(buf[i] == ' '){
			buf[i] = '\0';
			result[n++] = buf+1+i;
		}
		i++;
	}

	return result;
}

int readLine(int fd, char **line, int *size){

	char buff, *str = *line;
	int elems, res;

	elems = res = 0;

	while((res = read(fd, &buff, 1)) > 0 && buff != '\n' && buff != '\0'){
		if(elems > *size){
			if(realloc(str, (*size)*2)==NULL){
				return -1;
			}
			*size *= 2;
		}
		str[elems++] = buff;
	}

	if(elems > *size){
		if(realloc(str, (*size) + 1) == NULL){
			return -1;
		}
		*size += 1; 
	}

	str[elems] = '\0';
	*line = str;

	if(res > 0)
		return elems;
	else if (res == 0)
		return 0;
	else
		return -1;

}

void fazCenas(){

	char nome[] = "testes.txt";

	int file, size, size_line = 100;
	char *line, **strcomp = (char**) malloc(6*sizeof(char*));
	
	line = calloc(size_line, sizeof(char));

	file = open(nome, O_RDWR);

	while((size = readLine(file, &line, &size_line)) > 0){
		strcomp = split(line);
		printf("-----> %s\n", strcomp[1]);
	}
}



