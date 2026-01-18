#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <commctrl.h> 
#include <string>
#include <vector>
#include "Zone.h"

#define ID_BTN_PARK 201
#define ID_BTN_UNPARK 202
#define ID_INPUT_PLATE 203
#define ID_COMBO_TYPE 204

class ParkingGUI {
public:
    static Zone* currentZone;
    static HWND hPlateInput, hTypeCombo;
    static HBRUSH hSkyBlueBrush, hStatBrush;

    static BOOL CALLBACK SetFont(HWND child, LPARAM font) {
        SendMessage(child, WM_SETFONT, (WPARAM)font, TRUE);
        return TRUE;
    }

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE: {
            HFONT hUIFont = CreateFontA(18, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, "Segoe UI");
            hSkyBlueBrush = CreateSolidBrush(RGB(135, 206, 235));
            hStatBrush = CreateSolidBrush(RGB(245, 245, 245));

            // Controls
            CreateWindowW(L"Static", L"Vehicle Plate:", WS_VISIBLE | WS_CHILD, 40, 460, 110, 20, hwnd, NULL, NULL, NULL);
            hPlateInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 458, 140, 25, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);

            CreateWindowW(L"Static", L"Select Type:", WS_VISIBLE | WS_CHILD, 40, 495, 110, 20, hwnd, NULL, NULL, NULL);
            hTypeCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 150, 493, 140, 200, hwnd, (HMENU)ID_COMBO_TYPE, NULL, NULL);
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Car");
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Bike");
            SendMessageW(hTypeCombo, CB_SETCURSEL, 0, 0);

            CreateWindowW(L"Button", L"PARK NOW", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 310, 455, 110, 65, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            CreateWindowW(L"Button", L"RELEASE", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 430, 455, 110, 65, hwnd, (HMENU)ID_BTN_UNPARK, NULL, NULL);

            EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)hUIFont);
        } break;

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lp;
            FillRect(pdis->hDC, &pdis->rcItem, hSkyBlueBrush);
            SetBkMode(pdis->hDC, TRANSPARENT);
            SetTextColor(pdis->hDC, RGB(0, 50, 100));
            wchar_t buf[20];
            GetWindowTextW(pdis->hwndItem, buf, 20);
            DrawTextW(pdis->hDC, buf, -1, &pdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            FrameRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));
            return TRUE;
        } break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Clean White Background
            RECT fullRect; GetClientRect(hwnd, &fullRect);
            FillRect(hdc, &fullRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

            // Stats Header Box
            RECT statBox = { 30, 20, 530, 80 };
            FillRect(hdc, &statBox, hStatBrush);
            FrameRect(hdc, &statBox, (HBRUSH)GetStockObject(GRAY_BRUSH));

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(20, 60, 120));
            
            // Logic for Stats
            int occupied = 0, total = 0;
            AreaNode* countArea = currentZone->getAreaHead();
            while(countArea) {
                SlotNode* countSlot = countArea->area->getHead();
                while(countSlot) {
                    total++;
                    if(countSlot->slot->getStatus() == 1) occupied++;
                    countSlot = countSlot->next;
                }
                countArea = countArea->next;
            }

            TextOutA(hdc, 50, 40, ("Live Occupancy: " + std::to_string(occupied) + " / " + std::to_string(total)).c_str(), 22);

            // Draw Floors
            int y = 110;
            AreaNode* areaNode = currentZone->getAreaHead();
            while (areaNode) {
                SetTextColor(hdc, RGB(100, 100, 100));
                std::string fLabel = "FLOOR: " + areaNode->area->getAreaName();
                TextOutA(hdc, 40, y, fLabel.c_str(), fLabel.length());
                y += 30;

                SlotNode* slotNode = areaNode->area->getHead();
                int x = 40;
                while (slotNode) {
                    bool busy = (slotNode->slot->getStatus() == 1);
                    HBRUSH hBr = busy ? CreateSolidBrush(RGB(255, 107, 107)) : CreateSolidBrush(RGB(123, 237, 159));
                    
                    // Main Slot Box
                    RECT r = { x, y, x + 85, y + 60 };
                    FillRect(hdc, &r, hBr);
                    FrameRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

                    SetTextColor(hdc, RGB(255, 255, 255));
                    TextOutA(hdc, x + 5, y + 5, ("Slot " + std::to_string(slotNode->slot->getSlotId())).c_str(), 6);
                    
                    if(busy) {
                        TextOutA(hdc, x + 5, y + 30, slotNode->slot->getPlate().c_str(), slotNode->slot->getPlate().length());
                        // Draw a small "Car" indicator
                        RECT carIndicator = { x + 60, y + 45, x + 80, y + 55 };
                        FillRect(hdc, &carIndicator, (HBRUSH)GetStockObject(WHITE_BRUSH));
                    }

                    DeleteObject(hBr);
                    x += 95;
                    slotNode = slotNode->next;
                }
                y += 100;
                areaNode = areaNode->next;
            }
            EndPaint(hwnd, &ps);
        } break;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_PARK) {
                char pl[20], ty[20];
                GetWindowTextA(hPlateInput, pl, 20);
                int s = SendMessage(hTypeCombo, CB_GETCURSEL, 0, 0);
                SendMessageA(hTypeCombo, CB_GETLBTEXT, s, (LPARAM)ty);
                if (strlen(pl) > 0) { currentZone->parkVehicle(new Vehicle(pl, ty)); InvalidateRect(hwnd, NULL, TRUE); }
            } else if (LOWORD(wp) == ID_BTN_UNPARK) {
                char pl[20]; GetWindowTextA(hPlateInput, pl, 20);
                if (currentZone->releaseVehicle(pl)) InvalidateRect(hwnd, NULL, TRUE);
            }
            break;

        case WM_CTLCOLORSTATIC: return (LRESULT)GetStockObject(WHITE_BRUSH);
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(Zone& z) {
        currentZone = &z;
        WNDCLASSW wc = {0};
        wc.lpszClassName = L"InnoGUI";
        wc.lpfnWndProc = WindowProcedure;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);
        CreateWindowW(L"InnoGUI", L"Smart Parking Pro", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 580, 620, NULL, NULL, GetModuleHandle(NULL), NULL);
        MSG m; while (GetMessage(&m, NULL, 0, 0)) { TranslateMessage(&m); DispatchMessage(&m); }
    }
};

Zone* ParkingGUI::currentZone = nullptr;
HWND ParkingGUI::hPlateInput = nullptr, ParkingGUI::hTypeCombo = nullptr;
HBRUSH ParkingGUI::hSkyBlueBrush = nullptr, ParkingGUI::hStatBrush = nullptr;

#endif