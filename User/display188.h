#ifndef DISPLAY188_H
#define DISPLAY188_H
#include <stdbool.h>  // 包含布尔类型支持
#include <stdio.h>

// 定义结构体 display188_t
struct display188_t
{
    int digit_all;              // 保存所有位的数字
    int digit_100;              // 保存百位的数字
    int digit_10;               // 保存十位的数字
    int digit_1;                // 保存个位的数字
    void (*LED1_DRIVER)(bool);  // 指向控制LED1的函数指针
    void (*LED2_DRIVER)(bool);  // 指向控制LED2的函数指针
    void (*LED3_DRIVER)(bool);  // 指向控制LED3的函数指针
    void (*LED4_DRIVER)(bool);  // 指向控制LED4的函数指针
    void (*LED5_DRIVER)(bool);  // 指向控制LED5的函数指针
};
extern struct display188_t display188_var;

void display188_1(int digit1to9);
void display188_10(int digit1to9);
void display188_100(int digit1to9);

void display188_refresh_handle();

#endif