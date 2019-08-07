#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* line=NULL;

//Prototypes
void readLine();
void addEntity();
void addRelation();
void printReport();
void deleteEntity();
void deleteRelation();


void main(int argc, char* argv[]) {
	line=(char*)malloc(64, sizeof(char));
	if (line == NULL) {
		printf("Errore allocazione riga: ending process.");
		return;
	}

	while (line!="end\n") {
		readLine();
	}		//end while

}			//end of Main()


//Methods
void readLine() {
	fgets(line, 7, stdin);
	
	if (!strcmp(line, "addent")) { addEntity(); }
	else if (!strcmp(line, "addrel")) { addRelation(); }
	else if (!strcmp(line, "report")) { printReport(); }
	else if (!strcmp(line, "delent")) { deleteEntity(); }
	else if (!strcmp(line, "delrel")) { deleteRelation(); }
}

void addEntity() {
	fgets(line, 64, stdin);
	printf("(addent) %s", line);

}

void addRelation() {
	fgets(line, 64, stdin);
	printf("(addrel) %s", line);

}

void printReport() {
	printf("(report)\n");

}

void deleteEntity() {
	fgets(line, 64, stdin);
	printf("(delent)%s", line);

}

void deleteRelation() {
	fgets(line, 64, stdin);
	printf("(delrel)%s", line);

}
