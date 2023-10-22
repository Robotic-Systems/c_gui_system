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
#define HASH_MAX_VARS 20         /** Maximum amount of variables that can be stored in hashmap */
#define MAX_PAGE_COUNT 50        /** Maximum number of supported pages*/
#define MAX_TAG_DATA_LENGTH 64   /** Maximum length of data strings that is not displayed on gui */
#define MAX_KEY_LENGTH 20        /** Maximum length key strings  */
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
    GUI_OK,              /** The function executed succesfuly */
    GUI_ERR,             /** The function failed */
    GUI_INIT_PGE_BRACE,  /** There was a mismatch in page start or end braces */
    GUI_INIT_VAR_BRACE,  /** There was a mismatch in variable start or end braces */
}gui_status_t;


/**
 * @brief For describing execution status of variable operations
 */
typedef enum{
    GUI_VAR_OK,    /** The variable operation executed successfuly*/
    GUI_VAR_ERR,   /** The variable operation failed */
}gui_variable_status_t;

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
 * @brief Parses the "guiXml" for pages and variables, saves the pages indexs and also creates the variables hasmap 
 * this is also where high level xml formatting check will take place.
 * @note The start of a page is defined as "<page>" the index where the ">" occurs and end is </page> where the "<" occurs 
 * 
 * @return gui_status_t 
 */
gui_status_t gui_parse_xml();

/**
 * @brief Returns page information 
 * 
 * @param [in] pageNumber - page number to query 
 * @param [out] p_startIndex - start index of the page in the gui xml 
 * @param [out] p_endIndex   - end index of the page in the gui xml 
 * @return gui_status_t 
 */
gui_status_t gui_get_page_position(int16_t pageNumber, uint32_t * p_startIndex , uint32_t * p_endIndex);

/**
 * @brief Takes in string inputs of a variable name, type and value and create a hasmap instance 
 * of those details 
 * 
 * @param [in] variableName     - Desied variable name string 
 * @param [in] variableType     - Desied variable type string, if inputted type is not supported will return error  
 * @param [in] variableValue    - Desied variable value string  
 * @return gui_variable_status_t - creation status code 
 */
gui_variable_status_t gui_create_var(const char *variableName,const char *variableType,const char *variableValue);

/**
 * @brief Querys the uint16_t hashmap for the variableKey and returns its value
 * 
 * @param [in] variableKey - Name associated with the variable as defined in the xml 
 * @param [out] p_value - Variable value  
 * @return gui_variable_status_t 
 */
gui_variable_status_t gui_get_uint16_var(const char *variableKey,uint16_t *p_value);

/**
 * @brief Can update uint16_t variables from there variable key 
 * 
 * @param [in] variableKey - Key of variable to be updated 
 * @param [in] value - Value to update variable to 
 * @return gui_variable_status_t 
 */
gui_variable_status_t gui_update_uint16_var(const char *variableKey,uint16_t value);

/**
 * @brief Updates the gui, should be placed in a callback function and called periodically 
 * 
 * @return gui_status_t 
 */
gui_status_t gui_update();
#endif