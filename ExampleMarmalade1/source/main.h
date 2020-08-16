#ifndef _MAIN_H_
#define _MAIN_H_

#define DEG2RAD(x) (((x) / 360.0f) * (2.0f * (float) M_PI))

extern void DrawLineRGB(int x1, int y1, int x2, int y2, int r, int g, int b);
extern void DrawLine(int x1, int y1, int x2, int y2, int16 color);

#endif //< ifndef _MAIN_H_
