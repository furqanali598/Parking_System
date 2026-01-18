#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <commctrl.h> // Required for Dropdown (Combobox)
#include <string>
#include "Zone.h"

// Button and Input IDs
#define ID_BTN_PARK 201
#define ID_BTN_UNPARK 202
#define ID_INPUT_PLATE 203
#define ID_COMBO_TYPE 204

class ParkingGUI {
public:
    static Zone* currentZone;
    static HWND hPlateInput, hTypeCombo;
    static HBRUSH hSkyBlueBrush;

    // Helper function to set fonts for all UI elements
    static BOOL CALLBACK SetFont(HWND child, LPARAM font) {
        SendMessage(child, WM_SETFONT, (WPARAM)font, TRUE);
        return TRUE;
    }

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE: {
            // Create a professional UI font
            HFONT hUIFont = CreateFontA(18, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            hSkyBlueBrush = CreateSolidBrush(RGB(135, 206, 235)); // Sky Blue

            // 1. Plate Number Input
            CreateWindowW(L"Static", L"Plate Number:", WS_VISIBLE | WS_CHILD, 50, 420, 120, 25, hwnd, NULL, NULL, NULL);
            hPlateInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 420, 150, 25, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);

            // 2. Vehicle Type Dropdown
            CreateWindowW(L"Static", L"Vehicle Type:", WS_VISIBLE | WS_CHILD, 50, 455, 120, 25, hwnd, NULL, NULL, NULL);
            hTypeCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 180, 455, 150, 200, hwnd, (HMENU)ID_COMBO_TYPE, NULL, NULL);
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Car");
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Bike");
            SendMessageW(hTypeCombo, CB_SETCURSEL, 0, 0);

            // 3. Sky Blue Buttons (BS_OWNERDRAW allows custom coloring)
            CreateWindowW(L"Button", L"PARK VEHICLE", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 350, 418, 130, 30, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            CreateWindowW(L"Button", L"UNPARK", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 350, 453, 130, 30, hwnd, (HMENU)ID_BTN_UNPARK, NULL, NULL);

            // Apply font to all labels, inputs, and buttons
            EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)hUIFont);
        } break;

        case WM_DRAWITEM: {
            // This case draws the "Sky Blue" buttons
            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lp;
            if (pdis->CtlID == ID_BTN_PARK || pdis->CtlID == ID_BTN_UNPARK) {
                FillRect(pdis->hDC, &pdis->rcItem, hSkyBlueBrush);
                SetBkMode(pdis->hDC, TRANSPARENT);
                
                wchar_t text[20];
                GetWindowTextW(pdis->hwndItem, text, 20);
                DrawTextW(pdis->hDC, text, -1, &pdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                // Draw a simple border
                FrameRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(GRAY_BRUSH));
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
                    InvalidateRect(hwnd, NULL, TRUE); // Refresh visuals
                }
            } else if (LOWORD(wp) == ID_BTN_UNPARK) {
                char plate[20];
                GetWindowTextA(hPlateInput, plate, 20);
                if (currentZone->releaseVehicle(plate)) {
                    InvalidateRect(hwnd, NULL, TRUE);
                } else {
                    MessageBoxA(hwnd, "Vehicle not found!", "Error", MB_ICONERROR);
                }
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // White Background
            HBRUSH hBg = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &ps.rcPaint, hBg);
            DeleteObject(hBg);

            SetBkMode(hdc, TRANSPARENT);
            
            // Draw Title
            HFONT hTitleFont = CreateFontA(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, "Segoe UI");
            SelectObject(hdc, hTitleFont);
            SetTextColor(hdc, RGB(20, 60, 120));
            TextOutA(hdc, 50, 20, "SMART PARKING MANAGEMENT SYSTEM", 31);
            DeleteObject(hTitleFont);

            int y = 80;
            AreaNode* areaNode = currentZone->getAreaHead();
            while (areaNode) {
                SetTextColor(hdc, RGB(80, 80, 80));
                TextOutA(hdc, 50, y, areaNode->area->getAreaName().c_str(), areaNode->area->getAreaName().length());
                y += 35;

                SlotNode* slotNode = areaNode->area->getHead();
                int x = 50;
                while (slotNode) {
                    bool isBusy = (slotNode->slot->getStatus() == 1);
                    HBRUSH hBr = isBusy ? CreateSolidBrush(RGB(235, 77, 75)) : CreateSolidBrush(RGB(106, 176, 76));
                    
                    RECT r = { x, y, x + 85, y + 65 };
                    FillRect(hdc, &r, hBr);
                    FrameRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

                    SetTextColor(hdc, RGB(255, 255, 255));
                    std::string sId = "Slot " + std::to_string(slotNode->slot->getSlotId());
                    TextOutA(hdc, x + 5, y + 10, sId.c_str(), sId.length());

                    if (isBusy) {
                        // Display the plate number inside the busy slot
                        TextOutA(hdc, x + 5, y + 35, slotNode->slot->getPlate().c_str(), slotNode->slot->getPlate().length());
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
        wc.lpszClassName = L"FinalModernGUI";
        wc.lpfnWndProc = WindowProcedure;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);

        CreateWindowW(L"FinalModernGUI", L"Parking Control Center", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 100, 100, 580, 600, NULL, NULL, hInst, NULL);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UnregisterClassW(L"FinalModernGUI", hInst);
    }
};

// Initialize static members
Zone* ParkingGUI::currentZone = nullptr;
HWND ParkingGUI::hPlateInput = nullptr;
HWND ParkingGUI::hTypeCombo = nullptr;
HBRUSH ParkingGUI::hSkyBlueBrush = nullptr;

#endif