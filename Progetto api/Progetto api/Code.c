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
	//RelQueuePointer tail;
} RelStruct;
typedef RelStruct* RelStructPointer;

	//global variables:
char* line = NULL;
int highestNumOfRel = 0;
EntityPointer entHead = NULL;
RelStructPointer relHead = NULL;

//Prototypes
void readLine();
void addEntity(char*);
void lookForEnt();
void addRelation(char*, char*, char*);
void addRelToQueue(char*, char*, char*);
void printReport(char*, char**);
char** chooseEntsToPrint(RelQueuePointer);
bool isNewRelation(char*, char*, char*, bool);
void deleteRelation(char*, char*, char*);
void deleteRelationFromQueue(char*, char*, char*);
void deleteEntity(char*);
void deleteEntityFromQueue(char*);
void putTreeNode(EntityPointer);
void isQueueEmpty(RelStructPointer);
void recursiveSearch(EntityPointer, char*);


int main(int argc, char* argv[]) {
	line=(char*)malloc(32 * sizeof(char));
	if (line == NULL) {
		printf("Errore allocazione riga: ending process.");
		return 1;
	}
	readLine();
	printf("\n");
	return 0;
}	//end of Main()


//Methods
void readLine() {
	RelStructPointer relPointer = NULL;
	while (strcmp(line, "end") != 0) {
		fscanf(stdin, "%s", line);

		if (!strcmp(line, "addent")) {
			fscanf(stdin, "%s", line);
			addEntity(line);
		}
		else if (!strcmp(line, "addrel")) { lookForEnt(); }
		else if (!strcmp(line, "report")) {
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
		else if (!strcmp(line, "delent")) {
			fscanf(stdin, "%s", line);
			deleteEntity(line);
			deleteEntityFromQueue(line);
		}
		else if (!strcmp(line, "delrel")) { lookForEnt(); }
		
	}
}


void addEntity(char* nameEnt) {
	EntityPointer entPoint = (EntityPointer)calloc(sizeof(Entity), 1);
	EntityPointer entSearch = entHead;
	int endOfLine;

	if (entPoint != NULL) {
		entPoint->name = (char*)malloc(sizeof(char) * 32);
		if (entPoint->name != NULL) { strcpy(entPoint->name, nameEnt); }

		if (entSearch == NULL) {	//enthead does not exist
			entHead = entPoint;
		}
		else {
			while ((strcmp(entSearch->name, nameEnt) != 0)) {
				if (strcmp(entSearch->name, nameEnt) < 0) {
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
			if (strcmp(entSearch->name, nameEnt) == 0) { return; }
			else if (strcmp(entSearch->name, nameEnt) < 0) {
				entSearch->sonDx = entPoint;
			}
			else {
				entSearch->sonSx = entPoint;
			}
		}
		//order the tree
	}
	else printf("Errore nell'allocazione di entPoint (addEntity): ending process.");
}


void lookForEnt() {
	int i = 0;
	char* nameEnt = (char*)calloc(32, sizeof(char));
	char* nameReceiver = (char*)calloc(32, sizeof(char));
	char* nameRel = (char*)calloc(32, sizeof(char));
	fscanf(stdin, "%s", nameEnt);
	fscanf(stdin, "%s", nameReceiver);
	fscanf(stdin, "%s", nameRel);
	if (!(strcmp(line, "addrel"))) {
		if (isNewRelation(nameRel, nameReceiver, nameEnt, false)) {
			addRelation(nameEnt, nameReceiver, nameRel);
			addRelToQueue(nameEnt, nameReceiver, nameRel);
		}
	}
	else  {	//delrel
		if (!(isNewRelation(nameRel, nameReceiver, nameEnt, true))) {
			deleteRelation(nameEnt, nameReceiver, nameRel);
			deleteRelationFromQueue(nameEnt, nameReceiver, nameRel);
		}
	}
}


void addRelation(char* nameEnt, char* nameReceiver, char* nameRel) {	//to the first data-structure
	EntityPointer entityPointer = (EntityPointer)malloc(sizeof(Entity));
	RelPointer relPointer = NULL;
	bool entNotExists = 1;
	bool isNewRel = 1;
	entityPointer = entHead;

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
			relPointer->nameRel = (char*)malloc(32);
			strcpy(relPointer->nameRel, nameRel);
			relPointer->receiver = (char*)malloc(32);
			strcpy(relPointer->receiver, nameReceiver);
			relPointer->relNext = entityPointer->relPointer;
			entityPointer->relPointer = relPointer;
		}
	}
}	//end of addRelation()


void addRelToQueue(char* ent, char* receiver, char* rel) {
	bool isNewRelName = 1;
	//adds to the second data structure the relation.
	RelStructPointer temp = relHead, tempBef = NULL;
	RelQueuePointer queuePointer = NULL;
	RelQueuePointer queuePointerBack = NULL;
	RelQueuePointer queuePointerFront = NULL;

	while ((isNewRelName) && (temp != NULL)) {
		if (!strcmp(temp->nameRel, rel)) isNewRelName = 0;
		else temp = temp->relNext;
	}
	if (isNewRelName) {		//creates the rel and the Queue
		temp = (RelStructPointer)malloc(sizeof(RelStruct));
		temp->nameRel = (char*)malloc(sizeof(char) * 32);
		strcpy(temp->nameRel, rel);
		temp->head = (RelQueuePointer)calloc(1, sizeof(RelQueue));
		temp->head->receiver = (char*)malloc(32);
		strcpy(temp->head->receiver, receiver);
		temp->head->numOfRels++;
		//temp->tail = temp->head;
		if (relHead == NULL) {
			relHead = temp;
			relHead->relNext = NULL;
		}
		else {
			tempBef = relHead;
			while (tempBef->relNext != NULL) {
				if ((strcmp(tempBef->relNext->nameRel, temp->nameRel)) < 0) {
					tempBef = tempBef->relNext;
				}
				else break;
			}
			if (tempBef == relHead) {
				if ((strcmp(tempBef->nameRel, temp->nameRel)) > 0) {
					temp->relNext = relHead;
					relHead = temp;
				}
				else {
					temp->relNext = relHead->relNext;
					relHead->relNext = temp;
				}
			}
			else {
				temp->relNext = tempBef->relNext;
				tempBef->relNext = temp;
			}
		}
	}
	else {		//adds to the Queue the new rel
		if (strcmp(temp->head->receiver, receiver) > 0) {
			queuePointer = (RelQueuePointer)calloc(1, sizeof(RelQueue));
			queuePointer->receiver = (char*)malloc(32);
			strcpy(queuePointer->receiver, receiver);
			queuePointer->numOfRels++;
			queuePointer->relNext = temp->head;
			temp->head->relBef = queuePointer;
			temp->head = queuePointer;
		}/*
		else if (strcmp(temp->tail->receiver, receiver) < 0) {
			QueuePointer = (RelQueuePointer)calloc(1, sizeof(RelQueue));
			QueuePointer->receiver = (char*)malloc(64);
			strcpy(QueuePointer->receiver, receiver);
			QueuePointer->numOfRels++;
			QueuePointer->relBef = temp->tail;
			temp->tail->relNext = QueuePointer;
			temp->tail = QueuePointer;
		}*/
		else {	//looks for the right place to put the receiver
			queuePointer = temp->head;
			while ((queuePointer != NULL) && (strcmp(queuePointer->receiver, receiver) != 0)) {
				if (strcmp(queuePointer->receiver, receiver) < 0) {
					queuePointer = queuePointer->relNext;
				}
				else {
					break;
				}
			}
			if (queuePointer == NULL) {
				queuePointerBack = temp->head;
				while (queuePointerBack->relNext != NULL) {
					queuePointerBack = queuePointerBack->relNext;
				}
				queuePointer = (RelQueuePointer)calloc(1, sizeof(RelQueue));
				queuePointer->receiver = (char*)malloc(32);
				strcpy(queuePointer->receiver, receiver);
				queuePointer->numOfRels++;
				queuePointer->relBef = queuePointerBack;
				queuePointerBack->relNext = queuePointer;
			}
			else if (!(strcmp(queuePointer->receiver, receiver))) {	//increases numOfRel
				queuePointer->numOfRels++;
			}
			else {
				queuePointerBack = temp->head;
				queuePointerFront = temp->head;
				while (queuePointerBack->relNext != queuePointer) {
					queuePointerBack = queuePointerBack->relNext;
				}
				while (queuePointerFront != queuePointer) {
					queuePointerFront = queuePointerFront->relNext;
				}
				queuePointer = (RelQueuePointer)calloc(1, sizeof(RelQueue));
				queuePointer->receiver = (char*)malloc(32);
				strcpy(queuePointer->receiver, receiver);
				queuePointer->numOfRels++;
				queuePointer->relNext = queuePointerFront;
				queuePointer->relBef = queuePointerBack;
				queuePointerBack->relNext = queuePointer;
				queuePointerFront->relBef = queuePointer;
			}
		}
	}
}	//end of addRelToQueue


bool isNewRelation(char* nameRel, char* receiver, char* user, bool isDelrel) {
	EntityPointer structTemp = entHead;
	RelPointer relPointer = NULL;

	while (structTemp != NULL) {
		if (strcmp(structTemp->name, receiver) != 0) { 
			if (strcmp(structTemp->name, receiver) < 0) { structTemp = structTemp->sonDx; }
			else { structTemp = structTemp->sonSx; }
		}
		else break;
	}
	if (structTemp == NULL) { 
		if (isDelrel) { return true; }
		else return false;
	}	//the ent does not exist
	else {
		structTemp = entHead;
		while (structTemp != NULL) {
			if (strcmp(structTemp->name, user) != 0) {
				if (strcmp(structTemp->name, user) < 0) { structTemp = structTemp->sonDx; }
				else { structTemp = structTemp->sonSx; }
			}
			else break;
		}
		if (structTemp == NULL) {
			if (isDelrel) { return true; }
			else return false;
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
				entsToPrint[j] = NULL;
				j++;
			}
			i = 0;
			j = 0;
			entsToPrint[i] = (char*)calloc(sizeof(char), 32);
			strcpy(entsToPrint[i], queuePointer->receiver);
			i++;
		}
		else if (highestNumOfRel == queuePointer->numOfRels) {
			entsToPrint[i] = (char*)malloc(sizeof(char) * 32);
			strcpy(entsToPrint[i], queuePointer->receiver);
			i++;
		}
		queuePointer = queuePointer->relNext;
		if (i == 15) printf("entsToPrint deve essere riallocato perchè è troppo piccolo");
	}
	return entsToPrint;
}


