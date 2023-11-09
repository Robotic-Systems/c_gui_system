#include "gui.h"

/***********************/
/* PRIVATE DEFINITIONS */
/***********************/

/*****************/
/* PRIVATE TYPES */
/*****************/
/**
 * @brief Structure for the uint16_t Hashmap 
*/
typedef struct KeyValuePairUint16
{
    char key[MAX_KEY_LENGTH];
    uint16_t value;
    bool b_isUsed;
} KeyValuePairUint16;


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
/*********************/
/* PRIVATE VARIABLES */
/*********************/
int16_t pageCount = 0;     /** Number of pages found in xml after parsing*/
int16_t varCount  = 0;     /** Number of Variables found in xml after parsing*/
const char* guiXml = NULL; /** XML string that contains the whole menu system*/
page_params_t pages[MAX_PAGE_COUNT] = {0};
KeyValuePairUint16 HashMapUint16[HASH_MAX_VARS];  /** Hash map for uint16_t menu variables*/
extern font_list_t font_master_list[NUM_FONT_TYPES]; 

/*********************************/
/* PRIVATE FUNCTION DECLARATIONS */
/*********************************/
uint32_t hash_index(const char * key); /** Generates a hash index from a key */

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
gui_status_t gui_init(write_function p_lcdWrite, const char* xmlString)
{
    if(p_lcdWrite == NULL || xmlString == NULL)
    {
        return GUI_ERR;
    }
    
    guiXml = xmlString;
    pageCount = 0;
    varCount  = 0;

   

    // Initalise hashmap 
    for (int i = 0; i < HASH_MAX_VARS; i++) {
        strncpy(HashMapUint16[i].key, "Nothing", MAX_KEY_LENGTH - 1);
        HashMapUint16[i].b_isUsed = false;
        HashMapUint16[i].value = 0;
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
            }else{return GUI_INIT_VAR_BRACE;}
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
            }else{return GUI_INIT_PGE_BRACE;}
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
        return GUI_INIT_PGE_BRACE;
    }
    if(b_isInVar)
    {
        return GUI_INIT_VAR_BRACE;
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
    *p_endIndex   = pages[pageNumber].endIndex;
    return GUI_OK;
}

gui_variable_status_t gui_create_var(const char *variableName,const char *variableType,const char *variableValue)
{
    if(strlen(variableName)>MAX_KEY_LENGTH-1)
    {
        return GUI_VAR_ERR;
    }
    uint32_t index = hash_index(variableName);

    if (strncmp(variableType, "uint16_t", 9) == 0) {
        // Extracting value 
        uint16_t value = (uint16_t)atoi(variableValue);
        // Linear probing
        while (HashMapUint16[index].b_isUsed) 
        {
            index = (index + 1) % HASH_MAX_VARS;
        }
        // Statically allocated memory
        HashMapUint16[index].b_isUsed = true;
        strncpy(HashMapUint16[index].key, variableName, MAX_KEY_LENGTH - 1);
        HashMapUint16[index].key[MAX_KEY_LENGTH - 1] = '\0'; // Null-terminate the string
        HashMapUint16[index].value = value;
        return GUI_VAR_OK;
    }

    return GUI_VAR_ERR;
}

gui_variable_status_t gui_get_uint16_var(const char *variableKey,uint16_t *p_value)
{
    uint32_t index = hash_index(variableKey);
    while (index < HASH_MAX_VARS)
     {
        if (HashMapUint16[index].b_isUsed && strcmp(HashMapUint16[index].key, variableKey) == 0) {
            *p_value = HashMapUint16[index].value;
            return GUI_VAR_OK;
        }
        index++;
    }
    return GUI_VAR_ERR; // Return GUI_VAR_ERR if variable is not found
}
    
