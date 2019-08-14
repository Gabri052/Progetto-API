#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
	struct Entity* sonDx;
	struct Entity* sonSx;
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
char* line = NULL;
int highestNumOfRel = 0;
EntityPointer entHead = NULL;
RelStructPointer relHead = NULL;

//Prototypes
void readLine();
void addEntity();
void lookForEnt();
void addRelation(char*, char*, char*);
void addRelToQueue(char*, char*, char*);
void printReport(char*, char**);
char** chooseEntsToPrint(RelQueuePointer);
bool isNewRelation(char*, char*, char*);
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
	readLine();
	return;
}			//end of Main()


//Methods
void readLine() {
	RelStructPointer relPointer = NULL;
	while (strcmp(line, "end\n") != 0) {
		fgets(line, 8, stdin);

		if (!strcmp(line, "addent ")) {
			addEntity();
		}
		else if (!strcmp(line, "addrel ")) { lookForEnt(); }
		else if (!strcmp(line, "report\n")) {
			relPointer = relHead;
			if (relPointer == NULL) {
				printf("none");
			}
			while (relPointer != NULL) {
				printReport(relPointer->nameRel, chooseEntsToPrint(relPointer->head));
				relPointer = relPointer->relNext;
			}
			printf("\n");
		}
		/*
		else if (!strcmp(line, "delent ")) { deleteEntity(); }
		else if (!strcmp(line, "delrel ")) { deleteRelation(); }
		*/
	}
}


void addEntity() {
	EntityPointer entPoint = (EntityPointer)calloc(sizeof(Entity), 1);
	EntityPointer entSearch = entHead;
	int endOfLine;

	if (entPoint != NULL) {
		//fscanf_s(stdin, "%s", line);
		fgets(line, 64, stdin);
		endOfLine = strlen(line) - 1;
		line[endOfLine] = '\0';
		entPoint->name = (char*)malloc(sizeof(char) * 64);
		if (entPoint->name != NULL) { strcpy_s(entPoint->name, 64, line); }

		if (entSearch == NULL) {	//enthead does not exist
			entHead = entPoint;
		}
		else {
			while ((strcmp(entSearch->name, line) != 0)) {
				if (strcmp(entSearch->name, line) < 0) {
					if (entSearch->sonDx != NULL) {
						entSearch = entSearch->sonDx;
					}
					else break;
				}
				else {
					if (entSearch->sonSx != NULL) {
						entSearch = entSearch->sonSx;
					}
					else break;
				}
			}
			if (strcmp(entSearch->name, line) < 0) {
				entSearch->sonDx = entPoint;
			}
			else {
				entSearch->sonSx = entPoint;
			}
		}
		//order the tree
	}
	else printf("Errore nell'allocazione di entPoint (addEntity): ending process.");
	printf("");
}


void lookForEnt() {
	int i = 0;
	char* nameEnt = (char*)calloc(64, sizeof(char));
	char* nameReceiver = (char*)calloc(64, sizeof(char));
	char* nameRel = (char*)calloc(64, sizeof(char));
	fscanf_s(stdin, "%s", nameEnt, 64);
	fscanf_s(stdin, "%s", nameReceiver, 64);
	fscanf_s(stdin, "%s", nameRel, 64);
	if (isNewRelation(nameRel, nameReceiver, nameEnt)) {
		addRelation(nameEnt, nameReceiver, nameRel);	//nameRel termina con '\n'
		addRelToQueue(nameEnt, nameReceiver, nameRel);
	}
}


void addRelation(char* nameEnt, char* nameReceiver, char* nameRel) {	//to the first data-structure
	EntityPointer entityPointer = (EntityPointer)malloc(sizeof(Entity));
	RelPointer relPointer = NULL;
	bool entNotExists = 1;
	bool isNewRel = 1;
	entityPointer = entHead;

	/*while ((entityPointer != NULL) && (entNotExists)) {	in teoria isNewRelation() dovrebbe fare questo lavoro
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
	}*/
	while ((entityPointer != NULL) && (entNotExists)) {
		if (strcmp(entityPointer->name, nameEnt) == 0) {
			entNotExists = 0;
		}
		else {
			if (strcmp(entityPointer->name, nameEnt) < 0) {
				entityPointer = entityPointer->sonDx;
			}
			else {
				entityPointer = entityPointer->sonSx;
			}
		}
	}
	if (entNotExists) return;
	else {	//if they exist
		relPointer = entityPointer->relPointer;
		while ((relPointer != NULL) && (isNewRel)) {	//checks if the rel is already added (1) -- is it necessary?
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
}	//end of addRelation()


void addRelToQueue(char* ent, char* receiver, char* rel) {	//funzia se la rel c'è già?
	bool isNewRelName = 1;
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
				QueuePointerFront->relBef = QueuePointer;
			}
		}
	}
}	//end of addRelToQueue


bool isNewRelation(char* nameRel, char* receiver, char* user) {
	EntityPointer structTemp = entHead;
	RelPointer relPointer = NULL;

	while (structTemp != NULL) {
		if (strcmp(structTemp->name, user) != 0) { 
			if (strcmp(structTemp->name, user) < 0) {
				structTemp = structTemp->sonDx;
			}
			else {
				structTemp = structTemp->sonSx;
			}
		}
		else break;
	}
	if (structTemp == NULL) { 
		return false; 
	}	//the ent does not exist
	else {
		relPointer = structTemp->relPointer;
		while ((relPointer != NULL)) {
			if ((strcmp(relPointer->nameRel, nameRel) == 0) && (strcmp(relPointer->receiver, receiver) == 0)) {
				return false;
			}
			else relPointer = relPointer->relNext;
		}
	}
	return true;
}


void printReport(char* nameRel, char** nameEnts) {
	int i = 0;
	if (relHead == NULL) {
		printf("none");
	}
	else {
		printf("%s ", nameRel);
		while (nameEnts[i] != NULL) {
			printf("%s ", nameEnts[i]);
			i++;
		}
		printf("%d; ", highestNumOfRel);
	}
}	//end of printReport()


char** chooseEntsToPrint(RelQueuePointer relQueuePointer) {
	RelQueuePointer queuePointer = relQueuePointer;
	int j = 0, i = 0;
	char** entsToPrint = (char**)calloc(sizeof(char*), 15);	//fifteen ents receive the same number of the same relation.
	highestNumOfRel = 0;
	while (queuePointer != NULL) {
		if (highestNumOfRel < queuePointer->numOfRels) {
			highestNumOfRel = queuePointer->numOfRels;
			while (entsToPrint[j] != NULL) {
				free(entsToPrint[j]);
				j++;
			}
			i = 0;
			j = 0;
			entsToPrint[i] = (char*)calloc(sizeof(char), 64);
			strcpy_s(entsToPrint[i], 64, queuePointer->receiver);
			i++;
		}
		else if (highestNumOfRel == queuePointer->numOfRels) {
			entsToPrint[i] = (char*)malloc(sizeof(char) * 64);
			strcpy_s(entsToPrint[i], 64, queuePointer->receiver);
			i++;
		}
		queuePointer = queuePointer->relNext;
		if (i == 15) printf("entsToPrint deve essere riallocato perchè è troppo piccolo");
	}
	return entsToPrint;
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
