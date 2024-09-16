#include "gui.h"
#include <stdarg.h>

/***********************/
/* PRIVATE DEFINITIONS */
/***********************/
#define SKIP_WHITESPACE(str) \
    do { \
        while ((*str == ' ' || *str == '\n' || *str == ',' || *str == '\r' || *str == '\t') && (*str != '\0')) { \
            str++; \
        } \
    } while (0)

#define SKIP_TO_WHITESPACE(str) \
    do { \
        while (!(*str == ' ' || *str == '\n' || *str == ',' || *str == '\r' || *str == '\t')) { \
            str++; \
        } \
    } while (0)

#define SKIP_TO(str,char) \
    do { \
        while (*str != char && *str != '\0') { \
            str++; \
        } \
    } while (0)
/*****************/
/* PRIVATE TYPES */
/*****************/
/**
 * @brief Structure for the int32_t Hashmap 
*/
typedef struct KeyValuePairInt32
{
    char key[MAX_KEY_LENGTH];
    int32_t value;
    bool b_isUsed;
} KeyValuePairInt32;

/**
 * @brief Structure for the float Hashmap 
*/
typedef struct KeyValuePairFlt
{
    char key[MAX_KEY_LENGTH];
    float value;
    bool b_isUsed;
} KeyValuePairFlt;

/**
 * @brief Dictionary used to map the text alignment from the enum to there 
 * associated string used in xml 
 * 
 */
text_alignment_opt_t txtAlignmentDic[TEXT_ALIGNMENT_OPTIONS] = {
    {CENTERED, "center"},
    {LEFT,     "left"},
    {RIGHT,    "right"}
};

/**
 * @brief Dictionary used to map the vert text alignment from the enum to there 
 * associated string used in xml 
 * 
 */
text_vert_alignment_opt_t txtVertAlignmentDic[TEXT_ALIGNMENT_OPTIONS] = {
    {CENTER,  "center"},
    {TOP,       "top"},
    {BOTTOM,    "bottom"}
};
/*********************/
/* PRIVATE VARIABLES */
/*********************/
int16_t pageCount = 0;     /** Number of pages found in xml after parsing*/
int16_t varCount  = 0;     /** Number of Variables found in xml after parsing*/
const char* guiXml = NULL; /** XML string that contains the whole menu system*/
page_params_t pages[MAX_PAGE_COUNT] = {0};
KeyValuePairInt32 HashMapInt32[HASH_MAX_VARS];  /** Hash map for int32_t menu variables*/
KeyValuePairFlt HashMapFlt[HASH_MAX_VARS];  /** Hash map for float menu variables*/

extern font_list_t font_master_list[NUM_FONT_TYPES]; 
/*****************************/
/* PRIVATE FUNCTION POINTERS */
/*****************************/
write_function bitMapWrite = NULL; /** Function pointer that can be used to write to the display*/
log_function logWrite      = NULL; /** Function pointer that can be used to write to the logger output*/

/*********************************/
/* PRIVATE FUNCTION DECLARATIONS */
/*********************************/
uint32_t hash_index(const char * key);                   /** Generates a hash index from a key */
gui_status_t help_operand_do(const char *operandObjectString); /** Sets the var name it finds first equal to the next value or var it finds*/
gui_status_t help_find_font(const char *fontObjectString,bool *b_haveFoundFont,char *fontName,uint8_t *fontIndex); /** Returns the font name in the passed in string */
gui_status_t help_find_font_size(const char *fontSizeObjectString, bool *b_haveFoundFontSize, bool b_haveFoundFont, uint8_t fontIndex, uint8_t *fontSize, uint8_t *fontSizeIndex);

void help_log(const char *message, ...);                  /** Function to be used to log messages */

/********************************/
/* PRIVATE FUNCTION DEFINITIONS */
/********************************/
uint32_t hash_index(const char * key)
{
    uint32_t hash = 0;
    while (*key) {
        hash = (hash * 31) + (*key);
        key++;
    }
    return hash % HASH_MAX_VARS;
}
/*******************************/
/* PUBLIC FUNCTION DEFINITIONS */
/*******************************/
gui_status_t gui_init(write_function p_lcdWrite, log_function p_logFunction, const char* xmlString)
{
    logWrite    = p_logFunction;
    if(p_lcdWrite == NULL || xmlString == NULL)
    {
        help_log("GUI ERROR:  Init Fail, check the xmlString and p_lcdWrite arguments are not NULL!");
        return GUI_ERR;
    }
    bitMapWrite = p_lcdWrite;
    guiXml      = xmlString;
    pageCount = 0;
    varCount  = 0;

   

    // Initalise hashmap 
    for (int i = 0; i < HASH_MAX_VARS; i++) {
        strncpy(HashMapInt32[i].key, "Nothing", MAX_KEY_LENGTH - 1);
        HashMapInt32[i].b_isUsed = false;
        HashMapInt32[i].value = 0;
        strncpy(HashMapFlt[i].key, "Nothing", MAX_KEY_LENGTH - 1);
        HashMapFlt[i].b_isUsed = false;
        HashMapFlt[i].value = 0;
    }
    
    // Parsing the XML and creating hashmap and page index
    return gui_parse_xml();
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
    const char *nameSrtIdx  = {0}; // Position where the last name start tag was found 
    const char *nameEndIdx  = {0}; // Position where the last name end tag was found 
    const char *valueSrtIdx = {0}; // Position where the last value start tag was found 
    const char *valueEndIdx = {0}; // Position where the last value end tag was found 
    const char *typeSrtIdx  = {0}; // Position where the last type start tag was found 
    const char *typeEndIdx  = {0}; // Position where the last type end tag was found 

    // Tag string parameters
    char lastName[MAX_TAG_DATA_LENGTH]   = {0};  // Last string that was read from within name tags
    char lastValue[MAX_TAG_DATA_LENGTH]  = {0};  // Last string that was read from within value tags
    char lastType[MAX_TAG_DATA_LENGTH]   = {0};  // Last string that was read from within type tags
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
    const char*xmlCopy = guiXml;
    while (*xmlCopy != '\0') {

        
        // VARIABLE TAG CHECK 
        ///////////////////// 
        if (strncmp(xmlCopy, "<variable>", 10) == 0) 
        {
            b_isInVar   = true;
        }
        else if (strncmp(xmlCopy, "</variable>", 11) == 0) 
        {
            if (b_isInVar) 
            {
                b_isInVar = false;
                varCount++;
                // Creating Variable 
                gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
                if(createStatus != GUI_VAR_OK){return GUI_ERR;}
            }else
            {
                help_log("GUI ERROR: Variable tag mismatch!");
                return GUI_INIT_VAR_BRACE;
            }
        }
        
        
        // PAGE TAG CHECK 
        ///////////////// 
        if (strncmp(xmlCopy, "<page>", 6) == 0) 
        {
            b_isInPage = true;
            pages[pageCount].startIndex = currentIndex + 5;
        } 
        else if (strncmp(xmlCopy, "</page>", 7) == 0) 
        {
            if (b_isInPage) 
            {
                pages[pageCount].endIndex = currentIndex;
                pageCount++;
                
                b_isInPage = false;
            }
            else
            {
                help_log("GUI ERROR: Page tag mismatch!");
                return GUI_INIT_PGE_BRACE;
            }
        }

        // NAME TAG CHECK 
        /////////////////
        if (strncmp(xmlCopy, "<name>", 6) == 0) 
        {
            b_isInTag = true;
            nameSrtIdx = xmlCopy + 6;
        } 
        else if (strncmp(xmlCopy, "</name>", 7) == 0) 
        {
            if (b_isInTag) 
            {
                nameEndIdx = xmlCopy;
                b_isInTag  = false;
                // Extract name
                if ((nameSrtIdx >= 0) && (nameEndIdx >= nameSrtIdx)) 
                {
                    int nameLength = nameEndIdx - nameSrtIdx;
                    if (nameLength < MAX_TAG_DATA_LENGTH) {
                        strncpy(lastName, nameSrtIdx, nameLength);
                        lastName[nameLength] = '\0';  // Null-terminate the string
                    }
                }
            }
        }

        // VALUE TAG CHECK 
        //////////////////
        if (strncmp(xmlCopy, "<value>", 7) == 0) 
        {
            b_isInTag = true;
            valueSrtIdx = xmlCopy + 7;
        } 
        else if (strncmp(xmlCopy, "</value>", 8) == 0) 
        {
            if (b_isInTag) 
            {
                valueEndIdx = xmlCopy;
                b_isInTag = false;
            }
            // Extract Value 
            if ((valueSrtIdx >= 0) && (valueEndIdx >= valueSrtIdx)) 
            {
                int valueLength = valueEndIdx - valueSrtIdx;
                if (valueLength < MAX_TAG_DATA_LENGTH) {
                    strncpy(lastValue, valueSrtIdx, valueLength);
                    lastValue[valueLength] = '\0';  // Null-terminate the string
                }
            }
        }

        // TYPE TAG CHECK 
        //////////////////
        if (strncmp(xmlCopy, "<type>", 6) == 0) 
        {
            b_isInTag = true;
            typeSrtIdx = xmlCopy + 6;
        } 
        else if (strncmp(xmlCopy, "</type>", 7) == 0) 
        {
            if (b_isInTag) 
            {
                typeEndIdx = xmlCopy;
                b_isInTag = false;
                // Extract Type 
                if ((typeSrtIdx >= 0) && (typeEndIdx >= typeSrtIdx)) 
                {
                    int typeLength = typeEndIdx - typeSrtIdx;
                    if (typeLength < MAX_TAG_DATA_LENGTH) {
                        strncpy(lastType, typeSrtIdx, typeLength);
                        lastType[typeLength] = '\0';  // Null-terminate the string
                    }
                }
            }
        }
        xmlCopy++;
        currentIndex++;
    }

    // Checking for mimatched braces
    if(b_isInPage)
    {
       help_log("GUI ERROR: Page tag mismatch!");
       return GUI_INIT_PGE_BRACE;
    }
    if(b_isInVar)
    {
        help_log("GUI ERROR: Variable tag mismatch for '%s'!", lastName);
        return GUI_INIT_VAR_BRACE;
    }
    // Printing Successful Parse 
    help_log("GUI: Successful init! Contains %d Var and %d pages",varCount,pageCount);
    return GUI_OK;
}