void deleteRelation(char* nameEnt, char* nameReceiver, char* nameRel) {
	//from the first data structure.
	EntityPointer entTemp = entHead;
	RelPointer relTemp = NULL, relTempBack = NULL;

	while ((entTemp != NULL) && (strcmp(entTemp->name, nameEnt) != 0)) {
		if (strcmp(entTemp->name, nameEnt) < 0) {
			entTemp = entTemp->sonDx;
		}
		else {
			entTemp = entTemp->sonSx;
		}
	}
	if (entTemp == NULL) { return; }
	else {
		relTemp = entTemp->relPointer;
		while (relTemp != NULL) {
			if ((strcmp(relTemp->receiver, nameReceiver) == 0) && (strcmp(relTemp->nameRel, nameRel) == 0)) break;
			else relTemp = relTemp->relNext;
		}
		if (relTemp == NULL) { return; }
		else {
			relTempBack = entTemp->relPointer;
			if (relTempBack == relTemp) {
				entTemp->relPointer = relTemp->relNext;
			}
			else {
				while (relTempBack->relNext != relTemp) {
					relTempBack = relTempBack->relNext;
				}
				relTempBack->relNext = relTemp->relNext;
			}
			free(relTemp);
		}
	}
}


void deleteRelationFromQueue(char* nameEnt, char* nameReceiver, char* nameRel) {
	RelStructPointer relStructTemp = relHead;
	RelQueuePointer relQueueTemp = NULL;

	while ((relStructTemp != NULL) && (strcmp(relStructTemp->nameRel, nameRel) != 0)) {
		relStructTemp = relStructTemp->relNext;
	}
	if (relStructTemp == NULL) {
		return;
	}
	else {
		if (strcmp(relStructTemp->head->receiver, nameReceiver) == 0) {
			if (relStructTemp->head->numOfRels == 1) {
				relQueueTemp = relStructTemp->head;
				relStructTemp->head = relStructTemp->head->relNext;
				//if (strcmp(relStructTemp->tail->receiver, nameReceiver) == 0) { relStructTemp->tail = NULL; }
				if (relStructTemp->head != NULL) { relStructTemp->head->relBef = NULL; }
				free(relQueueTemp);
				isQueueEmpty(relStructTemp);
			}
			else {
				relStructTemp->head->numOfRels--;
			}
		}/*
		else if (strcmp(relStructTemp->tail->receiver, nameReceiver) == 0) {
			if (relStructTemp->tail->numOfRels == 1) {
				relQueueTemp = relStructTemp->tail;
				relStructTemp->tail = relQueueTemp->relBef;
				free(relQueueTemp);
				relStructTemp->tail->relNext = NULL;
				isQueueEmpty(relStructTemp);
			}
			else {
				relStructTemp->tail->numOfRels--;
			}
		}*/
		else {
			relQueueTemp = relStructTemp->head;
			while ((relQueueTemp != NULL) && (strcmp(relQueueTemp->receiver, nameReceiver) != 0)) {
				relQueueTemp = relQueueTemp->relNext;
			}
			if (relQueueTemp == NULL){
				return; 
			}
			else {
				if (relQueueTemp->numOfRels == 1) {
					if (relQueueTemp->relNext == NULL) { relQueueTemp->relBef->relNext = NULL; }
					else {
						relQueueTemp->relBef->relNext = relQueueTemp->relNext;
						relQueueTemp->relNext->relBef = relQueueTemp->relBef;
					}
					free(relQueueTemp);
					relQueueTemp = NULL;
					isQueueEmpty(relStructTemp);
				}
				else {	relQueueTemp->numOfRels--;	}
			}
		}
	}
}


