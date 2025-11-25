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

/* Portable case-insensitive compare for short strings like "Bus", "Train" */
static int sameMode(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        char ca = (char) tolower((unsigned char) *a);
        char cb = (char) tolower((unsigned char) *b);
        if (ca != cb) return 0;
        a++; b++;
    }
    return (*a == '\0' && *b == '\0');
}

/* ================================
   SEAT ALLOCATION (DATE-AWARE)
   ================================ */
int getAvailableSeat(const char mode[], const char date[]) {
    int taken[MAX_SEATS] = {0};

    /* Mark seats already taken for this mode AND date */
    for (int i = 0; i < bookingCount; i++) {
        if (sameMode(bookings[i].mode, mode)) {
            if (bookings[i].date[0] != '\0' && strcmp(bookings[i].date, date) == 0) {
                int s = bookings[i].seatNumber;
                if (s > 0 && s <= MAX_SEATS) {
                    taken[s - 1] = 1;
                }
            }
        }
    }

    for (int i = 0; i < MAX_SEATS; i++) {
        if (!taken[i]) return i + 1;
    }

    return -1;
}

/* ================================
   FARE CALCULATION
   ================================ */
float calculateFare(const char mode[], const char source[], const char destination[]) {
    /* Simple fixed fares — tweak as needed */
    if (sameMode(mode, "Bus")) return 150.0f;
    if (sameMode(mode, "Train")) return 450.0f;
    if (sameMode(mode, "Flight")) return 2500.0f;
    return 0.0f;
}