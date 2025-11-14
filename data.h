/* data.h
 * Handles persistent storage for UniTravel.
 *
 * Declares functions for reading/writing bookings.txt
 * and generating unique ticket IDs.
 */

#ifndef DATA_H
#define DATA_H

#include "booking.h"

/* Load all bookings from file into the array */
void loadDataFromFile();

/* Write all current bookings from memory to bookings.txt */
void saveDataToFile();

/* Generate a unique ticket ID based on current data */
int generateTicketID();

#endif