gui_variable_status_t gui_update_uint16_var(const char *variableKey,uint16_t value)
{
    uint32_t index = hash_index(variableKey);
    while (index < HASH_MAX_VARS)
     {
        if (HashMapUint16[index].b_isUsed && strcmp(HashMapUint16[index].key, variableKey) == 0) {
            HashMapUint16[index].value = value;
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
        return GUI_ERR;
    }

    // Creating loop variables 
    uint16_t width  = {0}; /** Width of bitmap in pixels */
    uint16_t height = {0}; /** Height of bitmap in pixels */
    int16_t posX    = {0}; /** What column to place the top left corner of bitmap in  */
    int16_t posY    = {0}; /** What row to place the top left corner of bitmap in  */
    bool b_haveFoundSize = false; 
    bool b_haveFoundPosi = false; 
    while (*strBitmap != '\0')
    {
        // SIZE TAG CHECK 
        ////////////////// 
        if (strncmp(strBitmap, "<size>", 6) == 0) 
        {
            if (sscanf(strBitmap, "<size>%hu,%hu</size>", &width, &height) == 2) 
            {
                b_haveFoundSize = true;
            }
        }

        // POSITION TAG CHECK 
        ///////////////////// 
        else if (strncmp(strBitmap, "<position>", 10) == 0) 
        {
            if (sscanf(strBitmap, "<position>%hd,%hd</position>", &posY, &posX) == 2) 
            {
                b_haveFoundPosi = true;
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
                return GUI_ERR;
            }
        }
        strBitmap++;
    }

    // Extracting bitmap data 
    strBitmap += 6;
    // Position & Height Logic 
    // Row iterator 
    for(int32_t itr_row = posY; itr_row < (height+posY); itr_row++)
    {
        for(int32_t itr_col = posX; itr_col < (width+posX); itr_col++)
        {
            // Skipping white-space chars 
            while (*strBitmap == ' ' || *strBitmap == '\n' || *strBitmap == ',' || *strBitmap == '\r' || *strBitmap == '\t') 
            {
                strBitmap++;
            }
            // Extracting ith data point 
            uint8_t bit = 0;

            if (sscanf(strBitmap, "%hhd", &bit) != 1) 
            {
                // char destination[11];  // Destination buffer to hold the copied characters
                // strncpy(destination, strBitmap, 10);
                // printf("%s \n", destination);
                // printf("PosX = %d \n", posX);
                // printf("PosY = %d \n", posY);
                // printf("[row, col] = [%d, %d] \n", itr_row,itr_col);
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
    while (*strBitmap == ' ' || *strBitmap == '\n' || *strBitmap == ',' || *strBitmap == '\r' || *strBitmap == '\t') 
    {
        strBitmap++;
    }
    if (strncmp(strBitmap, "</data>", 7) == 0) 
    {
        strBitmap+=7;
    }
    while (*strBitmap == ' ' || *strBitmap == '\n' || *strBitmap == ',' || *strBitmap == '\r' || *strBitmap == '\t') 
    {
        strBitmap++;
    }
    if (strncmp(strBitmap, "</bitMap>", 9) == 0) 
    {
        return GUI_OK;
    }
    return GUI_ERR;
}
    

gui_status_t gui_render_text(uint8_t bitMap[ROWS][COLUMNS],const char *textObjectString)
{
    if (strncmp(textObjectString, "<text>", 6) != 0) 
    {
        return GUI_ERR;
    }


    // Creating loop variables 
    char fontName[MAX_TAG_DATA_LENGTH] = {'\0'};
    uint8_t fontSize = {0};
    uint8_t fontIndex = 0;
    // uint8_t alignmentOpt = 0;
    bool b_haveFoundFont      = false; 
    bool b_haveFoundFontSize  = false; 
    bool b_haveFoundAlignment = false; 
    bool b_haveFoundPosition  = false; 
    bool b_haveFoundContent   = false; 


    while (*textObjectString != '\0')
    {
        // FONT TAG CHECK 
        ////////////////// 
        if (strncmp(textObjectString, "<font>", 6) == 0) 
        {
            if (sscanf(textObjectString, "<font>%63[^<]", fontName) == 1) 
            {
                // check font master list for the font namme 
                for(uint8_t iter_font = 0; iter_font < NUM_FONT_TYPES; iter_font ++)
                {
                    if(strncmp(fontName, font_master_list[iter_font].fontName, strlen(fontName)) == 0)
                    {
                        fontIndex = iter_font;
                        b_haveFoundFont = true;
                        break;
                    }
                }
                if(!b_haveFoundFont)
                {
                    return GUI_ERR;
                }
                
            }
        }
        // FONT-SIZE TAG CHECK 
        //////////////////////
        if (strncmp(textObjectString, "<font-size>", 11) == 0) 
        {
            if ((sscanf(textObjectString, "<font-size>%hhu</font-size>", &fontSize) == 1)&&b_haveFoundFont) 
            {
                for(uint8_t iter_fontSize = 0; iter_fontSize < MAX_FONT_SIZES; iter_fontSize ++)
                {
                    if(font_master_list[fontIndex].sizes[iter_fontSize] == fontSize)
                    {
                        b_haveFoundFontSize = true;
                        break;
                    }
                }   
            }
            if(!b_haveFoundFontSize)
            {
                return GUI_ERR;
            }
        }

        // ALIGNMENT TAG CHECK 
        //////////////////////
        if (strncmp(textObjectString, "<alignment>", 11) == 0) 
        {
            char alignmentName[MAX_TAG_DATA_LENGTH] = {'\0'};
            if ((sscanf(textObjectString, "<alignment>%63[^<]", alignmentName) == 1)&&b_haveFoundFont) 
            {
                for(uint8_t iter_txtOpt = 0; iter_txtOpt < TEXT_ALIGNMENT_OPTIONS; iter_txtOpt ++)
                {
                    if(strncmp(txtAlignmentDic[iter_txtOpt].alignmentName, alignmentName, strlen(fontName)) == 0)
                    {
                        b_haveFoundAlignment = true;
                        // alignmentOpt = txtAlignmentDic[iter_txtOpt].alignmentEnum;
                        break;
                    }
                }   
            }
            if(!b_haveFoundAlignment)
            {
                return GUI_ERR;
            }
        }

        // POSITION TAG CHECK 
        //////////////////////
        if (strncmp(textObjectString, "<position>", 10) == 0) 
        {
            b_haveFoundPosition = true;
        }

        // CONTENT TAG CHECK 
        //////////////////////
        if (strncmp(textObjectString, "<content>", 9) == 0) 
        {
            b_haveFoundContent = true;
        }
        textObjectString++;
    }

    if(b_haveFoundFont && b_haveFoundFontSize && b_haveFoundAlignment && b_haveFoundPosition && b_haveFoundContent)
    {
        return GUI_OK;
    }

    // Calculate string width 
    
    // Using alignment tag, width and position, work out where top left corner needs to go 

    // start rendering the bitmap letter by letter indexing position by letter width

    return GUI_ERR;
}

uint8_t gui_get_char_width(uint8_t fontNameIdx ,uint8_t fontSizeIdx, char character)
{
    const char *glyphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\",./<>?";
    const char *found = strchr(glyphs, character);

    uint8_t *ptr = font_master_list[fontNameIdx].p_sizeArray[fontSizeIdx];
    if((ptr == NULL) || (found == NULL))
    {
        return 0;
    }
    return ptr[found - glyphs];
}
