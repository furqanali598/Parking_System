#include "GUI_Manager.h"

int main() {
    GlobalManager network;

    // 1. Mall 1 (2 floors basement parking)
    Zone* mall = new Zone("Mall 1", 10, 10);
    mall->addArea("Basement 1"); mall->addSlotsToArea("Basement 1", 8);
    mall->addArea("Basement 2"); mall->addSlotsToArea("Basement 2", 8);
    network.addBuilding(mall);

    // 2. Askari Tower (4 floors parking)
    Zone* askari = new Zone("Askari Tower", 50, 50);
    for(int i=1; i<=4; i++) {
        std::string name = "Floor " + std::to_string(i);
        askari->addArea(name); askari->addSlotsToArea(name, 8);
    }
    network.addBuilding(askari);

    // 3. Monal (6 floors parking)
    Zone* monal = new Zone("Monal", 100, 100);
    for(int i=1; i<=6; i++) {
        std::string name = "Level " + std::to_string(i);
        monal->addArea(name); monal->addSlotsToArea(name, 8);
    }
    network.addBuilding(monal);

    ParkingGUI gui;
    gui.run(network); 

    return 0;
}