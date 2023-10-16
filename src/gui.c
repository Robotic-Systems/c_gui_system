#include "gui.h"

/***********************/
/* PRIVATE DEFINITIONS */
/***********************/

/*****************/
/* PRIVATE TYPES */
/*****************/

/*********************/
/* PRIVATE VARIABLES */
/*********************/
int16_t pageCount = 0;     /** Number of pages found in xml after parsing*/
const char* guiXml = NULL; /** XML string that contains the whole menu system*/
page_params_t pages[MAX_PAGE_COUNT] = {0};
/*********************************/
/* PRIVATE FUNCTION DECLARATIONS */
/*********************************/

/********************************/
/* PRIVATE FUNCTION DEFINITIONS */
/********************************/

/*******************************/
/* PUBLIC FUNCTION DEFINITIONS */
/*******************************/
gui_status_t gui_init(write_function p_lcdWrite, const char* xmlString)
{
    if(p_lcdWrite == NULL || xmlString == NULL)
    {
        return GUI_ERR;
    }
    
    guiXml = xmlString;
    pageCount = 0;
    gui_parse_for_pages();

    return GUI_OK;

}

int16_t gui_get_page_count()
{
    return pageCount;
}

int16_t gui_get_variable_count()
{
    return 0;
}

gui_status_t gui_parse_for_pages()
{
    bool b_isInPage = 0; // 1 when inside a <page> element
    uint32_t currentIndex = 0; // Initialize the current index
    while (*guiXml != '\0') {
        if (strncmp(guiXml, "<page>", 6) == 0) 
        {
            b_isInPage = true;
            pages[pageCount].startIndex = currentIndex + 5;
        } 
        else if (strncmp(guiXml, "</page>", 7) == 0) 
        {
            if (b_isInPage) 
            {
                pages[pageCount].endIndex = currentIndex;
                pageCount++;
                b_isInPage = false;
            }
        }
        guiXml++;
        currentIndex++;
    }

    return GUI_OK;
}


gui_status_t gui_get_page_position(int16_t pageNumber, uint32_t * p_startIndex , uint32_t * p_endIndex)
{
    if(pageNumber > pageCount)
    {
        return GUI_ERR;
    }
    *p_startIndex = pages[pageNumber].startIndex;
    *p_endIndex = pages[pageNumber].endIndex;
    return GUI_OK;
}