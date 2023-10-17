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
int16_t varCount  = 0;     /** Number of Variables found in xml after parsing*/
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
    varCount  = 0;

    // Parsing the XML and creating hashmap and 
    // page index
    gui_parse_xml();

    return GUI_OK;

}

int16_t gui_get_page_count()
{
    return pageCount;
}

int16_t gui_get_variable_count()
{
    return varCount;
}

gui_status_t gui_parse_xml()
{
    bool b_isInPage = 0; // 1 when inside a <page> element
    bool b_isInVar  = 0;  // 1 when inside a <variable> element
    bool b_isInTag  = 0;  // General bool for all other tags that should not contain any children tags
    // Tag idx parameters 
    uint32_t varStartIdx __attribute__((unused)) = {0}; // Position where the last variable tag was found 
    uint32_t varEndIdx   __attribute__((unused)) = {0}; // Position where the last variable end tag was found 
    const char *nameSrtIdx  __attribute__((unused)) = {0}; // Position where the last name start tag was found 
    const char *nameEndIdx  __attribute__((unused)) = {0}; // Position where the last name end tag was found 
    uint32_t valueSrtIdx __attribute__((unused)) = {0}; // Position where the last value start tag was found 
    uint32_t valueEndIdx __attribute__((unused)) = {0}; // Position where the last value end tag was found 
    uint32_t typeSrtIdx  __attribute__((unused)) = {0}; // Position where the last type start tag was found 
    uint32_t typeEndIdx  __attribute__((unused)) = {0}; // Position where the last type end tag was found 

    // Tag string parameters
    char lastName[MAX_TAG_DATA_LENGTH]  __attribute__((unused)) = {0};  // Last string that was read from within name tags
    char lastValue[MAX_TAG_DATA_LENGTH] __attribute__((unused)) = {0};  // Last string that was read from within value tags
    char lastType[MAX_TAG_DATA_LENGTH]  __attribute__((unused)) = {0};  // Last string that was read from within type tags
    uint32_t currentIndex = 0; // Initialize the current index

    /**
     * This can definatly be made more efficent by only serching for tags where they are likely to 
     * be, i.e we dont need to serch for any type or value tags if we are not inside a variable 
     * we also dont need to check for variables or pages if we are not inside the variables or 
     * pages tags.
     * 
     * It is fine for the proof of concept for now
     * 
     */
    while (*guiXml != '\0') {


        // VARIABLE TAG CHECK 
        ///////////////////// 
        if (strncmp(guiXml, "<variable>", 10) == 0) 
        {
            varStartIdx = currentIndex + 9;
            b_isInVar   = true;
        }
        else if (strncmp(guiXml, "</variable>", 11) == 0) 
        {
            if (b_isInVar) 
            {
                varEndIdx = currentIndex;
                b_isInVar = false;
                varCount++;
            }
        }
        

        // PAGE TAG CHECK 
        ///////////////// 
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

        // NAME TAG CHECK 
        /////////////////
        if (strncmp(guiXml, "<name>", 6) == 0) 
        {
            b_isInTag = true;
            nameSrtIdx = guiXml + 6;
        } 
        else if (strncmp(guiXml, "</name>", 7) == 0) 
        {
            if (b_isInTag) 
            {
                nameEndIdx = guiXml;
                b_isInTag  = false;
                // Extract name
                if ((nameSrtIdx >= 0) && (nameEndIdx >= nameSrtIdx)) {
                    int nameLength = nameEndIdx - nameSrtIdx;
                    if (nameLength < MAX_TAG_DATA_LENGTH) {
                        strncpy(lastName, nameSrtIdx, nameLength);
                        lastName[nameLength] = '\0';  // Null-terminate the string
                    }
                    printf("NAME FOUND: %s \n",lastName);

                }
            }
        }

        // // VALUE TAG CHECK 
        // //////////////////
        // if (strncmp(guiXml, "<value>", 7) == 0) 
        // {
        //     b_isInTag = true;
        //     valueSrtIdx = currentIndex + 6;
        // } 
        // else if (strncmp(guiXml, "</value>", 8) == 0) 
        // {
        //     if (b_isInTag) 
        //     {
        //         valueEndIdx = currentIndex;
        //         b_isInTag = false;
        //     }
        // }

        // // TYPE TAG CHECK 
        // //////////////////
        // if (strncmp(guiXml, "<type>", 6) == 0) 
        // {
        //     b_isInTag = true;
        //     typeSrtIdx = currentIndex + 5;
        // } 
        // else if (strncmp(guiXml, "</type>", 7) == 0) 
        // {
        //     if (b_isInTag) 
        //     {
        //         typeEndIdx = currentIndex;
        //         b_isInTag = false;
        //     }
        // }

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

gui_variable_status_t gui_get_uint16_var(const char *variableKey,uint16_t *p_value)
{
    return GUI_VAR_ERR;
}
    
