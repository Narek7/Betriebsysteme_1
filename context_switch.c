#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#define ITERATIONS 1000000 // Anzahl der Kontextwechsel

// Globale Variablen für Synchronisation
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&lock2); // Warte auf den Main-Thread
        pthread_mutex_unlock(&lock1); // Signalisiere den Main-Thread
    }
    return NULL;
}

int main() {
    struct timespec start, end;
    pthread_t thread;

    // Lock 2 zuerst sperren, damit der zweite Thread blockiert startet
    pthread_mutex_lock(&lock2);

    // Thread erstellen
    pthread_create(&thread, NULL, thread_func, NULL);

    // Messung starten
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&lock1); // Warte auf den Thread
        pthread_mutex_unlock(&lock2); // Signalisiere den Thread
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Warte auf Thread-Ende
    pthread_join(thread, NULL);

    // Dauer berechnen (in Nanosekunden)
    uint64_t total_time = (end.tv_sec - start.tv_sec) * 1000000000 +
                          (end.tv_nsec - start.tv_nsec);

    double avg_time = total_time / (double)(ITERATIONS * 2); // 2 Wechsel pro Iteration

    // Ergebnisse ausgeben
    printf("Durchschnittliche Kontextwechselzeit: %.2f ns\n", avg_time);

    return 0;
}