gui_status_t gui_get_page_position(int16_t pageNumber, uint32_t * p_startIndex , uint32_t * p_endIndex)
{
    if(pageNumber > pageCount)
    {
        help_log("GUI ERROR: Page %d does not exist!", pageNumber);
        return GUI_ERR;
    }
    *p_startIndex = pages[pageNumber].startIndex;
    *p_endIndex   = pages[pageNumber].endIndex;
    return GUI_OK;
}

gui_variable_status_t gui_create_var(const char *variableName,const char *variableType,const char *variableValue)
{
    if(strlen(variableName)>MAX_KEY_LENGTH-1)
    {
        help_log("GUI ERROR: Variable '%s' name too long!", variableName);
        return GUI_VAR_ERR;
    }
    uint32_t index = hash_index(variableName);

    if (strncmp(variableType, "int32_t", 9) == 0) {
        // Extracting value 
        int32_t value = (int32_t)atoi(variableValue);
        // Linear probing
        while (HashMapInt32[index].b_isUsed) 
        {
            index = (index + 1) % HASH_MAX_VARS;
        }
        // Statically allocated memory
        HashMapInt32[index].b_isUsed = true;
        strncpy(HashMapInt32[index].key, variableName, MAX_KEY_LENGTH - 1);
        HashMapInt32[index].key[MAX_KEY_LENGTH - 1] = '\0'; // Null-terminate the string
        HashMapInt32[index].value = value;
        return GUI_VAR_OK;
    }
    else if(strncmp(variableType, "float", 5) == 0) 
    {
        // Extracting value 
        float value = (float)atof(variableValue);
        // Linear probing
        while (HashMapFlt[index].b_isUsed) 
        {
            index = (index + 1) % HASH_MAX_VARS;
        }
        // Statically allocated memory
        HashMapFlt[index].b_isUsed = true;
        strncpy(HashMapFlt[index].key, variableName, MAX_KEY_LENGTH - 1);
        HashMapFlt[index].key[MAX_KEY_LENGTH - 1] = '\0'; // Null-terminate the string
        HashMapFlt[index].value = value;
        return GUI_VAR_OK;
    }
    help_log("GUI ERROR: Type '%s' not supported!", variableType);
    return GUI_VAR_ERR;
}

gui_variable_status_t gui_get_int32_var(const char *variableKey,int32_t *p_value)
{
    uint32_t index = hash_index(variableKey);
    while (index < HASH_MAX_VARS)
     {
        if (HashMapInt32[index].b_isUsed && strcmp(HashMapInt32[index].key, variableKey) == 0) {
            *p_value = HashMapInt32[index].value;
            return GUI_VAR_OK;
        }
        index++;
    }
    return GUI_VAR_ERR; // Return GUI_VAR_ERR if variable is not found
}


gui_variable_status_t gui_get_float_var(const char *variableKey,float *p_value)
{
    uint32_t index = hash_index(variableKey);
    while (index < HASH_MAX_VARS)
     {
        if (HashMapFlt[index].b_isUsed && strcmp(HashMapFlt[index].key, variableKey) == 0) {
            *p_value = HashMapFlt[index].value;
            return GUI_VAR_OK;
        }
        index++;
    }
    return GUI_VAR_ERR; // Return GUI_VAR_ERR if variable is not found
}
gui_variable_status_t gui_update_int32_var(const char *variableKey,int32_t value)
{
    uint32_t index = hash_index(variableKey);
    while (index < HASH_MAX_VARS)
     {
        if (HashMapInt32[index].b_isUsed && strcmp(HashMapInt32[index].key, variableKey) == 0) {
            HashMapInt32[index].value = value;
            return GUI_VAR_OK;
        }
        index++;
    }
    return GUI_VAR_ERR; // Return GUI_VAR_ERR if variable is not found
}

gui_variable_status_t gui_update_float_var(const char *variableKey,float value)
{
    uint32_t index = hash_index(variableKey);
    while (index < HASH_MAX_VARS)
     {
        if (HashMapFlt[index].b_isUsed && strcmp(HashMapFlt[index].key, variableKey) == 0) {
            HashMapFlt[index].value = value;
            return GUI_VAR_OK;
        }
        index++;
    }
    return GUI_VAR_ERR; // Return GUI_VAR_ERR if variable is not found
}

gui_status_t gui_render_bitmap(uint8_t bitMap[ROWS][COLUMNS],const char *bitmapString)
{
    const char*strBitmap = bitmapString;
    if (strncmp(strBitmap, "<bitMap>", 8) != 0) 
    {
        help_log("GUI ERROR: No bitmap start tag found!");
        return GUI_ERR;
    }

    // Creating loop variables 
    int32_t width  = {0}; /** Width of bitmap in pixels */
    int32_t height = {0}; /** Height of bitmap in pixels */
    int32_t posX    = {0}; /** What column to place the top left corner of bitmap in  */
    int32_t posY    = {0}; /** What row to place the top left corner of bitmap in  */
    bool b_haveFoundSize = false; 
    bool b_haveFoundPosi = false; 
    
    while (*strBitmap != '\0')
    {
        // SIZE TAG CHECK 
        ////////////////// 
        if (!b_haveFoundSize && (strncmp(strBitmap, "<size>", 6) == 0)) 
        {
            if (sscanf(strBitmap, "<size>%d,%d</size>", &width, &height) == 2) 
            {
                b_haveFoundSize = true;
            }
        }
        // POSITION TAG CHECK 
        ///////////////////// 
        else if (!b_haveFoundPosi && (strncmp(strBitmap, "<position>", 10) == 0)) 
        {
            int32_t posVars[2] = {0};
            char returnTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
            gui_status_t posStatus = gui_parse_tag_val(strBitmap,"position",returnTagName,posVars,2,&b_haveFoundPosi);
            if(posStatus != GUI_OK)
            {
                return posStatus;
            }
            if(b_haveFoundPosi)
            {
                posY = posVars[0];
                posX = posVars[1];
            }
        }
        // DATA TAG CHECK
        /////////////////
        else if (strncmp(strBitmap, "<data>", 6) == 0) 
        {
            if(b_haveFoundSize && b_haveFoundPosi)
            {
                break;
            }
            else 
            {
                if(!b_haveFoundPosi)
                {
                    help_log("GUI ERROR: Bitmap Postion not found!");
                }
                if(!b_haveFoundSize)
                {
                    help_log("GUI ERROR: Bitmap Size not found!");
                }
                return GUI_ERR;
            }
        }
        // strBitmap++;
        SKIP_TO_WHITESPACE(strBitmap);
        SKIP_TO(strBitmap,'<');
    }

    // Extracting bitmap data 
    SKIP_TO_WHITESPACE(strBitmap);
    // Position & Height Logic 
    // Row iterator 
    for(int32_t itr_row = posY; itr_row < (height+posY); itr_row++)
    {
        for(int32_t itr_col = posX; itr_col < (width+posX); itr_col++)
        {
            // Skipping white-space chars 
            SKIP_WHITESPACE(strBitmap);
            // Extracting ith data point 
            uint8_t bit = 0;

            if (sscanf(strBitmap, "%hhd", &bit) != 1) 
            {
                help_log("GUI ERROR: Found un-Bitmappable chracter '%c' in bitmap!", *strBitmap);
                return GUI_ERR;   
            }
            strBitmap++;
            


            if((itr_row < 0) || (itr_row >= ROWS)||(itr_col<0) || (itr_col >= COLUMNS))
            {
                continue;
            }
            bitMap[itr_row][itr_col] = bit;
        }
    }
    // Looking for the final braces. 
    SKIP_WHITESPACE(strBitmap);
    if (strncmp(strBitmap, "</data>", 7) == 0) 
    {
        strBitmap+=7;
    }
    SKIP_WHITESPACE(strBitmap);
    if (strncmp(strBitmap, "</bitMap>", 9) == 0) 
    {
        return GUI_OK;
    }
    help_log("GUI ERROR: No bitmap end tag found!");
    return GUI_ERR;
}

