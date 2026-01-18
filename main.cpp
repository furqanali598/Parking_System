#include <iostream>
#include "Zone.h"
#include "Vehicle.h"

int main() {
    Zone northZone("North Sector");
    northZone.addArea("Block-A");
    northZone.addArea("Block-B");
    northZone.addSlotsToArea("Block-A", 2); // Small number for testing
    northZone.addSlotsToArea("Block-B", 2);

    int choice;
    std::string plate;

    while (true) {
        std::cout << "\n--- Smart Parking System ---" << std::endl;
        std::cout << "1. Display Zone Status" << std::endl;
        std::cout << "2. Park a Vehicle" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

       std::cout << "1. Display Zone Status\n2. Park a Vehicle\n3. Unpark a Vehicle\n4. Exit\nChoice: ";
        std::cin >> choice;

        if (choice == 1) {
            northZone.displayZoneSummary();
        } else if (choice == 2) {
            std::cout << "Enter Plate: "; std::cin >> plate;
            northZone.parkVehicle(plate);
        } else if (choice == 3) {
            std::cout << "Enter Plate to Unpark: "; std::cin >> plate;
            northZone.releaseVehicle(plate);
        } else if (choice == 4) {
            break;
        }
    }
    return 0;
}