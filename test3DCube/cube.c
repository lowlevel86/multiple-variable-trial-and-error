//compile with:
//tcc -mwindows -o cube.exe cube.c

//execute directly with:
//tcc -run cube.c

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "mvtrialanderror.h"

#define WIN_WIDTH 320 //window size
#define WIN_HEIGHT 320

#define TRUE 1
#define FALSE 0
#define ID_TIMER 1

int canvasWidth;
int canvasHeight;
int xCenter, yCenter;
BITMAPINFO pbmi[40];
char canvas[(WIN_WIDTH*3 + WIN_WIDTH%4) * WIN_HEIGHT];
float zBuff[WIN_WIDTH * WIN_HEIGHT];


inline double round(double val)
{    
   return floor(val + 0.5);
}


void clearCanvas()
{
   int i;
   int bytesWidth = canvasWidth * 3 + canvasWidth % 4;
   float cameraViewDistance = -120;
   
   for (i=0; i < bytesWidth * canvasHeight; i++)
   {
      canvas[i] = 0x0;
   }
   
   for (i=0; i < canvasWidth * canvasHeight; i++)
   {
      zBuff[i] = cameraViewDistance;
   }
}


void drawPt(float xPt, float yPt, float zPt, float rf, float gf, float bf)
{
   int x, y;
   char r, g, b;
   float perspctv = 200;
   float cameraLens = 120;//less than perspctv
   float cameraViewDistance = -120;
   int bytesWidth = canvasWidth * 3 + canvasWidth % 4;
   
   //change size relative to distance from camera
   x = round(xPt * perspctv / (perspctv - zPt) + xCenter);
   y = round(yPt * perspctv / (perspctv - zPt) + yCenter);
   
   //change brightness relative to distance from camera
   if (zPt >= 0)
   {
      r = rf*128 + zPt/cameraLens * (rf*127);
      g = gf*128 + zPt/cameraLens * (gf*127);
      b = bf*128 + zPt/cameraLens * (bf*127);
   }
   else
   {
      r = rf*128 - zPt/cameraViewDistance * (rf*127);
      g = gf*128 - zPt/cameraViewDistance * (gf*127);
      b = bf*128 - zPt/cameraViewDistance * (bf*127);
   }
   
   if (zPt <= cameraLens)
   if (zPt >= cameraViewDistance)
   if ((x >= 0) && (x < canvasWidth) && (y >= 0) && (y < canvasHeight))
   if (zPt >= zBuff[x+canvasWidth*y])
   {
      canvas[2+x*3+bytesWidth*y] = r;
      canvas[1+x*3+bytesWidth*y] = g;
      canvas[0+x*3+bytesWidth*y] = b;
      zBuff[x+canvasWidth*y] = zPt;
   }
}


