#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "C:\Users\jorge_1dr5m1r\OneDrive\Desktop\GitHub\DDD-Urdiales\tda_map\map.c"
#include "C:\Users\jorge_1dr5m1r\OneDrive\Desktop\GitHub\DDD-Urdiales\tda_pq\pq.c"

#define TABLE_TAMANO 128

#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"

const char* COLORES[] = {
    RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};
#define NUM_COLORES (sizeof(COLORES) / sizeof(COLORES[0]))

typedef struct{
    char letra;
    int frecuencia;
}LetraFrecuencia;

typedef struct HuffmanNode{
    char letra;
    int frecuencia;
    struct HuffmanNode *izquierda;
    struct HuffmanNode *derecha;
}HuffmanNode;

int hash_function(void *key){
    char *char_key = (char*)key;
    return (unsigned int) (*char_key) % TABLE_TAMANO;
}
int key_equals(void *key1, void *key2){
    return (*(char*) key1) == (*(char*) key2);
}

int compare_frecuencia(const void *a, const void *b){
    const LetraFrecuencia *f1 = (const LetraFrecuencia *)a;
    const LetraFrecuencia *f2 = (const LetraFrecuencia *)b;
    if(f1->frecuencia != f2->frecuencia){
        return f1->frecuencia - f2->frecuencia;
    }
    return f1->letra - f2->letra;
}

int compare_node(void *a, void *b){
    HuffmanNode *node1 = (HuffmanNode*) a;
    HuffmanNode *node2 = (HuffmanNode*) b;
    return node1->frecuencia - node2->frecuencia;
}

HuffmanNode* crear_huffmanNodes(char letra, int frecuencia){
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->letra = letra;
    node->frecuencia = frecuencia;
    node->izquierda = NULL;
    node->derecha = NULL;
    return node;
}

void print_arbol(HuffmanNode* raiz, int nivel){
    if(raiz == NULL) return;
    const char* color_actual = COLORES[nivel % NUM_COLORES];

    for(int i = 0; i<nivel; i++) printf("  ");

    if(raiz->letra == '\0'){
        printf("%s*(%d)%s\n", color_actual,raiz->frecuencia, RESET);
    } else if(raiz->letra == ' ') {
        printf("%sEspacio(%d)%s\n",color_actual, raiz->frecuencia, RESET);
    }else{
        printf("%s%c(%d)%s\n",color_actual, raiz->letra, raiz->frecuencia,RESET);
    }
    print_arbol(raiz->izquierda, nivel + 1);
    print_arbol(raiz->derecha, nivel + 1);
}
void free_arbol(HuffmanNode* raiz){
    if(raiz == NULL) return;
    free_arbol(raiz->izquierda);
    free_arbol(raiz->derecha);
    free(raiz);
}

void generar_codigo(HuffmanNode* raiz, char* codigo_actual, int profundidad, map* diccionario) {
    // Si el nodo es nulo, terminar recursion
    if (raiz == NULL) return;

    //Guardar en diccionario
    if (raiz->izquierda == NULL && raiz->derecha == NULL) { // Verificar si es un nodo hoja
        // Crear una clave para el caracter
        char* clave = (char*)malloc(sizeof(char));
        *clave = raiz->letra;

        // Crear codigo binario del caracter y reservar memoria para el codigo
        char* codigo = (char*)malloc((profundidad + 1) * sizeof(char));
        
        // Copiar el codigo generado
        strncpy(codigo, codigo_actual, profundidad);
        // Agregar terminador nulo
        codigo[profundidad] = '\0';

        // Guardar el par (caracter, codigo) en diccionario
        map_put(diccionario, clave, codigo);
        return;
    }
    if (raiz->izquierda) { // Rama izquierda
        //Agregar 0 si es izquierda
        codigo_actual[profundidad] = '0';
        generar_codigo(raiz->izquierda, codigo_actual, profundidad + 1, diccionario);
    }
    if (raiz->derecha) { // Rama derecha
        // Agregar 1 si es derecha
        codigo_actual[profundidad] = '1';
        generar_codigo(raiz->derecha, codigo_actual, profundidad + 1, diccionario);
    }
}

