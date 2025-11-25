/* booking.h
 * Header file for booking-related structures and functions
 *
 * Declares the Ticket struct and core reservation operations.
 * Shared by main.c, booking.c, data.c, and utils.c.
 */

#ifndef BOOKING_H
#define BOOKING_H

#define MAX_SEATS 50
#define MAX_BOOKINGS 200

/* Ticket structure: defines what a single booking looks like */
typedef struct {
    int ticketID;
    char passengerName[50];
    char source[30];
    char destination[30];
    char mode[10];      /* "Bus", "Train", or "Flight" */
    int seatNumber;
    float fare;
    char date[12];      /* YYYY-MM-DD */
} Ticket;

/* Global array + counter declared elsewhere (in booking.c) */
extern Ticket bookings[MAX_BOOKINGS];
extern int bookingCount;

/* Core booking operations */
void bookTicket();
void cancelTicket();
void displayBookings();
void showSeatAvailability(char mode[]);

#endif
