#ifndef SPSC_QUEUE_H
#define SPSC_QUEUE_H

#include <stdatomic.h>
#include <stdlib.h>
#include <stdalign.h>

#define EMPTY_VALUE -123

struct Node {
  int val;
  struct Node* next;
};

struct Node * create_new_node () {
  struct Node* new_node = (struct Node*) malloc (sizeof (struct Node));
  new_node->next = NULL;

  return new_node;
}

struct SPSC_QUEUE {
  alignas(128) _Atomic(struct Node*) head;
  alignas(128) _Atomic(struct Node*) tail;
};

void spsc_push (struct SPSC_QUEUE* queue, int val) {
  struct Node* new_dummy = create_new_node ();
  struct Node* current_tail = atomic_load(&queue->tail); 
  
  current_tail->val = val;
  current_tail->next = new_dummy;
  atomic_store (&queue->tail, new_dummy);
}

int spsc_try_pop (struct SPSC_QUEUE* queue) {
  struct Node* current_head = atomic_load (&queue->head);
  
  if (current_head == atomic_load (&queue->tail)) {
    return EMPTY_VALUE;
  }
  
  int val = current_head->val;
  struct Node* next_node = current_head->next;
   
  free (current_head);
  atomic_store (&queue->head, next_node);

  return val;
}

void SPSC_QUEUE_init (struct SPSC_QUEUE* queue) {
  struct Node* dummy_node = create_new_node ();
  atomic_store (&queue->head, dummy_node);
  atomic_store (&queue->tail, dummy_node);
}

#endif