void codificar_texto(char* texto, map* diccionario) {
    printf("\nTexto codificado:\n");

    //Recorrer cada caracter del texto
    for (int i = 0; texto[i] != '\0'; i++) {
        // Obtener el valor de cada uno
        char clave = texto[i];
        //Buscar el valor en el diccionario 
        char* codigo = (char*)map_get(diccionario, &clave);
        
        //Imprimir si se econtro
        if (codigo != NULL) {
            printf("%s", codigo);
        }
    }
    printf("\n");
}

void decodificar_texto(char* texto_comprimido, HuffmanNode* raiz) {
    printf("\nTexto descomprimido:\n");
    // Comenzamos en la raiz del arbol de Huffman
    HuffmanNode* nodo_actual = raiz;
    // Recorremos cada bit del texto comprimido
    for (int i = 0; texto_comprimido[i] != '\0'; i++) {
        // Si es 0, vamos a la izquierda
        if (texto_comprimido[i] == '0') {
            nodo_actual = nodo_actual->izquierda;
        } 
        // Si es 1, vamos a la derecha
        else if (texto_comprimido[i] == '1') {
            nodo_actual = nodo_actual->derecha;
        } 
        // Ignorar otros caracteres
        else {
            continue;
        }
        // Si llegamos a un nodo hoja 
        if (nodo_actual->izquierda == NULL && nodo_actual->derecha == NULL) {
            // Imprimir la letra
            printf("%c", nodo_actual->letra);
            
            // Regresar a la raiz para comenzar a decodificar el siguiente 
            nodo_actual = raiz;
        }
    }
    printf("\n");
}

int main(){
    char codigo[100];
    printf("Escriba la oracion a comprimir: \n");
    scanf(" %99[^\n]", codigo);

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
                int *new_count = (int*)malloc(sizeof(int));
                *new_count = 1;
                map_put(m,key,new_count);
            }
        }
    }

    for (int i=0; i<TABLE_TAMANO;i++){
        node*n = m->hashTable[i];
        while(n!=NULL){
            frecuencias[total_letras].letra = *(char*)n->key;
            frecuencias[total_letras].frecuencia = *(int*)n->value;
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

    pq* cola =pq_create(total_letras, compare_node);
    for(int i = 0; i<total_letras;i++){
        HuffmanNode* node = crear_huffmanNodes(frecuencias[i].letra, frecuencias[i].frecuencia);
        pq_offer(cola, node);
    }

    while(pq_size(cola) > 1){
        HuffmanNode* izq = (HuffmanNode*)pq_poll(cola);
        HuffmanNode* der = (HuffmanNode*)pq_poll(cola);
        HuffmanNode* padre = crear_huffmanNodes('\0', izq->frecuencia + der->frecuencia);
        padre->izquierda = izq;
        padre->derecha = der;
        pq_offer(cola, padre);
    }

    HuffmanNode* raiz = (HuffmanNode*)pq_poll(cola);
    
    printf("\nArbol de Huffman:\n");
    print_arbol(raiz, 0);

    for(int i = 0; i<TABLE_TAMANO; i++){
        node *actual = m->hashTable[i];
        while(actual != NULL){
            free(actual->value);
            free(actual->key);
            node *temp = actual;
            actual = actual->next;
            free(temp);
        }   
    }
//==============
    map *diccionario = map_create(TABLE_TAMANO, hash_function, key_equals);
    char codigo_actual[100];
    generar_codigo(raiz, codigo_actual, 0, diccionario);

    printf("\nTabla de códigos Huffman:\n");
    for (int i = 0; i < TABLE_TAMANO; i++) {
        node *n = diccionario->hashTable[i];
        while (n != NULL) {
            printf("%c: %s\n", *(char*)n->key, (char*)n->value);
            n = n->next;
        }
    }

    codificar_texto(codigo, diccionario);


    map_destroy(diccionario);
//==============
    map_destroy(m);
    free_arbol(raiz);
    free(cola);

    return 0;
}