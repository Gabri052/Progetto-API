#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//first data-structure.
typedef struct Rel {
	char* nameRel;
	char* receiver;
	struct Rel* relNext;
} Rel;
typedef Rel* RelPointer;

typedef struct Entity {
	char* name;
	RelPointer relPointer;
	struct Entity* entNext;
} Entity;
typedef Entity* EntityPointer;

//second data-structure.
typedef struct RelQueue {
	char* receiver;
	int numOfRels;
	struct RelQueue* relBef;
	struct RelQueue* relNext;
} RelQueue;
typedef RelQueue* RelQueuePointer;

typedef struct RelStruct {
	char* nameRel;
	struct RelStruct* relNext;
	RelQueuePointer head;
	RelQueuePointer tail;
} RelStruct;
typedef RelStruct* RelStructPointer;

	//global variables:
char* line=NULL;
EntityPointer entHead = NULL;

RelStructPointer relHead = NULL;

//Prototypes
void readLine();
void addEntity();
void lookForEnt();
void addRelation(char*, char*, char*);
void addRelToQueue(char*, char*, char*);
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
	while (line != "end\n") {
		readLine();
	}
	return;
}			//end of Main()


//Methods
void readLine() {
	fgets(line, 8, stdin);
	
	if (!strcmp(line, "addent ")) { 
		addEntity();
	}
	else if (!strcmp(line, "addrel ")) { lookForEnt();	}
	else if (!strcmp(line, "report ")) { printReport(); }
	/*
	else if (!strcmp(line, "delent ")) { deleteEntity(); }
	else if (!strcmp(line, "delrel ")) { deleteRelation(); }
	*/
}


void addEntity() {
	EntityPointer entPoint = (EntityPointer)malloc(sizeof(Entity));
	int endOfLine;

	if (entPoint != NULL) {
		fgets(line, 64, stdin);
		endOfLine = strlen(line) - 1;
		line[endOfLine] = '\0';
		entPoint->relPointer = NULL;
		entPoint->entNext = NULL;
		entPoint->name = (char*)malloc(sizeof(char) * 64);
		if (entPoint->name != NULL) { strcpy_s(entPoint->name, 64, line); }
		
		entPoint->entNext = entHead;
		entHead = entPoint;
	}
	else printf("Errore nell'allocazione di entPoint (addEntity): ending process.");
}


void lookForEnt() {
	int i = 0;
	char* nameEnt = (char*)calloc(64, sizeof(char));
	char* nameReceiver = (char*)calloc(64, sizeof(char));
	char* nameRel = (char*)calloc(64, sizeof(char));
	fscanf_s(stdin, "%s", nameEnt, 64);
	fscanf_s(stdin, "%s", nameReceiver, 64);
	fscanf_s(stdin, "%s", nameRel, 64);
	addRelation(nameEnt, nameReceiver, nameRel);	//nameRel termina con '\n'
	addRelToQueue(nameEnt, nameReceiver, nameRel);
}


void addRelation(char* nameEnt, char* nameReceiver, char* nameRel) {	//to the first data-structure
	EntityPointer entityPointer = (EntityPointer)malloc(sizeof(Entity));
	RelPointer relPointer = NULL;
	int entNotExists = 1;
	int isNewRel = 1;
	entityPointer = entHead;

	while ((entityPointer != NULL) && (entNotExists)) {
		if (!(strcmp(entityPointer->name, nameReceiver))) {
			entityPointer = entHead;
			while ((entityPointer != NULL) && (entNotExists)) {
				if (!(strcmp(entityPointer->name, nameEnt))) {
					entNotExists = 0;	//both the receiver and the ent exist
				}
				else entityPointer = entityPointer->entNext;
			}
		}
		else entityPointer = entityPointer->entNext;
	}
	relPointer = entityPointer->relPointer;
	if (!entNotExists) {	//if they exist
		while ((relPointer != NULL) && (isNewRel)) {	//checks if the rel is already added (1)
			if (!(strcmp(relPointer->nameRel, nameRel)) && !(strcmp(relPointer->receiver, nameReceiver))) {
				isNewRel = 0;	//the same rel has been found->it's not a new rel->not to be added
			}
			else relPointer = relPointer->relNext;
		}
		if (!isNewRel) {	//the rel already exists->nothing has to be done
			return;
		}
		else {
			relPointer = (RelPointer)malloc(sizeof(Rel));
			relPointer->nameRel = (char*)malloc(64);
			strcpy_s(relPointer->nameRel, 64, nameRel);
			relPointer->receiver = (char*)malloc(64);
			strcpy_s(relPointer->receiver, 64, nameReceiver);
			relPointer->relNext = entityPointer->relPointer;
			entityPointer->relPointer = relPointer;
		}
	}
	else {	//if the two entities do not exist
		return;
	}
}	//end of addRelation()


