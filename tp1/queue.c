// https://

#include "queue.h"
#include <stdlib.h>

typedef struct node node_t;

struct node {
  void* data;
  node_t* next;
};

struct queue {
  node_t* first;
  node_t* last;
};

node_t* create_node(void *data) {
  node_t* new_node = malloc(sizeof(node_t));
  if(!new_node) return NULL;
  
  new_node->data = data;
  new_node->next = NULL;

  return new_node;
}

queue_t *create_queue(void) {
  queue_t* new_queue = malloc(sizeof(queue_t));
  if(!new_queue) return NULL;

  new_queue->first = NULL;
  new_queue->last = NULL;

  return new_queue;
}

void destroy_queue(queue_t *queue, void (*destroy_fn)(void *)) {
  while(!is_empty_queue(queue)) {
    void* data = delete_element_queue(queue);
    if(destroy_fn) destroy_fn(data);
  }
  free(queue);
}

bool is_empty_queue(const queue_t *queue) {
  return queue->first == NULL;
}

bool add_element_queue(queue_t *queue, void *data) {
  node_t* new_node = create_node(data);
  if(!new_node) return false;

  if(is_empty_queue(queue)) queue->first = new_node;
  else queue->last->next = new_node;
  queue->last = new_node;

  return true;
}

void *return_first_queue(const queue_t *queue) {
  if(is_empty_queue(queue)) return NULL;
  return queue->first->data;
}

void *delete_element_queue(queue_t *queue) {
  if(is_empty_queue(queue)) return NULL;

  node_t* first_element = queue->first;
  void* data = return_first_queue(queue);

  queue->first = first_element->next;
  if(!queue->first) queue->last = NULL;
  free(first_element);

  return data;
}