void rot(float *horiP, float *vertP, float degrees)
{
   float h, v;
   float hUc;
   float vUc;
   float hLine1, vLine1;
   float hLine2, vLine2;
   
   if (degrees != degrees) // check if NaN
   return;
   
   hUc = cos(degrees * (M_PI * 2.0 / 360.0));
   vUc = sin(degrees * (M_PI * 2.0 / 360.0));
   
   hLine1 = hUc;
   vLine1 = vUc;
   hLine2 = -vUc;
   vLine2 = hUc;

   h = *vertP * hLine2 + *horiP * vLine2;
   v = *horiP * vLine1 + *vertP * hLine1;
   *horiP = h;
   *vertP = v;
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szAppName[] = TEXT("Use Arrow Keys To Change Test Points");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
   HWND hwnd;
   MSG msg;
   WNDCLASS wndclass;

   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = WndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = hInstance;
   wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = 0;
   wndclass.lpszMenuName = NULL;
   wndclass.lpszClassName = szAppName;

   RegisterClass(&wndclass);
   hwnd = CreateWindow(szAppName, szAppName,
                       WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
                       0, 0, WIN_WIDTH, WIN_HEIGHT,
                       NULL, NULL, hInstance, NULL);
   ShowWindow(hwnd, iCmdShow);
   UpdateWindow(hwnd);
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   unsigned int i;
   static HDC hdc;
   static PAINTSTRUCT ps;
   static char winTextBuff[256];
   
   static int layerSzSum[11];
   static double topMostArray[3] = {65, 65, 65};
   static double bottomMostArray[3] = {-65, -65, -65};
   static double valueArray[3];
   
   static unsigned int pointCntBgn = 0;
   static unsigned int underflow = 0;
   static float pointCntBgnIncDec = 1;
   static unsigned int pointCntSz = 729;
   
   static float rotateCube = 0;
   static int zDistance = 0;
   static float xPt;
   static float yPt;
   static float zPt;
   
   
   if (WM_CREATE == message)
   {
      pbmi->bmiHeader.biSize = 40;
      pbmi->bmiHeader.biWidth = WIN_WIDTH;
      pbmi->bmiHeader.biHeight = WIN_HEIGHT;
      pbmi->bmiHeader.biPlanes = 1;
      pbmi->bmiHeader.biBitCount = 24;
      pbmi->bmiHeader.biCompression = BI_RGB;
      pbmi->bmiHeader.biSizeImage = WIN_WIDTH * WIN_HEIGHT;
      pbmi->bmiHeader.biXPelsPerMeter = 0;
      pbmi->bmiHeader.biYPelsPerMeter = 0;
      pbmi->bmiHeader.biClrUsed = 0;
      pbmi->bmiHeader.biClrImportant = 0;
      
      SetTimer(hwnd, ID_TIMER, 50, NULL);
      
      layerSzSum[0] = sumCurrentAndPriorLayers(3, 0);
      layerSzSum[1] = sumCurrentAndPriorLayers(3, 1);
      layerSzSum[2] = sumCurrentAndPriorLayers(3, 2);
      layerSzSum[3] = sumCurrentAndPriorLayers(3, 3);
      layerSzSum[4] = sumCurrentAndPriorLayers(3, 4);
      layerSzSum[5] = sumCurrentAndPriorLayers(3, 5);
      layerSzSum[6] = sumCurrentAndPriorLayers(3, 6);
      layerSzSum[7] = sumCurrentAndPriorLayers(3, 7);
      layerSzSum[8] = sumCurrentAndPriorLayers(3, 8);
      layerSzSum[9] = sumCurrentAndPriorLayers(3, 9);
      layerSzSum[10] = sumCurrentAndPriorLayers(3, 10);
      
      return 0;
   }
   
   if (WM_SIZE == message)
   {
      canvasWidth = LOWORD(lParam);
      canvasHeight = HIWORD(lParam);
      xCenter = canvasWidth / 2;
      yCenter = canvasHeight / 2;
      
      pbmi->bmiHeader.biWidth = canvasWidth;
      pbmi->bmiHeader.biHeight = canvasHeight;
      pbmi->bmiHeader.biSizeImage = canvasWidth * canvasHeight;
      
      return 0;
   }

   if (WM_TIMER == message)
   {
      rotateCube += 1.0;
      
      clearCanvas();
      
      //draw generated values to screen as 3D point locations
      for (i=pointCntBgn+underflow; i < pointCntBgn + pointCntSz; i++)
      {
         generateRangedValues(&topMostArray[0], &bottomMostArray[0], 3, i, &valueArray[0]);
         
         xPt = valueArray[0];
         yPt = valueArray[1];
         zPt = valueArray[2];
         
         rot(&yPt, &zPt, rotateCube);
         rot(&xPt, &zPt, rotateCube);
         
         zPt += zDistance;
         
         if (i < layerSzSum[0])
         {
            drawPt(xPt, yPt, zPt, 1.0, 1.0, 1.0);
            drawPt(xPt+2, yPt, zPt, 1.0, 1.0, 1.0);
            drawPt(xPt-2, yPt, zPt, 1.0, 1.0, 1.0);
            drawPt(xPt, yPt+2, zPt, 1.0, 1.0, 1.0);
            drawPt(xPt, yPt-2, zPt, 1.0, 1.0, 1.0);
         }
         
         if ((i >= layerSzSum[0]) && (i < layerSzSum[1]))
         drawPt(xPt, yPt, zPt, 0.0, 1.0, 1.0);
         
         if ((i >= layerSzSum[1]) && (i < layerSzSum[2]))
         drawPt(xPt, yPt, zPt, 1.0, 1.0, 0.0);
         
         if ((i >= layerSzSum[2]) && (i < layerSzSum[3]))
         drawPt(xPt, yPt, zPt, 0.0, 0.0, 1.0);
         
         if ((i >= layerSzSum[3]) && (i < layerSzSum[4]))
         drawPt(xPt, yPt, zPt, 1.0, 0.7, 0.7);
         
         if ((i >= layerSzSum[4]) && (i < layerSzSum[5]))
         drawPt(xPt, yPt, zPt, 1.0, 0.0, 0.0);
         
         if ((i >= layerSzSum[5]) && (i < layerSzSum[6]))
         drawPt(xPt, yPt, zPt, 0.0, 0.5, 0.5);
         
         if ((i >= layerSzSum[6]) && (i < layerSzSum[7]))
         drawPt(xPt, yPt, zPt, 0.5, 0.5, 0.0);
         
         if ((i >= layerSzSum[7]) && (i < layerSzSum[8]))
         drawPt(xPt, yPt, zPt, 0.0, 0.5, 0.0);
         
         if ((i >= layerSzSum[8]) && (i < layerSzSum[9]))
         drawPt(xPt, yPt, zPt, 0.5, 0.0, 0.5);
         
         if ((i >= layerSzSum[9]) && (i < layerSzSum[10]))
         drawPt(xPt, yPt, zPt, 1.0, 0.0, 1.0);
         
         if (i >= layerSzSum[10])
         drawPt(xPt, yPt, zPt, 0.0, 1.0, 0.0);
      }  
      
      InvalidateRect(hwnd, NULL, TRUE);
      UpdateWindow(hwnd);
      
      hdc = GetDC(hwnd);
      SetDIBitsToDevice(hdc, 0, 0, canvasWidth, canvasHeight,
                        0, 0, 0, canvasHeight, canvas, pbmi, DIB_RGB_COLORS);
      ReleaseDC(hwnd, hdc);
      
      return 0;
   }
   
   if (WM_MOUSEWHEEL == message)
   {
      if (HIWORD(wParam) == 120)
      zDistance += 5;
      
      if (HIWORD(wParam) == 65416)
      zDistance -= 5;
      
      return 0;
   }
   
   if (WM_KEYDOWN == message)
   {
      //printf("keydown value %i\n", LOWORD(wParam));
      //fflush(stdout);
      
      if (LOWORD(wParam) == 37)//Left
      pointCntBgn -= pointCntBgnIncDec;
      
      if (LOWORD(wParam) == 39)//Right
      pointCntBgn += pointCntBgnIncDec;
      
      
      if (LOWORD(wParam) == 38)//Up
      pointCntSz += 1;
      
      if (LOWORD(wParam) == 40)//Down
      pointCntSz -= 1;
      
      if (!pointCntSz)
      pointCntSz = 1;
      
      
      //start point count at 0
      if (LOWORD(wParam) == 13)//Enter
      {
         pointCntBgnIncDec = 1;
         pointCntBgn = 0;
         rotateCube = 0;
      }
      
      //slow the point count increment speed if too fast
      if ((LOWORD(wParam) == 37) || (LOWORD(wParam) == 39))
      if (pointCntBgnIncDec < 0x00FFFFFF)
      pointCntBgnIncDec *= 1.1;
      
      //account underflow
      if (pointCntBgn > pointCntBgn + pointCntSz)
      underflow = 0xFFFFFFFF - pointCntBgn + 1;
      else
      underflow = 0;
      
      //display the starting and ending point count locations
      if (underflow)
      sprintf(winTextBuff, "Underflow %u - %u [Enter] Normalize", pointCntBgn+underflow, pointCntBgn + pointCntSz);
      else
      sprintf(winTextBuff, "%u - %u [Enter] Normalize", pointCntBgn, pointCntBgn + pointCntSz);
      SetWindowText(hwnd, TEXT(winTextBuff));
      
      
      if (LOWORD(wParam) == 27)//Esc
      {
         KillTimer(hwnd, ID_TIMER);
         PostQuitMessage(0);
      }
      
      return 0;
   }
   
   if (WM_KEYUP == message)
   {
      //restart the point count increment speed
      if ((LOWORD(wParam) == 37) || (LOWORD(wParam) == 39))
      pointCntBgnIncDec = 1;
      
      return 0;
   }
   
   if (WM_DESTROY == message)
   {
      KillTimer(hwnd, ID_TIMER);
      PostQuitMessage(0);
      return 0;
   }
   
   return DefWindowProc(hwnd, message, wParam, lParam);
}
