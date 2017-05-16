#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

struct Node{
	int id;
	int connections[10];
	char* comp;
	struct Node *prox;
};
typedef struct Node node;

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

void iniciaLista(node *REDE, int id, char* comp){

	node* aux = REDE;
	aux->id = id;
	aux->connections[0] = '\0';
	aux->comp = malloc(100*sizeof(char));
	aux->comp = comp;
	aux->prox = NULL;
}

void insereLista(node *REDE, int id, char* comp){

	node* aux = REDE;
	node* newnode = (node*)malloc(sizeof(node));

	newnode->id = id;
	newnode->connections[0] = '\0';
	newnode->comp = malloc(100*sizeof(char));
	newnode->comp = comp;

	while(aux->prox != NULL){
		aux = aux->prox;	
	}

	aux->prox = newnode;
	aux->prox->prox = NULL;

}

void lerLista(node *REDE){

	node *aux = REDE;

	while(aux){
		printf("%d, %s\n", aux->id, aux->comp);
		aux = aux->prox;
	}

}

char* agrupaWinFil(char **linha){

	char *line = malloc(100*sizeof(char));

	strcpy(line, linha[2]);
	strcat(line, " ");
	strcat(line, linha[3]);
	strcat(line, " ");
	strcat(line, linha[4]);
	strcat(line, " ");
	strcat(line, linha[5]);

	return line;
}

char* agrupaConstTee(char **linha){

	char *line = malloc(100*sizeof(char));

	strcpy(line, linha[2]);
	strcat(line, " ");
	strcat(line, linha[3]);

	return line;
}

char* agrupaSpawn(char **linha){

	char *line = malloc(100*sizeof(char));

	strcpy(line, linha[2]);
	strcat(line, " ");
	strcat(line, linha[3]);
	strcat(line, " ");
	strcat(line, linha[4]);

	return line;	
}

void insereConnections(node *REDE, char **linha){

	node* aux = REDE;

	int idA, i, j, num;

	idA = atoi(linha[1]);

	while(aux){

		if(aux->id == idA){
			for(i=2,j=0; linha[i] != '\0'; i++,j++){
				num = atoi(linha[i]);
				aux->connections[j] = num;
			}
			aux->connections[j]='\0';
		}

		aux = aux->prox;
	}

}

void lerApenas(node *REDE){

	int i;
	node* aux = REDE;

	while(aux){
		for(i = 0; aux->connections[i] != '\0'; i++){
			printf("id: %d------> connections :%d\n", aux->id ,aux->connections[i]);
		}
		aux = aux->prox;
	}

}

void inserirStruct(char **line, node *REDE, int flag){

	int num;
	char *linha = malloc(100*sizeof(char));
	char **linhaFich = (char**) malloc(100*sizeof(char*));
	linhaFich = line;

	if(!strcmp(line[0], "node")){

		if(!strcmp(line[2], "window") || !strcmp(line[2], "filter")){
			linha = agrupaWinFil(linhaFich);
			num = atoi(line[1]);

			if(!flag)
				iniciaLista(REDE, num, linha);
			else
				insereLista(REDE, num, linha);
		}
		else{
			if(!strcmp(line[2], "const") || !strcmp(line[2], "tee")){
				linha = agrupaConstTee(linhaFich);
				num = atoi(line[1]);
				insereLista(REDE, num, linha);
			}
			else{
				linha = agrupaSpawn(linhaFich);
				num = atoi(line[1]);
				insereLista(REDE, num, linha);
			}
		}
	}
	else{
		if(!strcmp(line[0], "connect")){
			insereConnections(REDE, linhaFich);
		}
	}

}

void lerInserir(){

	char *nome = "testes.txt";

	node *REDE = (node*)malloc(sizeof(node));

	int file, size, size_line = 100, flag = 0;
	char *line, **strcomp = (char**) malloc(100*sizeof(char*));
	
	line = calloc(size_line, sizeof(char));

	file = open(nome, O_RDWR);

	while((size = readLine(file, &line, &size_line)) > 0){
		strcomp = split(line);
		if(!flag){
			inserirStruct(strcomp, REDE, flag);
			flag = 1;
		}
		else{
			inserirStruct(strcomp, REDE, flag);
		}
	}

	//lerLista(REDE);
	//lerApenas(REDE);

}

















