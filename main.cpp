#include "parser.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    static HWND hEdit;
    static HWND hText;

    switch (uMsg) {

    case WM_CREATE: {

        VerticalLayout layout{ 20, 15, 20 };

        hText = CreateWindowEx(
            0, L"STATIC", L"Result",
            WS_CHILD | WS_VISIBLE,
            layout.marginX, layout.Append(layout.height), 300, layout.height,
            hwnd, (HMENU)101, GetModuleHandle(nullptr), nullptr
        );

        hEdit = CreateWindowEx(
            0, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            layout.marginX, layout.Append(layout.height), 300, layout.height,
            hwnd, (HMENU)ID::ID_EDIT_BOX, NULL, NULL
        );

        CreateWindowEx(
            0, L"BUTTON", L"=",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            layout.marginX, layout.Append(layout.height), 120, layout.height,
            hwnd, (HMENU)ID::ID_BUTTON, NULL, NULL
        );

        return 0;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == (WPARAM)ID::ID_BUTTON && HIWORD(wParam) == BN_CLICKED) {

            std::wstring buf;
            buf.resize(512);
            int len = GetWindowText(hEdit, &buf[0], (int)buf.size());
            buf.resize(len);
            Parser expr1{ buf,hText };

        }
        return 0;


    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    const wchar_t* CLASS_NAME = L"WindowClass";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {

        return 0;
    }

    ShowWindow(hwnd, nShowCmd);

    MSG msg = { };

    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;

}