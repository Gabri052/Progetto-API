#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rel {
	char name[64];
	char receiver[128];
	struct Rel* relPointer;
} Rel;
typedef Rel* RelPointer;

typedef struct Entity {
	char* name;
	RelPointer relPointer;
	struct Entity* entNext;
} Entity;
typedef Entity* EntityPointer;

char* line=NULL;
EntityPointer entHead = NULL;
EntityPointer entRear = NULL;

//Prototypes
void readLine();
EntityPointer addEntity();
void addRelation();
void printReport();
/*		non servono per il primo test.
void deleteEntity();
void deleteRelation();
*/


void main(int argc, char* argv[]) {
	line=(char*)malloc(64 * sizeof(char));
	if (line == NULL) {
		printf("Errore allocazione riga: ending process.");
		return;
	}

	while (line!="end\n") {
		readLine();
	}		//end while

	return;
}			//end of Main()


//Methods
void readLine() {
	fgets(line, 7, stdin);
	
	if (!strcmp(line, "addent")) { 
		if (entHead == NULL) {
			entHead = addEntity();
			entRear = entHead;
		}
		else entRear = addEntity();
	}
	else if (!strcmp(line, "addrel")) { addRelation();	}
	else if (!strcmp(line, "report")) { printReport(); }
	/*
	else if (!strcmp(line, "delent")) { deleteEntity(); }
	else if (!strcmp(line, "delrel")) { deleteRelation(); }
	*/
}

EntityPointer addEntity() {
	EntityPointer entPoint = (EntityPointer)malloc(sizeof(Entity));
	if (entPoint != NULL) {
		fgets(line, 64, stdin);
		entPoint->relPointer = NULL;
		entPoint->entNext = NULL;
		entPoint->name = (char*)malloc(sizeof(char) * 64);
		if (line != "") { strcpy_s(entPoint->name, 64, line); }

		if (entHead != NULL) {
			entRear->entNext = entPoint;
		}
		return entPoint;
	}
	else printf("Errore nell'allocazione di entPoint (addEntity): ending process.");
	return NULL;
}

void addRelation() {
	fgets(line, 64, stdin);
	printf("(addrel) %s", line);

}

void printReport() {
	printf("(report)\n");

}

/*
void deleteEntity() {
	fgets(line, 64, stdin);
	printf("(delent)%s", line);

}

void deleteRelation() {
	fgets(line, 64, stdin);
	printf("(delrel)%s", line);

}		non servono per il primo test.
*/
