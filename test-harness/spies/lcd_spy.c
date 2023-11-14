#include "lcd_spy.h"        



uint8_t lcdFrame[ROWS][COLUMNS] = {0};

void lcd_spy_init()
{
    for (int ith_col = 0; ith_col < COLUMNS; ith_col++) {
        for (int ith_row = 0; ith_row < ROWS; ith_row++) {
            lcdFrame[ith_row][ith_col] = LCD_UNDEFINED;
        }
    }
    return;
}

void lcd_spy_get_Frame(uint8_t frameOut[ROWS][COLUMNS])
{
    for (int ith_col = 0; ith_col < COLUMNS; ith_col++) {
        for (int ith_row = 0; ith_row < ROWS; ith_row++) {
            frameOut[ith_row][ith_col]= lcdFrame[ith_row][ith_col];
        }
    }
    return;
}

void lcd_spy_write(uint8_t bitMap[ROWS][COLUMNS], uint16_t columns, uint16_t rows)
{
    for (int ith_col = 0; ith_col < columns; ith_col++) {
        for (int ith_row = 0; ith_row < rows; ith_row++) {
            lcdFrame[ith_row][ith_col] = bitMap[ith_row][ith_col];
        }
    }
    return;
}