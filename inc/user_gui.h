#ifndef USER_GUI_HEADER_H
#define USER_GUI_HEADER_H

#include <stdint.h>

/**
 * @brief Screen Dimesion defines 
 * 
*/
#define ROWS 64 /** LCD pixel rows */
#define COLUMNS 102 /** LCD pixel columns */

/**
 * @brief The amount of defined font types 
 */
#define NUM_FONT_TYPES 2
#define MAX_FONT_SIZES 5

/**
 * @brief User defined fonts that can be used for the current gui
 * 
 */
typedef struct 
{
    char fontName[64];                       /** String that defines the name of font */
    uint8_t sizes[MAX_FONT_SIZES];           /** Array of avaliable font sizes for that font */
    uint8_t *p_sizeArray[MAX_FONT_SIZES];    /** Array of pointers to the size of each chacter */
    uint8_t *p_charBitmaps[MAX_FONT_SIZES];  /** Array of pointers to the bitmap of each character*/
} font_list_t;





#endif 