gui_status_t gui_render_int32_entry(uint8_t bitMap[ROWS][COLUMNS], const char *intergerEnterString)
{
     if (strncmp(intergerEnterString, "<intEntry>", 10) != 0) 
    {
        help_log("GUI ERROR: No Int Enter starting tag found!");
        return GUI_ERR;
    }

        // INIT VARS 
    //////////////////////
    uint8_t fontSize  = {0};
    uint8_t fontSizeIndex  = {0};
    int8_t fontIndex = 0;
    int32_t cursorValue = 0;
    int32_t entryValue = 0;
    int32_t digitsValue = 0;
    int32_t maxValue = 0;
    int32_t minValue = 0;
    int32_t incrementerValue = 0;



    char entryVarTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
    char incremeneterTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
    char returnTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
    char heading[MAX_TAG_DATA_LENGTH] = {'\0'};
    char fontName[MAX_TAG_DATA_LENGTH] = {'\0'};
    char valueString[MAX_TAG_DATA_LENGTH] = {'\0'};

    // char options[MAX_TAG_DATA_LENGTH] = {'\0'};

    bool b_haveFoundCursor = false;
    bool b_haveFoundFont   = false;
    bool b_haveFoundFontSize   = false;
    bool b_haveFoundPosi   = false;
    bool b_haveFoundDigits = false;
    bool b_haveFoundMax = false;
    bool b_haveFoundMin = false;
    bool b_haveFoundVar = false;
    bool b_haveFoundEnd = false;
    bool b_haveFoundHeading = false;
    bool b_haveFoundIncrementer = false;



    while(*intergerEnterString != '\0')
    {
        // CURSOR TAG CHECK 
        ///////////////////
        if(!b_haveFoundCursor && (strncmp(intergerEnterString, "<cursor>",7) == 0))
        {
            gui_status_t cursorStatus = gui_parse_tag_val(intergerEnterString,"cursor",returnTagName,&cursorValue,1,&b_haveFoundCursor);
            if(cursorStatus != GUI_OK)
            {
                return cursorStatus;
            }
        }
         // FONT TAG CHECK 
        ///////////////////
        else if(!b_haveFoundFont && (strncmp(intergerEnterString, "<font>",6) == 0))
        {
            if(GUI_ERR == help_find_font(intergerEnterString,&b_haveFoundFont,fontName, &fontIndex))
            {
                return GUI_ERR;
            }
        }
        // SIZE TAG CHECK 
        ///////////////////
        else if(!b_haveFoundFontSize && (strncmp(intergerEnterString, "<font-size>",11) == 0))
        {
            if(GUI_ERR == help_find_font_size(intergerEnterString,&b_haveFoundFontSize, b_haveFoundFont,fontIndex,&fontSize,&fontSizeIndex))
            {
                return GUI_ERR;
            }
        }
        // POSITION TAG CHECK 
        /////////////////////
        else if(!b_haveFoundPosi && (strncmp(intergerEnterString, "<position>",10) == 0))
        {
            b_haveFoundPosi= true;
        }
        // DIGITS TAG CHECK 
        /////////////////////
        else if(!b_haveFoundDigits && (strncmp(intergerEnterString, "<digits>",8) == 0))
        {
            char returnTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
            gui_status_t digitsStatus = gui_parse_tag_val(intergerEnterString,"digits",returnTagName,&digitsValue,1,&b_haveFoundDigits);
            if(digitsStatus != GUI_OK)
            {
                return digitsStatus;
            }
        }
        // MAX TAG CHECK 
        /////////////////////
        else if(!b_haveFoundMax && (strncmp(intergerEnterString, "<max>",5) == 0))
        {
            gui_status_t maxStatus = gui_parse_tag_val(intergerEnterString,"max",returnTagName,&maxValue,1,&b_haveFoundMax);
            if(maxStatus != GUI_OK)
            {
                return maxStatus;
            }
        }
        // MIN TAG CHECK 
        /////////////////////
        else if(!b_haveFoundMin && (strncmp(intergerEnterString, "<min>",5) == 0))
        {
            gui_status_t minStatus = gui_parse_tag_val(intergerEnterString,"min",returnTagName,&minValue,1,&b_haveFoundMin);
            if(minStatus != GUI_OK)
            {
                return minStatus;
            }
        }
        // HEADING TAG CHECK 
        ////////////////////
        else if(!b_haveFoundHeading && (strncmp(intergerEnterString, "<heading>",9) == 0))
        {
            if (sscanf(intergerEnterString, "<heading>%63[^<]", heading) == 1)
            {
                b_haveFoundHeading = true;
            } 
        }
        // VAR TAG CHECK 
        /////////////////////
        else if(!b_haveFoundVar && (strncmp(intergerEnterString, "<variable>",10) == 0))
        {
            gui_status_t variableStatus = gui_parse_tag_val(intergerEnterString,"variable",entryVarTagName,&entryValue,1,&b_haveFoundVar);
            if(variableStatus != GUI_OK)
            {
                return variableStatus;
            }
        }
        // INCREMENETER TAG CHECK 
        /////////////////////////
        else if(!b_haveFoundIncrementer && (strncmp(intergerEnterString, "<incrementer>",13) == 0))
        {
            gui_status_t variableStatus = gui_parse_tag_val(intergerEnterString,"incrementer",incremeneterTagName,&incrementerValue,1,&b_haveFoundIncrementer);
            if(variableStatus != GUI_OK)
            {
                return variableStatus;
            }
        }
        // END TAG CHECK 
        ////////////////
        else if(!b_haveFoundEnd && (strncmp(intergerEnterString, "</intEntry>",11) == 0))
        {
            b_haveFoundEnd = true;
        }
        SKIP_TO_WHITESPACE(intergerEnterString);
        SKIP_TO(intergerEnterString,'<');
    }


    if(!b_haveFoundIncrementer||!b_haveFoundHeading||!b_haveFoundEnd||!b_haveFoundCursor||!b_haveFoundFont||!b_haveFoundFontSize||!b_haveFoundPosi||!b_haveFoundDigits||!b_haveFoundMax||!b_haveFoundMin||!b_haveFoundVar)

    {
        if(!b_haveFoundCursor){help_log("GUI ERROR: No Cursor starting tag found!");}
        if(!b_haveFoundFont){help_log("GUI ERROR: No Font starting tag found!");}
        if(!b_haveFoundFontSize){help_log("GUI ERROR: No Font-Size starting tag found!");}
        if(!b_haveFoundPosi){help_log("GUI ERROR: No Position starting tag found!");}
        if(!b_haveFoundDigits){help_log("GUI ERROR: No Digits starting tag found!");}
        if(!b_haveFoundMax){help_log("GUI ERROR: No Max starting tag found!");}
        if(!b_haveFoundMin){help_log("GUI ERROR: No Min starting tag found!");}
        if(!b_haveFoundVar){help_log("GUI ERROR: No Variable starting tag found!");}
        if(!b_haveFoundEnd){help_log("GUI ERROR: No End Interger Entery tag found!");}
        if(!b_haveFoundHeading){help_log("GUI ERROR: Heading Not Found!");}
        if(!b_haveFoundIncrementer){help_log("GUI ERROR: Incremeneter not found!");}

        return GUI_ERR;
    }

    // RENDER HEADING 
    /////////////////
    // WIDTH CALC
    size_t txtLen = strlen(heading);
    int32_t txtPxWidth = 0; /** The pixel width of the string */
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if((heading[itr_text] == '\"') || (heading[itr_text] == '\n'))
        {
            continue;
        }
        txtPxWidth += gui_get_char_width(fontIndex ,fontSizeIndex, heading[itr_text]);
    }
    // RENDER LOOP
    int16_t colPos = 0;
    int16_t rowPos = 0;
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if(heading[itr_text] == '\"')
        {
            continue;
        }
        if(heading[itr_text] == '\n')
        {
            rowPos += fontSize+3; /** ToDo- Add the leading calculation here */
            colPos = 0;
            continue;
        }
        if(gui_write_char(fontIndex ,fontSizeIndex, rowPos, colPos, bitMap, heading[itr_text],0) != GUI_OK)
        {
            return GUI_ERR;
        }
        colPos += gui_get_char_width(fontIndex ,fontSizeIndex, heading[itr_text]);
    }
    // RENDER DIGIT
    ///////////////
    // INCREMENTING NUMBER
    if ((incrementerValue != 0) && (cursorValue > 0) && (cursorValue < (digitsValue+1)))
    {
        if (digitsValue==2)
        {
            if (cursorValue == 1)
            {
                entryValue += 10*incrementerValue;
            }
            else if(cursorValue == 2)
            {
               entryValue += 1*incrementerValue; 
            }
        }
        else if (digitsValue == 3)
        {
            if (cursorValue == 1)
            {
                entryValue += 100*incrementerValue;
            }
            else if(cursorValue == 2)
            {
               entryValue += 10*incrementerValue; 
            }
            else if(cursorValue == 3)
            {
               entryValue += 1*incrementerValue; 
            }
        }
        gui_update_int32_var(incremeneterTagName,0);
        gui_update_int32_var(entryVarTagName,entryValue);

    }
    // MIN MAX VAR CHECK 
    if(entryValue>maxValue)
    {
        gui_update_int32_var(entryVarTagName,maxValue);
        entryValue = maxValue;
    }
    else if (entryValue<minValue)
    {
        gui_update_int32_var(entryVarTagName,minValue);
        entryValue = minValue;
    }

    // FORMATTING NUMBER
    char fmt[20];
    sprintf(fmt, "%%0%dd", digitsValue);
    snprintf(valueString, sizeof(valueString), fmt, entryValue);


    txtLen = strlen(valueString);
    txtPxWidth = 0; /** The pixel width of the string */
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if((valueString[itr_text] == '\"') || (valueString[itr_text] == '\n'))
        {
            continue;
        }
        txtPxWidth += gui_get_char_width(fontIndex ,fontSizeIndex, valueString[itr_text]);
    }
    // RENDER LOOP
    rowPos += fontSize+3; /** ToDo- Add the leading calculation here */
    colPos = (int16_t)((COLUMNS/2)-txtPxWidth/2);
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if(valueString[itr_text] == '\"')
        {
            continue;
        }
        if(valueString[itr_text] == '\n')
        {
            rowPos += fontSize+3; /** ToDo- Add the leading calculation here */
            colPos = 0;
            continue;
        }
        if(gui_write_char(fontIndex ,fontSizeIndex, rowPos, colPos, bitMap, valueString[itr_text],((cursorValue-1)==itr_text)) != GUI_OK)
        {
            return GUI_ERR;
        }
        colPos += gui_get_char_width(fontIndex ,fontSizeIndex, valueString[itr_text])+2;
    }

    return GUI_OK;
}

