#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <commctrl.h> 
#include <string>
#include "Zone.h"

#define ID_BTN_PARK 201
#define ID_BTN_UNPARK 202
#define ID_BTN_SEARCH 205 // New ID for Search
#define ID_INPUT_PLATE 203
#define ID_COMBO_TYPE 204

class ParkingGUI {
public:
    static Zone* currentZone;
    static HWND hPlateInput, hTypeCombo;
    static HBRUSH hSkyBlueBrush, hStatBrush, hHighlightBrush;
    static std::string searchQuery; // Stores the plate we are looking for

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
            hHighlightBrush = CreateSolidBrush(RGB(255, 255, 0)); // Yellow Highlight

            // Controls
            CreateWindowW(L"Static", L"Plate Number:", WS_VISIBLE | WS_CHILD, 40, 460, 110, 20, hwnd, NULL, NULL, NULL);
            hPlateInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 458, 140, 25, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);

            CreateWindowW(L"Static", L"Vehicle Type:", WS_VISIBLE | WS_CHILD, 40, 495, 110, 20, hwnd, NULL, NULL, NULL);
            hTypeCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 150, 493, 140, 200, hwnd, (HMENU)ID_COMBO_TYPE, NULL, NULL);
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Car");
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Bike");
            SendMessageW(hTypeCombo, CB_SETCURSEL, 0, 0);

            // Action Buttons
            CreateWindowW(L"Button", L"PARK", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 310, 455, 70, 65, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            CreateWindowW(L"Button", L"RELEASE", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 390, 455, 75, 65, hwnd, (HMENU)ID_BTN_UNPARK, NULL, NULL);
            CreateWindowW(L"Button", L"FIND", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 475, 455, 70, 65, hwnd, (HMENU)ID_BTN_SEARCH, NULL, NULL);

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
            FrameRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(GRAY_BRUSH));
            return TRUE;
        } break;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_PARK) {
                char pl[20], ty[20];
                GetWindowTextA(hPlateInput, pl, 20);
                int s = SendMessage(hTypeCombo, CB_GETCURSEL, 0, 0);
                SendMessageA(hTypeCombo, CB_GETLBTEXT, s, (LPARAM)ty);
                if (strlen(pl) > 0) { 
                    currentZone->parkVehicle(new Vehicle(pl, ty)); 
                    searchQuery = ""; // Clear search when parking
                    InvalidateRect(hwnd, NULL, TRUE); 
                }
            } else if (LOWORD(wp) == ID_BTN_UNPARK) {
                char pl[20]; GetWindowTextA(hPlateInput, pl, 20);
                if (currentZone->releaseVehicle(pl)) {
                    searchQuery = "";
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            } else if (LOWORD(wp) == ID_BTN_SEARCH) {
                char pl[20]; GetWindowTextA(hPlateInput, pl, 20);
                searchQuery = pl; // Set the highlight query
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT fullRect; GetClientRect(hwnd, &fullRect);
            FillRect(hdc, &fullRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

            // Stats Logic
            int occupied = 0, total = 0;
            AreaNode* tempA = currentZone->getAreaHead();
            while(tempA) {
                SlotNode* tempS = tempA->area->getHead();
                while(tempS) { total++; if(tempS->slot->getStatus() == 1) occupied++; tempS = tempS->next; }
                tempA = tempA->next;
            }

            // Stats Header
            RECT statBox = { 30, 20, 540, 70 };
            FillRect(hdc, &statBox, hStatBrush);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(20, 60, 120));
            std::string stats = "Total Slots: " + std::to_string(total) + "  |  Occupied: " + std::to_string(occupied);
            TextOutA(hdc, 50, 35, stats.c_str(), stats.length());

            int y = 100;
            AreaNode* areaNode = currentZone->getAreaHead();
            while (areaNode) {
                SetTextColor(hdc, RGB(120, 120, 120));
                std::string fName = "FLOOR: " + areaNode->area->getAreaName();
                TextOutA(hdc, 40, y, fName.c_str(), fName.length());
                y += 35;

                SlotNode* slotNode = areaNode->area->getHead();
                int x = 40;
                while (slotNode) {
                    bool busy = (slotNode->slot->getStatus() == 1);
                    HBRUSH hBr;
                    
                    // INNOVATION: Highlight logic
                    if (busy && searchQuery != "" && slotNode->slot->getPlate() == searchQuery) {
                        hBr = hHighlightBrush; // Glowing Yellow
                    } else {
                        hBr = busy ? CreateSolidBrush(RGB(255, 107, 107)) : CreateSolidBrush(RGB(123, 237, 159));
                    }
                    
                    RECT r = { x, y, x + 85, y + 65 };
                    FillRect(hdc, &r, hBr);
                    FrameRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

                    SetTextColor(hdc, (hBr == hHighlightBrush) ? RGB(0,0,0) : RGB(255, 255, 255));
                    TextOutA(hdc, x + 5, y + 10, ("Slot " + std::to_string(slotNode->slot->getSlotId())).c_str(), 6);
                    if(busy) TextOutA(hdc, x + 5, y + 35, slotNode->slot->getPlate().c_str(), slotNode->slot->getPlate().length());

                    if (hBr != hHighlightBrush && hBr != hHighlightBrush) DeleteObject(hBr);
                    x += 95;
                    slotNode = slotNode->next;
                }
                y += 110;
                areaNode = areaNode->next;
            }
            EndPaint(hwnd, &ps);
        } break;

        case WM_CTLCOLORSTATIC: return (LRESULT)GetStockObject(WHITE_BRUSH);
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(Zone& z) {
        currentZone = &z;
        WNDCLASSW wc = {0};
        wc.lpszClassName = L"InnoGUI_Search";
        wc.lpfnWndProc = WindowProcedure;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);
        CreateWindowW(L"InnoGUI_Search", L"Smart Parking Pro + Search", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 580, 650, NULL, NULL, GetModuleHandle(NULL), NULL);
        MSG m; while (GetMessage(&m, NULL, 0, 0)) { TranslateMessage(&m); DispatchMessage(&m); }
    }
};

Zone* ParkingGUI::currentZone = nullptr;
HWND ParkingGUI::hPlateInput = nullptr, ParkingGUI::hTypeCombo = nullptr;
HBRUSH ParkingGUI::hSkyBlueBrush = nullptr, ParkingGUI::hStatBrush = nullptr, ParkingGUI::hHighlightBrush = nullptr;
std::string ParkingGUI::searchQuery = "";

#endif