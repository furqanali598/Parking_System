#include "Vehicle.h"
#include "ParkingSlot.h"
#include "ParkingArea.h"
#include "Zone.h"          // 1. Define the data structures first
#include "GUI_Manager.h"

int main() {
    Zone northZone("Terminal 1");

    // Adding Floors
    northZone.addArea("1st Floor");
    northZone.addArea("2nd Floor");

    // Adding Slots to Floors (6 slots per floor looks best in Blueprint)
    northZone.addSlotsToArea("1st Floor", 6);
    northZone.addSlotsToArea("2nd Floor", 6);

    ParkingGUI gui;
    gui.run(northZone);

    return 0;
}