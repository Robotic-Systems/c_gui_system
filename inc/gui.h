#ifndef GUI_HEADER_H
#define GUI_HEADER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
/***********************/
/* PUBLIC DEFINITIONS */
/***********************/

/**
 * @brief Maximum defines 
 * 
*/
#define MAX_PAGE_COUNT 50 /** Maximum number of supported pages*/
/**
 * @brief User defines that will be moved to a user code file in near future 
 * 
*/
#define ROWS 64 /** LCD pixel rows */
#define COLUMNS 102 /** LCD pixel columns */
/****************/
/* PUBLIC TYPES */
/****************/

/**
 * @brief Function Pointer type for the lcd write function 
*/
typedef void (*write_function)(uint8_t bitMap[][ROWS], uint16_t columns, uint16_t rows);


/**
 * @brief For describing execution status of function.
 */
typedef enum{
    GUI_OK,   /** The function executed succesfuly */
    GUI_ERR   /** The function failed */
}gui_status_t;

/**
 * @brief For storing page parameters 
 * 
 */
typedef struct
{
    uint32_t startIndex;   /** The start index of the page */
    uint32_t endIndex;     /** The end index of the page */
}page_params_t;

/********************/
/* PUBLIC VARIABLES */
/********************/

/********************************/
/* PUBLIC FUNCTION DECLARATIONS */
/********************************/

/**
 * @brief Init function for the gui system, must be called before other gui functions used
 * 
 * @param [in] p_lcdWrite - pointer to the lcd write function
 * @param [in] xmlString - xml string where the gui system has been defined  
 * @return gui_status_t - init execution status 
 */
gui_status_t gui_init(write_function p_lcdWrite, const char* xmlString);

/**
 * @brief Returns the number of pages in the current gui system 
 * 
 * @return int16_t - number of pages
 */
int16_t gui_get_page_count();

/**
 * @brief Returns the number of variables in the current gui system 
 * 
 * @return int16_t - Number of variables 
 */
int16_t gui_get_variable_count();


/**
 * @brief Parses the "guiXml" for pages, then saves the pages start and end index to a private variable,
 * @note The start of a page is defined as "<page>" the index where the ">" occurs and end is </page> where the "<" occurs 
 * 
 * @return gui_status_t 
 */
gui_status_t gui_parse_for_pages();

/**
 * @brief Returns page information 
 * 
 * @param [in] pageNumber - page number to query 
 * @param [out] p_startIndex - start index of the page in the gui xml 
 * @param [out] p_endIndex   - end index of the page in the gui xml 
 * @return gui_status_t 
 */
gui_status_t gui_get_page_position(int16_t pageNumber, uint32_t * p_startIndex , uint32_t * p_endIndex);
#endif