
#ifndef _LIST_H_
#define _LIST_H_

#include <Arduino.h>

#define listSize(list) (list).size;

struct node_t{
  node_t *next;
  void *datum;
};

struct list_t{
  node_t *root;
  node_t *curr;
  int size;
};

list_t *listNew();
list_t *listAdd(list_t *l, void *d);

#endif
