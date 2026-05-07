#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBERS 1024
#define BUFFER_CAPACITY 8
#define NUM_CONSUMERS 4

typedef struct {
  int *data;
  int capacity;

  int head;
  int tail;
  int size;

  pthread_mutex_t mutex;

  bool done;
} bounded_buffer_t;

bool bb_empty(bounded_buffer_t *bb) { return bb->size == 0; }

bool bb_full(bounded_buffer_t *bb) { return bb->size == bb->capacity; }

void bb_push(bounded_buffer_t *bb, int value) {
  assert(!bb_full(bb));

  bb->data[bb->tail] = value;
  bb->tail = (bb->tail + 1) % bb->capacity;
  bb->size++;
}

int bb_pop(bounded_buffer_t *bb) {
  assert(!bb_empty(bb));

  int value = bb->data[bb->head];
  bb->head = (bb->head + 1) % bb->capacity;
  bb->size--;

  return value;
}

void bb_finish(bounded_buffer_t *bb) {
  pthread_mutex_lock(&bb->mutex);
  bb->done = true;
  pthread_mutex_unlock(&bb->mutex);
}

void bb_block_push(bounded_buffer_t *bb, int value) {
  pthread_mutex_lock(&bb->mutex);

  int spins = 0;

  while (bb_full(bb)) {
    spins++;

    pthread_mutex_unlock(&bb->mutex);

    // Simulate useless spinning work
    // usleep(100);

    pthread_mutex_lock(&bb->mutex);
  }

  if (spins > 0) {
    printf("[Producer spun %d times waiting for space]\n", spins);
  }

  bb_push(bb, value);

  pthread_mutex_unlock(&bb->mutex);
}

int bb_block_pop(bounded_buffer_t *bb, bool *done) {
  pthread_mutex_lock(&bb->mutex);

  int spins = 0;

  while (bb_empty(bb) && !bb->done) {
    spins++;

    pthread_mutex_unlock(&bb->mutex);

    // Simulate useless spinning work
    // usleep(100);

    pthread_mutex_lock(&bb->mutex);
  }

  if (spins > 0) {
    printf("[Consumer %lu spun %d times waiting for data]\n", pthread_self(),
           spins);
  }

  int value = 0;

  if (bb_empty(bb) && bb->done) {
    *done = true;
  } else {
    *done = false;
    value = bb_pop(bb);
  }

  pthread_mutex_unlock(&bb->mutex);

  return value;
}

void *producer_thread(void *arg) {
  bounded_buffer_t *buf = (bounded_buffer_t *)arg;

  for (int i = 0; i < NUMBERS; ++i) {
    printf("Producing %d\n", i);

    bb_block_push(buf, i);

    // Producer faster than consumers
    usleep(100);
  }

  bb_finish(buf);

  return NULL;
}

void *consumer_thread(void *arg) {
  bounded_buffer_t *buf = (bounded_buffer_t *)arg;

  while (1) {
    bool done = false;

    int number = bb_block_pop(buf, &done);

    if (done) {
      break;
    }

    printf("Consumer %lu consumed %d\n", pthread_self(), number);

    // Slow consumers to create contention
    usleep(5000);
  }

  return NULL;
}

int main() {
  bounded_buffer_t buffer;

  buffer.capacity = BUFFER_CAPACITY;
  buffer.data = malloc(sizeof(int) * BUFFER_CAPACITY);

  buffer.head = 0;
  buffer.tail = 0;
  buffer.size = 0;
  buffer.done = false;

  pthread_mutex_init(&buffer.mutex, NULL);

  pthread_t producer;
  pthread_t consumers[NUM_CONSUMERS];

  pthread_create(&producer, NULL, producer_thread, &buffer);

  for (int i = 0; i < NUM_CONSUMERS; ++i) {
    pthread_create(&consumers[i], NULL, consumer_thread, &buffer);
  }

  pthread_join(producer, NULL);

  for (int i = 0; i < NUM_CONSUMERS; ++i) {
    pthread_join(consumers[i], NULL);
  }

  pthread_mutex_destroy(&buffer.mutex);

  free(buffer.data);

  return 0;
}