void addRelToQueue(char* ent, char* receiver, char* rel) {	//funzia se la rel c'è già?
	int isNewRelName = 1;
	//adds to the second data structure the relation.
	RelStructPointer temp;// = (RelStructPointer)malloc(sizeof(RelStruct));
	RelQueuePointer QueuePointer = NULL;
	RelQueuePointer QueuePointerBack = NULL;
	RelQueuePointer QueuePointerFront = NULL;


	temp = relHead;
	while ((isNewRelName) && (temp != NULL)) {
		if (!strcmp(temp->nameRel, rel)) isNewRelName = 0;
		else temp = temp->relNext;
	}
	if (isNewRelName) {		//creates the rel and the Queue
		temp = (RelStructPointer)malloc(sizeof(RelStruct));
		temp->nameRel = (char*)malloc(sizeof(char) * 64);
		strcpy_s(temp->nameRel, 64, rel);
		temp->head = (RelQueuePointer)calloc(1, sizeof(RelQueue));
		temp->head->receiver = (char*)malloc(64);
		strcpy_s(temp->head->receiver, 64, receiver);
		temp->head->numOfRels++;
		temp->tail = temp->head;
		temp->relNext = relHead;
		relHead = temp;
	}
	else {		//adds to the Queue the new rel
		if (strcmp(temp->head->receiver, receiver) > 0) {
			QueuePointer = (RelQueuePointer)calloc(1, sizeof(RelQueue));
			QueuePointer->receiver = (char*)malloc(64);
			strcpy_s(QueuePointer->receiver, 64, receiver);
			QueuePointer->numOfRels++;
			QueuePointer->relNext = temp->head;
			temp->head->relBef = QueuePointer;
			temp->head = QueuePointer;
		}
		else if (strcmp(temp->tail->receiver, receiver) < 0) {
			QueuePointer = (RelQueuePointer)calloc(1, sizeof(RelQueue));
			QueuePointer->receiver = (char*)malloc(64);
			strcpy_s(QueuePointer->receiver, 64, receiver);
			QueuePointer->numOfRels++;
			QueuePointer->relBef = temp->tail;
			temp->tail->relNext = QueuePointer;
			temp->tail = QueuePointer;
		}
		else {	//looks for the right place to put the receiver
			QueuePointer = temp->head;
			while ((QueuePointer != NULL) && (strcmp(QueuePointer->receiver, receiver) != 0)) {
				if (strcmp(QueuePointer->receiver, receiver) < 0) {
					QueuePointer = QueuePointer->relNext;
				}
				else {
					break;
				}
			}
			if (!(strcmp(QueuePointer->receiver, receiver))) {	//increases numOfRel
				QueuePointer->numOfRels++;
			}
			else {
				QueuePointerBack = temp->head;
				QueuePointerFront = temp->head;
				while (QueuePointerBack->relNext != QueuePointer) {
					QueuePointerBack = QueuePointerBack->relNext;
				}
				while (QueuePointerFront != QueuePointer) {
					QueuePointerFront = QueuePointerFront->relNext;
				}
				QueuePointer = (RelQueuePointer)calloc(1, sizeof(RelQueue));
				QueuePointer->receiver = (char*)malloc(64);
				strcpy_s(QueuePointer->receiver, 64, receiver);
				QueuePointer->numOfRels++;
				QueuePointer->relNext = QueuePointerFront;
				QueuePointer->relBef = QueuePointerBack;
				QueuePointerBack->relNext = QueuePointer;
				QueuePointerFront->relBef = QueuePointer;	//non aggiorna relHead perchè sono con QueuePointer!!
				printf("(report)\n");
			}
		}
	}
}	//end of addRelToQueue


void printReport() {
	printf("(report)\n");

}	//end of printReport()

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