gui_status_t gui_render_list(uint8_t bitMap[ROWS][COLUMNS], const char *listObjectString)
{
    if (strncmp(listObjectString, "<list>", 6) != 0) 
    {
        help_log("GUI ERROR: No List starting tag found!");
        return GUI_ERR;
    }
    // INIT VARS 
    //////////////////////
    uint8_t fontSize  = {0};
    uint8_t fontSizeIndex  = {0};
    int8_t fontIndex = 0;
    int32_t cursorValue = 0;
    char fontName[MAX_TAG_DATA_LENGTH] = {'\0'};
    char options[MAX_TAG_DATA_LENGTH] = {'\0'};

    bool b_haveFoundCursor = false;
    bool b_haveFoundFont   = false;
    bool b_haveFoundFontSize   = false;
    bool b_haveFoundPosi   = false;
    bool b_haveFoundOpt    = false;
    bool b_haveFoundEnd    = false;

    


    while(*listObjectString != '\0')
    {
        // CURSOR TAG CHECK 
        ///////////////////
        if(!b_haveFoundCursor && (strncmp(listObjectString, "<cursor>",7) == 0))
        {
            char returnTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
            gui_status_t cursorStatus = gui_parse_tag_val(listObjectString,"cursor",returnTagName,&cursorValue,1,&b_haveFoundCursor);
            if(cursorStatus != GUI_OK)
            {
                return cursorStatus;
            }
        }
        // FONT TAG CHECK 
        ///////////////////
        else if(!b_haveFoundFont && (strncmp(listObjectString, "<font>",6) == 0))
        {
            if(GUI_ERR == help_find_font(listObjectString,&b_haveFoundFont,fontName, &fontIndex))
            {
                return GUI_ERR;
            }
        }
        // SIZE TAG CHECK 
        ///////////////////
        else if(!b_haveFoundFontSize && (strncmp(listObjectString, "<font-size>",11) == 0))
        {
            if(GUI_ERR == help_find_font_size(listObjectString,&b_haveFoundFontSize, b_haveFoundFont,fontIndex,&fontSize,&fontSizeIndex))
            {
                return GUI_ERR;
            }
        }
        // POSITION TAG CHECK 
        /////////////////////
        else if(!b_haveFoundPosi && (strncmp(listObjectString, "<position>",10) == 0))
        {
            b_haveFoundPosi= true;
        }
        // OPTIONS TAG CHECK 
        ////////////////////
        else if(!b_haveFoundOpt && (strncmp(listObjectString, "<options>",9) == 0))
        {
            gui_status_t optionStatus = gui_parse_tag_str(listObjectString,"options",options,&b_haveFoundOpt);
            if(optionStatus != GUI_OK)
            {
                return optionStatus;
            }
        }
        // END TAG CHECK 
        ////////////////
        else if(!b_haveFoundEnd && (strncmp(listObjectString, "</list>",7) == 0))
        {
            b_haveFoundEnd = true;
        }
        SKIP_TO_WHITESPACE(listObjectString);
        SKIP_TO(listObjectString,'<');
    }
    
    // ERROR CHECK 
    //////////////
    if(!b_haveFoundCursor || !b_haveFoundFont || !b_haveFoundFontSize || !b_haveFoundPosi || !b_haveFoundOpt || !b_haveFoundEnd)
    {
        if(!b_haveFoundCursor){help_log("GUI ERROR: No Cursor starting tag found!");}
        if(!b_haveFoundFont){help_log("GUI ERROR: No Font starting tag found!");}
        if(!b_haveFoundFontSize){help_log("GUI ERROR: No Font-Size starting tag found!");}
        if(!b_haveFoundPosi){help_log("GUI ERROR: No Position starting tag found!");}
        if(!b_haveFoundOpt){help_log("GUI ERROR: No Options starting tag found!");}
        if(!b_haveFoundEnd){help_log("GUI ERROR: No End List tag found!");}
        return GUI_ERR;
    }

    // WIDTH CALC
    size_t txtLen = strlen(options);
    int32_t txtPxWidth = 0; /** The pixel width of the string */
    int16_t numberOfOptions = 1; 
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if((options[itr_text] == '\"') || (options[itr_text] == '\n'))
        {
            numberOfOptions += 1;
            continue;
        }
        txtPxWidth += gui_get_char_width(fontIndex ,fontSizeIndex, options[itr_text]);
    }
    // HANDLING WRAPPING
    if (cursorValue > (numberOfOptions-1))
    {
        cursorValue = 0;
        gui_update_int32_var("cursor",cursorValue);
    }
    else if (cursorValue < 0)
    {
        cursorValue = numberOfOptions-1;
        gui_update_int32_var("cursor",cursorValue);
    }
    // uint32_t selectedOptionTop = cursorValue*(fontSize+3)+fontSize;
    uint32_t selectedOptionBottom = cursorValue*(fontSize+3)+2*fontSize;
    uint32_t maxDisplayOptions = (ROWS/(fontSize+3));
    int16_t optionOffset = 0;

    char* optionsString = options;
    if (selectedOptionBottom > ROWS)
    {   
        uint32_t rowsToRemove = cursorValue-maxDisplayOptions+1;
        for(int itr_options = 0; itr_options<rowsToRemove; itr_options++)
        {
            optionsString = strchr(optionsString,'\n')+1;
            optionOffset += 1;
        } 
    }
    // WRITING LOOP 
    int16_t colPos = 0;
    int16_t rowPos = 0;
    int16_t optionRendering = 0;
    txtLen = strlen(optionsString);
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if(optionsString[itr_text] == '\"')
        {
            continue;
        }
        if(optionsString[itr_text] == '\n')
        {
            rowPos += fontSize+3; /** ToDo- Add the leading calculation here */
            colPos = 0;
            optionRendering += 1;
            continue;
        }
        if(gui_write_char(fontIndex ,fontSizeIndex, rowPos, colPos, bitMap, optionsString[itr_text],(cursorValue == (optionRendering+optionOffset))) != GUI_OK)
        {
            return GUI_ERR;
        }
        colPos += gui_get_char_width(fontIndex ,fontSizeIndex, optionsString[itr_text]);
    }
    return GUI_OK;
}

