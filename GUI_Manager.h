#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <SFML/Graphics.hpp>
#include "Zone.h"

class ParkingGUI {
public:
    void run(Zone& northZone) {
        // Create a window
        sf::RenderWindow window(sf::VideoMode(800, 600), "Smart Parking Visualizer");

        // Load a font for labels (You'll need a .ttf file in your folder)
        sf::Font font;
        bool hasFont = font.loadFromFile("arial.ttf"); 

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color(30, 30, 30)); // Background

            // DRAWING LOGIC: Loop through Areas and Slots
            float xOffset = 50.0f;
            float yOffset = 50.0f;

            AreaNode* tempArea = northZone.getAreaHead(); // You'll need a getter in Zone.h for this
            while (tempArea) {
                SlotNode* tempSlot = tempArea->area->getHead();
                
                while (tempSlot) {
                    sf::RectangleShape slotRect(sf::Vector2f(120.0f, 70.0f));
                    slotRect.setPosition(xOffset, yOffset);
                    slotRect.setOutlineThickness(3);
                    slotRect.setOutlineColor(sf::Color::White);

                    // Color logic based on DSA status
                    if (tempSlot->slot->getStatus() == OCCUPIED) {
                        slotRect.setFillColor(sf::Color(255, 80, 80)); // Red
                    } else {
                        slotRect.setFillColor(sf::Color(80, 255, 80)); // Green
                    }

                    window.draw(slotRect);
                    
                    xOffset += 150.0f; // Move to next slot position
                    if (xOffset > 700) { // Wrap to next line
                        xOffset = 50.0f;
                        yOffset += 100.0f;
                    }
                    tempSlot = tempSlot->next;
                }
                tempArea = tempArea->next;
            }

            window.display();
        }
    }
};

#endif