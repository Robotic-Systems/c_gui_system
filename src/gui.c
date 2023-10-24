#include "gui.h"

/***********************/
/* PRIVATE DEFINITIONS */
/***********************/

/*****************/
/* PRIVATE TYPES */
/*****************/
typedef struct KeyValuePairUint16
{
    char key[MAX_KEY_LENGTH];
    uint16_t value;
    bool b_isUsed;
} KeyValuePairUint16;

/*********************/
/* PRIVATE VARIABLES */
/*********************/
int16_t pageCount = 0;     /** Number of pages found in xml after parsing*/
int16_t varCount  = 0;     /** Number of Variables found in xml after parsing*/
const char* guiXml = NULL; /** XML string that contains the whole menu system*/
page_params_t pages[MAX_PAGE_COUNT] = {0};

KeyValuePairUint16 HashMapUint16[HASH_MAX_VARS];  /** Hash map for uint16_t menu variables*/


/*********************************/
/* PRIVATE FUNCTION DECLARATIONS */
/*********************************/
uint32_t hash_index(const char * key); /** Generates a hash index from a key */
// hash_update(map, 'key', value)

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
    uint32_t varStartIdx __attribute__((unused)) = {0}; // Position where the last variable tag was found 
    uint32_t varEndIdx   __attribute__((unused)) = {0}; // Position where the last variable end tag was found 
    const char *nameSrtIdx  = {0}; // Position where the last name start tag was found 
    const char *nameEndIdx  = {0}; // Position where the last name end tag was found 
    const char *valueSrtIdx = {0}; // Position where the last value start tag was found 
    const char *valueEndIdx = {0}; // Position where the last value end tag was found 
    const char *typeSrtIdx  = {0}; // Position where the last type start tag was found 
    const char *typeEndIdx  = {0}; // Position where the last type end tag was found 

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
    const char*xmlCopy = guiXml;
    while (*xmlCopy != '\0') {


        // VARIABLE TAG CHECK 
        ///////////////////// 
        if (strncmp(xmlCopy, "<variable>", 10) == 0) 
        {
            varStartIdx = currentIndex + 9;
            b_isInVar   = true;
        }
        else if (strncmp(xmlCopy, "</variable>", 11) == 0) 
        {
            if (b_isInVar) 
            {
                varEndIdx = currentIndex;
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

gui_status_t gui_render_bitmap(uint8_t bitMap[COLUMNS][ROWS],const char *bitmapString)
{
    const char*strBitmap = bitmapString;
    if (strncmp(strBitmap, "<bitMap>", 8) != 0) 
    {
        return GUI_VAR_ERR;
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
            if (sscanf(strBitmap, "<position>%hd,%hd</position>", &posX, &posY) == 2) 
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
    // Row iterator 
    for(uint16_t itr_row = posX; itr_row < height; itr_row++)
    {
        for(uint16_t itr_col = posY; itr_col < width; itr_col++)
        {
            // Skipping white-space chars 
            while (*strBitmap == ' ' || *strBitmap == '\n' || *strBitmap == ',' || *strBitmap == '\r' || *strBitmap == '\t') 
            {
                strBitmap++;
            }
            // Extracting ith data point 
            if (sscanf(strBitmap, "%hhd", &bitMap[itr_row][itr_col]) != 1) 
            {
                return GUI_ERR;   
            }
            strBitmap++;
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
    
