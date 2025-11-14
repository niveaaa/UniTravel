/* data.c
 * Implements file handling for UniTravel.
 *
 * Handles saving/loading bookings from bookings.txt
 * and generating unique ticket IDs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "booking.h"

/* ================================
   LOAD FROM FILE (bookings.txt)
   ================================ */
void loadDataFromFile() {
    FILE *fp = fopen("bookings.txt", "r");
    if (!fp) {
        /* File doesn't exist yet, which is fine */
        return;
    }

    bookingCount = 0;

    while (!feof(fp)) {
        Ticket t;
        if (fscanf(fp, "%d,%49[^,],%29[^,],%29[^,],%9[^,],%d,%f\n",
                   &t.ticketID,
                   t.passengerName,
                   t.source,
                   t.destination,
                   t.mode,
                   &t.seatNumber,
                   &t.fare) == 7) {
            bookings[bookingCount++] = t;
        }
    }

    fclose(fp);
}

/* ================================
   SAVE TO FILE (bookings.txt)
   ================================ */
void saveDataToFile() {
    FILE *fp = fopen("bookings.txt", "w");
    if (!fp) {
        printf("Error saving data.\n");
        return;
    }

    for (int i = 0; i < bookingCount; i++) {
        Ticket *t = &bookings[i];

        fprintf(fp, "%d,%s,%s,%s,%s,%d,%.2f\n",
                t->ticketID,
                t->passengerName,
                t->source,
                t->destination,
                t->mode,
                t->seatNumber,
                t->fare);
    }

    fclose(fp);
}

/* ================================
   GENERATE UNIQUE TICKET ID
   ================================ */
int generateTicketID() {
    int maxID = 1000;

    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].ticketID > maxID) {
            maxID = bookings[i].ticketID;
        }
    }

    return maxID + 1;
}
