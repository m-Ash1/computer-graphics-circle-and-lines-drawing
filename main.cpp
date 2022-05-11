#include <Windows.h>
#include <cmath>

void swap(int& x1, int& y1, int& x2, int& y2)
{
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}
int Round(double x)
{
    return (int)(x + 0.5);
}

void Draw8Points(HDC hdc , int xc, int yc, int x, int y, COLORREF c){
    SetPixel(hdc, xc+x,yc+y,c);
    SetPixel(hdc, xc-x,yc+y,c);
    SetPixel(hdc, xc-x,yc-y,c);
    SetPixel(hdc, xc+x,yc-y,c);

    SetPixel(hdc, xc+y,yc+x,c);
    SetPixel(hdc, xc+y,yc-x,c);
    SetPixel(hdc, xc-y,yc-x,c);
    SetPixel(hdc, xc-y,yc+x,c);
}
void parametricline(HDC hdc,int x1,int y1,int x2,int y2){
    double dx=x2-x1;
    double dy=y2-y1;
    for(double t=0;t<1;t+=0.001){
        int x=x1+(dx*t);
        int y=y1+(dy*t);
        SetPixel(hdc,x,y,RGB(250,1,1));
    }
}

void Draw8Lines(HDC hdc , int xc, int yc, int x , int y, int r){
    parametricline(hdc , xc , yc ,xc+r , yc );
    parametricline(hdc , xc , yc ,xc-r , yc );
    parametricline(hdc , xc , yc ,xc, yc+r );
    parametricline(hdc , xc , yc ,xc, yc-r);

}


void DrawCircle2(HDC hdc, int xc, int yc, int R, COLORREF c){
    int x = 0;
    int y = R;
    Draw8Points(hdc , xc , yc, 0 , R , c);
    int d = 1-R;
    int d1 = 2 , d2 = 5-2*R;
    while(x<y){
        if (d<0){
            d+=d1;
            d1+=2;
            d2+=2;
        }else{
            d+=d2;
            d1+=2;
            d2+=4;
            y--;
        }
        x++;

        Draw8Points(hdc , xc, yc , x, y, c);
    }
    parametricline(hdc,xc+x,yc+y,xc-x,yc-y);
    parametricline(hdc,xc-x,yc+y,xc+x,yc-y);
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    hdc = GetDC(hwnd);
    static int x1 = 0 , y1 = 0 , x2 = 0 , y2 = 0;
    static  double r = 0;
    switch (mcode)
    {
        case WM_LBUTTONDOWN: {
            x1 = LOWORD(lp);
            y1 = HIWORD(lp);
            ReleaseDC(hwnd, hdc);
            break;
        }
        case WM_RBUTTONDOWN: {
            x2 = (int) LOWORD(lp);
            y2 = (int) HIWORD(lp);
            r =sqrt(pow(x2 - x1, 2) +
                          pow(y2 - y1, 2) * 1.0);
            DrawCircle2(hdc , x1 ,  y1 , Round(r) , RGB(200,0,0));
            Draw8Lines(hdc , x1 , y1 , x2 , y2 , Round(r));
            ReleaseDC(hwnd, hdc);
            break;
        }
        case WM_CLOSE: {
            DestroyWindow(hwnd);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
