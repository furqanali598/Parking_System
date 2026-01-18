#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>

class Vehicle {
private:
    std::string plateNumber;
    std::string type;     // "Car" or "Bike"
    time_t entryTime;     // Stores the timestamp of when the vehicle parked

public:
    // Constructor
    Vehicle(std::string plate, std::string vType) {
        plateNumber = plate;
        type = vType;
        entryTime = time(0); // Captures current system time
    }

    // Getters
    std::string getPlate() const { 
        return plateNumber; 
    }

    std::string getType() const { 
        return type; 
    }

    time_t getEntryTime() const { 
        return entryTime; 
    }
};

#endif