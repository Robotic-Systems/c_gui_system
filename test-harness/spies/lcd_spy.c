#include "lcd_spy.h"        



uint8_t lcdFrame[COLUMNS][ROWS] = {0};

void lcd_spy_init()
{
    for (int ith_col = 0; ith_col < COLUMNS; ith_col++) {
        for (int ith_row = 0; ith_row < ROWS; ith_row++) {
            lcdFrame[ith_col][ith_row] = LCD_UNDEFINED;
        }
    }
    return;
}

void lcd_spy_get_Frame(uint8_t frameOut[][ROWS])
{
    for (int ith_col = 0; ith_col < COLUMNS; ith_col++) {
        for (int ith_row = 0; ith_row < ROWS; ith_row++) {
            frameOut[ith_col][ith_row] = lcdFrame[ith_col][ith_row];
        }
    }
    return;
}

void lcd_spy_write(uint8_t bitMap[][ROWS], uint16_t columns, uint16_t rows)
{
    for (int ith_col = 0; ith_col < columns; ith_col++) {
        for (int ith_row = 0; ith_row < rows; ith_row++) {
            lcdFrame[ith_col][ith_row] = bitMap[ith_col][ith_row];
        }
    }
    return;
}