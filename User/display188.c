#include "display188.h"


display188_t display188_var;

static void display188_analyse_handle()
{
    switch (display188_var.digit_1)
    {
        case 0:
            display188_var.digit_1_seg = 0XE888;
        break;
    } 
    switch(display188_var.digit_10)
    {
        case 0:
            display188_var.digit_10_seg = 0X0770;
        break;
    }
    switch(display188_var.digit_100)
    {
        case 0:
            display188_var.digit_100_seg = 0X0000;
        break;
    }
}

void display188_refresh_handle()
{
    enum STATE_E
    {
        STATE_NONE,
        STATE_P1,
        STATE_P2,
        STATE_P3,
        STATE_P4,
    };
    static int state         = STATE_NONE;
    display188_var.digit_1   = display188_var.digit_all % 10;
    display188_var.digit_10  = display188_var.digit_all / 10 % 10;
    display188_var.digit_100 = display188_var.digit_all / 100 % 10;
switch (display188_var.digit_1)
{
    case 0:
        display188_var.digit_1_seg = 0xE888;
        break;
    case 1:
        display188_var.digit_1_seg = 0x8080;
        break;
    case 2:
        display188_var.digit_1_seg = 0xD808;
        break;
    case 3:
        display188_var.digit_1_seg = 0xD880;
        break;
    case 4:
        display188_var.digit_1_seg = 0xB080;
        break;
    case 5:
        display188_var.digit_1_seg = 0x7880;
        break;
    case 6:
        display188_var.digit_1_seg = 0x7888;
        break;
    case 7:
        display188_var.digit_1_seg = 0x8880;
        break;
    case 8:
        display188_var.digit_1_seg = 0xF888;
        break;
    case 9:
        display188_var.digit_1_seg = 0xF880;
        break;
}

switch (display188_var.digit_10)
{
    case 0:
        display188_var.digit_10_seg = 0x0770;
        break;
    case 1:
        display188_var.digit_10_seg = 0x0420;
        break;
    case 2:
        display188_var.digit_10_seg = 0x0741;
        break;
    case 3:
        display188_var.digit_10_seg = 0x0661;
        break;
    case 4:
        display188_var.digit_10_seg = 0x0431;
        break;
    case 5:
        display188_var.digit_10_seg = 0x0271;
        break;
    case 6:
        display188_var.digit_10_seg = 0x0371;
        break;
    case 7:
        display188_var.digit_10_seg = 0x0460;
        break;
    case 8:
        display188_var.digit_10_seg = 0x0771;
        break;
    case 9:
        display188_var.digit_10_seg = 0x0671;
        break;
}

switch (display188_var.digit_100)
{
    case 0:
        display188_var.digit_100_seg = 0x0000;
        break;
    case 1:
        display188_var.digit_100_seg = 0x0006;
        break;
}

    switch (state)
    {
    case STATE_NONE:
        state = STATE_P1;
        break;
    case STATE_P1:
        {
            display188_var.LED12345_INPUT();
            display188_var.LED1_DRIVER(0);
            int p1_seg = 0x0000;
            p1_seg = display188_var.digit_1_seg & 0XF000;
            if(p1_seg & 0X8000)
            {
                display188_var.LED2_DRIVER(1);
            }
            if(p1_seg & 0X4000)
            {
                display188_var.LED3_DRIVER(1);
            }
            if(p1_seg & 0X2000)
            {
                display188_var.LED4_DRIVER(1);
            }
            if(p1_seg & 0X1000)
            {
                display188_var.LED5_DRIVER(1);
            }
        }
        state = STATE_P2;
        break;
    case STATE_P2:
        {
            display188_var.LED12345_INPUT();
            display188_var.LED2_DRIVER(0);
            int p2_seg = 0x0000;
            int p2_seg1 = 0x0000;
            int p2_seg2 = 0x0000;
            p2_seg1 = display188_var.digit_1_seg & 0X0F00;
            p2_seg2 = (display188_var.digit_10_seg & 0X0F00);
            p2_seg = p2_seg1 | p2_seg2;
            if(p2_seg & 0X0800)
            {
                display188_var.LED1_DRIVER(1);
            }
            if(p2_seg & 0X0400)
            {
                display188_var.LED3_DRIVER(1);
            }
            if(p2_seg & 0X0200)
            {
                display188_var.LED4_DRIVER(1);
            } 
            if(p2_seg & 0X0100)
            {
                display188_var.LED5_DRIVER(1);
            }   
        }
        state = STATE_P3;
        break;
    case STATE_P3:
        {display188_var.LED12345_INPUT();
            display188_var.LED3_DRIVER(0);
            int p3_seg = 0x0000;
            int p3_seg1 = 0x0000;
            int p3_seg2 = 0x0000;
            p3_seg1 = display188_var.digit_1_seg & 0X00F0;
            p3_seg2 = (display188_var.digit_10_seg & 0X00F0);
            p3_seg = p3_seg1 | p3_seg2;
            if(p3_seg & 0X0080)
            {
                display188_var.LED1_DRIVER(1);
            }
            if(p3_seg & 0X0040)
            {
                display188_var.LED2_DRIVER(1);
            }  
            if(p3_seg & 0X0020)
            {
                display188_var.LED4_DRIVER(1);
            } 
            if(p3_seg & 0X0010)
            {
                display188_var.LED5_DRIVER(1);
            }  
        }
        state = STATE_P4;
        break;
    case STATE_P4:
        {display188_var.LED12345_INPUT();
            display188_var.LED4_DRIVER(0);
            int p4_seg = 0x0000;
            int p4_seg1 = 0x0000;
            int p4_seg2 = 0x0000;
            int p4_seg3 = 0x0000;
            p4_seg1 = display188_var.digit_1_seg & 0X000F;
            p4_seg2 = (display188_var.digit_10_seg & 0X000F);
            p4_seg3 = (display188_var.digit_100_seg & 0X000F);
            p4_seg = p4_seg1 | p4_seg2 | p4_seg3;
            if(p4_seg & 0X0008)
            {
                display188_var.LED1_DRIVER(1);
            }
            if(p4_seg & 0X0004)
            {
                display188_var.LED2_DRIVER(1);
            }
            if(p4_seg & 0X0002)
            {
                display188_var.LED3_DRIVER(1);
            } 
            if(p4_seg & 0X0001)
            {
                display188_var.LED5_DRIVER(1);
            }
        }
        state = STATE_P1;
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
