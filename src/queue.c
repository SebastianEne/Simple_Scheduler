#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


struct queue_s *enqueue(struct queue_s *q, void *element, ssize_t size)
{
  struct queue_s *head, *last;

  if (q == NULL)
    {
      q = (struct queue_s *) malloc(sizeof(struct queue_s));
      q->head = NULL;
      q->data = element;
      q->data_size = size;

      return q;
    }

  head = q;
  last = q;
  while (q->head != NULL)
    {
      q = q->head;
      last = q;
    }

  last->head = (struct queue_s *) malloc(sizeof(struct queue_s));
  last->head->data      = element;
  last->head->data_size = size;
  last->head->head = NULL;

  return head;
}

struct queue_s *dequeue(struct queue_s *q)
{
  struct queue_s *next;

  if (q == NULL)
    {
      return NULL;
    }

  next = q->head;

  free(q->data);
  free(q);

  return next;
}

struct queue_s *rotate(struct queue_s *q)
{
  struct queue_s *first, *before_first, *before_last;

  if (q == NULL)
    {
      return q;
    }

  if (q->head == NULL)
    {
      return q;
    }

  before_first = q;
  first = q->head;

  while (q->head != NULL)
    {
      before_last = q;
      q = q->head;
    }

  before_last->head = before_first;
  before_first->head = NULL;

  if (first->head == NULL)
    {
      first->head = before_first;
    }
  return first;
}