void deleteEntity(char* nameEnt) {
	EntityPointer entPointerTemp = entHead, entPointerBack = NULL;
	RelPointer relPointerTemp = NULL;

	if (entHead != NULL) { recursiveSearch(entHead, nameEnt); }
	while (entPointerTemp != NULL) {
		if (!(strcmp(entPointerTemp->name, nameEnt))) {
			break;
		}
		else {
			if (strcmp(entPointerTemp->name, nameEnt) < 0) {
				entPointerTemp = entPointerTemp->sonDx;
			}
			else {
				entPointerTemp = entPointerTemp->sonSx;
			}
		}
	}
	if (entPointerTemp == NULL) { return; }
	else {
		relPointerTemp = entPointerTemp->relPointer;
		while (relPointerTemp != NULL) {
			deleteRelationFromQueue(entPointerTemp->name, relPointerTemp->receiver, relPointerTemp->nameRel);	//deletes the relations which the entity has.
			relPointerTemp = relPointerTemp->relNext;
		}
		if (entPointerTemp == entHead) {
			if ((entPointerTemp->sonDx == NULL) && (entPointerTemp->sonSx == NULL)) {
				entHead = NULL;
			}
			else {
				entHead = entHead->sonDx;
				putTreeNode(entPointerTemp->sonSx);
			}
		}
		else {
			entPointerBack = entHead;
			while ((entPointerBack->sonDx != entPointerTemp) && (entPointerBack->sonSx != entPointerTemp)) {
				if (strcmp(entPointerBack->name, nameEnt) < 0) {
					entPointerBack = entPointerBack->sonDx;
				}
				else {
					entPointerBack = entPointerBack->sonSx;
				}
			}
			if (entPointerBack->sonDx == entPointerTemp) {
				entPointerBack->sonDx = entPointerTemp->sonDx;
				putTreeNode(entPointerTemp->sonSx);
			}
			else {
				entPointerBack->sonSx = entPointerTemp->sonSx;
				putTreeNode(entPointerTemp->sonDx);
			}
		}
	}
	free(entPointerTemp);
}


