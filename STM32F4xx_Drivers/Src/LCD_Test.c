/*
 * LCD_Test.c
 *
 *  Created on: Mar 28, 2026
 *      Author: Lenovo
 */
#include <stdio.h>
#include "ds1307.h"
#include "lcd.h"

int main(){

    LCD_Init();

    LCD_PrintString("RTC Test...");

	return 0;
}
