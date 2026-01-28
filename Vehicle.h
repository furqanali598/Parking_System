#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle {
private:
    std::string plate;
    std::string type;

public:
    Vehicle(std::string p, std::string t) : plate(p), type(t) {}
    std::string getPlate() { return plate; }
    std::string getType() { return type; }
};

#endif