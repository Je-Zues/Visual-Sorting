#pragma once
#include <windows.h>
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

struct UIVEC2 {
    float x, y;
};

struct SLIDER {
    Gdiplus::RectF rect;
    Gdiplus::Color filled, notFilled;
    int percent;
    float width;

};

struct UITEXT {
    UIVEC2 rect;
    int fontSize;
    Gdiplus::Color color;
    std::string _text;
};


class UI {

private:

    HWND                hWnd;
    WNDCLASS            wndClass;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    PAINTSTRUCT  ps;
    HDC hdc;
    HDC hdcBuffer = NULL;
    HBITMAP hBmBuffer = NULL;

    int _width, _height;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

    HDC getBuffer()const { return this->hdcBuffer; }

    void drawText(const wchar_t* text, UIVEC2 pos, Gdiplus::Color color, UINT8 fontSize, Gdiplus::Graphics& graphics);
    void drawBox(Gdiplus::RectF rect, Gdiplus::Color color, float penWidth, Gdiplus::Graphics& graphics);

    void drawBoxes(Gdiplus::RectF* rects, size_t amount, Gdiplus::Color color, float penWidth, Gdiplus::Graphics& graphics);

    void drawCircle(int radius, UIVEC2 origin, Gdiplus::Color color, float penWidth, Gdiplus::Graphics& graphics);

    void fillRect(Gdiplus::RectF rect, Gdiplus::Color color, Gdiplus::Graphics& graphics);

    void drawSlider(SLIDER& slider, Gdiplus::Graphics& graphics);


    void drawLine(UIVEC2 start, UIVEC2 end, Gdiplus::Color color, Gdiplus::Graphics& graphics);


    void Init(const wchar_t* windowName, size_t initialX, size_t initialY, size_t width, size_t height);
    void startDraw();
    void stopDraw();
    int Update();

};
