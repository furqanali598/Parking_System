#include <iostream>
#include <string>
#include "Vehicle.h"
#include "Zone.h"

int main() {
    Zone northZone("North Sector");
    northZone.addArea("Block-A");
    northZone.addArea("Block-B");
    
    // Adding initial slots
    northZone.addSlotsToArea("Block-A", 5);
    northZone.addSlotsToArea("Block-B", 5);

    int choice;
    std::string plate; // <--- Make sure this is here!

    while (true) {
        std::cout << "\n--- Smart Parking System ---" << std::endl;
        std::cout << "1. Display Zone Status" << std::endl;
        std::cout << "2. Park a Vehicle" << std::endl;
        std::cout << "3. Unpark a Vehicle" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            northZone.displayZoneSummary();
        } 
        // --- ADD YOUR CODE HERE ---
        else if (choice == 2) {
            std::string vType;
            std::cout << "Enter License Plate: ";
            std::cin >> plate;
            std::cout << "Enter Type (Car/Bike): ";
            std::cin >> vType;
            
            // Create a new vehicle object and pass it to the zone
            Vehicle* newVehicle = new Vehicle(plate, vType);
            northZone.parkVehicle(newVehicle);
        }
        // --------------------------
        else if (choice == 3) {
            std::cout << "Enter Plate to Unpark: ";
            std::cin >> plate;
            northZone.releaseVehicle(plate);
        } else if (choice == 4) {
            break;
        }
    }
    return 0;
}