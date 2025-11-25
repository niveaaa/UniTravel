/* utils.h
 * Utility helpers for UniTravel.
 *
 * Declares seat allocation and fare calculation helpers.
 */

#ifndef UTILS_H
#define UTILS_H

/* Returns the next available seat number for a given mode.
   Returns -1 if no seats left. */
int getAvailableSeat(const char mode[], const char date[]);

/* Calculates the fare based on mode + source + destination */
float calculateFare(const char mode[], const char source[], const char destination[]);

#endif