gui_status_t gui_render_text(uint8_t bitMap[ROWS][COLUMNS],const char *textObjectString)
{
    if (strncmp(textObjectString, "<text>", 6) != 0) 
    {
        help_log("GUI ERROR: No Text starting tag found!");
        return GUI_ERR;
    }


    // Creating loop variables 
    char fontName[MAX_TAG_DATA_LENGTH] = {'\0'};
    uint8_t fontSize  = {0};
    uint8_t fontSizeIndex  = {0};
    uint8_t fontIndex = 0;
    int32_t posX      = {0}; /** What column to place text in  */
    int32_t posY      = {0}; /** What row to place text in*/
    char text[MAX_TAG_DATA_LENGTH] = {'\0'};
    uint8_t alignmentOpt     = 0;
    uint8_t vertAlignmentOpt = 0;
    // Bools 
    bool b_haveFoundFont      = false; 
    bool b_haveFoundFontSize  = false; 
    bool b_haveFoundAlignment = false; 
    bool b_haveFoundVertAlignment = false; 
    bool b_haveFoundPosition  = false; 
    bool b_haveFoundContent   = false; 
    bool b_haveFoundInvert = false;
    bool b_invert   = false; 


    while (*textObjectString != '\0')
    {
        // FONT TAG CHECK 
        ////////////////// 
        if ((!b_haveFoundFont) && (strncmp(textObjectString, "<font>", 6) == 0)) 
        {
            if(GUI_ERR == help_find_font(textObjectString,&b_haveFoundFont,fontName, &fontIndex))
            {
                return GUI_ERR;
            }
        }
        // FONT-SIZE TAG CHECK 
        //////////////////////
        else if ((!b_haveFoundFontSize) && (strncmp(textObjectString, "<font-size>", 11) == 0))  
        {
            if(GUI_ERR == help_find_font_size(textObjectString,&b_haveFoundFontSize, b_haveFoundFont,fontIndex,&fontSize,&fontSizeIndex))
            {
                return GUI_ERR;
            }
        }
        // ALIGNMENT TAG CHECK 
        //////////////////////
        else if ((!b_haveFoundAlignment)&&(strncmp(textObjectString, "<alignment>", 11) == 0))  
        {
            char alignmentName[MAX_TAG_DATA_LENGTH] = {'\0'};
            if ((sscanf(textObjectString, "<alignment>%63[^<]", alignmentName) == 1)&&b_haveFoundFont) 
            {
                for(uint8_t iter_txtOpt = 0; iter_txtOpt < TEXT_ALIGNMENT_OPTIONS; iter_txtOpt ++)
                {
                    if(strncmp(txtAlignmentDic[iter_txtOpt].alignmentName, alignmentName, strlen(fontName)) == 0)
                    {
                        b_haveFoundAlignment = true;
                        alignmentOpt = txtAlignmentDic[iter_txtOpt].alignmentEnum;
                        break;
                    }
                }   
            }
            if(!b_haveFoundAlignment)
            {
                help_log("GUI ERROR: Alignment '%s' is not defined!", alignmentName);
                return GUI_ERR;
            }
        }
        // VERT-ALIGNMENT TAG CHECK 
        ///////////////////////////
        else if((!b_haveFoundVertAlignment) && ((strncmp(textObjectString, "<vert-alignment>", 16)) == 0)) 
        {
            char alignmentName[MAX_TAG_DATA_LENGTH] = {'\0'};
            if ((sscanf(textObjectString, "<vert-alignment>%63[^<]", alignmentName) == 1)&&b_haveFoundFont) 
            {
                for(uint8_t iter_txtOpt = 0; iter_txtOpt < TEXT_ALIGNMENT_OPTIONS; iter_txtOpt ++)
                {
                    if(strncmp(txtVertAlignmentDic[iter_txtOpt].alignmentName, alignmentName, strlen(fontName)) == 0)
                    {
                        b_haveFoundVertAlignment = true;
                        vertAlignmentOpt = txtVertAlignmentDic[iter_txtOpt].alignmentEnum;
                        break;
                    }
                }   
            }
            if(!b_haveFoundVertAlignment)
            {
                return GUI_ERR;
            }
        }
        // POSITION TAG CHECK 
        //////////////////////
        else if ((!b_haveFoundPosition)&&(strncmp(textObjectString, "<position>", 10) == 0)) 
        {
            int32_t posVars[2] = {0};
            char returnTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
            gui_status_t posStatus = gui_parse_tag_val(textObjectString,"position",returnTagName,posVars,2,&b_haveFoundPosition);
            if(posStatus != GUI_OK)
            {
                return posStatus;
            }
            if(b_haveFoundPosition)
            {
                posY = posVars[0];
                posX = posVars[1];
            }
        }
        // INVERT TAG CHECK 
        //////////////////////
        else if ((!b_haveFoundInvert)&&(strncmp(textObjectString, "<invert>", 8) == 0)) 
        {
            char returnTagName[MAX_TAG_DATA_LENGTH]  = {'\0'};
            int32_t invertInt = 0;
            gui_status_t invertStatus = gui_parse_tag_val(textObjectString,"invert",returnTagName,&invertInt,1,&b_haveFoundInvert);
            if(invertStatus != GUI_OK)
            {
                return invertStatus;
            }
            if(b_haveFoundInvert)
            {
                b_invert = (invertInt>0);
            }
        }
        // CONTENT TAG CHECK 
        //////////////////////
        else if((!b_haveFoundContent)&&(strncmp(textObjectString, "<content>", 9) == 0))
        {
            gui_status_t contentStatus = gui_parse_tag_str(textObjectString,"content",text,&b_haveFoundContent);
            if(contentStatus != GUI_OK)
            {
                return contentStatus;
            }
        }
        // END OF PAGE CHECK 
        ///////////////////
        else if (strncmp(textObjectString, "</text>", 7) == 0) 
        {
            break;
        }
        // textObjectString++;
        SKIP_TO_WHITESPACE(textObjectString);
        SKIP_TO(textObjectString,'<');

    }
    
    
    if(!b_haveFoundFont || !b_haveFoundFontSize || !b_haveFoundAlignment || !b_haveFoundPosition || !b_haveFoundContent || !b_haveFoundVertAlignment)
    {
        if(!b_haveFoundPosition){help_log("GUI ERROR: Position tags not found!");}
        if(!b_haveFoundFontSize){help_log("GUI ERROR: Font size tags not found!");}
        if(!b_haveFoundAlignment){help_log("GUI ERROR: Alignment tags not found!");}
        if(!b_haveFoundContent){help_log("GUI ERROR: Content tags not found!");}
        if(!b_haveFoundVertAlignment){help_log("GUI ERROR: Vert-alignment not found");}
        return GUI_ERR;
    }
    // CHECK IF TEXT CONTAINS VARS
    // Core text 
    char *quoteToken = strtok(text, "\"");
    char coreText[64];
    char formatSpecifier[10];

    if(quoteToken != NULL)
    {
      strncpy(coreText, quoteToken, sizeof(coreText) - 1);
    }
    // Variable 
    quoteToken = strtok(NULL, "\"");
    // char *modoToken = strtok(coreText, "\%");
    if(quoteToken != NULL)
    {
        SKIP_WHITESPACE(quoteToken);
        if(sscanf(coreText, "%*[^:]%*[: ]%9s", formatSpecifier))
        {
            char *ptr_fmt = formatSpecifier;
            while(*ptr_fmt != '\0')
            {
                if(*ptr_fmt =='d')
                {
                    int32_t var = (int32_t)atoi(quoteToken);
                    sprintf(coreText, coreText, var);
                    break;
                }
                else if (*ptr_fmt =='f')
                {
                    float var = (float)atof(quoteToken);
                    sprintf(coreText, coreText, var);
                    break;
                }
                ++ptr_fmt;
            }
        }
    }
    // WIDTH CALC
    size_t txtLen = strlen(coreText);
    int32_t txtPxWidth = 0; /** The pixel width of the string */
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if((coreText[itr_text] == '\"') || (coreText[itr_text] == '\n'))
        {
            continue;
        }
        txtPxWidth += gui_get_char_width(fontIndex ,fontSizeIndex, coreText[itr_text]);
    }

    // ALIGNMENT CALC
    int16_t topLeftCol =0; //18
    int16_t topLeftRow =0; //29
    // Horz
    if(alignmentOpt == 0)
    {
        // Calculate where top left would go 
        topLeftCol = (int16_t)(posX-txtPxWidth/2);
    }
    else if(alignmentOpt == 1)
    {
        topLeftCol = (posX);
    }
    else if(alignmentOpt == 2)
    {
        topLeftCol = (posX-txtPxWidth);
    }
    // Vert
    if(vertAlignmentOpt == 0)
    {
        topLeftRow = (int16_t)(posY-fontSize/2);
    }
    else if(vertAlignmentOpt == 1)
    {
        topLeftRow = posY;
    }
    else if(vertAlignmentOpt == 2)
    {
        topLeftRow = posY-fontSize;
    }
    // WRITING LOOP 
    int16_t colPos = topLeftCol;
    int16_t rowPos = topLeftRow;
    for(int itr_text = 0; itr_text < txtLen; itr_text++)
    {
        if(coreText[itr_text] == '\"')
        {
            continue;
        }
        if(coreText[itr_text] == '\n')
        {
            rowPos += fontSize+3; /** ToDo- Add the leading calculation here */
            colPos = topLeftCol;
            continue;
        }
        if(gui_write_char(fontIndex ,fontSizeIndex, rowPos, colPos, bitMap, coreText[itr_text],b_invert) != GUI_OK)
        {
            return GUI_ERR;
        }
        colPos += gui_get_char_width(fontIndex ,fontSizeIndex, coreText[itr_text]);
    }
    // start rendering the bitmap letter by letter indexing position by letter width

    return GUI_OK;
}

