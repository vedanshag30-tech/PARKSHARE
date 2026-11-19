#ifndef MODELS_H
#define MODELS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ParkingSpot model
struct ParkingSpot {
    string spot_id;
    string owner_id;
    double hourly_rate;
};

// Booking model
struct Booking {
    string booking_id;
    string spot_id;
    string driver_id;
    int start_hour;      // 24-hour integer window (e.g., 14 for 2:00 PM)
    int end_hour;
    string status;       // "CONFIRMED", "ACTIVE", "COMPLETED", "CANCELLED"
    double total_cost;
    string created_at;
};

// In-memory database tables
vector<ParkingSpot> parking_spots_table;
vector<Booking> bookings_table;

// Enforces status validation and zero-double-booking guarantee
bool save_booking(Booking new_booking) {
    // 1. Validate status
    if (new_booking.status != "CONFIRMED" &&
        new_booking.status != "ACTIVE" &&
        new_booking.status != "COMPLETED" &&
        new_booking.status != "CANCELLED") {
        cout << "ERROR: Invalid booking status!" << endl;
        return false;
    }

    // 2. Prevent double-booking on active/confirmed reservations
    for (int i = 0; i < bookings_table.size(); i++) {
        Booking existing = bookings_table[i];

        if (existing.spot_id == new_booking.spot_id && existing.status != "CANCELLED") {
            bool overlaps = (new_booking.start_hour < existing.end_hour) &&
                            (new_booking.end_hour > existing.start_hour);
            if (overlaps) {
                cout << "ERROR: Double booking prevented for spot " << new_booking.spot_id << "!" << endl;
                return false;
            }
        }
    }

    // 3. Save valid booking
    bookings_table.push_back(new_booking);
    cout << "SUCCESS: Booking " << new_booking.booking_id << " confirmed!" << endl;
    return true;
}

// Cancel an existing booking so the slot opens up again
bool cancel_booking(string booking_id) {
    for (int i = 0; i < bookings_table.size(); i++) {
        if (bookings_table[i].booking_id == booking_id) {
            bookings_table[i].status = "CANCELLED";
            cout << "SUCCESS: Booking " << booking_id << " cancelled. Slot is now free." << endl;
            return true;
        }
    }
    cout << "ERROR: Booking ID not found." << endl;
    return false;
}

#endif