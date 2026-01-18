#include <iostream>
#include <string>
#include "Zone.h"
#include "Vehicle.h"

int main() {
    // 1. Setup the System
    Zone northZone("North Sector");
    northZone.addArea("Block-A");
    northZone.addArea("Block-B");

    // 2. Initialize slots in each block
    northZone.addSlotsToArea("Block-A", 3);
    northZone.addSlotsToArea("Block-B", 2);

    int choice;
    std::string plate;

    while (true) {
        std::cout << "\n====================================" << std::endl;
        std::cout << "     SMART PARKING SYSTEM MENU      " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Display All Blocks Status" << std::endl;
        std::cout << "2. Park a Vehicle (Car/Bike)" << std::endl;
        std::cout << "3. Unpark a Vehicle (Calculate Fee)" << std::endl;
        std::cout << "4. Exit System" << std::endl;
        std::cout << "------------------------------------" << std::endl;
        std::cout << "Enter Choice: ";
        // Input Validation: Prevents infinite loops if a non-integer is entered
        if (!(std::cin >> choice)) {
            std::cout << "\n[!] Invalid input. Please enter a number (1-4)." << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            northZone.displayZoneSummary();
        } 
        else if (choice == 2) {
            std::string vType;
            std::cout << "Enter License Plate: ";
            std::cin >> plate;
            std::cout << "Enter Vehicle Type (Car/Bike): ";
            std::cin >> vType;
            
            // Dynamically create a new vehicle and pass it to the zone
            Vehicle* newVehicle = new Vehicle(plate, vType);
            northZone.parkVehicle(newVehicle);
        }
        else if (choice == 3) {
            std::cout << "Enter Plate to Unpark: ";
            std::cin >> plate;
            
            // Check if unparking was successful
            if (!northZone.releaseVehicle(plate)) {
                std::cout << "\n[!] ERROR: Vehicle '" << plate << "' not found in any block." << std::endl;
            }
        } 
        else if (choice == 4) {
            std::cout << "Exiting system. Total revenue has been logged. Goodbye!" << std::endl;
            break;
        } 
        else {
            std::cout << "[!] Invalid option. Please select 1-4." << std::endl;
        }
    }
    return 0;
}