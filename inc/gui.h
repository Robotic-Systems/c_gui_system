#ifndef GUI_HEADER_H
#define GUI_HEADER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "user_gui.h"
/***********************/
/* PUBLIC DEFINITIONS */
/***********************/
/**
 * @brief Maximum defines 
 * 
*/
#define HASH_MAX_VARS 20         /** Maximum amount of variables that can be stored of each type hashmap */
#define MAX_PAGE_COUNT 50        /** Maximum number of supported pages*/
#define MAX_TAG_DATA_LENGTH 64   /** Maximum length of data strings that is not displayed on gui */
#define MAX_KEY_LENGTH 20        /** Maximum length key strings  */
#define TEXT_ALIGNMENT_OPTIONS 3        /** Number of text alignment options */
#define MAX_TAG_NAME_LENGTH 64   /** Maximum supported length of a tag*/
/****************/
/* PUBLIC TYPES */
/****************/
/**
 * @brief Function Pointer type for the lcd write function 
*/
typedef void (*write_function)(uint8_t bitMap[ROWS][COLUMNS], uint16_t columns, uint16_t rows);


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

/**
 * @brief Text horizontal alignment enums 
 * 
 */
typedef enum {
    CENTERED,     /** Place text's center on the position*/
    LEFT,         /** Place text's left side on the position*/
    RIGHT,        /** Place text's right side on the position*/
}text_alignment_enum_t;
/**
 * @brief Text verticle alignment enums 
 * 
 */
typedef enum {
    CENTER ,           /** Place text's center  on the position*/
    TOP    ,           /** Place text's top  on the position*/
    BOTTOM ,          /** Place text's bottom on the position*/
}text_vert_alignment_enum_t;

/**
 * @brief Struct to create the text alignment dictionary 
 * 
 */
typedef struct {
    text_alignment_enum_t alignmentEnum; /** Enum accocaited with Alignment option*/
    const char *alignmentName;           /** String accocaited with Alignment option*/
} text_alignment_opt_t;
/**
 * @brief Struct to create the vert text alignment dictionary 
 * 
 */
typedef struct {
    text_vert_alignment_enum_t alignmentEnum; /** Enum accocaited with Alignment option*/
    const char *alignmentName;                /** String accocaited with Alignment option*/
} text_vert_alignment_opt_t;

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
 * @brief Querys the int32_t hashmap for the variableKey and returns its value
 * 
 * @param [in] variableKey - Name associated with the variable as defined in the xml 
 * @param [out] p_value - Variable value  
 * @return gui_variable_status_t 
 */
gui_variable_status_t gui_get_int32_var(const char *variableKey,int32_t *p_value);

/**
 * @brief Can update int32_t variables from there variable key 
 * 
 * @param [in] variableKey - Key of variable to be updated 
 * @param [in] value - Value to update variable to 
 * @return gui_variable_status_t 
 */
gui_variable_status_t gui_update_int32_var(const char *variableKey,int32_t value);

/**
 * @brief Take a text string that contains a bitMap and renders that bitmap onto the passed in
 * bitmap.
 * @note The bitmap settings MUST be placed before the data in XML otherwise error will be thrown 
 * @param [out] bitMap      - The output bit map 
 * @param [in] bitmapString - String that contains the bitmap in xml format 
 * @return gui_status_t - execution status  
 */
gui_status_t gui_render_bitmap(uint8_t bitMap[ROWS][COLUMNS],const char *bitmapString);


/**
 * @brief Takes text sting that contains a "<text>" tag, and renders that text onto screen 
 * 
 * @param [out] bitMap     - The output bit map 
 * @param textObjectString - String that contains the Text in xml format  
 * @return gui_status_t 
 */
gui_status_t gui_render_text(uint8_t bitMap[ROWS][COLUMNS],const char *textObjectString);

/**
 * @brief Returns the width in pixels of the input character for that specific font and size 
 * 
 * @param [in] fontNameIdx  - index of the fonts name in the font_master_list
 * @param [in] fontSizeIdx  - index of the fonts size in the font_master_list
 * @param [in] character    - Character for which to query the width  
 * @return character width in pixels, will return 0 if request fails 
 */
uint8_t gui_get_char_width(uint8_t fontNameIdx ,uint8_t fontSizeIdx, char character);

/**
 * @brief Takes inputs and writes chaacter to the bitmap 
 * 
 * @param [in] fontNameIdx - index of the fonts name in the font_master_list
 * @param [in] fontSizeIdx - index of the fonts size in the font_master_list 
 * @param [in] row         - Row to start writing the chacter bitmap on 
 * @param [in] col         - Column to start writng chacter bitmap on 
 * @param [in] b_bitMapInvert - Boolean that can be used to invert the chacter bitmap
 * @param [out] bitMap     - the output bitmap 
 * @param [in] character   - Chacter to be written to bitmap 
 * @return gui_status_t - execution status 
 */
gui_status_t gui_write_char(uint8_t fontNameIdx, uint8_t fontSizeIdx, int16_t row, int16_t col, uint8_t bitMap[ROWS][COLUMNS], char character,bool b_bitMapInvert);

/**
 * @brief When called reads the pageIndex variable and updates the gui, this is the only function that will 
 * call the write_function defined when calling the init function.
 * 
 * @return gui_status_t - execution status of the update    
 */
gui_status_t gui_update();

/**
 * @brief Takes in string that contains <operand> tag and performs the scripted operation 
 * 
 * @param operandObjectString - String that contains the operand 
 * @return gui_status_t - operand execution status 
 */
gui_status_t gui_execute_operand(const char *operandObjectString);

/**
 * @brief Parses the input string looking for the start and end tags specified by tag name, the resultant string found between them
 * if a val tag is found then its corresponding value will be extracted and added to that position in the string. If there is any 
 * error finding the value then an error will be returned
 * 
 * 
 * @param [in] tagString - The string that could contain the tag that you need to find  
 * @param [in] tagName - the name of the tag that is being parsed for, i.e if you want to parse for a "<content>" tag you should pass in "content"
 * @param [out] result - The resultant extracted sting 
 * @param [out] b_isFound - Succsess boolean that returns true if the tag was found 
 * @return gui_status_t - Tag parsing execution status 
 */
gui_status_t gui_parse_tag_str(const char *tagString,const char *tagName, char result[MAX_TAG_DATA_LENGTH], bool *b_isFound);

/**
 * @brief Parses the input string looking for the start and end tags specified by tag name, the resultant uint16 value found between them
 * if a val tag is found then its corresponding value will be extracted and returned. If there is any 
 * error finding the value then an error will be returned
 * 
 * @param [in] tagString  - The string that could contain the tag that you need to find  
 * @param [in] tagName  - the name of the tag that is being parsed for, i.e if you want to parse for a "<content>" tag you should pass in "content"
 * @param [out] p_value  - The resultant extracted value 
 * @param [out] numReturn  - Number of values to return, if less then number of return found error is caused
 * @param [out] b_isFound  - Succsess boolean that returns true if the tag was found 
 * @return gui_status_t  - Tag parsing execution status 
 */
gui_status_t gui_parse_tag_val(const char *tagString,const char *tagName, int32_t *p_value,uint8_t numReturn, bool *b_isFound);

#endif