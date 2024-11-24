#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "C:\Users\jorge_1dr5m1r\OneDrive\Desktop\GitHub\DDD-Urdiales\tda_map\map.c"

#define TABLE_TAMANO 128

typedef struct{
    char letra;
    int frecuencia;
}LetraFrecuencia;

unsigned int hash_function(char *key){
    return (unsigned int) (*key) % TABLE_TAMANO;
}
int key_equals(void *key1, void *key2){
    return (*(char*) key1) == (*(char *) key2);
}
int compare_frecuencia(const void *a, const void *b){
    const LetraFrecuencia *f1 = (const LetraFrecuencia *)a;
    const LetraFrecuencia *f2 = (const LetraFrecuencia *)b;
    if(f1->frecuencia != f2->frecuencia){
        return f1->frecuencia - f2->frecuencia;
    }
    return f1->letra - f2->letra;
}

int main(){
    char codigo[100];
    printf("Escriba la oracion a comprimir: \n");
    scanf("%99[^\n]", codigo);

    map *m = map_create(TABLE_TAMANO, hash_function, key_equals);
    LetraFrecuencia frecuencias[TABLE_TAMANO];
    int total_letras = 0;

    for (int i = 0; codigo[i] != '\0'; i++){
        if (isalpha(codigo[i]) || codigo[i] == ' '){
            char c = isalpha(codigo[i]) ? tolower(codigo[i]) : codigo[i];
            
            int *count = (int*)map_get(m, &c);
            if(count!= NULL){
                (*count)++;
            }else{
                char *key = (char*)malloc(sizeof(char));
                *key = c;
                int *new_count = (int*)malloc(sizeof(char));
                *new_count = 1;
                map_put(m,key,new_count);
            }
        }
    }

    for (int i=0; i<TABLE_TAMANO;i++){
        node*n = m->hashTable[i];
        while(n!=NULL){
            frecuencias[total_letras].letra = *(char*)n->key;
            frecuencias[total_letras].frecuencia = *(char*)n->value;
            total_letras++;
            n = n->next;
        }
    }
    qsort(frecuencias, total_letras, sizeof(LetraFrecuencia), compare_frecuencia);

    printf("Letras repetidas: \n");
    for(int i = 0; i < total_letras; i++){
        if (frecuencias[i].letra == ' '){
             printf("Espacio: %d\n", frecuencias[i].frecuencia);
        } else{
            printf("%c: %d\n",frecuencias[i].letra, frecuencias[i].frecuencia);
        }
    }

    for(int i = 0; i<TABLE_TAMANO; i++){
        node *actual = m->hashTable[i];
        while(actual != NULL){
            free(actual->value);
            free(actual->key);
            actual = actual->next;
        }   
    }
    map_destroy(m);
    return 0;
}