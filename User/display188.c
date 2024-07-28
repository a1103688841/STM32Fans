#include "display188.h"


struct display188_t display188_var;
void display188_refresh_handle()
{
    enum STATE_E
    {
        STATE_NONE,
        STATE_1,
        STATE_10,
        STATE_100,
    };
    static int state         = STATE_NONE;
    display188_var.digit_1   = display188_var.digit_all % 10;
    display188_var.digit_10  = display188_var.digit_all / 10 % 10;
    display188_var.digit_100 = display188_var.digit_all / 100 % 10;
    switch (state)
    {
    case STATE_NONE:
        state = STATE_1;
        break;
    case STATE_1:
        display188_1(display188_var.digit_1);
        state = STATE_10;
        break;
    case STATE_10:
        display188_10(display188_var.digit_10);
        state = STATE_100;
        break;
    case STATE_100:
        display188_10(display188_var.digit_100);
        state = STATE_1;
        break;
    }
}
static inline void B1(bool x)
{
    if (x)
    {
        display188_var.LED3_DRIVER(1);
        display188_var.LED4_DRIVER(0);
    }
    else
    {
        display188_var.LED3_DRIVER(0);
        display188_var.LED4_DRIVER(0);
    }
}
static inline void C1(bool x)
{
    if (x)
    {
        display188_var.LED2_DRIVER(1);
        display188_var.LED4_DRIVER(0);
    }
    else
    {
        display188_var.LED2_DRIVER(0);
        display188_var.LED4_DRIVER(0);
    }
}
static inline void A2(bool x)
{
    if (x)
    {
        display188_var.LED2_DRIVER(1);
        display188_var.LED3_DRIVER(0);
    }
    else
    {
        display188_var.LED2_DRIVER(0);
        display188_var.LED3_DRIVER(0);
    }
}
static inline void B2(bool x)
{
    if (x)
    {
        display188_var.LED3_DRIVER(1);
        display188_var.LED2_DRIVER(0);
    }
    else
    {
        display188_var.LED3_DRIVER(0);
        display188_var.LED2_DRIVER(0);
    }
}

static inline void C2(bool x)
{
    if (x)
    {
        display188_var.LED4_DRIVER(1);
        display188_var.LED3_DRIVER(0);
    }
    else
    {
        display188_var.LED4_DRIVER(0);
        display188_var.LED3_DRIVER(0);
    }
}

static inline void D2(bool x)
{
    if (x)
    {
        display188_var.LED4_DRIVER(1);
        display188_var.LED2_DRIVER(0);
    }
    else
    {
        display188_var.LED4_DRIVER(0);
        display188_var.LED2_DRIVER(0);
    }
}
static inline void E2(bool x)
{
    if (x)
    {
        display188_var.LED5_DRIVER(1);
        display188_var.LED2_DRIVER(0);
    }
    else
    {
        display188_var.LED5_DRIVER(0);
        display188_var.LED2_DRIVER(0);
    }
}

static inline void F2(bool x)
{
    if (x)
    {
        display188_var.LED5_DRIVER(1);
        display188_var.LED3_DRIVER(0);
    }
    else
    {
        display188_var.LED5_DRIVER(0);
        display188_var.LED3_DRIVER(0);
    }
}
static inline void G2(bool x)
{
    if (x)
    {
        display188_var.LED5_DRIVER(1);
        display188_var.LED4_DRIVER(0);
    }
    else
    {
        display188_var.LED5_DRIVER(0);
        display188_var.LED4_DRIVER(0);
    }
}
static inline void A3(bool x)
{
    if (x)
    {
        display188_var.LED1_DRIVER(1);
        display188_var.LED2_DRIVER(0);
    }
    else
    {
        display188_var.LED1_DRIVER(0);
        display188_var.LED2_DRIVER(0);
    }
}
static inline void B3(bool x)
{
    if (x)
    {
        display188_var.LED2_DRIVER(1);
        display188_var.LED1_DRIVER(0);
    }
    else
    {
        display188_var.LED2_DRIVER(0);
        display188_var.LED1_DRIVER(0);
    }
}

static inline void C3(bool x)
{
    if (x)
    {
        display188_var.LED1_DRIVER(1);
        display188_var.LED3_DRIVER(0);
    }
    else
    {
        display188_var.LED1_DRIVER(0);
        display188_var.LED3_DRIVER(0);
    }
}

