#include <iostream>
#include "Zone.h"
#include "Vehicle.h"

int main() {
    // 1. Create a Zone
    Zone northZone("North Sector");

    // 2. Add Areas to that Zone
    northZone.addArea("Block-A");
    northZone.addArea("Block-B");

    // 3. IMPORTANT: Get a reference to the areas and add slots 
    // (We will add a helper in Zone.h next to make this easier, 
    // but for now, let's verify the display works with slots)
    
    northZone.displayZoneSummary();

    std::cout << "\nSuccess! The hierarchy is working." << std::endl;
    return 0;
}