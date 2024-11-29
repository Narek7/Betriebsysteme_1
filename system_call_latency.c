#include <stdio.h>
#include <unistd.h>    // Für getpid()
#include <time.h>      // Für clock_gettime()
#include <stdint.h>    // Für uint64_t-Datentyp

#define ITERATIONS 1000000 // Anzahl der System-Call-Aufrufe

int main() {
    struct timespec start, end; // Strukturen für hochauflösende Zeitmessungen
    uint64_t total_time = 0;    // Gesamte Zeit für alle Aufrufe (in Nanosekunden)
    uint64_t min_time = UINT64_MAX; // Kleinster gemessener Wert (initial auf maximalen Wert gesetzt)

    for (int i = 0; i < ITERATIONS; i++) {
        // Startzeit vor dem Systemaufruf messen
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Systemaufruf: getpid() (liefert die Prozess-ID zurück)
        getpid();

        // Endzeit nach dem Systemaufruf messen
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Dauer des Aufrufs in Nanosekunden berechnen
        uint64_t time_ns = (end.tv_sec - start.tv_sec) * 1000000000 +
                           (end.tv_nsec - start.tv_nsec);

        // Gesamte Zeit summieren
        total_time += time_ns;

        // Aktualisiere die minimale gemessene Zeit
        if (time_ns < min_time) {
            min_time = time_ns;
        }
    }

    // Durchschnittliche Latenz berechnen
    double avg_time = total_time / (double)ITERATIONS;

    // Ergebnisse ausgeben
    printf("System-Call-Latenz (getpid):\n");
    printf("  Minimale Latenz: %lu ns\n", min_time);
    printf("  Durchschnittliche Latenz: %.2f ns\n", avg_time);

    return 0;
}