static inline void D3(bool x)
{
    if (x)
    {
        display188_var.LED3_DRIVER(1);
        display188_var.LED1_DRIVER(0);
    }
    else
    {
        display188_var.LED3_DRIVER(0);
        display188_var.LED1_DRIVER(0);
    }
}
static inline void E3(bool x)
{
    if (x)
    {
        display188_var.LED1_DRIVER(1);
        display188_var.LED4_DRIVER(0);
    }
    else
    {
        display188_var.LED1_DRIVER(0);
        display188_var.LED4_DRIVER(0);
    }
}

static inline void F3(bool x)
{
    if (x)
    {
        display188_var.LED4_DRIVER(1);
        display188_var.LED1_DRIVER(0);
    }
    else
    {
        display188_var.LED4_DRIVER(0);
        display188_var.LED1_DRIVER(0);
    }
}
static inline void G3(bool x)
{
    if (x)
    {
        display188_var.LED5_DRIVER(1);
        display188_var.LED1_DRIVER(0);
    }
    else
    {
        display188_var.LED5_DRIVER(0);
        display188_var.LED1_DRIVER(0);
    }
}
void display188_1(int digit1to9)
{
    switch (digit1to9)
    {
    case 9:
        A3(1);
        F3(1);
        G3(1);
        B3(1);
        C3(1);
        D3(1);
        E3(0);        break;
    case 8:
        A3(1);
        F3(1);
        G3(1);
        C3(1);
        D3(1);
        E3(1);
        B3(1);
        break;
    case 7:
        A3(1);
        B3(1);
        C3(1);
        F3(0);
        G3(0);
        E3(0);
        D3(0);
        break;
    case 6:
        A3(1);
        F3(1);
        E3(1);
        D3(1);
        C3(1);
        G3(1);
        B3(0);
        break;
    case 5:
        A3(1);
        F3(1);
        G3(1);
        C3(1);
        D3(1);
        E3(0);
        B3(0);
        break;
    case 4:
        F3(1);
        G3(1);
        B3(1);
        C3(1);
        A3(0);
        E3(0);
        D3(0);
        break;
    case 3:
        A3(1);
        B3(1);
        G3(1);
        C3(1);
        D3(1);
        F3(0);
        E3(0);
        break;
    case 2:
        A3(1);
        B3(1);
        G3(1);
        E3(1);
        D3(1);
        C3(0);
        F3(0);
        break;
    case 1:
        B3(1);
        C3(1);
        A3(0);
        G3(0);
        D3(0);
        F3(0);
        E3(0);
        break;
    case 0:
        A3(1);
        F3(1);
        E3(1);
        D3(1);
        C3(1);
        B3(1);
        G3(0);
        break;
    default:
        A3(0);
        F3(0);
        E3(0);
        D3(0);
        C3(0);
        B3(0);
        G3(0);
        break;
    }
}
void display188_10(int digit1to9)
{
    switch (digit1to9)
    {
    case 9:
        A2(1);
        F2(1);
        G2(1);
        B2(1);
        C2(1);
        D2(1);
        E2(0);        break;
    case 8:
        A2(1);
        F2(1);
        G2(1);
        C2(1);
        D2(1);
        E2(1);
        B2(1);
        break;
    case 7:
        A2(1);
        B2(1);
        C2(1);
        F2(0);
        G2(0);
        E2(0);
        D2(0);
        break;
    case 6:
        A2(1);
        F2(1);
        E2(1);
        D2(1);
        C2(1);
        G2(1);
        B2(0);
        break;
    case 5:
        A2(1);
        F2(1);
        G2(1);
        C2(1);
        D2(1);
        E2(0);
        B2(0);
        break;
    case 4:
        F2(1);
        G2(1);
        B2(1);
        C2(1);
        A2(0);
        E2(0);
        D2(0);
        break;
    case 2:
        A2(1);
        B2(1);
        G2(1);
        C2(1);
        D2(1);
        F2(0);
        E2(0);
        break;
    case 1:
        B2(1);
        C2(1);
        A2(0);
        G2(0);
        D2(0);
        F2(0);
        E2(0);
        break;
    case 0:
        A2(1);
        F2(1);
        E2(1);
        D2(1);
        C2(1);
        B2(1);
        G2(0);
        break;
    default:
        A2(0);
        F2(0);
        E2(0);
        D2(0);
        C2(0);
        B2(0);
        G2(0);
        break;
    }
}
void display188_100(int digit1to9)
{
    if(digit1to9 == 1)
    {
        B1(1);
        C1(1);
    }
    else{
                B1(0);
        C1(0);
    }
}
