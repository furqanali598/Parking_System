#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <string>
#include "Zone.h"

// Unique IDs for buttons
#define ID_REFRESH_BUTTON 101
#define ID_EXIT_BUTTON 102

class ParkingGUI {
public:
    static Zone* currentZone; // Static pointer so the Window Procedure can see the data

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE:
            // Create a "Refresh" Button
            CreateWindowW(L"Button", L"Refresh Map", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          50, 400, 120, 40, hwnd, (HMENU)ID_REFRESH_BUTTON, NULL, NULL);
            
            // Create an "Exit" Button
            CreateWindowW(L"Button", L"Close View", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          200, 400, 120, 40, hwnd, (HMENU)ID_EXIT_BUTTON, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_REFRESH_BUTTON) {
                InvalidateRect(hwnd, NULL, TRUE); // Tells Windows to redraw the screen
            }
            else if (LOWORD(wp) == ID_EXIT_BUTTON) {
                PostQuitMessage(0);
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Set Background Color
            HBRUSH brush = CreateSolidBrush(RGB(240, 240, 240));
            FillRect(hdc, &ps.rcPaint, brush);
            DeleteObject(brush);

            // Title Text
            SetTextColor(hdc, RGB(0, 102, 204));
            TextOutA(hdc, 50, 20, "--- SMART PARKING SYSTEM LIVE VIEW ---", 38);

            // Loop through data and draw
            int yPos = 60;
            AreaNode* tempArea = currentZone->getAreaHead();
            while (tempArea) {
                TextOutA(hdc, 50, yPos, ("BLOCK: " + tempArea->area->getAreaName()).c_str(), 15);
                yPos += 25;

                SlotNode* tempSlot = tempArea->area->getHead();
                while (tempSlot) {
                    std::string slotInfo = "Slot " + std::to_string(tempSlot->slot->getSlotId()) + ": ";
                    if (tempSlot->slot->getStatus() == 1) {
                        SetTextColor(hdc, RGB(200, 0, 0)); // Red for Busy
                        slotInfo += "[ OCCUPIED ] " + tempSlot->slot->getPlate();
                    } else {
                        SetTextColor(hdc, RGB(0, 150, 0)); // Green for Free
                        slotInfo += "[ AVAILABLE ]";
                    }
                    TextOutA(hdc, 70, yPos, slotInfo.c_str(), slotInfo.length());
                    yPos += 20;
                    tempSlot = tempSlot->next;
                }
                yPos += 15;
                tempArea = tempArea->next;
            }

            EndPaint(hwnd, &ps);
        } break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(Zone& northZone) {
        currentZone = &northZone;
        WNDCLASSW wc = {0};
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"ParkingWindowClass";
        wc.lpfnWndProc = WindowProcedure;

        if (!RegisterClassW(&wc)) return;

        HWND hwnd = CreateWindowW(L"ParkingWindowClass", L"Live Parking Dashboard",
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  100, 100, 500, 550, NULL, NULL, NULL, NULL);

        MSG msg = {0};
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UnregisterClassW(L"ParkingWindowClass", GetModuleHandle(NULL));
    }
};

// Initialize static pointer
Zone* ParkingGUI::currentZone = nullptr;

#endif