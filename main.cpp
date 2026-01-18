#include <windows.h>
#include <iostream>
#include <string>
#include "Zone.h"
#include "Vehicle.h"

// MOVE THE LOGO OUTSIDE OF MAIN
void displayLogo() {
    std::cout << "\033[33m"; // Switch to Yellow
    std::cout << "  ____                  _     ____               _    " << std::endl;
    std::cout << " / ___| _ __ ___   __ _| _|_ |  _ \\  __ _  _ __ | | __" << std::endl;
    std::cout << " \\___ \\| '_ ` _ \\ / _` |/ _` || |_) |/ _` || '__|| |/ /" << std::endl;
    std::cout << "  ___) || | | | | | (_| || (_| ||  __/| (_| || |   |   < " << std::endl;
    std::cout << " |____/|_| |_| |_|\\__,_|\\__,_||_|    \\__,_||_|   |_|\\_\\" << std::endl;
    std::cout << "                                                       " << std::endl;
    std::cout << "           SMART PARKING MANAGEMENT SYSTEM             " << std::endl;
    std::cout << "\033[0m"; // Reset to default color
    std::cout << "-------------------------------------------------------" << std::endl;
}

int main() {
    // This enables ANSI colors in the Windows terminal
    system(""); 
    
    // Call the logo function
    displayLogo();

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
            
            Vehicle* newVehicle = new Vehicle(plate, vType);
            northZone.parkVehicle(newVehicle);
        }
        else if (choice == 3) {
            std::cout << "Enter Plate to Unpark: ";
            std::cin >> plate;
            
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