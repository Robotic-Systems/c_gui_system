#include "gui.h"

/***********************/
/* PRIVATE DEFINITIONS */
/***********************/
#define SKIP_WHITESPACE(str) \
    do { \
        while (*str == ' ' || *str == '\n' || *str == ',' || *str == '\r' || *str == '\t') { \
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
        while (*str != char) { \
            str++; \
        } \
    } while (0)
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
KeyValuePairUint16 HashMapUint16[HASH_MAX_VARS];  /** Hash map for uint16_t menu variables*/
extern font_list_t font_master_list[NUM_FONT_TYPES]; 
/*****************************/
/* PRIVATE FUNCTION POINTERS */
/*****************************/
write_function bitMapWrite = NULL; /** Function pointer that can be used to write to the display*/
/*********************************/
/* PRIVATE FUNCTION DECLARATIONS */
/*********************************/
uint32_t hash_index(const char * key);                   /** Generates a hash index from a key */
gui_status_t help_set_var_equal(const char *operandObjectString); /** Sets the var name it finds first equal to the next value or var it finds*/
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
    bitMapWrite = p_lcdWrite;
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
            SKIP_WHITESPACE(strBitmap);
            // Extracting ith data point 
            uint8_t bit = 0;

            if (sscanf(strBitmap, "%hhd", &bit) != 1) 
            {
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
    uint8_t fontSize  = {0};
    uint8_t fontSizeIndex  = {0};
    uint8_t fontIndex = 0;
    int16_t posX      = {0}; /** What column to place text in  */
    int16_t posY      = {0}; /** What row to place text in*/
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
    bool b_invert   = false; 

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
                        fontSizeIndex = iter_fontSize;
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
                        alignmentOpt = txtAlignmentDic[iter_txtOpt].alignmentEnum;
                        break;
                    }
                }   
            }
            if(!b_haveFoundAlignment)
            {
                return GUI_ERR;
            }
        }

        // VERT-ALIGNMENT TAG CHECK 
        ///////////////////////////
        if (strncmp(textObjectString, "<vert-alignment>", 16) == 0) 
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
        if (strncmp(textObjectString, "<position>", 10) == 0) 
        {
            if (sscanf(textObjectString, "<position>%hd,%hd</position>", &posY, &posX) == 2) 
            {
                b_haveFoundPosition = true;
            } 
        }


        // INVERT TAG CHECK 
        //////////////////////
        if (strncmp(textObjectString, "<invert>", 8) == 0) 
        {

            uint16_t invertInt = 0;
            // Skip to >
            SKIP_TO(textObjectString,'>');
            if (!strncmp(textObjectString, "><var>",5)) 
            {
                char varName[MAX_KEY_LENGTH];
                // Extracting value 
                if ((sscanf(textObjectString, "><var>%63[^</]", varName) == 1)) 
                {
                    gui_variable_status_t fetchStatus = gui_get_uint16_var(varName, &invertInt);
                    if(fetchStatus!=GUI_VAR_OK)
                    {
                        return GUI_ERR;
                    }
                }
            }
            // Assume its a value 
            else 
            {
                // Inverting via value 
                if (sscanf(textObjectString, ">%hd</invert>", &invertInt) != 1) 
                {
                    return GUI_ERR;
                }
            } 
    
            b_invert = (invertInt>0);
        }

        // CONTENT TAG CHECK 
        //////////////////////
        gui_status_t contentStatus = gui_parse_tag_str(textObjectString,"content",text,&b_haveFoundContent);
        if(contentStatus != GUI_OK)
        {
            return contentStatus;
        }
        
        textObjectString++;
    }
    
    
    if(!b_haveFoundFont || !b_haveFoundFontSize || !b_haveFoundAlignment || !b_haveFoundPosition || !b_haveFoundContent || !b_haveFoundVertAlignment)
    {
        return GUI_ERR;
    }
    // CHECK IF TEXT CONTAINS VARS
    // Core text 
    char *quoteToken = strtok(text, "\"");
    char coreText[64];
    uint16_t var = 0;
    if(quoteToken != NULL)
    {
      strncpy(coreText, quoteToken, sizeof(coreText) - 1);
    }
    // Variable 
    quoteToken = strtok(NULL, "\"");
    if(quoteToken != NULL)
    {
        SKIP_WHITESPACE(quoteToken);
        var = (uint16_t)atoi(quoteToken);
    }
    // ToDo: remove this snprintf 
    sprintf(coreText, coreText, var);

    // WIDTH CALC
    size_t txtLen = strlen(coreText);
    uint16_t txtPxWidth = 0; /** The pixel width of the string */
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
            printf("Render Error \n");
            printf(">>%c<< \n",coreText[itr_text] );
            return GUI_ERR;
        }
        colPos += gui_get_char_width(fontIndex ,fontSizeIndex, coreText[itr_text]);
        // printf("%c,%d ,%d  \n", text[itr_text],gui_get_char_width(fontIndex ,fontSizeIndex, text[itr_text]),colPos );
    }
    // start rendering the bitmap letter by letter indexing position by letter width

    return GUI_OK;
}

