#include <iostream>
#include <string>
#include "Zone.h"
#include "Vehicle.h"

int main() {
    // Initialization
    Zone northZone("North Sector");
    northZone.addArea("Block-A");
    northZone.addArea("Block-B");

    // Pre-fill slots
    northZone.addSlotsToArea("Block-A", 3);
    northZone.addSlotsToArea("Block-B", 2);

    int choice;
    std::string plate;

    while (true) {
        std::cout << "\n--- Smart Parking Menu ---" << std::endl;
        std::cout << "1. Display Zone Status" << std::endl;
        std::cout << "2. Park a Vehicle" << std::endl;
        std::cout << "3. Unpark a Vehicle" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter choice: ";
        
        if (!(std::cin >> choice)) { // Input validation
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            northZone.displayZoneSummary();
        } 
        else if (choice == 2) {
            std::string vType;
            std::cout << "Enter Plate Number: ";
            std::cin >> plate;
            std::cout << "Enter Type (Car/Bike): ";
            std::cin >> vType;
            
            Vehicle* newVehicle = new Vehicle(plate, vType);
            northZone.parkVehicle(newVehicle);
        }
        else if (choice == 3) {
            std::cout << "Enter Plate to Unpark: ";
            std::cin >> plate;
            
            if (!northZone.releaseVehicle(plate)) {
                std::cout << "\n[!] ERROR: Vehicle '" << plate << "' not found in the system." << std::endl;
            }
        } 
        else if (choice == 4) {
            std::cout << "Shutting down system. Goodbye!" << std::endl;
            break;
        } 
        else {
            std::cout << "Invalid option selected." << std::endl;
        }
    }

    return 0;
}