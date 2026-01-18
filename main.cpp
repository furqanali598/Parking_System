#include <windows.h>
#include "Zone.h"
#include "GUI_Manager.h"

int main() {
    // Setup Data
    Zone northZone("Main Terminal");
    northZone.addArea("Block-A");
    northZone.addArea("Block-B");
    northZone.addSlotsToArea("Block-A", 5);
    northZone.addSlotsToArea("Block-B", 5);

    // Launch GUI Directly
    ParkingGUI gui;
    gui.run(northZone);

    return 0;
}