uint8_t gui_get_char_width(uint8_t fontNameIdx ,uint8_t fontSizeIdx, char character)
{
    const char *glyphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";
    const char *found  = strchr(glyphs, character);

    uint8_t *ptr = font_master_list[fontNameIdx].p_sizeArray[fontSizeIdx];
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
        return GUI_ERR;
    }
    // Char layer index
    const char *glyphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";
    const char *found = strchr(glyphs, character);
    uint16_t layerIdx = (found - glyphs);
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

gui_status_t gui_update()
{
    uint16_t pageNumber = 0; // set to non zero value 
    gui_variable_status_t fetchStatus = gui_get_uint16_var("pageIndex", &pageNumber);
    if((pageNumber > pageCount) || (fetchStatus != GUI_VAR_OK))
    {
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
        //////////////////
        if(!strncmp(xmlCopy, "<text>", 6))
        {
            gui_status_t renderStatus = gui_render_text(bitMap,xmlCopy);
            if(renderStatus != GUI_OK)
            {
                return GUI_ERR;
            }
        }
        // BITMAP TAG CHECK 
        ////////////////// 
        if(!strncmp(xmlCopy, "<bitMap>", 8))
        {
            gui_status_t renderStatus = gui_render_bitmap(bitMap,xmlCopy);
            if(renderStatus != GUI_OK)
            {
                return GUI_ERR;
            }
        }

        // OPERAND TAG CHECK
        ////////////////////
        if(!strncmp(xmlCopy, "<operand>", 9))
        {
            gui_status_t renderStatus = gui_execute_operand(xmlCopy);
            if(renderStatus != GUI_OK)
            {
                return GUI_ERR;
            }
        }

        xmlCopy++;
        if(!strncmp(xmlCopy, "</page>", 7))
        {
            break;
        }
    }
    bitMapWrite(bitMap, COLUMNS,ROWS);
    return GUI_OK;
}

