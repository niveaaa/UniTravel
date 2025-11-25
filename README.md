# UniTravel – Ticket Reservation System

UniTravel is a console-based ticket reservation system written in C.  
It allows users to book, cancel, and view tickets for Bus, Train, and Flight travel, with date-wise seat availability and persistent storage using text files.

---

## Features

### 1. Mode Selection
The program provides three booking modules:
- Bus
- Train
- Flight

### 2. Multiple Ticket Booking
Users can:
- Enter the number of tickets they want to book.
- Provide multiple passenger names.
- Use a common source, destination, mode, and date for all tickets.
Each ticket is assigned a unique ticket ID and seat number.

### 3. Date-Wise Seat Availability
Seat allocation is specific to a travel date.  
Checking availability for a date shows:
- Total seats  
- Booked seats  
- Available seats  

### 4. Booking Function
The system:
- Collects passenger details
- Calculates fare based on travel mode
- Allocates seats per passenger
- Saves each booking to the data file

### 5. Cancellation Function
Users can cancel a ticket using its ticket ID.  
Cancelled tickets are removed from the system and the file data is updated.

### 6. View All Bookings
Displays:
- Ticket ID  
- Passenger Name  
- Source and Destination  
- Travel Mode  
- Date  
- Seat Number  
- Fare  

### 7. File Handling
All data is saved to `bookings.txt` using CSV-style storage.  
Bookings persist after the program is closed.

---

## Project Structure
 
 ```bash
 UniTravel/
│
├── main.c
├── booking.c
├── booking.h
├── data.c
├── data.h
├── utils.c
├── utils.h
├── bookings.txt
└── README.md
```

---

## How It Works

### Booking Flow

1. User selects booking option.
2. System asks for:
    - Number of tickets
    - Source
    - Destination
    - Mode
    - Date
3. System asks for each passenger name.
4. System allocates available seats for that date.
5. Tickets are saved to bookings.txt.

### Seat Availability

The system checks bookings for:
- Travel mode
- Travel date

It counts how many seats are booked and how many are free out of 50.

### Cancellation

The user enters the ticket ID, and the program removes that ticket from memory and file storage.

### File Format (bookings.txt)

Each line follows:
```bash
ticketID,passengerName,source,destination,mode,fare,date,seatNumber
```
Example:
```bash
1001,Nishita,Ghaziabad,Guna,Train,2500.00,2025-12-14,9
```

### Limitations

- Maximum bookings stored: 200
- Maximum seats per mode per date: 50
- Fare calculation uses fixed values per mode (can be improved)

### Future Enhancements

- Dynamic fare based on distance
- Admin login system
- Sorting and searching bookings
- Support for modifying existing tickets

---

mera SDF khatam h 🥀