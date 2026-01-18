#include <windows.h>
#include <iostream>
#include <string>
#include "Zone.h"
#include "Vehicle.h"
#include "GUI_Manager.h" // <--- Make sure this is here!

void displayLogo() {
    std::cout << "\033[33m"; 
    std::cout << "  ____                  _     ____               _    " << std::endl;
    std::cout << " / ___| _ __ ___   __ _| _|_ |  _ \\  __ _  _ __ | | __" << std::endl;
    std::cout << " \\___ \\| '_ ` _ \\ / _` |/ _` || |_) |/ _` || '__|| |/ /" << std::endl;
    std::cout << "  ___) || | | | | | (_| || (_| ||  __/| (_| || |   |   < " << std::endl;
    std::cout << " |____/|_| |_| |_|\\__,_|\\__,_||_|    \\__,_||_|   |_|\\_\\" << std::endl;
    std::cout << "                                                       " << std::endl;
    std::cout << "           SMART PARKING MANAGEMENT SYSTEM             " << std::endl;
    std::cout << "\033[0m-------------------------------------------------------" << std::endl;
}

int main() {
    system(""); 
    displayLogo();

    Zone northZone("North Sector");
    northZone.addArea("Block-A");
    northZone.addArea("Block-B");
    northZone.addSlotsToArea("Block-A", 3);
    northZone.addSlotsToArea("Block-B", 2);

    int choice;
    std::string plate;

    while (true) {
        std::cout << "\n1. Display Status (Terminal)" << std::endl;
        std::cout << "2. Park a Vehicle" << std::endl;
        std::cout << "3. Unpark a Vehicle" << std::endl;
        std::cout << "4. Open Live Map (GUI)" << std::endl; // <--- NEW OPTION
        std::cout << "5. Exit" << std::endl;
        std::cout << "Enter Choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            northZone.displayZoneSummary();
        } 
        else if (choice == 2) {
            std::string vType;
            std::cout << "Plate: "; std::cin >> plate;
            std::cout << "Type (Car/Bike): "; std::cin >> vType;
            northZone.parkVehicle(new Vehicle(plate, vType));
        }
        else if (choice == 3) {
            std::cout << "Plate to Unpark: "; std::cin >> plate;
            if (!northZone.releaseVehicle(plate)) 
                std::cout << "Vehicle not found!" << std::endl;
        } 
        else if (choice == 4) { // <--- GUI TRIGGER
            ParkingGUI gui;
            gui.run(northZone);
        }
        else if (choice == 5) {
            break;
        }
    }
    return 0;
}