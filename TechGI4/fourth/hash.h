#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define TABLE_SIZE 256

struct element{
		short int key;
		short int value;
		bool used;
};

short int hash(short int key) {
    // return the hash function
    //short int val = (((3+key)*5+100)/3)%TABLE_SIZE;
    short int val = (key) % TABLE_SIZE;
    if(val < 0)
        val *= -1;
    assert(val <= 255);
    assert(val >= 0);
    return val;
}

short int sound(short int key) {
    //return the next option
    short int val = (key+1)%TABLE_SIZE;
    assert(val <= 255);
    assert(val >= 0);
    return val;
}

bool set(struct element *table, short int key, short int value){
    short int hash_value = hash(key);
    int counter = 0;
    while(counter <= TABLE_SIZE) {
        if(table[hash_value].key == key && table[hash_value].used == true){
			table[hash_value].value = value;
			return true;
        }
        hash_value = sound(hash_value);
        counter ++;
    }
    hash_value = hash(key);
    counter = 0;
    while(table[hash_value].used == true) {
        if(counter > TABLE_SIZE) {
            return false;
        }
        hash_value = sound(hash_value);
        counter++;
    }
    table[hash_value].value = value;
    table[hash_value].key = key;
    table[hash_value].used = true;
	return true;
}

bool get(struct element *table, short int key, short int *value){
    short int hash_value = hash(key);
    int counter = 0;
    while(table[hash_value].key != key || table[hash_value].used == false) {
        if(counter > TABLE_SIZE) {
            return false;
        }
        hash_value = sound(hash_value);
        counter++;
    }
    *value = table[hash_value].value;
    return true;
}

bool delete(struct element *table, short int key){
    short int hash_value = hash(key);
    int counter = 0;
    while(table[hash_value].key != key || table[hash_value].used == false) {
        if(counter > TABLE_SIZE) {
            return false;
        }
        hash_value = sound(hash_value);
        counter++;
    }
    table[hash_value].used = false;
    return true;
}

struct element *initTable(){
    struct element *table = calloc(sizeof(struct element), TABLE_SIZE);
	int i;
	for(i=0; i<TABLE_SIZE; i++){
		table[i].used  = false;
		table[i].key   = 0;
		table[i].value = 0;
	}
    return table;
}
void deleteTable(struct element *table) {
    free(table);
}
