#ifndef LCD_SPY_H
#define LCD_SPY_H

#include <stdint.h>
#define COLUMNS 102
#define ROWS    64
#define LCD_UNDEFINED 99
/**
 * @brief Initalised the LCD spy modle 
 * 
 */
void lcd_spy_init();

/**
 * @brief returns the last frame written to the spy
 * @param [in] pointer to array where which to store the current frame 
 */
void lcd_spy_get_Frame(uint8_t frameOut[ROWS][COLUMNS]);
#endif

/**
 * @brief function that the gui code will used to write to the 'lcd'
 * @param [in] bitMap - the bitmap to be written to the lcd 
 * @param [in] colums - number of columns in bitmap 
 * @param [in] rows   - number of rows in bitmap 
 */
void lcd_spy_write(uint8_t bitMap[ROWS][COLUMNS], uint16_t columns, uint16_t rows);
