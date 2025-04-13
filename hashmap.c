#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long capacidad = map->capacity ;
    if (capacidad == map->size) return ;
    for(int i = 0 ; i < capacidad ; i++){
        if (map->buckets[i] == NULL) continue ;
        if (is_equal(map->buckets[i]->key, key)) return ;
    }
    long posicion = hash(key, capacidad) % capacidad ;
    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
    {
        posicion = (posicion + 1) % capacidad ;
    }
    Pair *par = (Pair *) malloc(sizeof(Pair)) ;
    par->key = key ;
    par->value = value ;
    map->buckets[posicion] = par ;
    map->size ++ ;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair **buckets_auxiliar = map->buckets ;
    long old_capacity = map->capacity ; 
    map->capacity *= 2 ;
    long new_capacity = map->capacity ;
    Pair ** nuevo_buckets = (Pair **) malloc(sizeof(Pair *) * new_capacity) ;
    for(long i = 0 ; i < new_capacity ; i++){
        nuevo_buckets[i] = (Pair *) malloc(sizeof(Pair)) ;
        nuevo_buckets[i] = NULL ;
    }
    map->buckets = nuevo_buckets ;
    map->size = 0 ;
    for(long i = 0 ; i < old_capacity ; i++){
        if (buckets_auxiliar[i] == NULL) continue ;
        insertMap(map, buckets_auxiliar[i]->key, buckets_auxiliar[i]->value) ;
    }


}


HashMap * createMap(long capacity) {
    HashMap *mapa = (HashMap *) malloc(sizeof(HashMap)) ;
    mapa->current = -1 ;
    mapa->capacity = capacity ;
    mapa->size = 0 ;
    mapa->buckets = (Pair **) malloc(sizeof(Pair *) * capacity) ;
    for(long i = 0 ; i < capacity ; i++){
        mapa->buckets[i] = (Pair *) malloc(sizeof(Pair)) ;
        mapa->buckets[i] = NULL ;
    }
    return mapa ;
}

void eraseMap(HashMap * map,  char * key) {    
    long capacidad = map->capacity ;
    long posible_pos = hash(key, capacidad) ; 
    while (1){
        if (map->buckets[posible_pos] == NULL) return ;
        if (is_equal(map->buckets[posible_pos]->key, key)){
            map->buckets[posible_pos]->key = NULL ;
            map ->size -- ;
            return ;
        }
        posible_pos = (posible_pos + 1) % capacidad ;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long capacidad = map->capacity ;
    
    long posible_pos = hash(key, capacidad) ;
    if (is_equal(map->buckets[posible_pos]->key, key)){ 
        map->current = posible_pos ;
        return map->buckets[posible_pos] ;
    }
    while (1){
        posible_pos = (posible_pos + 1) % capacidad ;
        Pair * par = map->buckets[posible_pos] ;
        if (par == NULL) break ;
        if (is_equal(par->key, key)) return par ;
    }
    return NULL ;
}

Pair * firstMap(HashMap * map) {
    long capacidad = map->capacity ;
    Pair *par ;
    for(int i = 0 ; i < capacidad ; i++){
        par = map->buckets[i] ;
        if (par != NULL) {
            if (par->key != NULL){
                map->current = i ;
                return par ;
            } 
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    long capacidad = map->capacity ;
    long current = map->current ;
    Pair *par ;
    for(int i = current + 1 ; i < capacidad ; i++){
        par = map->buckets[i] ;
        if (par != NULL) {
            if (par->key != NULL){
                map->current = i ;
                return par ;
            } 
        }
    }
    return NULL;
}