void putTreeNode(EntityPointer entPoint) {
	EntityPointer entTemp = entHead;
	if (entPoint != NULL) {
		while (entTemp != NULL) {
			if (strcmp(entTemp->name, entPoint->name) < 0) {
				if (entTemp->sonDx == NULL) { break; }
				else entTemp = entTemp->sonDx;
			}
			else {
				if (entTemp->sonSx == NULL) { break; }
				else entTemp = entTemp->sonSx;
			}
		}
		if (entTemp == NULL) { entHead = entPoint; }
		else {
			if (strcmp(entTemp->name, entPoint->name) < 0) {
				entTemp->sonDx = entPoint;
			}
			else { entTemp->sonSx = entPoint; }
		}
	}
}


void deleteEntityFromQueue(char* nameEnt) {
	RelStructPointer relStructTemp = relHead;
	RelQueuePointer	relQueueTemp = NULL;

	while (relStructTemp != NULL) {
		relQueueTemp = relStructTemp->head;
		while (relQueueTemp != NULL) {
			if (!(strcmp(relQueueTemp->receiver, nameEnt))) {
				if (relQueueTemp->relBef != NULL) { 
					relQueueTemp->relBef->relNext = relQueueTemp->relNext;
				}
				if (relQueueTemp->relNext != NULL) {
					relQueueTemp->relNext->relBef = relQueueTemp->relBef;
				}
				if (relQueueTemp == relStructTemp->head) { 
					relStructTemp->head = relQueueTemp->relNext; 
					isQueueEmpty(relStructTemp);
				}
				/*else if (relQueueTemp == relStructTemp->tail) { 
					relStructTemp->tail = relQueueTemp->relBef;
					relStructTemp->tail->relNext = NULL;
				}*/
				free(relQueueTemp);
				break;
			}
			else { relQueueTemp = relQueueTemp->relNext; }
		}
		relStructTemp = relStructTemp->relNext;
	}
}


