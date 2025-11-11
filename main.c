/* main.c
 * UniTravel - main menu / program control
 *
 * Responsibilities:
 *  - Load data at startup
 *  - Present main menu and accept user choices
 *  - Dispatch to booking/cancellation/display functions
 *  - Save data before exiting
 *
 * Keeps I/O simple and robust for console use.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "booking.h"   /* declares bookTicket, cancelTicket, displayBookings, showSeatAvailability */
#include "data.h"      /* declares loadDataFromFile, saveDataToFile */

#define INPUT_BUF_SIZE 32

static void flush_stdin(void) {
    /* simple stdin flush for safety when mixing scanf/fgets, won't break anything */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

int main(void) {
    char input[INPUT_BUF_SIZE];
    int choice = 0;

    /* Initialize/load persisted bookings */
    loadDataFromFile();

    while (1) {
        printf("\n======================================\n");
        printf("         UniTravel Reservation\n");
        printf("======================================\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. View All Bookings\n");
        printf("4. Show Seat Availability (Bus/Train/Flight)\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        /* use fgets to avoid scanf pitfalls */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nInput error — exiting.\n");
            break;
        }

        /* strip newline */
        input[strcspn(input, "\r\n")] = '\0';
        choice = atoi(input);

        switch (choice) {
            case 1:
                bookTicket();
                break;

            case 2:
                cancelTicket();
                break;

            case 3:
                displayBookings();
                break;

            case 4: {
                char mode[16];
                printf("Enter mode (Bus / Train / Flight): ");
                if (fgets(mode, sizeof(mode), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                mode[strcspn(mode, "\r\n")] = '\0';
                showSeatAvailability(mode);
                break;
            }

            case 5:
                printf("Saving data and exiting... Good choice.\n");
                saveDataToFile();
                return 0;

            default:
                printf("Invalid choice. Please enter a number 1-5.\n");
        }
    }

    /* fallback save */
    saveDataToFile();
    return 0;
}
