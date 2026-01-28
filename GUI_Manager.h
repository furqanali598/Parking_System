#ifndef GUI_MANAGER_ENHANCED_H
#define GUI_MANAGER_ENHANCED_H

#include <windows.h>
#include <string>
#include <commctrl.h>
#include "Zone.h"

#pragma comment(lib, "comctl32.lib")

// Enhanced color scheme
#define COLOR_PRIMARY RGB(41, 128, 185)        // Blue
#define COLOR_PRIMARY_DARK RGB(31, 97, 141)    // Dark Blue
#define COLOR_SUCCESS RGB(39, 174, 96)         // Green
#define COLOR_DANGER RGB(231, 76, 60)          // Red
#define COLOR_WARNING RGB(243, 156, 18)        // Orange
#define COLOR_LIGHT RGB(236, 240, 241)         // Light Gray
#define COLOR_DARK RGB(44, 62, 80)             // Dark Gray
#define COLOR_WHITE RGB(255, 255, 255)
#define COLOR_OCCUPIED RGB(255, 107, 107)      // Light Red
#define COLOR_AVAILABLE RGB(129, 236, 236)     // Light Cyan

// Control IDs
#define ID_BTN_PARK 201
#define ID_BTN_RELEASE 202
#define ID_BTN_SEARCH 207
#define ID_BTN_REFRESH 208
#define ID_INPUT_PLATE 203
#define ID_COMBO_TYPE 204
#define ID_COMBO_BUILDING 205
#define ID_COMBO_FLOOR 206
#define ID_STATUS_BAR 209

class ParkingGUI {
public:
    static Zone* currentZone;
    static GlobalManager* globalNet;
    static HWND hPlateInput, hTypeCombo, hBuildingCombo, hFloorCombo, hStatusBar;
    static HFONT hTitleFont, hButtonFont, hLabelFont, hSlotFont, hEmojiFont;

    static void UpdateFloorCombo() {
        if (!hFloorCombo || !currentZone) return;
        SendMessage(hFloorCombo, CB_RESETCONTENT, 0, 0);
        AreaNode* temp = currentZone->getAreaHead();
        while (temp) {
            SendMessageA(hFloorCombo, CB_ADDSTRING, 0, (LPARAM)temp->area->getAreaName().c_str());
            temp = temp->next;
        }
        SendMessage(hFloorCombo, CB_SETCURSEL, 0, 0);
        UpdateStatusBar();
    }

    static void UpdateStatusBar() {
        if (!hStatusBar || !currentZone) return;
        int total = 0, occupied = 0, available = 0;
        AreaNode* areaNode = currentZone->getAreaHead();
        while(areaNode) {
            SlotNode* slotNode = areaNode->area->getHead();
            while(slotNode) {
                total++;
                if(slotNode->slot->getStatus() == 1) occupied++;
                else available++;
                slotNode = slotNode->next;
            }
            areaNode = areaNode->next;
        }
        std::string status = "Total Slots: " + std::to_string(total) + 
                           " | Available: " + std::to_string(available) + 
                           " | Occupied: " + std::to_string(occupied);
        SendMessageA(hStatusBar, SB_SETTEXT, 0, (LPARAM)status.c_str());
    }

