#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <commctrl.h> // Required for Dropdown
#include <string>
#include "Zone.h"

#define ID_BTN_PARK 201
#define ID_BTN_UNPARK 202
#define ID_INPUT_PLATE 203
#define ID_COMBO_TYPE 204

class ParkingGUI {
public:
    static Zone* currentZone;
    static HWND hPlateInput, hTypeCombo;
    static HBRUSH hSkyBlueBrush;

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE: {
            HFONT hFont = CreateFontA(18, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            hSkyBlueBrush = CreateSolidBrush(RGB(135, 206, 235)); // Sky Blue

            // Inputs
            CreateWindowW(L"Static", L"Plate Number:", WS_VISIBLE | WS_CHILD, 50, 420, 120, 25, hwnd, NULL, NULL, NULL);
            hPlateInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 420, 150, 25, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);

            CreateWindowW(L"Static", L"Vehicle Type:", WS_VISIBLE | WS_CHILD, 50, 455, 120, 25, hwnd, NULL, NULL, NULL);
            // Dropdown (Combobox)
            hTypeCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 180, 455, 150, 200, hwnd, (HMENU)ID_COMBO_TYPE, NULL, NULL);
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Car");
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Bike");
            SendMessageW(hTypeCombo, CB_SETCURSEL, 0, 0);

            // Sky Blue Buttons
            CreateWindowW(L"Button", L"PARK VEHICLE", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 350, 418, 130, 30, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            CreateWindowW(L"Button", L"UNPARK", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 350, 453, 130, 30, hwnd, (HMENU)ID_BTN_UNPARK, NULL, NULL);

            EnumChildWindows(hwnd, [](HWND child, LPARAM font) -> BOOL {
                SendMessage(child, WM_SETFONT, font, TRUE);
                return TRUE;
            }, (LPARAM)hFont);
        } break;

        case WM_DRAWITEM: { // Handles coloring the buttons sky blue
            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lp;
            if (pdis->CtlID == ID_BTN_PARK || pdis->CtlID == ID_BTN_UNPARK) {
                FillRect(pdis->hDC, &pdis->rcItem, hSkyBlueBrush);
                SetBkMode(pdis->hDC, TRANSPARENT);
                
                wchar_t text[20];
                GetWindowTextW(pdis->hwndItem, text, 20);
                DrawTextW(pdis->hDC, text, -1, &pdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                return TRUE;
            }
        } break;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_PARK) {
                char plate[20], type[20];
                GetWindowTextA(hPlateInput, plate, 20);
                int sel = SendMessage(hTypeCombo, CB_GETCURSEL, 0, 0);
                SendMessageA(hTypeCombo, CB_GETLBTEXT, sel, (LPARAM)type);
                
                if (strlen(plate) > 0) {
                    currentZone->parkVehicle(new Vehicle(plate, type));
                    SetWindowTextA(hPlateInput, "");
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            } else if (LOWORD(wp) == ID_BTN_UNPARK) {
                char plate[20];
                GetWindowTextA(hPlateInput, plate, 20);
                // Note: Ensure your Zone::releaseVehicle prints or returns the fee
                if (currentZone->releaseVehicle(plate)) {
                    InvalidateRect(hwnd, NULL, TRUE);
                } else {
                    MessageBoxA(hwnd, "Plate not found!", "Error", MB_ICONERROR);
                }
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH hBg = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &ps.rcPaint, hBg);
            DeleteObject(hBg);

            SetBkMode(hdc, TRANSPARENT);
            HFONT hTitleFont = CreateFontA(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            SelectObject(hdc, hTitleFont);
            SetTextColor(hdc, RGB(20, 60, 120));
            TextOutA(hdc, 50, 20, "SMART PARKING DASHBOARD", 24);

            int y = 80;
            AreaNode* areaNode = currentZone->getAreaHead();
            while (areaNode) {
                SetTextColor(hdc, RGB(50, 50, 50));
                TextOutA(hdc, 50, y, areaNode->area->getAreaName().c_str(), areaNode->area->getAreaName().length());
                y += 30;

                SlotNode* slotNode = areaNode->area->getHead();
                int x = 50;
                while (slotNode) {
                    bool isBusy = (slotNode->slot->getStatus() == 1);
                    HBRUSH hBr = isBusy ? CreateSolidBrush(RGB(230, 60, 60)) : CreateSolidBrush(RGB(60, 200, 80));
                    RECT r = { x, y, x + 85, y + 65 };
                    FillRect(hdc, &r, hBr);
                    
                    SetTextColor(hdc, RGB(255, 255, 255));
                    // 1. Show Slot ID and Plate Number
                    std::string txt = "Slot " + std::to_string(slotNode->slot->getSlotId());
                    TextOutA(hdc, x + 5, y + 10, txt.c_str(), txt.length());
                    
                    if(isBusy) {
                        HFONT smallFont = CreateFontA(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, "Arial");
                        SelectObject(hdc, smallFont);
                        TextOutA(hdc, x + 5, y + 35, slotNode->slot->getPlate().c_str(), slotNode->slot->getPlate().length());
                        DeleteObject(smallFont);
                    }

                    DeleteObject(hBr);
                    x += 95;
                    slotNode = slotNode->next;
                }
                y += 110;
                areaNode = areaNode->next;
            }
            EndPaint(hwnd, &ps);
        } break;

        case WM_CTLCOLORSTATIC:
            SetBkColor((HDC)wp, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(WHITE_BRUSH);

        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(Zone& northZone) {
        currentZone = &northZone;
        HINSTANCE hInst = GetModuleHandle(NULL);
        WNDCLASSW wc = {0};
        wc.lpszClassName = L"FinalGUIClass";
        wc.lpfnWndProc = WindowProcedure;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);

        CreateWindowW(L"FinalGUIClass", L"Smart Parking Control Center", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 100, 600, 600, NULL, NULL, hInst, NULL);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
};

Zone* ParkingGUI::currentZone = nullptr;
HWND ParkingGUI::hPlateInput = nullptr;
HWND ParkingGUI::hTypeCombo = nullptr;
HBRUSH ParkingGUI::hSkyBlueBrush = nullptr;

#endif