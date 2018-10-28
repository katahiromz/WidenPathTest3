// WidenPathTest3.cpp --- Test for WidenPath
// Copyright (C) 2018 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include <windows.h>
#include <windowsx.h>

HWND g_hCmb1;
HWND g_hCmb2;
HWND g_hCmb3;
HWND g_hCmb4;
HWND g_hCmb5;

struct NAMEANDVALUE
{
    const char *name;
    DWORD value;
};
#define DEFINE_PAIR(data) { #data, data }

static const NAMEANDVALUE g_pen_styles[] =
{
    DEFINE_PAIR(PS_ALTERNATE),
    DEFINE_PAIR(PS_SOLID),      // default
    DEFINE_PAIR(PS_DASH),
    DEFINE_PAIR(PS_DOT),
    DEFINE_PAIR(PS_DASHDOT),
    DEFINE_PAIR(PS_DASHDOTDOT),
    DEFINE_PAIR(PS_NULL),
};

static const NAMEANDVALUE g_end_caps[] =
{
    DEFINE_PAIR(PS_ENDCAP_FLAT),
    DEFINE_PAIR(PS_ENDCAP_ROUND),   // default
    DEFINE_PAIR(PS_ENDCAP_SQUARE),
};

static const NAMEANDVALUE g_joins[] =
{
    DEFINE_PAIR(PS_JOIN_BEVEL),
    DEFINE_PAIR(PS_JOIN_MITER),
    DEFINE_PAIR(PS_JOIN_ROUND),     // default
};

static const NAMEANDVALUE g_fill_modes[] =
{
    DEFINE_PAIR(ALTERNATE),
    DEFINE_PAIR(WINDING),           // default
};

static const NAMEANDVALUE g_widths[] =
{
    DEFINE_PAIR(0),
    DEFINE_PAIR(1),
    DEFINE_PAIR(2),
    DEFINE_PAIR(4),
    DEFINE_PAIR(8),
    DEFINE_PAIR(16),     // default
    DEFINE_PAIR(32),
};

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    g_hCmb1 = GetDlgItem(hwnd, cmb1);
    g_hCmb2 = GetDlgItem(hwnd, cmb2);
    g_hCmb3 = GetDlgItem(hwnd, cmb3);
    g_hCmb4 = GetDlgItem(hwnd, cmb4);
    g_hCmb5 = GetDlgItem(hwnd, cmb5);

    for (SIZE_T i = 0; i < ARRAYSIZE(g_pen_styles); ++i)
    {
        ComboBox_AddString(g_hCmb1, g_pen_styles[i].name);
    }
    ComboBox_SetCurSel(g_hCmb1, 1);

    for (SIZE_T i = 0; i < ARRAYSIZE(g_end_caps); ++i)
    {
        ComboBox_AddString(g_hCmb2, g_end_caps[i].name);
    }
    ComboBox_SetCurSel(g_hCmb2, 1);

    for (SIZE_T i = 0; i < ARRAYSIZE(g_joins); ++i)
    {
        ComboBox_AddString(g_hCmb3, g_joins[i].name);
    }
    ComboBox_SetCurSel(g_hCmb3, 2);

    for (SIZE_T i = 0; i < ARRAYSIZE(g_fill_modes); ++i)
    {
        ComboBox_AddString(g_hCmb4, g_fill_modes[i].name);
    }
    ComboBox_SetCurSel(g_hCmb4, 1);

    for (SIZE_T i = 0; i < ARRAYSIZE(g_widths); ++i)
    {
        ComboBox_AddString(g_hCmb5, g_widths[i].name);
    }
    ComboBox_SetCurSel(g_hCmb5, 5);

    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(hwnd, id);
        break;
    case cmb1:
    case cmb2:
    case cmb3:
    case cmb4:
    case cmb5:
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
}

struct LINES
{
    INT count;
    POINT points[5];
};

#define ROWS 3
#define COLUMNS 4

static const LINES g_polylines[ROWS][COLUMNS] =
{
    {
        {
            2, { { 0, 0 }, { 1, 1 } },
        },
        {
            2, { { 1, 0 }, { 0, 1 } },
        },
        {
            2, { { 0, 0 }, { 0, 1 } },
        },
        {
            2, { { 0, 0 }, { 1, 0 } },
        },
    },
    {
        {
            4, { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } },
        },
        {
            4, { { 0, 1 }, { 0, 0 }, { 1, 0 }, { 1, 1 } },
        },
        {
            4, { { 1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } },
        },
        {
            4, { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } },
        },
    },
    {
        {
            4, { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
        },
        {
            4, { { 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 1 } },
        },
        {
            4, { { 0, 1 }, { 0, 0 }, { 1, 1 }, { 1, 0 } },
        },
        {
            4, { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        },
    },
};

#define GRID_SIZE  120

void DoDrawLines(HDC hdc, INT ix, INT iy)
{
    INT x = ix * GRID_SIZE + 40;
    INT y = iy * GRID_SIZE + 40;

    INT count = g_polylines[iy][ix].count;
    const POINT *points = g_polylines[iy][ix].points;
    if (count > 5)
    {
        MessageBoxA(NULL, "BAD DATA.", NULL, MB_ICONERROR);
        return;
    }

    POINT pts[5];
    for (int i = 0; i < count; ++i)
    {
        pts[i].x = x + points[i].x * GRID_SIZE * 2 / 3;
        pts[i].y = y + points[i].y * GRID_SIZE * 2 / 3;
    }

    Polyline(hdc, pts, count);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    if (HDC hdc = BeginPaint(hwnd, &ps))
    {
        INT i0 = ComboBox_GetCurSel(g_hCmb1);
        INT i1 = ComboBox_GetCurSel(g_hCmb2);
        INT i2 = ComboBox_GetCurSel(g_hCmb3);
        INT i3 = ComboBox_GetCurSel(g_hCmb4);
        INT i4 = ComboBox_GetCurSel(g_hCmb5);

        LOGBRUSH lb;
        GetObject(GetStockObject(BLACK_BRUSH), sizeof(lb), &lb);
        lb.lbColor = RGB(0, 0, 0);

        DWORD dwStyle = PS_GEOMETRIC |
                        g_pen_styles[i0].value |
                        g_end_caps[i1].value |
                        g_joins[i2].value;

        if (HPEN hPen = ExtCreatePen(dwStyle, g_widths[i4].value, &lb, 0, NULL))
        {
            HGDIOBJ hPenOld = SelectObject(hdc, hPen);
            if (HBRUSH hbr = CreateSolidBrush(RGB(0, 0, 0)))
            {
                HGDIOBJ hbrOld = SelectObject(hdc, hbr);
                for (int iy = 0; iy < ROWS; ++iy)
                {
                    for (int ix = 0; ix < COLUMNS; ++ix)
                    {
                        SetPolyFillMode(hdc, g_fill_modes[i3].value);
                        BeginPath(hdc);
                        {
                            DoDrawLines(hdc, ix, iy);
                        }
                        EndPath(hdc);

                        WidenPath(hdc);

                        FillPath(hdc);
                    }
                }
                SelectObject(hdc, hbrOld);
                DeleteObject(hbr);
            }
            SelectObject(hdc, hPenOld);
            DeleteObject(hPen);
        }
        EndPaint(hwnd, &ps);
    }
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
    }
    return 0;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);
    return 0;
}
