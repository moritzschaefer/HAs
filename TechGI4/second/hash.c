#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define TABLE_SIZE 256

struct element{
		short int key;
		short int value;
		bool used;
};

int main(){
	struct element table[TABLE_SIZE];
	initTable(table);

	/*	for(i=0; i<TABLE_SIZE; i++){
		table[hash(i)].key=i;
		printf("%i\n", table[i].key);}*/
return 0;
}


bool set(key, struct element table[], *value){
	int j;
	for(j=0;j<TABLE_SIZE;j++){
		if (table[key % TABLE_SIZE+j].key == key){			//nach key suchen
			table[key % TABLE_SIZE+j].value = *value;
			return true;
		}													//wenn nicht vorhanden
	for(j=0;j<TABLE_SIZE;j++){								//hinzufügen
		if (!table[key % TABLE_SIZE+j].used){
			table[key % TABLE_SIZE+j].key = key;
			return true;
		}
	}
	return false;
}



bool get(key, struct element table[], short int *value){
	int j;	
	for(j=0;j<TABLE_SIZE;j++){
		if (table[key % TABLE_SIZE+j].key == key){
			*value = table[key % TABLE_SIZE+j].value;
			return true;
	}
	return false;
}	



bool delete(key, struct element table[]){
	int j;	
	for(j=0;j<TABLE_SIZE;j++){
		if (table[key % TABLE_SIZE+j].key == key){
			table[key % TABLE_SIZE+j].used = 0;
			return true;
	}
	return false;
}



void initTable(struct element table[]){
	int i;
	for(i=0; i<TABLE_SIZE; i++){
		table[i].used  = 0;
		table[i].key   = 0;
		table[i].value = 0;
	}
}
