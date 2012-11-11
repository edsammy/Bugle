
#include "list.h"
  
node_t *nodeNew(void *d)
{
  node_t *node = (node_t*)malloc(sizeof(node_t));
  node->next = 0;
  node->datum = d;
  return node;
}
  
list_t *listNew()
{
  list_t *list = (list_t*)malloc(sizeof(list_t));
  list->size = 0;
  list->root = 0;
  list->curr = 0;
  return list; 
}

list_t *listAdd(list_t *l, void *d)
{
  if(l->root == 0)
  {
    l->root = nodeNew(d);
    l->curr = l->root;
  }
  else
  {
    l->curr->next = nodeNew(d);
    l->curr = l->curr->next;  
  }
  l->size++;
}
