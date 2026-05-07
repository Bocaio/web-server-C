# Multi-Threads

In Multithreading, to use shared memory, You have to use **Mutex**(Mutual Exclusion) to avoid concurrency bugs like **Atomicity** and **Data Race** bugs.

---

## Producer/Consumer Pattern

In a Producer/Consumer Pattern, Avoid Busy-waiting and use **Conditional Variables** to wait and broadcast signal (`pthread_cond_t`, `pthread_cond_wait`,`pthread_cond_broadcast`)