void isQueueEmpty(RelStructPointer relStruct) {	//checks whether the structRel has a relQueue empty; if so, it frees it.
	RelQueuePointer	relQueueTemp = relStruct->head;
	RelStructPointer relTemp = relHead;

	if (relStruct->head == NULL) {
		if (relTemp == relStruct) {
			relHead = relStruct->relNext;
		}
		else {
			while (relTemp->relNext != relStruct) {
				relTemp = relTemp->relNext;
			}
			relTemp->relNext = relStruct->relNext;
		}
	}
}


void recursiveSearch(EntityPointer entTemp, char* receiver) {
	RelPointer relTemp = entTemp->relPointer;
	RelPointer relTempBack = entTemp->relPointer;

	while (relTemp != NULL) {
		if (!(strcmp(relTemp->receiver, receiver))) {
			if (entTemp->relPointer == relTemp) {
				entTemp->relPointer = entTemp->relPointer->relNext;
				relTemp = entTemp->relPointer;
				if (relTemp == NULL) { break; }
			}
			else {
				while (relTempBack->relNext != relTemp) {
					relTempBack = relTempBack->relNext;
				}
				relTempBack->relNext = relTemp->relNext;
			}
		}
		relTemp = relTemp->relNext;
	}
	if (entTemp->sonDx != NULL) { recursiveSearch(entTemp->sonDx, receiver); }
	if (entTemp->sonSx != NULL) { recursiveSearch(entTemp->sonSx, receiver); }
}