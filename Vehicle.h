#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>

class Vehicle {
private:
    std::string plateNumber;
    std::string type;
    time_t entryTime;

public:
    Vehicle(std::string plate, std::string t) : plateNumber(plate), type(t) {
        entryTime = time(0);
    }

    std::string getPlate() const { return plateNumber; }
    std::string getType() const { return type; }
    time_t getEntryTime() const { return entryTime; }
};

#endif