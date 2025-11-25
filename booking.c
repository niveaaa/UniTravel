/* booking.c
 * Implements booking, cancellation, and display logic.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "booking.h"
#include "data.h"
#include "utils.h"

/* Global storage (declared here, referenced via extern in booking.h) */
Ticket bookings[MAX_BOOKINGS];
int bookingCount = 0;

/* Helper: lowercase compare for mode matching */
static int sameMode(const char *a, const char *b) {
    return strcasecmp(a, b) == 0;
}

/* ================================
   BOOK A TICKET
   ================================ */
void bookTicket() {
    if (bookingCount >= MAX_BOOKINGS) {
        printf("System storage full. No more bookings allowed.\n");
        return;
    }

    char buf[64];
    int numTickets = 1;

    printf("\n--- Book Ticket ---\n");

    /* Common inputs for all tickets */
    printf("Number of tickets to book: ");
    if (fgets(buf, sizeof(buf), stdin) == NULL) return;
    numTickets = atoi(buf);
    if (numTickets <= 0) {
        printf("Invalid number.\n");
        return;
    }
    if (bookingCount + numTickets > MAX_BOOKINGS) {
        printf("Not enough storage to add %d tickets.\n", numTickets);
        return;
    }

    Ticket temp;
    /* Read common source/destination/mode/date */
    printf("Source: ");
    fgets(temp.source, sizeof(temp.source), stdin);
    temp.source[strcspn(temp.source, "\n")] = '\0';

    printf("Destination: ");
    fgets(temp.destination, sizeof(temp.destination), stdin);
    temp.destination[strcspn(temp.destination, "\n")] = '\0';

    printf("Mode (Bus / Train / Flight): ");
    fgets(temp.mode, sizeof(temp.mode), stdin);
    temp.mode[strcspn(temp.mode, "\n")] = '\0';

    printf("Date (YYYY-MM-DD): ");
    fgets(temp.date, sizeof(temp.date), stdin);
    temp.date[strcspn(temp.date, "\n")] = '\0';

    /* Compute fare for one ticket once */
    float singleFare = calculateFare(temp.mode, temp.source, temp.destination);

    for (int i = 0; i < numTickets; i++) {
        Ticket t = temp; /* copy common fields */
        printf("Passenger %d Name: ", i + 1);
        fgets(t.passengerName, sizeof(t.passengerName), stdin);
        t.passengerName[strcspn(t.passengerName, "\n")] = '\0';

        /* Seat allocation per ticket */
        t.seatNumber = getAvailableSeat(t.mode);
        if (t.seatNumber == -1) {
            printf("No seats available for this mode (stopped after %d of %d).\n", i, numTickets);
            break;
        }

        t.fare = singleFare;
        t.ticketID = generateTicketID();

        bookings[bookingCount++] = t;
        printf("Ticket created: ID %d, Seat %d, Fare %.2f, Name: %s\n",
               t.ticketID, t.seatNumber, t.fare, t.passengerName);
    }

    /* Save all changes */
    saveDataToFile();

    printf("\nBooked up to %d tickets. Total charged: %.2f\n",
           numTickets, singleFare * (float)numTickets);
}

/* ================================
   CANCEL A TICKET
   ================================ */
void cancelTicket() {
    char buf[32];
    int id, found = -1;

    printf("\n--- Cancel Ticket ---\n");
    printf("Enter Ticket ID: ");
    fgets(buf, sizeof(buf), stdin);
    id = atoi(buf);

    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].ticketID == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Ticket not found.\n");
        return;
    }

    /* Shift items left */
    for (int i = found; i < bookingCount - 1; i++) {
        bookings[i] = bookings[i + 1];
    }
    bookingCount--;

    saveDataToFile();

    printf("Ticket %d cancelled successfully.\n", id);
}

/* ================================
   DISPLAY ALL BOOKINGS
   ================================ */
void displayBookings() {
    printf("\n--- All Bookings ---\n");

    if (bookingCount == 0) {
        printf("No bookings found.\n");
        return;
    }

    for (int i = 0; i < bookingCount; i++) {
        Ticket *t = &bookings[i];
        printf("\nTicket ID: %d\n", t->ticketID);
        printf("Name: %s\n", t->passengerName);
        printf("From: %s   To: %s\n", t->source, t->destination);
        printf("Mode: %s\n", t->mode);
        printf("Seat: %d\n", t->seatNumber);
        printf("Fare: %.2f\n", t->fare);
    }
}

/* ================================
   SHOW SEAT AVAILABILITY
   ================================ */
void showSeatAvailability(char mode[]) {
    printf("\n--- Seat Availability for %s ---\n", mode);

    int taken[MAX_SEATS] = {0};

    /* Mark seats already booked */
    for (int i = 0; i < bookingCount; i++) {
        if (sameMode(bookings[i].mode, mode)) {
            if (bookings[i].seatNumber > 0 && bookings[i].seatNumber <= MAX_SEATS)
                taken[bookings[i].seatNumber - 1] = 1;
        }
    }

    printf("Available seats: ");
    int any = 0;
    for (int i = 0; i < MAX_SEATS; i++) {
        if (!taken[i]) {
            printf("%d ", i + 1);
            any = 1;
        }
    }

    if (!any) {
        printf("None");
    }

    printf("\n");
}
