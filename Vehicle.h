#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>
enum VehicleType { SEDAN, SUV, VAN, MOTORBIKE };
class Vehicle {
private:
    std::string licensePlate;
    VehicleType type;
public:
    Vehicle(std::string plate, VehicleType t) : licensePlate(plate), type(t) {}

    std::string getLicensePlate() const { return licensePlate; }
    VehicleType getType() const { return type; }
};
#endif