uint8_t gui_get_char_width(uint8_t fontNameIdx ,uint8_t fontSizeIdx, char character)
{
    const char *glyphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";
    const char *found  = strchr(glyphs, character);

    const uint8_t *ptr = font_master_list[fontNameIdx].p_sizeArray[fontSizeIdx];
    if((ptr == NULL) || (found == NULL))
    {
        return 0;
    }
    return ptr[found - glyphs];
}

gui_status_t gui_write_char(uint8_t fontNameIdx, uint8_t fontSizeIdx, int16_t row, int16_t col, uint8_t bitMap[ROWS][COLUMNS], char character, bool b_bitMapInvert)
{
    // GETTING LOOP PARAMS
    ////////////////////////
    // Charactor width 
    uint8_t charHeight = font_master_list[fontNameIdx].sizes[fontSizeIdx];
    uint8_t charWidth  = gui_get_char_width(fontNameIdx, fontSizeIdx, character);
    uint8_t arrayWidth = font_master_list[fontNameIdx].p_sizeArray[fontSizeIdx][95];
    if(charWidth == 0)
    {
        help_log("GUI ERROR: ASCII Char '%d' is not supported!", character);
        return GUI_ERR;
    }
    // Char layer index
    const char *glyphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";
    const char *found = strchr(glyphs, character);
    int32_t layerIdx = (found - glyphs);
    // Charactor bitmap pointer 
    uint8_t *p_charBitmap = (uint8_t *)font_master_list[fontNameIdx].p_charBitmaps[fontSizeIdx];
    // Indexing a chacter              

    // PRINTING CHAR 2 BITMAP 
    /////////////////////////
    for(int32_t itr_row = 0; itr_row < charHeight; itr_row++)
    {
        for(int32_t itr_col = 0; itr_col < charWidth; itr_col++)
        {
            // Checking write is within bounds
            if(((itr_row+row) < 0) || ((itr_row+row) >= ROWS)||((itr_col+col)<0) || ((itr_col+col) >= COLUMNS))
            {
                continue;
            }
            //                                                  [               layer             ]  [        row       ]     [ col ]
            bitMap[itr_row+row][itr_col+col] = *(p_charBitmap + layerIdx*(charHeight * arrayWidth) + itr_row*(arrayWidth)  +  itr_col);
            // Invert the chacter if needed
            if(b_bitMapInvert)
            {
                // Inverting the color of the bitmap
                bitMap[itr_row+row][itr_col+col] = ~bitMap[itr_row+row][itr_col+col];
            }
        }
    }
    return GUI_OK;
}

