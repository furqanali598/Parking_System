#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <string>
#include "Zone.h"

#define ID_BTN_PARK 201
#define ID_BTN_UNPARK 202
#define ID_INPUT_PLATE 203
#define ID_INPUT_TYPE 204

class ParkingGUI {
public:
    static Zone* currentZone;
    static HWND hPlateInput, hTypeInput;

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE: {
            HFONT hNormalFont = CreateFontA(18, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            // Input Labels
            HWND hL1 = CreateWindowW(L"Static", L"Plate Number:", WS_VISIBLE | WS_CHILD, 50, 420, 120, 25, hwnd, NULL, NULL, NULL);
            hPlateInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 420, 150, 25, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);

            HWND hL2 = CreateWindowW(L"Static", L"Type (Car/Bike):", WS_VISIBLE | WS_CHILD, 50, 455, 120, 25, hwnd, NULL, NULL, NULL);
            hTypeInput = CreateWindowW(L"Edit", L"Car", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 455, 150, 25, hwnd, (HMENU)ID_INPUT_TYPE, NULL, NULL);

            // Buttons
            HWND hBtnPark = CreateWindowW(L"Button", L"PARK VEHICLE", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 350, 418, 130, 30, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            HWND hBtnUnpark = CreateWindowW(L"Button", L"UNPARK", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 350, 453, 130, 30, hwnd, (HMENU)ID_BTN_UNPARK, NULL, NULL);

            // Set font for inputs and labels
            SendMessage(hL1, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
            SendMessage(hL2, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
            SendMessage(hPlateInput, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
            SendMessage(hTypeInput, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
            SendMessage(hBtnPark, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
            SendMessage(hBtnUnpark, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
        } break;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_PARK) {
                char plate[20], type[20];
                GetWindowTextA(hPlateInput, plate, 20);
                GetWindowTextA(hTypeInput, type, 20);
                if (strlen(plate) > 0) {
                    currentZone->parkVehicle(new Vehicle(plate, type));
                    SetWindowTextA(hPlateInput, "");
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            } else if (LOWORD(wp) == ID_BTN_UNPARK) {
                char plate[20];
                GetWindowTextA(hPlateInput, plate, 20);
                if (currentZone->releaseVehicle(plate)) {
                    MessageBoxA(hwnd, "Vehicle Released!", "Success", MB_OK);
                } else {
                    MessageBoxA(hwnd, "Plate not found!", "Error", MB_ICONERROR);
                }
                InvalidateRect(hwnd, NULL, TRUE);
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
            
            // Title
            HFONT hTitleFont = CreateFontA(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            SelectObject(hdc, hTitleFont);
            SetTextColor(hdc, RGB(20, 60, 120)); // Navy Blue
            TextOutA(hdc, 50, 20, "SMART PARKING MANAGEMENT SYSTEM", 31);
            DeleteObject(hTitleFont);

            // Drawing Areas and Slots
            HFONT hLabelFont = CreateFontA(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            SelectObject(hdc, hLabelFont);

            int y = 80;
            AreaNode* areaNode = currentZone->getAreaHead();
            while (areaNode) {
                SetTextColor(hdc, RGB(50, 50, 50));
                TextOutA(hdc, 50, y, areaNode->area->getAreaName().c_str(), areaNode->area->getAreaName().length());
                y += 40;

                SlotNode* slotNode = areaNode->area->getHead();
                int x = 50;
                while (slotNode) {
                    // Draw Rounded-look Rectangles
                    HBRUSH hBr = (slotNode->slot->getStatus() == 1) ? CreateSolidBrush(RGB(230, 60, 60)) : CreateSolidBrush(RGB(60, 200, 80));
                    RECT r = { x, y, x + 75, y + 60 };
                    FillRect(hdc, &r, hBr);
                    FrameRect(hdc, &r, (HBRUSH)GetStockObject(GRAY_BRUSH));

                    SetTextColor(hdc, RGB(255, 255, 255));
                    std::string sId = std::to_string(slotNode->slot->getSlotId());
                    TextOutA(hdc, x + 30, y + 20, sId.c_str(), sId.length());

                    DeleteObject(hBr);
                    x += 90;
                    slotNode = slotNode->next;
                }
                y += 100;
                areaNode = areaNode->next;
            }
            DeleteObject(hLabelFont);
            EndPaint(hwnd, &ps);
        } break;

        case WM_CTLCOLORSTATIC: {
            // This makes the text labels have a white background to match the window
            HDC hdcStatic = (HDC)wp;
            SetBkColor(hdcStatic, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }

        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(Zone& northZone) {
        currentZone = &northZone;
        HINSTANCE hInst = GetModuleHandle(NULL);
        WNDCLASSW wc = {0};
        wc.lpszClassName = L"ModernGUIClass";
        wc.lpfnWndProc = WindowProcedure;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);

        CreateWindowW(L"ModernGUIClass", L"Parking Control Center", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 200, 100, 550, 580, NULL, NULL, hInst, NULL);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UnregisterClassW(L"ModernGUIClass", hInst);
    }
};

Zone* ParkingGUI::currentZone = nullptr;
HWND ParkingGUI::hPlateInput = nullptr;
HWND ParkingGUI::hTypeInput = nullptr;

#endif