    static void DrawRoundedRect(HDC hdc, RECT rect, int radius, COLORREF color) {
        HBRUSH brush = CreateSolidBrush(color);
        HPEN pen = CreatePen(PS_SOLID, 1, color);
        SelectObject(hdc, brush);
        SelectObject(hdc, pen);
        RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, radius, radius);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    static void DrawGradientRect(HDC hdc, RECT rect, COLORREF color1, COLORREF color2) {
        TRIVERTEX vertex[2];
        vertex[0].x = rect.left;
        vertex[0].y = rect.top;
        vertex[0].Red = GetRValue(color1) << 8;
        vertex[0].Green = GetGValue(color1) << 8;
        vertex[0].Blue = GetBValue(color1) << 8;
        vertex[0].Alpha = 0x0000;

        vertex[1].x = rect.right;
        vertex[1].y = rect.bottom;
        vertex[1].Red = GetRValue(color2) << 8;
        vertex[1].Green = GetGValue(color2) << 8;
        vertex[1].Blue = GetBValue(color2) << 8;
        vertex[1].Alpha = 0x0000;

        GRADIENT_RECT gRect = {0, 1};
        GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
    }

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE: {
            // Initialize Common Controls
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_BAR_CLASSES;
            InitCommonControlsEx(&icex);

            // Create fonts
            hTitleFont = CreateFontW(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            hButtonFont = CreateFontW(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            hLabelFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            hSlotFont = CreateFontW(13, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            hEmojiFont = CreateFontW(28, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI Emoji");

            // Status Bar
            hStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, 
                WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 
                0, 0, 0, 0, hwnd, (HMENU)ID_STATUS_BAR, GetModuleHandle(NULL), NULL);

            // Labels with better styling
            HWND hLabelBuilding = CreateWindowW(L"Static", L"🏢 BUILDING:", 
                WS_VISIBLE | WS_CHILD, 150, 625, 100, 22, hwnd, NULL, NULL, NULL);
            SendMessage(hLabelBuilding, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            hBuildingCombo = CreateWindowW(L"Combobox", NULL, 
                WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 
                250, 622, 160, 200, hwnd, (HMENU)ID_COMBO_BUILDING, NULL, NULL);
            SendMessage(hBuildingCombo, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            HWND hLabelFloor = CreateWindowW(L"Static", L"📍 FLOOR:", 
                WS_VISIBLE | WS_CHILD, 430, 625, 80, 22, hwnd, NULL, NULL, NULL);
            SendMessage(hLabelFloor, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            hFloorCombo = CreateWindowW(L"Combobox", NULL, 
                WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 
                510, 622, 140, 200, hwnd, (HMENU)ID_COMBO_FLOOR, NULL, NULL);
            SendMessage(hFloorCombo, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            HWND hLabelPlate = CreateWindowW(L"Static", L"🚗 LICENSE PLATE:", 
                WS_VISIBLE | WS_CHILD, 150, 665, 130, 22, hwnd, NULL, NULL, NULL);
            SendMessage(hLabelPlate, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            hPlateInput = CreateWindowW(L"Edit", L"", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_UPPERCASE, 
                280, 662, 130, 28, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);
            SendMessage(hPlateInput, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            hTypeCombo = CreateWindowW(L"Combobox", NULL, 
                WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 
                420, 662, 100, 200, hwnd, (HMENU)ID_COMBO_TYPE, NULL, NULL);
            SendMessage(hTypeCombo, WM_SETFONT, (WPARAM)hLabelFont, TRUE);
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"🚗 Car");
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"🏍️ Bike");
            SendMessageW(hTypeCombo, CB_SETCURSEL, 0, 0);

            // Buttons with better spacing
            CreateWindowW(L"Button", L"🅿️ PARK", 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
                540, 658, 110, 35, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            
            CreateWindowW(L"Button", L"🚪 RELEASE", 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
                660, 658, 110, 35, hwnd, (HMENU)ID_BTN_RELEASE, NULL, NULL);
            
            CreateWindowW(L"Button", L"🔍 SEARCH", 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
                780, 658, 110, 35, hwnd, (HMENU)ID_BTN_SEARCH, NULL, NULL);

            CreateWindowW(L"Button", L"🔄 REFRESH", 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
                900, 658, 110, 35, hwnd, (HMENU)ID_BTN_REFRESH, NULL, NULL);

            // Set button fonts
            HWND hBtnPark = GetDlgItem(hwnd, ID_BTN_PARK);
            HWND hBtnRelease = GetDlgItem(hwnd, ID_BTN_RELEASE);
            HWND hBtnSearch = GetDlgItem(hwnd, ID_BTN_SEARCH);
            HWND hBtnRefresh = GetDlgItem(hwnd, ID_BTN_REFRESH);
            SendMessage(hBtnPark, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(hBtnRelease, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(hBtnSearch, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(hBtnRefresh, WM_SETFONT, (WPARAM)hButtonFont, TRUE);

            // Populate buildings
            for (Zone* z : globalNet->getBuildings()) {
                SendMessageA(hBuildingCombo, CB_ADDSTRING, 0, (LPARAM)z->getName().c_str());
            }
            SendMessageW(hBuildingCombo, CB_SETCURSEL, 0, 0);
            UpdateFloorCombo();
        } break;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wp;
            SetTextColor(hdcStatic, COLOR_DARK);
            SetBkMode(hdcStatic, TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        } break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect; GetClientRect(hwnd, &rect);
            int winWidth = rect.right;

            // Draw gradient background for header
            RECT headerRect = {0, 0, winWidth, 90};
            DrawGradientRect(hdc, headerRect, COLOR_PRIMARY, COLOR_PRIMARY_DARK);

            // Title
            SelectObject(hdc, hTitleFont);
            SetTextColor(hdc, COLOR_WHITE);
            SetBkMode(hdc, TRANSPARENT);
            RECT rTitle = {0, 10, winWidth, 50};
            DrawTextW(hdc, L"🅿️ Smart Parking Management", -1, &rTitle, DT_CENTER | DT_SINGLELINE);

            // Subtitle
            HFONT hSubFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            SelectObject(hdc, hSubFont);
            std::string bName = "📍 " + currentZone->getName() + " Parking Complex";
            std::wstring wbName(bName.begin(), bName.end());
            RECT rSub = {0, 50, winWidth, 75};
            DrawTextW(hdc, wbName.c_str(), -1, &rSub, DT_CENTER | DT_SINGLELINE);

            // Main content area background
            RECT contentRect = {10, 95, winWidth - 10, 610};
            HBRUSH contentBrush = CreateSolidBrush(RGB(250, 250, 250));
            FillRect(hdc, &contentRect, contentBrush);
            DeleteObject(contentBrush);

            AreaNode* areaNode = currentZone->getAreaHead();
            int areaY = 105;

            while(areaNode) {
                // Floor header with gradient background
                RECT floorHeaderRect = {30, areaY, winWidth - 30, areaY + 30};
                DrawGradientRect(hdc, floorHeaderRect, COLOR_LIGHT, RGB(220, 220, 220));
                
                SelectObject(hdc, hButtonFont);
                SetTextColor(hdc, COLOR_DARK);
                std::string fName = "🏢 " + areaNode->area->getAreaName();
                std::wstring wfName(fName.begin(), fName.end());
                RECT rFloor = {35, areaY + 5, winWidth - 35, areaY + 28};
                DrawTextW(hdc, wfName.c_str(), -1, &rFloor, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

                // Slot statistics
                int totalSlots = 0, occupiedSlots = 0;
                SlotNode* countNode = areaNode->area->getHead();
                while(countNode) {
                    totalSlots++;
                    if(countNode->slot->getStatus() == 1) occupiedSlots++;
                    countNode = countNode->next;
                }
                std::string stats = std::to_string(totalSlots - occupiedSlots) + "/" + std::to_string(totalSlots) + " Available";
                std::wstring wStats(stats.begin(), stats.end());
                RECT rStats = {winWidth - 200, areaY + 5, winWidth - 35, areaY + 28};
                DrawTextW(hdc, wStats.c_str(), -1, &rStats, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

                SlotNode* slotNode = areaNode->area->getHead();
                int col = 0;
                int startX = (winWidth - (8 * 125)) / 2;

                while(slotNode) {
                    int curX = startX + (col * 125);
                    int curY = areaY + 40;
                    RECT rSlot = {curX, curY, curX + 115, curY + 75};
                    
                    bool occupied = (slotNode->slot->getStatus() == 1);
                    COLORREF slotColor = occupied ? COLOR_OCCUPIED : COLOR_AVAILABLE;
                    
                    // Draw slot with rounded corners and shadow effect
                    RECT shadowRect = {curX + 2, curY + 2, curX + 117, curY + 77};
                    DrawRoundedRect(hdc, shadowRect, 10, RGB(200, 200, 200));
                    DrawRoundedRect(hdc, rSlot, 10, slotColor);
                    
                    SetBkMode(hdc, TRANSPARENT);

                    if(!occupied) {
                        // Free slot
                        SetTextColor(hdc, COLOR_PRIMARY_DARK);
                        SelectObject(hdc, hSlotFont);
                        RECT textRect = {curX, curY + 28, curX + 115, curY + 48};
                        DrawTextW(hdc, L"✓ AVAILABLE", -1, &textRect, DT_CENTER | DT_SINGLELINE);
                    } else {
                        // Occupied slot
                        SelectObject(hdc, hEmojiFont);
                        std::string type = slotNode->slot->getVehicleType();
                        RECT emojiRect = {curX, curY + 5, curX + 115, curY + 35};
                        if(type == "Bike") DrawTextW(hdc, L"🏍️", -1, &emojiRect, DT_CENTER | DT_SINGLELINE);
                        else DrawTextW(hdc, L"🚗", -1, &emojiRect, DT_CENTER | DT_SINGLELINE);

                        SelectObject(hdc, hSlotFont);
                        SetTextColor(hdc, RGB(139, 0, 0));
                        std::string p = slotNode->slot->getPlate();
                        std::wstring wp(p.begin(), p.end());
                        RECT plateRect = {curX, curY + 45, curX + 115, curY + 65};
                        DrawTextW(hdc, wp.c_str(), -1, &plateRect, DT_CENTER | DT_SINGLELINE);
                    }
                    
                    slotNode = slotNode->next; 
                    col++;
                }
                areaY += 125;
                areaNode = areaNode->next;
            }
            
            DeleteObject(hSubFont);
            EndPaint(hwnd, &ps);
        } break;

        case WM_COMMAND: {
            if (LOWORD(wp) == ID_COMBO_BUILDING && HIWORD(wp) == CBN_SELCHANGE) {
                int index = SendMessage(hBuildingCombo, CB_GETCURSEL, 0, 0);
                currentZone = globalNet->getBuildings()[index];
                UpdateFloorCombo();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            if (LOWORD(wp) == ID_BTN_PARK) {
                char p[20], t[20], f[50];
                GetWindowTextA(hPlateInput, p, 20);
                int typeIdx = SendMessage(hTypeCombo, CB_GETCURSEL, 0, 0);
                const char* type = (typeIdx == 0) ? "Car" : "Bike";
                SendMessageA(hFloorCombo, CB_GETLBTEXT, SendMessage(hFloorCombo, CB_GETCURSEL, 0, 0), (LPARAM)f);
                
                if (strlen(p) == 0) {
                    MessageBoxA(hwnd, "Please enter a license plate number!", "Input Required", MB_OK | MB_ICONWARNING);
                    return 0;
                }
                
                if (currentZone->parkInSpecificArea(new Vehicle(p, type), f)) {
                    std::string successMsg = "✓ Vehicle parked successfully!\n\nPlate: " + std::string(p) + 
                                           "\nType: " + std::string(type) + 
                                           "\nFloor: " + std::string(f);
                    MessageBoxA(hwnd, successMsg.c_str(), "Success", MB_OK | MB_ICONINFORMATION);
                    SetWindowTextA(hPlateInput, "");
                    InvalidateRect(hwnd, NULL, TRUE);
                } else {
                    MessageBoxA(hwnd, "❌ No available slots in this area!", "Parking Full", MB_OK | MB_ICONERROR);
                }
            }
            if (LOWORD(wp) == ID_BTN_RELEASE) {
                char p[20]; 
                GetWindowTextA(hPlateInput, p, 20);
                if (strlen(p) == 0) {
                    MessageBoxA(hwnd, "Please enter a license plate number!", "Input Required", MB_OK | MB_ICONWARNING);
                    return 0;
                }
                
                std::string type = currentZone->releaseAndGetType(p);
                if (!type.empty()) {
                    int price = (type == "Bike") ? 50 : 100;
                    std::string msg = "═══════════════════════════\n"
                                     "     PARKING RECEIPT\n"
                                     "═══════════════════════════\n\n"
                                     "License Plate: " + std::string(p) + "\n"
                                     "Vehicle Type:  " + type + "\n"
                                     "Parking Fee:   Rs. " + std::to_string(price) + "\n\n"
                                     "═══════════════════════════\n"
                                     "   Thank you for parking!\n"
                                     "   Drive safely! 🚗\n"
                                     "═══════════════════════════";
                    MessageBoxA(hwnd, msg.c_str(), "Receipt", MB_OK | MB_ICONINFORMATION);
                    SetWindowTextA(hPlateInput, "");
                    InvalidateRect(hwnd, NULL, TRUE);
                } else {
                    MessageBoxA(hwnd, "❌ Vehicle not found in this building!\n\nPlease check:\n• License plate number\n• Selected building", "Not Found", MB_OK | MB_ICONERROR);
                }
            }
            if (LOWORD(wp) == ID_BTN_SEARCH) {
                char p[20]; 
                GetWindowTextA(hPlateInput, p, 20);
                if (strlen(p) > 0) {
                    globalNet->searchGlobal(p);
                } else {
                    MessageBoxA(hwnd, "Please enter a license plate number to search!", "Input Required", MB_OK | MB_ICONWARNING);
                }
            }
            if (LOWORD(wp) == ID_BTN_REFRESH) {
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateStatusBar();
                MessageBoxA(hwnd, "✓ Display refreshed!", "Refresh", MB_OK | MB_ICONINFORMATION);
            }
        } break;

        case WM_SIZE: {
            SendMessage(hStatusBar, WM_SIZE, 0, 0);
        } break;

        case WM_DESTROY: {
            DeleteObject(hTitleFont);
            DeleteObject(hButtonFont);
            DeleteObject(hLabelFont);
            DeleteObject(hSlotFont);
            DeleteObject(hEmojiFont);
            PostQuitMessage(0);
        } break;

        default: return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(GlobalManager& gm) {
        globalNet = &gm;
        currentZone = globalNet->getBuildings()[0];
        
        WNDCLASSW wc = {0};
        wc.lpszClassName = L"EnhancedParkingGUI";
        wc.lpfnWndProc = WindowProcedure;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        RegisterClassW(&wc);
        
        CreateWindowW(L"EnhancedParkingGUI", L"🅿️ Smart Parking Management System", 
                      WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                      100, 50, 1150, 800, NULL, NULL, wc.hInstance, NULL);
        
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) { 
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }
    }
};

Zone* ParkingGUI::currentZone = nullptr;
GlobalManager* ParkingGUI::globalNet = nullptr;
HWND ParkingGUI::hPlateInput = nullptr;
HWND ParkingGUI::hTypeCombo = nullptr;
HWND ParkingGUI::hBuildingCombo = nullptr;
HWND ParkingGUI::hFloorCombo = nullptr;
HWND ParkingGUI::hStatusBar = nullptr;
HFONT ParkingGUI::hTitleFont = nullptr;
HFONT ParkingGUI::hButtonFont = nullptr;
HFONT ParkingGUI::hLabelFont = nullptr;
HFONT ParkingGUI::hSlotFont = nullptr;
HFONT ParkingGUI::hEmojiFont = nullptr;

#endif