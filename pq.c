#include <stdlib.h>
#include "pq.h"

struct pq_str {
  void ** heap;		/* heap es el arreglo que almacenará los datos 
                     a manera de un heap */
  int capacity;	  /* capacidad/tamaño físico del arreglo **/
  int size;				/* tamaño lógico de la cola de prioridad 
                     <--> posición del siguiente elemento a insertar */
  CompareFunc compare;	/* Función de comparación entre dos datos */
};

pq * pq_create(int initialCapacity, CompareFunc compare) {
  pq * queue = (pq * ) malloc(sizeof(struct pq_str));
  if(initialCapacity < 1) initialCapacity = 1;
  queue->capacity     = initialCapacity;
  queue->size         = 0;
  queue->compare      = compare;
  queue->heap         = (void *) malloc(initialCapacity * sizeof(void *));
  return queue;
}

int parent(int child) 
{
  //if(child == 0) return -1;
  return (child - 1) / 2;   // (0 - 1) / 2 = -1 / 2 = 0
}

void swap(void ** a, void ** b) {
  void * c = *a;
  *a = *b;
  *b =  c;
}

void pq_offer(pq * q, void * data)
{
  if(q->size == q->capacity){
    q->capacity *= 2;
    q->heap = realloc(q->heap, q->capacity * sizeof(void*));
  }

  int curr = q->size;
  q->heap[curr] = data;
  q->size++;

  while(curr > 0){
    int p = parent(curr);
    if(q->compare(q->heap[curr], q->heap[p]) >= 0) break;
    swap(&q->heap[curr], &q->heap[p]);
    curr = p;
  }
}

void * pq_poll(pq * q) 
{
  if(q->size == 0) return NULL;

  void* data = q->heap[0];
  q->size--;
  if(q->size > 0){
    q->heap[0] = q->heap[q->size];
    int curr = 0;
    while(1){
      int l = left(curr);
      int r = right(curr);
      int chico = curr;
      
      if(l < q->size && q->compare(q->heap[l], q->heap[chico]) < 0){
        chico = l;
        }
      if(r < q->size && q->compare(q->heap[r], q->heap[chico]) < 0){
        chico = r;
        }
      if(chico == curr) break;
      swap(&q->heap[curr], &q->heap[chico]);
      curr = chico;
    }
  } 
  return data;
}

void * pq_peek(pq * q)
{
  if(q->size == 0) return NULL;
  else return q->heap[0];

}

int left(int parent) {
  return 2 * parent + 1;
}

int right(int parent) {
  return 2 * parent + 2;
}



int pq_size(pq * q) 
{
  return q->size;
}
