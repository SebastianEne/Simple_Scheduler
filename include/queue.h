#ifndef __QUEUE_H
#define __QUEUE_H

struct queue_s
{
  struct queue_s *head;
  void *data;
  ssize_t data_size;
};

struct queue_s *enqueue(struct queue_s *q, void *element, ssize_t size);

struct queue_s *dequeue(struct queue_s *q);

struct queue_s *rotate(struct queue_s *q);

#endif //__QUEUE_H
