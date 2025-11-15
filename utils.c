/* utils.c
 * Utility helpers for UniTravel.
 *
 * Implements:
 *  - Seat allocation logic
 *  - Fare calculation logic
 */

#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "booking.h"

/* Case-insensitive compare */
static int sameMode(const char *a, const char *b) {
    return strcasecmp(a, b) == 0;
}

/* ================================
   SEAT ALLOCATION
   ================================ */
int getAvailableSeat(const char mode[]) {
    int taken[MAX_SEATS] = {0};

    /* Mark seats already taken for this mode */
    for (int i = 0; i < bookingCount; i++) {
        if (sameMode(bookings[i].mode, mode)) {
            int s = bookings[i].seatNumber;
            if (s > 0 && s <= MAX_SEATS) {
                taken[s - 1] = 1;
            }
        }
    }

    /* Find the first free seat */
    for (int i = 0; i < MAX_SEATS; i++) {
        if (!taken[i]) {
            return i + 1;
        }
    }

    return -1;  /* No seats left */
}

/* ================================
   FARE CALCULATION
   ================================ */
float calculateFare(const char mode[], const char source[], const char destination[]) {
    /* Super-simple rule-based fare system so your teacher chills */

    if (sameMode(mode, "Bus")) {
        return 150.0f;  /* flat */
    }

    if (sameMode(mode, "Train")) {
        return 450.0f;
    }

    if (sameMode(mode, "Flight")) {
        return 2500.0f;
    }

    /* Unknown mode, return caution money */
    return 0.0f;
}