gui_status_t gui_execute_operand(const char *operandObjectString)
{
    if (strncmp(operandObjectString, "<operand>",9 ) != 0) 
    {
        return GUI_ERR;
    }

    // Bools 
    bool b_haveFoundIf = false; 
    bool b_haveFoundOperation = false;
    bool b_haveFoundArg[2] = {false,false};
    bool b_haveFoundThen = false;
    bool b_isTrue = false; /** Wheather or not the evaluated operation is true or false*/

    uint16_t arguments[2] = {0,0};
    while (*operandObjectString != '\0')
    {

        // IF TAG CHECK 
        ////////////////// 
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
                        gui_variable_status_t fetchStatus = gui_get_uint16_var(varName, &arguments[itr_arg]);
                        if(fetchStatus==GUI_VAR_OK)
                        {
                            b_haveFoundArg[itr_arg] = true;
                        }
                    }
                }
                // CHECKING FOR VALUE
                else if(!strncmp(operandObjectString, "<value>",7)) 
                {
                    if (sscanf(operandObjectString, "<value>%hd</value>", &arguments[itr_arg]) == 1) 
                    {
                        b_haveFoundArg[itr_arg] = true;
                    } 
                }
                else
                {
                    break; 
                }
            }
            // Checking all has been found 
            if(!b_haveFoundOperation||!b_haveFoundArg[0]||!b_haveFoundArg[1])
            {
                return GUI_ERR; 
            }

            // Evaluating statement 
            b_isTrue = (arguments[0] == arguments[1]);
            

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
                if (help_set_var_equal(operandObjectString) != GUI_OK)
                {
                    return GUI_ERR;
                }
               
            }
            else if (!b_isTrue && !strncmp(operandObjectString, "<else>", 6)) 
            {
                if (help_set_var_equal(operandObjectString) != GUI_OK)
                {
                    return GUI_ERR;
                }
            }
        }


        operandObjectString++;
    }

    if(b_isTrue && !b_haveFoundThen)
    {
        return GUI_ERR; 
    }

    if(!b_haveFoundIf)
    {
        return GUI_ERR; 
    }

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
    uint16_t value = 0;
    char TxtData[MAX_TAG_DATA_LENGTH];
    // PARSING START TAG
    if(startTokens != NULL)
    {
        *b_isFound = true;
        if(endTokens == NULL)
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
        if(varSrtTok != NULL)
        {
            // EXTRACTING VAR CONTENTS
            size_t varStartPos = varSrtTok - TxtData + strlen("<var>");
            size_t varEndPos = varEndTok - TxtData;
            size_t varExtractedLength = varEndPos - varStartPos;

            char varTag[MAX_TAG_DATA_LENGTH];
            strncpy(varTag, TxtData + varStartPos, varExtractedLength);
            varTag[varExtractedLength] = '\0'; // Null-terminate the string

            // FETCHING VAR
            gui_variable_status_t fetchStatus = gui_get_uint16_var(varTag, &value);
            if (fetchStatus != GUI_VAR_OK) {
                return GUI_ERR;
            }

            // Replace the <var>...</var> with the fetched value
            char replacedTxtData[MAX_TAG_DATA_LENGTH];
            snprintf(replacedTxtData, MAX_TAG_DATA_LENGTH, "%.*s%d%s", (int)(varSrtTok - TxtData), TxtData, value, varEndTok + strlen("</var>"));
            
            // Copy the modified string back to TxtData
            strncpy(TxtData, replacedTxtData, MAX_TAG_DATA_LENGTH);
            TxtData[MAX_TAG_DATA_LENGTH - 1] = '\0'; // Ensure null-termination
        }
        // CREATING RETURN STRING 
        snprintf(rtnText, MAX_TAG_DATA_LENGTH, "%s",TxtData);
        

    }
    return GUI_OK;
}

gui_status_t help_set_var_equal(const char *operandObjectString)
{
    // Incrementing past <then> and skipping whitespace
    SKIP_TO_WHITESPACE(operandObjectString);
    SKIP_WHITESPACE(operandObjectString);
    // Check for the var and extract name 
    char varName[MAX_KEY_LENGTH];
    // Extracting value 
    if ((sscanf(operandObjectString, "<var>%63[^</]", varName) != 1)) 
    {
        return GUI_ERR; 
    }
    SKIP_TO_WHITESPACE(operandObjectString);
    SKIP_WHITESPACE(operandObjectString);
    // Check for the var and extract name 
    uint16_t value = 0;
    // Extracting value 
    if (sscanf(operandObjectString, "<value>%hd</value>", &value) != 1) 
    {
        char argVarName[MAX_KEY_LENGTH];
        if ((sscanf(operandObjectString, "<var>%63[^</]", argVarName) != 1)) 
        {
            return GUI_ERR; 
        }
        gui_variable_status_t fetchStatus = gui_get_uint16_var(argVarName, &value);
        if(fetchStatus!=GUI_VAR_OK)
        {
            return GUI_ERR; 
        }
    } 

    gui_variable_status_t updateStatus = gui_update_uint16_var(varName,value);
    if(updateStatus != GUI_VAR_OK)
    {
        return GUI_ERR;
    }
    return GUI_OK;
}
