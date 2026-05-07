#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define NUMBERS 1024
#define THREADS 8

bool is_prime(int n) {
  for (int i = 2; i < n; ++i) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}

typedef struct {
  int start_number;
  int end_number;
  int *primes;
  pthread_mutex_t *mutex;
} my_thread_args_t;

void *prime_thread(void *args_in) {
  my_thread_args_t *args = (my_thread_args_t *)args_in;
  for (int n = args->start_number; n < args->end_number; ++n) {
    if (is_prime(n)) {
      pthread_mutex_lock(args->mutex);
      *(args->primes) += 1;
      pthread_mutex_unlock(args->mutex);
    }
  }
  return NULL;
}

int main() {
  int primes;

  // Launch a thread to check every number.
  pthread_t threads[NUMBERS];
  // Create an array for thread args
  my_thread_args_t thread_args[NUMBERS];
  int numbers_per_thread = NUMBERS / THREADS;
  // Mutex Init
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  for (int i = 0; i < THREADS; ++i) {
    // use array of thread args since local variables (in Stack) can cause
    // undefined behavior
    thread_args[i] = (my_thread_args_t){
        .start_number = i == 0 ? 1 : i * numbers_per_thread,
        .end_number = (i + 1) * numbers_per_thread,
        .primes = &primes,
        .mutex = &mutex,
    };
    pthread_create(&threads[i], NULL, prime_thread, &thread_args[i]);
  }

  // Join all threads and print results when ready.
  for (int i = 1; i < NUMBERS; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("Prime Count is %d\n", primes);

  return 0;
}
