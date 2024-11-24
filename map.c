  #include "map.h"
  #include <stdlib.h>
  #include <stdio.h>
  
  struct node_str {
    void * key, * value;
    struct node_str * next;
  };
  
  typedef struct node_str node;
  
  struct map_str {
    hash_func hash;
    equals_func key_equals;
    int M, size;
    node ** hashTable;
  };
  
  /* Crea la memoria del arreglo subyacente con el numero
     de buckets determiando (M), guarda la hash function
     y la comparison function dentro del mapa */
  map * map_create(int M, hash_func hf, equals_func eq) 
  {
    map * m = (map *) malloc(sizeof(struct map_str));
    m->M = M;
    m->hash = hf;
    m->key_equals = eq;
    m->size = 0;
    m->hashTable = (node **) malloc(M * sizeof(node *));
    if(m->hashTable == NULL){
      free(m);
      return NULL;
    }
  
    for(int i = 0; i < M ; i ++) 
      m->hashTable[i] = NULL;
    return m;
  }
  
  /* Regresa el tamanio del mapa */
  int map_size (map * m) 
  {
    return m->size;
  }
  
  /* Crea un nodo, guarda el dato del cliente en el */
  node * createNode(void * key, void * value) {
    node * n = (node *) malloc(sizeof(struct node_str));
    n->key = key;
    n->value = value;
    n->next = NULL;
    return n;
  }
  
  /* Guarda un key-value pair, :
     1) dada la llave, generar el hash.
     2) operacion mod al hash para determinar que "bucket"
        le corresponde.
     3) Revisa si la llave ya ha sido insertada en se bucket
        3.1) Si no, se inserta en la raiz
        3.2) Si si, regresa FALSE.
  */
  void map_put(map * m, void * key, void * value)
  {
    /* obtener el bucket donde insertar */
    int bucket = m->hash(key) % m->M;
    node * n = m->hashTable[bucket];
    //printf("map_put:start, looking in bucket %d\n", bucket);
  
    /* recorre la lista ligada */
    while(n != NULL && n->next != NULL) 
    {
      /* por cada elemento revisar si la llave ya existe
         con la funcion equals provista por el cliente */
      if(m->key_equals(n->key, key))
      { 
        /* actualiza el valor (sobreescribe) */
        //printf("map_put:Found, update!\n");
        n->value = value;
        return;
      }
      else
      {
        /* no es igual? buscar adelante */
        //printf("map_put:Not here, keep searching\n");
        n = n->next;
      }
    }
    /* si llegamos al final, la llave no existe y debemos
       crearla y unirla al final de la lista, aqui pueden
       pasar 2 cosas, insertar el primer elemento (se debe
       actualizar la raiz) o insertar al final de un elemento
       existente */
    if(m->hashTable[bucket] == NULL) 
    {
      /* case: the bucket is empty */
      //printf("map_put:Creating first node at bucket %d\n", bucket);
      m->hashTable[bucket] = createNode(key, value);
      m->size ++;
    } 
    else 
    {
      //printf("map_put:key not found create new node\n");
      node * new = createNode(key, value);
      n->next = new;
      m->size ++;
    }
  }
  
  /* regresa TRUE si el mapa contiene la llave */
  boolean map_contains(map *m, void * key) 
  {
    int bucket = m->hash(key) % m->M;
    node * n = m->hashTable[bucket];
    while(n != NULL) 
    {
      if(m->key_equals(n->key, key)) 
        return TRUE;
      else
        n = n->next;
    }
    return FALSE;
  }

  void map_destroy(map *m) {
    for (int i = 0; i < m->M; i++) {
        node *n = m->hashTable[i];
        while (n != NULL) {
            node *temp = n;
            n = n->next;
            free(temp);
        }
    }
    free(m->hashTable);
    free(m);
}
  
  void * map_get(map *m, void *key) 
  {
    /* Busca la llave en el mapa y retorna el apuntador a al
       value asociado.
       
       1) localiza el bucket donde vamos a buscar
       2) recorre la lista hasta encontrar la llave, regresa
          el value que le corresponde o NULL si no lo encuentras */
    
    int bucket = m->hash(key) % m->M;
    node * n = m->hashTable[bucket];
    while(n != NULL) 
    {
      if(m->key_equals(n->key, key)) 
        break;
      else
        n = n->next;
    }
    return n != NULL ? n->value : NULL;
  }