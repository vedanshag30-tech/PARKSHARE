#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "models.h"

using namespace std;

// Connected WebSocket clients
vector<string> active_connections;

void connect_client(string client_name) {
    active_connections.push_back(client_name);
}

void disconnect_client(string client_name) {
    for (int i = 0; i < active_connections.size(); i++) {
        if (active_connections[i] == client_name) {
            active_connections.erase(active_connections.begin() + i);
            break;
        }
    }
}

void broadcast(string message) {
    cout << "\n--- BROADCASTING TO " << active_connections.size() << " CLIENTS ---" << endl;
    for (int i = 0; i < active_connections.size(); i++) {
        cout << "[To " << active_connections[i] << "]: " << message << endl;
    }
}

string get_utc_timestamp() {
    time_t now = time(0);
    char buf[30];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    return string(buf);
}

void root() {
    cout << "{\"status\": \"ParkShare C++ Engine Online\", \"docs_url\": \"/docs\"}" << endl;
}

int main() {
    // Initialize default system data
    root();
    connect_client("App_Client_1");
    connect_client("Admin_Dashboard");

    ParkingSpot default_spot = {"SPOT_A1", "OWNER_1", 50.0};
    parking_spots_table.push_back(default_spot);

    int choice = 0;
    while (true) {
        cout << "\n==========================================" << endl;
        cout << "   PARKSHARE C++ BACKEND ENGINE MENU" << endl;
        cout << "==========================================" << endl;
        cout << "1. List a New Parking Spot" << endl;
        cout << "2. Create a Parking Booking" << endl;
        cout << "3. Cancel a Parking Booking" << endl;
        cout << "4. View All Bookings" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            ParkingSpot spot;
            cout << "Spot ID (e.g. SPOT_B2): ";
            cin >> spot.spot_id;
            cout << "Owner ID (e.g. OWNER_2): ";
            cin >> spot.owner_id;
            cout << "Hourly Rate (Rs.): ";
            cin >> spot.hourly_rate;

            parking_spots_table.push_back(spot);
            cout << "SUCCESS: Spot " << spot.spot_id << " added!" << endl;
            broadcast("{\"event\": \"NEW_SPOT_LISTED\", \"spot_id\": \"" + spot.spot_id + "\"}");
        }
        else if (choice == 2) {
            Booking b;
            cout << "Booking ID (e.g. BK_101): ";
            cin >> b.booking_id;
            cout << "Spot ID (default SPOT_A1): ";
            cin >> b.spot_id;
            cout << "Driver ID (e.g. DRIVER_1): ";
            cin >> b.driver_id;
            cout << "Start Hour (0-23, e.g. 14): ";
            cin >> b.start_hour;
            cout << "End Hour (1-24, e.g. 16): ";
            cin >> b.end_hour;

            b.status = "CONFIRMED";
            b.total_cost = (b.end_hour - b.start_hour) * 50.0;
            b.created_at = get_utc_timestamp();

            if (save_booking(b)) {
                broadcast("{\"event\": \"BOOKING_CONFIRMED\", \"booking_id\": \"" + b.booking_id + "\", \"spot_id\": \"" + b.spot_id + "\"}");
            }
        }
        else if (choice == 3) {
            string bk_id;
            cout << "Enter Booking ID to cancel: ";
            cin >> bk_id;
            if (cancel_booking(bk_id)) {
                broadcast("{\"event\": \"BOOKING_CANCELLED\", \"booking_id\": \"" + bk_id + "\"}");
            }
        }
        else if (choice == 4) {
            cout << "\n--- CURRENT BOOKINGS (" << bookings_table.size() << ") ---" << endl;
            for (int i = 0; i < bookings_table.size(); i++) {
                Booking b = bookings_table[i];
                cout << b.booking_id << " | Spot: " << b.spot_id
                     << " | Time: " << b.start_hour << ":00 - " << b.end_hour << ":00"
                     << " | Status: " << b.status << " | Cost: Rs." << b.total_cost << endl;
            }
        }
        else if (choice == 5) {
            cout << "Shutting down ParkShare Engine..." << endl;
            break;
        }
        else {
            cout << "Invalid option. Try again." << endl;
        }
    }

    return 0;
}