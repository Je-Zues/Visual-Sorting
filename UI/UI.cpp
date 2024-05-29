#include "UI.h"

/* calls the objects wndProc to handle messages  */
LRESULT CALLBACK UI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UI* wind = (UI*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    return (wind->MyWndProc(hWnd, message, wParam, lParam));
}

LRESULT CALLBACK UI::MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /* messages handled */
    switch (message)
    {
    case WM_PAINT: break;

        /* user closed window */
    case WM_QUIT:
    case WM_DESTROY: {
        PostQuitMessage(-1);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void UI::drawText(const wchar_t* text, UIVEC2 pos, Gdiplus::Color color, UINT8 fontSize, Gdiplus::Graphics &graphics) {
    
    /* creating brush, font and point to draw text */

    Gdiplus::SolidBrush  brush(color);                                  //sets color of text
    Gdiplus::FontFamily  fontFamily(L"Times New Roman");                                        //sets font of text
    Gdiplus::Font        font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel); //sets font and size
    Gdiplus::PointF      pointF(pos.x, pos.y);                                                //position of text

    /* draws the string on buffer */
    graphics.DrawString(text, -1, &font, pointF, &brush);//draws
}
void UI::drawBox(Gdiplus::RectF rect, Gdiplus::Color color, float penWidth, Gdiplus::Graphics& graphics) {

    /* makes a pen object and draws a box on the buffer */
    Gdiplus::Pen pen(color, penWidth);
    graphics.DrawRectangle(&pen, rect);
}

void UI::drawBoxes(Gdiplus::RectF* rects, size_t amount, Gdiplus::Color color, float penWidth, Gdiplus::Graphics& graphics) {

    /* makes a pen object and draws multiple boxes on the buffer */
    Gdiplus::Pen pen(color, penWidth);
    graphics.DrawRectangles(&pen, rects, amount);
}

void UI::drawCircle(int radius, UIVEC2 origin, Gdiplus::Color color, float penWidth, Gdiplus::Graphics& graphics) {

    /* creates a perfect square starting from the origin coords */
    Gdiplus::RectF rect{ origin.x - (radius),  origin.y - radius ,(float)2 * radius, (float)2 * radius };

    /* creates a pen to draw on buffer */
    Gdiplus::Pen pen(color, penWidth);

    /* draws a circle on the buffer */
    graphics.DrawArc(&pen, rect, 0, 360);
}

void UI::fillRect(Gdiplus::RectF rect, Gdiplus::Color color, Gdiplus::Graphics& graphics) {

    /* creates a brush to color a rectangle */
    Gdiplus::SolidBrush  brush(color);

    /* fills the rectangle on the buffer */
    graphics.FillRectangle(&brush, rect);

}

//void UI::drawSlider(SLIDER& slider, Gdiplus::Graphics& graphics) {
//
//    fillRect(Gdiplus::RectF{ slider.rect.X ,slider.rect.Y , slider.rect.Width ,slider.rect.Height }, slider.filled,graphics);
//    fillRect(Gdiplus::RectF{ slider.rect.X ,slider.rect.Y , slider.rect.Width ,slider.rect.Height * ((100 - slider.percent) / 100.0f) }, slider.notFilled,graphics);
//
//}


void UI::drawLine(UIVEC2 start, UIVEC2 end, Gdiplus::Color color, Gdiplus::Graphics& graphics) {
    /* creates a pen object to draw line on buffer*/
    Gdiplus::Pen pen(color);
    
    /* draws a line on buffer */
    graphics.DrawLine(&pen, start.x, start.y, end.x, end.y);

}

void UI::Init(const wchar_t* windowName, size_t initialX, size_t initialY, size_t width, size_t height) {
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    /* creates a window basic properties */
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszClassName = TEXT("CustomUI");

    RegisterClass(&wndClass);

    /* creates a window with user specifications */
    hWnd = CreateWindow(
        TEXT("CustomUI"),   // window class name
        windowName,  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        initialX,            // initial x position
        initialY,            // initial y position
        width,            // initial x size
        height,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        NULL,                // program instance handle
        NULL);                    // creation parameters

    /* sets the objects address in hwnd data to use in wndproc */
    SetWindowLong(hWnd, GWLP_USERDATA, (long)this);

    /* creates a buffer to draw on */
    hdcBuffer = CreateCompatibleDC(NULL);
    hBmBuffer = CreateCompatibleBitmap(GetDC(hWnd), width, height);

    /* shows the window */
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    /* 2nd buffer */
    SelectObject(hdcBuffer, hBmBuffer);

    /* save the width and height of window in object */
    _width = width;
    _height = height;


}

void UI::startDraw() {
    /* starts the buffer to allow drawing */
    hdc = BeginPaint(this->hWnd, &ps);

    /* paints over buffer to 'erase' what it had */
    FillRect(hdcBuffer, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));

}
void UI::stopDraw() {
    /* copys 2nd buffer to 1st buffer */
    BitBlt(hdc, 0, 0, _width, _height, hdcBuffer, 0, 0, SRCCOPY);
    
    /* stop the drawing */
    EndPaint(this->hWnd, &ps);
}
int UI::Update() {
    static MSG msg;

    /* force a redraw of screen */
    InvalidateRect(this->hWnd, 0, 0);
    
    /* handle messages */
    if (PeekMessage(&msg, this->hWnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT || msg.message == WM_DESTROY || msg.message == WM_CLOSE) {
            /* releases the buffers */
            DeleteDC(hdcBuffer);
            DeleteObject(hBmBuffer);
            Gdiplus::GdiplusShutdown(gdiplusToken);
            
            /* returns -1 meaning window closed */
            return -1;
        }

        /* returns message if user wants to deal with certain messages */
        return msg.message;
    }
}