gui_status_t gui_execute_operand(const char *operandObjectString)
{
    if (strncmp(operandObjectString, "<operand>",9 ) != 0) 
    {
        help_log("GUI ERROR: No Operand starting tag found!");
        return GUI_ERR;
    }

    // Bools 
    bool b_haveFoundIf = false; 
    bool b_haveFoundOperation = false;
    bool b_haveFoundArg[2] = {false,false};
    bool b_haveFoundThen = false;
    bool b_isTrue = false; /** Wheather or not the evaluated operation is true or false*/

    int32_t arguments[2] = {0,0};
    while (*operandObjectString != '\0')
    {

        // IF TAG CHECK 
        ////////////////// 
        SKIP_TO(operandObjectString,'<');
        if (!strncmp(operandObjectString, "<if>", 4)) 
        {
            b_haveFoundIf = true;
            // Incrementing past <if> and skipping whitespace
            SKIP_TO_WHITESPACE(operandObjectString);
            SKIP_WHITESPACE(operandObjectString);
            
            // Checking operation is present
            if (!strncmp(operandObjectString, "<operation>", 11)) 
            {
                b_haveFoundOperation = true;
            }
            // Extract operation name 
            char operName[MAX_KEY_LENGTH];
            sscanf(operandObjectString, "<operation>%63[^</]", operName);
            // Going past the operation decleration to the 
            // variable/value decleration
            for(int itr_arg = 0; itr_arg <2; itr_arg ++)
            {
                SKIP_TO_WHITESPACE(operandObjectString);
                SKIP_WHITESPACE(operandObjectString);
                // CHECKING FOR VAR
                if (!strncmp(operandObjectString, "<var>",5)) 
                {
                    char varName[MAX_KEY_LENGTH];
                    // Extracting value 
                    if ((sscanf(operandObjectString, "<var>%63[^</]", varName) == 1)) 
                    {
                        gui_variable_status_t fetchStatus = gui_get_int32_var(varName, &arguments[itr_arg]);
                        if(fetchStatus==GUI_VAR_OK)
                        {
                            b_haveFoundArg[itr_arg] = true;
                        }
                        else
                        {
                            help_log("GUI ERROR: Fail to fetch Var '%s'!", varName);
                            return GUI_ERR;
                        }
                    }
                }
                // CHECKING FOR VALUE
                else if(!strncmp(operandObjectString, "<value>",7)) 
                {
                    if (sscanf(operandObjectString, "<value>%d</value>", &arguments[itr_arg]) == 1) 
                    {
                        b_haveFoundArg[itr_arg] = true;
                    } 
                }
                else
                {
                    break; 
                }
            }
            // Checking all has been found clear
            if(!b_haveFoundOperation||!b_haveFoundArg[0]||!b_haveFoundArg[1])
            {
                if(!b_haveFoundOperation){help_log("GUI ERROR: No Operation tag found");}
                else{help_log("GUI ERROR: Operation Argument missing!");}
                return GUI_ERR; 
            }

            // Evaluating statement 
            if(!strncmp(operName,"\"equal\"",8))
            {
                b_isTrue = (arguments[0] == arguments[1]);
            }
            else if(!strncmp(operName,"\"less-than\"",12))
            {
                b_isTrue = (arguments[0] < arguments[1]);
            }
            else if(!strncmp(operName,"\"greater-than\"",23))
            {
                b_isTrue = (arguments[0] > arguments[1]);
            }
            
            

        }
        // OPERAND END TAG CHECK
        ////////////////////////
        if (!strncmp(operandObjectString, "</operand>", 10)) 
        {
            break;
        }
        // THEN AND ELSES
        /////////////////
        if(b_haveFoundIf)
        {
            // THEN TAG CHECK 
            ////////////////// 
            if (b_isTrue && !strncmp(operandObjectString, "<then>", 6)) 
            {
                b_haveFoundThen = true;
                if (help_operand_do(operandObjectString) != GUI_OK)
                {
                    return GUI_ERR;
                }
               
            }
            else if (!b_isTrue && !strncmp(operandObjectString, "<else>", 6)) 
            {
                if (help_operand_do(operandObjectString) != GUI_OK)
                {
                    return GUI_ERR;
                }
            }
        }


        operandObjectString++;
    }

    if(b_isTrue && !b_haveFoundThen)
    {
        help_log("GUI ERROR: Operand then tag not found!");
        return GUI_ERR; 
    }

    if(!b_haveFoundIf)
    {
        help_log("GUI ERROR: No If tag found!");
        return GUI_ERR; 
    }

    return GUI_OK; 
}
gui_status_t gui_update()
{
    int32_t pageNumber = 0; 
    gui_variable_status_t fetchStatus = gui_get_int32_var("pageIndex", &pageNumber);
    if((pageNumber > pageCount) || (fetchStatus != GUI_VAR_OK))
    {
        if(fetchStatus != GUI_VAR_OK){help_log("GUI ERROR: PageIndex variable could not be found, please define!");}
        if(pageNumber > pageCount){help_log("GUI ERROR: Page '%d' does not exist!", pageNumber);}
        return GUI_ERR;
    }


    // GETTING PAGE INFO 
    ////////////////////
    uint32_t startIndex = 0;
    uint32_t endIndex   = 0;
    gui_status_t pageStatus = gui_get_page_position(pageNumber,&startIndex,&endIndex);
    if(pageStatus != GUI_OK)
    {
        
        return GUI_ERR; 
    }
    // PARSING PAGES 
    ////////////////////
    uint8_t bitMap[ROWS][COLUMNS];
    memset(bitMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    const char*xmlCopy = guiXml + startIndex;
    while(*xmlCopy < endIndex)
    {
        // TEXT TAG CHECK
        /////////////////
        if(!strncmp(xmlCopy, "<text>", 6))
        {
            gui_status_t renderStatus = gui_render_text(bitMap,xmlCopy);
            if(renderStatus != GUI_OK)
            {
                help_log("GUI ERROR: Could not render text on page '%d'", pageNumber);
                return GUI_ERR;
            }
        }
        // BITMAP TAG CHECK 
        ////////////////// 
        else if(!strncmp(xmlCopy, "<bitMap>", 8))
        {
            gui_status_t renderStatus = gui_render_bitmap(bitMap,xmlCopy);
            if(renderStatus != GUI_OK)
            {
                help_log("GUI ERROR: Could not render bit-map on page '%d'", pageNumber);
                return GUI_ERR;
            }
        }
        // LIST TAG CHECK 
        ////////////////// 
        else if(!strncmp(xmlCopy, "<list>", 6))
        {
            gui_status_t renderStatus = gui_render_list(bitMap,xmlCopy);
            if(renderStatus != GUI_OK)
            {
                help_log("GUI ERROR: Could not render list on page '%d'", pageNumber);
                return GUI_ERR;
            }
        }
        // OPERAND TAG CHECK
        ////////////////////
        else if(!strncmp(xmlCopy, "<operand>", 9))
        {
            gui_status_t renderStatus = gui_execute_operand(xmlCopy);
            if(renderStatus != GUI_OK)
            {
                return GUI_ERR;
            }
        }
        else if(!strncmp(xmlCopy, "</page>", 7))
        {
            break;
        }
        SKIP_TO_WHITESPACE(xmlCopy);
        SKIP_TO(xmlCopy,'<');

    }
    bitMapWrite(bitMap, COLUMNS,ROWS);
    return GUI_OK;
}
gui_status_t gui_parse_tag_str(const char *tagString,const char *tagName, char rtnText[MAX_TAG_DATA_LENGTH], bool *b_isFound)
{
    // CREATING START TAG
    char startTag[MAX_TAG_NAME_LENGTH];
    size_t startTagLen = strnlen(tagName,MAX_TAG_NAME_LENGTH)+3;
    snprintf(startTag, startTagLen, "<%s>",tagName);
    //Checking start tag is present
    if(!strncmp(tagString, startTag, startTagLen))
    {
        return GUI_OK;
    }
    const char *startTokens = strstr(tagString,startTag);
    // CREATING END TAG 
    char endTag[MAX_TAG_NAME_LENGTH];
    size_t endTagLen = strnlen(tagName,MAX_TAG_NAME_LENGTH)+4;
    snprintf(endTag, endTagLen, "</%s>",tagName);
    const char *endTokens = strstr(tagString,endTag);
    // OTHER TEMP VARS
    char TxtData[MAX_TAG_DATA_LENGTH];
    // PARSING START TAG
    if(startTokens != NULL)
    {
        *b_isFound = true;
        if(endTokens == NULL)
        {
            return GUI_ERR;
        }
        // CHECKING DATA LENGTH 
        if((endTokens - (startTokens + startTagLen))>MAX_TAG_DATA_LENGTH)
        {
            return GUI_ERR;
        }

        // GETTING LENGTHS 
        size_t startPos = (startTokens - tagString + startTagLen)-1;
        size_t endPos = endTokens - tagString;
        size_t extractedLength = endPos - startPos;
        // EXTRACTING STRING 
        strncpy(TxtData, tagString + startPos, extractedLength);
        TxtData[extractedLength] = '\0'; // Null-terminate the string
        // CHECKING FOR VAR
        const char *varSrtTok = strstr(TxtData,"<var>");
        const char *varEndTok = strstr(TxtData,"</var>");
        if((varSrtTok != NULL))
        {
            // TOKEN POSITION CHECK
            if(varEndTok<=varSrtTok)
            {
                return GUI_ERR;
            }
            // EXTRACTING VAR CONTENTS
            size_t varStartPos = varSrtTok - TxtData + strlen("<var>");
            size_t varEndPos = varEndTok - TxtData;
            size_t varExtractedLength = varEndPos - varStartPos;

            char varTag[MAX_TAG_DATA_LENGTH];
            strncpy(varTag, TxtData + varStartPos, varExtractedLength);
            varTag[varExtractedLength] = '\0'; // Null-terminate the string

            // FETCHING VAR
            // int32
            char replacedTxtData[MAX_TAG_DATA_LENGTH];
            uint32_t valueI = 0;
            gui_variable_status_t fetchStatus32 = gui_get_int32_var(varTag, &valueI);
            if (fetchStatus32 == GUI_VAR_OK)
            {
                snprintf(replacedTxtData, MAX_TAG_DATA_LENGTH, "%.*s%d%s", (int)(varSrtTok - TxtData), TxtData, valueI, varEndTok + strlen("</var>"));
            }
            // float 
            float valueF = 0;
            gui_variable_status_t fetchStatusFlt = gui_get_float_var(varTag, &valueF);
            if (fetchStatusFlt == GUI_VAR_OK) 
            {
                snprintf(replacedTxtData, MAX_TAG_DATA_LENGTH, "%.*s%f%s", (int)(varSrtTok - TxtData), TxtData, valueF, varEndTok + strlen("</var>"));
            }
            // error check 
            if(fetchStatusFlt != GUI_VAR_OK && fetchStatus32 != GUI_VAR_OK)
            {
                return GUI_ERR;
            }            
            
            // Copy the modified string back to TxtData
            strncpy(TxtData, replacedTxtData, MAX_TAG_DATA_LENGTH);
            TxtData[MAX_TAG_DATA_LENGTH - 1] = '\0'; // Ensure null-termination
        }
        // CREATING RETURN STRING 
        snprintf(rtnText, MAX_TAG_DATA_LENGTH, "%s",TxtData);
    }
    return GUI_OK;
}


gui_status_t gui_parse_tag_val(const char *tagString,const char *tagName,char *varName, int32_t *p_value,uint8_t numReturn, bool *b_isFound)
{
    // CREATING START TAG
    char startTag[MAX_TAG_NAME_LENGTH];
    size_t startTagLen = strnlen(tagName,MAX_TAG_NAME_LENGTH)+3;
    snprintf(startTag, startTagLen, "<%s>",tagName);
    //Checking start tag is present
    if(!strncmp(tagString, startTag, startTagLen))
    {
        return GUI_OK;
    }

    const char *startTokens = strstr(tagString,startTag);
     // CREATING END TAG 
    char endTag[MAX_TAG_NAME_LENGTH];
    size_t endTagLen = strnlen(tagName,MAX_TAG_NAME_LENGTH)+4;
    snprintf(endTag, endTagLen, "</%s>",tagName);
    const char *endTokens = strstr(tagString,endTag);
    // OTHER TEMP VARS
    char TxtData[MAX_TAG_DATA_LENGTH];
    char* dataPtr = TxtData;    
    // PARSING START TAG
    if(startTokens != NULL)
    {
        *b_isFound = true;
        if(endTokens == NULL)   
        {
            return GUI_ERR;
        }
        // CHECKING DATA LENGTH 
        if((endTokens - (startTokens + startTagLen))>MAX_TAG_DATA_LENGTH)
        {
            return GUI_ERR;
        }
        // GETTING LENGTHS 
        size_t startPos = (startTokens - tagString + startTagLen)-1;
        size_t endPos = endTokens - tagString;
        size_t extractedLength = endPos - startPos;
        // EXTRACTING VALUE OR VAR STRING 
        strncpy(TxtData, tagString + startPos, extractedLength);
        TxtData[extractedLength] = '\0'; // Null-terminate the string
        
        // START EXTRACTION LOOP 
        for(int itr_var = 0; itr_var <numReturn;itr_var++)
        {
            // CHECKING FOR VAR
            const char *varSrtTok = strstr(dataPtr,"<var>");
            const char *varEndTok = strstr(dataPtr,"</var>");
            if((varSrtTok != NULL))
            {
                // TOLKEN POSITION CHECK
                if(varEndTok<=varSrtTok)
                {
                    return GUI_ERR;
                }
                // EXTRACTING VAR CONTENTS
                size_t varStartPos = varSrtTok - TxtData + strlen("<var>");
                size_t varEndPos = varEndTok - TxtData;
                size_t varExtractedLength = varEndPos - varStartPos;

                strncpy(varName, TxtData + varStartPos, varExtractedLength);
                varName[varExtractedLength] = '\0'; // Null-terminate the string

                // FETCHING VAR
                gui_variable_status_t fetchStatus = GUI_VAR_ERR;
                if(numReturn > 1)
                {
                    fetchStatus = gui_get_int32_var(varName, &p_value[itr_var]);
                }
                else 
                {
                    fetchStatus = gui_get_int32_var(varName, p_value);
                }
                
                if (fetchStatus != GUI_VAR_OK)
                {
                    help_log("GUI ERROR: Variable '%s' does not exist!", varName);
                    return GUI_ERR;
                }
            }
            else
            {
                // TURNING INTO VALUE 
                if(numReturn > 1)
                {
                    p_value[itr_var] = atoi(dataPtr);  
                }
                else 
                {
                    *p_value = atoi(dataPtr);  
                }
            }
            // INCREMENTING POINTER
            if(numReturn>1)
            {
                SKIP_TO(dataPtr, ','); // Skipping to the next comma
                dataPtr++;
            }
        }
        
    }
    return GUI_OK;
}
gui_status_t help_operand_do(const char *operandObjectString)
{
    // Incrementing past <then> and skipping whitespace
    SKIP_TO_WHITESPACE(operandObjectString);
    SKIP_WHITESPACE(operandObjectString);
    // Check for do tag
    char oporName[MAX_KEY_LENGTH];
    if(sscanf(operandObjectString, "<do>%63[^</]", oporName)!=1)
    {
        return GUI_ERR; 
    }
    SKIP_TO_WHITESPACE(operandObjectString);
    SKIP_WHITESPACE(operandObjectString);
    // Check for the var and extract name 
    char varName[MAX_KEY_LENGTH];
    uint32_t varValue = 0;
    // Extracting var name
    if ((sscanf(operandObjectString, "<var>%63[^</]", varName) != 1)) 
    {
        return GUI_ERR; 
    }
    gui_variable_status_t fetchStatus = gui_get_int32_var(varName, &varValue);
    if(fetchStatus!=GUI_VAR_OK)
    {
        return GUI_ERR; 
    }
    SKIP_TO_WHITESPACE(operandObjectString);
    SKIP_WHITESPACE(operandObjectString);
    // Check for the var and extract name 
    int32_t value = 0;
    // Extracting value 
    if (sscanf(operandObjectString, "<value>%d</value>", &value) != 1) 
    {
        char argVarName[MAX_KEY_LENGTH];
        if ((sscanf(operandObjectString, "<var>%63[^</]", argVarName) != 1)) 
        {
            return GUI_ERR; 
        }
        gui_variable_status_t fetchStatus = gui_get_int32_var(argVarName, &value);
        if(fetchStatus!=GUI_VAR_OK)
        {
            return GUI_ERR; 
        }
    } 
    uint32_t returnVal = 0;
    if(!strncmp(oporName,"\"set-equal\"",63))
    {
        returnVal = value;
    }
    else if(!strncmp(oporName,"\"add\"",63))
    {
        returnVal = varValue + value;
    }
    else if(!strncmp(oporName,"\"minus\"",63))
    {
        returnVal = varValue - value;
    }
    else
    {
        help_log("GUI ERROR: do code %s not defined", oporName);
        return GUI_ERR;
    }
    // Writing back to var 
    gui_variable_status_t updateStatus = gui_update_int32_var(varName,returnVal);
    if(updateStatus != GUI_VAR_OK)
    {
        return GUI_ERR;
    }
    return GUI_OK;
}

gui_status_t help_find_font(const char *fontObjectString,bool *b_haveFoundFont,char *fontName,uint8_t *fontIndex)
{
    if (sscanf(fontObjectString, "<font>%63[^<]", fontName) == 1) 
    {
        // check font master list for the font namme 
        for(uint8_t iter_font = 0; iter_font < NUM_FONT_TYPES; iter_font ++)
        {
            if(strncmp(fontName, font_master_list[iter_font].fontName, strlen(fontName)) == 0)
            {
                *fontIndex = iter_font;
                *b_haveFoundFont = true;
                break;
            }
        }
        if(!(*b_haveFoundFont))
        {
            help_log("GUI ERROR: Font name '%s' does not exist!", fontName);
            return GUI_ERR;
        }
        return GUI_OK;
    }
    return GUI_ERR;
}



gui_status_t help_find_font_size(const char *fontSizeObjectString,bool *b_haveFoundFontSize, bool b_haveFoundFont,uint8_t fontIndex,uint8_t *fontSize,uint8_t *fontSizeIndex)
{

    if ((sscanf(fontSizeObjectString, "<font-size>%hhu</font-size>", fontSize) == 1) && !(*b_haveFoundFontSize)) 
    {          
        for(uint8_t iter_fontSize = 0; iter_fontSize < MAX_FONT_SIZES; iter_fontSize ++)
        {
            if(font_master_list[fontIndex].sizes[iter_fontSize] == *fontSize)
            {
                *b_haveFoundFontSize = true;
                *fontSizeIndex = iter_fontSize;
                break;
            }
        }   
    }
    char fontName[MAX_TAG_DATA_LENGTH] = {'\0'};
    if(b_haveFoundFont)
    {
      strcpy(fontName, font_master_list[fontIndex].fontName);
    }
    if(!(*b_haveFoundFontSize))
    {
        help_log("GUI ERROR: Font name '%s' does not exist at size '%d'!", fontName, *fontSize);
        return GUI_ERR;
    }
    return GUI_OK;
}

void help_log(const char *message, ...)
{
    if(logWrite == NULL)
    {
        return;
    }
    char formatted_message[MAX_GUI_LOG_SIZE+1] = {0};
    va_list args;
    va_start(args, message);
    vsnprintf(formatted_message, MAX_GUI_LOG_SIZE, message, args);
    va_end(args);
    logWrite(formatted_message);
}

