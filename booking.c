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

    char buf[128];
    int numTickets = 1;

    printf("\n--- Book Ticket ---\n");

    /* Number of tickets */
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

    Ticket common;
    /* Common inputs for all tickets */
    printf("Source: ");
    fgets(common.source, sizeof(common.source), stdin);
    common.source[strcspn(common.source, "\n")] = '\0';

    printf("Destination: ");
    fgets(common.destination, sizeof(common.destination), stdin);
    common.destination[strcspn(common.destination, "\n")] = '\0';

    printf("Mode (Bus / Train / Flight): ");
    fgets(common.mode, sizeof(common.mode), stdin);
    common.mode[strcspn(common.mode, "\n")] = '\0';

    printf("Date (YYYY-MM-DD): ");
    fgets(common.date, sizeof(common.date), stdin);
    common.date[strcspn(common.date, "\n")] = '\0';

    /* Compute fare for one ticket once */
    float singleFare = calculateFare(common.mode, common.source, common.destination);

    int created = 0;
    for (int i = 0; i < numTickets; i++) {
        Ticket t = common; /* copy common fields */

        printf("Passenger %d Name: ", i + 1);
        if (fgets(t.passengerName, sizeof(t.passengerName), stdin) == NULL) {
            printf("Input error.\n");
            break;
        }
        t.passengerName[strcspn(t.passengerName, "\n")] = '\0';

        /* Seat allocation per ticket (date-aware) */
        t.seatNumber = getAvailableSeat(t.mode, t.date);
        if (t.seatNumber == -1) {
            printf("No seats available for %s on %s (stopped after %d of %d).\n",
                   t.mode, t.date, created, numTickets);
            break;
        }

        t.fare = singleFare;
        t.ticketID = generateTicketID();

        bookings[bookingCount++] = t;
        created++;

        printf("Ticket created: ID %d, Seat %d, Fare %.2f, Name: %s, Date: %s\n",
               t.ticketID, t.seatNumber, t.fare, t.passengerName, t.date);
    }

    if (created > 0) {
        saveDataToFile();
    }

    printf("\nBooked %d tickets. Total charged: %.2f\n",
           created, singleFare * (float)created);
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
        printf("Mode: %s   Date: %s\n", t->mode, t->date[0] ? t->date : "N/A");
        printf("Seat: %d\n", t->seatNumber);
        printf("Fare: %.2f\n", t->fare);
    }
}

/* ================================
   SHOW SEAT AVAILABILITY
   ================================ */
void showSeatAvailability(char mode[]) {
    char date[12];
    int taken[MAX_SEATS] = {0};

    printf("Enter date to check availability (YYYY-MM-DD): ");
    if (fgets(date, sizeof(date), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    date[strcspn(date, "\r\n")] = '\0';

    printf("\n--- Seat Availability for %s on %s ---\n", mode, date);

    /* Mark seats taken for this mode + date */
    for (int i = 0; i < bookingCount; i++) {
        if (strcasecmp(bookings[i].mode, mode) == 0 &&
            strcmp(bookings[i].date, date) == 0) {

            int s = bookings[i].seatNumber;
            if (s > 0 && s <= MAX_SEATS)
                taken[s - 1] = 1;
        }
    }

    int totalSeats = MAX_SEATS;
    int booked = 0;

    for (int i = 0; i < MAX_SEATS; i++) {
        if (taken[i]) booked++;
    }

    int available = totalSeats - booked;

    printf("Total seats: %d\n", totalSeats);
    printf("Booked seats: %d\n", booked);
    printf("Available seats: %d\n", available);
}

