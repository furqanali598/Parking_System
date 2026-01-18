#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <commctrl.h> 
#include <string>
#include "Zone.h" // Include logic

#define ID_BTN_PARK 201
#define ID_BTN_UNPARK 202
#define ID_BTN_SEARCH 205
#define ID_INPUT_PLATE 203
#define ID_COMBO_TYPE 204

class ParkingGUI {
public:
    static Zone* currentZone;
    static HWND hPlateInput, hTypeCombo;
    static std::string searchQuery;

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE: {
            HFONT hUIFont = CreateFontA(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, "Segoe UI");
            CreateWindowW(L"Static", L"PLATE:", WS_VISIBLE | WS_CHILD, 20, 430, 60, 20, hwnd, NULL, NULL, NULL);
            hPlateInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 85, 428, 120, 25, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);
            hTypeCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 215, 428, 80, 200, hwnd, (HMENU)ID_COMBO_TYPE, NULL, NULL);
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Car");
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Bike");
            SendMessageW(hTypeCombo, CB_SETCURSEL, 0, 0);
            CreateWindowW(L"Button", L"PARK", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 310, 425, 90, 32, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            CreateWindowW(L"Button", L"RELEASE", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 410, 425, 90, 32, hwnd, (HMENU)ID_BTN_UNPARK, NULL, NULL);
            CreateWindowW(L"Button", L"SEARCH", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 510, 425, 90, 32, hwnd, (HMENU)ID_BTN_SEARCH, NULL, NULL);
        } break;

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lp;
            HBRUSH hBtnBr = CreateSolidBrush(RGB(52, 152, 219)); 
            FillRect(pdis->hDC, &pdis->rcItem, hBtnBr);
            SetBkMode(pdis->hDC, TRANSPARENT);
            SetTextColor(pdis->hDC, RGB(255, 255, 255));
            wchar_t b[20]; GetWindowTextW(pdis->hwndItem, b, 20);
            DrawTextW(pdis->hDC, b, -1, &pdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hBtnBr);
            return TRUE;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT clientRect; GetClientRect(hwnd, &clientRect);
            FillRect(hdc, &clientRect, (HBRUSH)CreateSolidBrush(RGB(245, 246, 250))); 

            HFONT hTitleFont = CreateFontA(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, "Segoe UI");
            HFONT hSlotFont = CreateFontA(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, "Segoe UI");
            HFONT hEmojiFont = CreateFontA(36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, "Segoe UI Emoji");

            AreaNode* areaNode = currentZone->getAreaHead();
            int areaY = 20;
            
            while(areaNode) {
                SelectObject(hdc, hTitleFont);
                SetTextColor(hdc, RGB(44, 62, 80));
                SetBkMode(hdc, TRANSPARENT);
                TextOutA(hdc, 25, areaY, areaNode->area->getAreaName().c_str(), areaNode->area->getAreaName().length());

                SlotNode* slotNode = areaNode->area->getHead();
                int col = 0;
                while(slotNode) {
                    bool busy = (slotNode->slot->getStatus() == 1);
                    bool highlighted = (busy && !searchQuery.empty() && slotNode->slot->getPlate() == searchQuery);
                    int curX = 25 + (col * 125);
                    int curY = areaY + 40;
                    RECT r = { curX, curY, curX + 110, curY + 90 };

                    HBRUSH hBr;
                    if (highlighted) hBr = CreateSolidBrush(RGB(255, 234, 167));
                    else if (busy) hBr = CreateSolidBrush(RGB(255, 121, 121)); // RED BG
                    else hBr = CreateSolidBrush(RGB(223, 249, 251)); // FREE BG
                    
                    FillRect(hdc, &r, hBr);
                    FrameRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

                    SelectObject(hdc, hSlotFont);
                    SetTextColor(hdc, RGB(45, 52, 54));
                    std::string sId = "S-" + std::to_string(slotNode->slot->getSlotId());
                    TextOutA(hdc, curX + 5, curY + 5, sId.c_str(), sId.length());

                    if(busy) {
                        SelectObject(hdc, hEmojiFont);
                        if (slotNode->slot->getType() == "Car") TextOutW(hdc, curX + 30, curY + 15, L"\xD83D\xDE97", 2); 
                        else TextOutW(hdc, curX + 30, curY + 15, L"\xD83C\xDFCD", 2); 
                        SelectObject(hdc, hSlotFont);
                        SetTextColor(hdc, RGB(0, 0, 0));
                        TextOutA(hdc, curX + 10, curY + 65, slotNode->slot->getPlate().c_str(), slotNode->slot->getPlate().length());
                    } else {
                        SelectObject(hdc, hSlotFont);
                        SetTextColor(hdc, RGB(38, 166, 91));
                        TextOutA(hdc, curX + 35, curY + 40, "FREE", 4);
                    }
                    DeleteObject(hBr);
                    slotNode = slotNode->next;
                    col++;
                }
                areaY += 150; 
                areaNode = areaNode->next;
            }
            DeleteObject(hTitleFont); DeleteObject(hSlotFont); DeleteObject(hEmojiFont);
            EndPaint(hwnd, &ps);
        } break;

        case WM_COMMAND: {
            if (LOWORD(wp) == ID_BTN_PARK) {
                char p[20], t[20]; GetWindowTextA(hPlateInput, p, 20);
                int s = SendMessage(hTypeCombo, CB_GETCURSEL, 0, 0);
                SendMessageA(hTypeCombo, CB_GETLBTEXT, s, (LPARAM)t);
                if (strlen(p) > 0) { currentZone->parkVehicle(new Vehicle(p, t)); searchQuery = ""; InvalidateRect(hwnd, NULL, TRUE); }
            } else if (LOWORD(wp) == ID_BTN_UNPARK) {
                char p[20]; GetWindowTextA(hPlateInput, p, 20);
                if (currentZone->releaseVehicle(p)) { searchQuery = ""; InvalidateRect(hwnd, NULL, TRUE); }
            } else if (LOWORD(wp) == ID_BTN_SEARCH) {
                char p[20]; GetWindowTextA(hPlateInput, p, 20);
                searchQuery = p; InvalidateRect(hwnd, NULL, TRUE);
            }
        } break;

        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(Zone& z) {
        currentZone = &z;
        HINSTANCE hInst = GetModuleHandle(NULL);
        WNDCLASSW wc = {0};
        wc.lpszClassName = L"DashboardGUI";
        wc.lpfnWndProc = WindowProcedure;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hInstance = hInst;
        RegisterClassW(&wc);
        CreateWindowW(L"DashboardGUI", L"Smart Parking Control Panel", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 850, 550, NULL, NULL, hInst, NULL);
        MSG m; while (GetMessage(&m, NULL, 0, 0)) { TranslateMessage(&m); DispatchMessage(&m); }
    }
};

Zone* ParkingGUI::currentZone = nullptr;
HWND ParkingGUI::hPlateInput = nullptr, ParkingGUI::hTypeCombo = nullptr;
std::string ParkingGUI::searchQuery = "";

#endif