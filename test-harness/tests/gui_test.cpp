#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../spies/lcd_spy.h"
    #include "gui.h"
    #include "../test_xml/test_xml.h"
    #include "../test_xml/test_error_xml.h"
    #include "../test_xml/test_bitmaps.h"
}
TEST_GROUP(GUITest) 
{
    void setup()
    {
        lcd_spy_init();
        gui_init(lcd_spy_write, zeroGui);
    }

    void teardown()
    {
    }



    #define IS_LCD_EQUAL_TO(num) do { \
        uint8_t currentFrame[ROWS][COLUMNS] = {0}; \
            lcd_spy_get_Frame(currentFrame); \
            for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
                for (int ith_row = 0; ith_row < ROWS; ith_row++) { \
                    LONGS_EQUAL_TEXT(num, currentFrame[ith_row][ith_col], "MISMATCH"); \
                } \
            } \
        } while (0)
    
    #define IS_BIT_EQUAL_TO_UP_TO(num,bitMap,row,col) do { \
            for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
                for (int ith_row = 0; ith_row < ROWS; ith_row++) { \
                     if((ith_row >= row) && (ith_col >= col)){\
                      break;\
                     }\
                    char str[64]; \
                    snprintf(str, 64, "MISMATCH ON Row: %d, Col: %d", ith_row, ith_col); \
                    LONGS_EQUAL_TEXT(num, bitMap[ith_row][ith_col], str); \
                } \
            } \
        } while (0)


    #define IS_BIT_MAP_EQUAL_BIT(bitMap, mainMap,posY,posX,width,height) do { \
        for (int ith_row = posY; ith_row < (height+posY); ith_row++) { \
            for (int ith_col = posX; ith_col < (width+posX); ith_col++) { \
                char str[64]; \
                if((ith_row < 0) || (ith_row >= ROWS)||(ith_col<0) || (ith_col >= COLUMNS)){\
                    continue;\
                }\
                snprintf(str, 64, "MISMATCH ON Row: %d, Col: %d, %d != %d", ith_row, ith_col, bitMap[ith_row-posY][ith_col-posX], mainMap[ith_row][ith_col]); \
                LONGS_EQUAL_TEXT(bitMap[ith_row-posY][ith_col-posX], mainMap[ith_row][ith_col], str); \
            } \
        } \
    } while (0)
    #define IS_BIT_REG_EQUAL_REG(bitMap, mainMap,posY,posX,width,height) do { \
        for (int ith_row = posY; ith_row < (height+posY); ith_row++) { \
            for (int ith_col = posX; ith_col < (width+posX); ith_col++) { \
                char str[64]; \
                if((ith_row < 0) || (ith_row >= ROWS)||(ith_col<0) || (ith_col >= COLUMNS)){\
                    continue;\
                }\
                snprintf(str, 64, "MISMATCH ON Row: %d, Col: %d, %d != %d", ith_row, ith_col, bitMap[ith_row][ith_col], mainMap[ith_row][ith_col]); \
                LONGS_EQUAL_TEXT(bitMap[ith_row][ith_col], mainMap[ith_row][ith_col], str); \
            } \
        } \
    } while (0)
    #define PRINT_BIT_MAP(rows, cols, bitMap) do { \
        printf("\n");\
        for (int ith_row = 0; ith_row < rows; ith_row++) { \
            for (int ith_col = 0; ith_col < cols; ith_col++) { \
                printf("%3d, ",bitMap[ith_row][ith_col]); \
            } \
            printf("\n");\
        } \
    } while (0)

    #define PRINT_ROW(rowNum, bitMap1, bitMap2)\
        printf("\n");\
        for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
            printf("%d: %d, %d \n",ith_col, bitMap1[ith_row][ith_col], bitMap2[ith_row][ith_col]); \
        } \
        printf("\n");\


    #define IS_LCD_EQUAL_BIT(num) do { \
        uint8_t currentFrame[ROWS][COLUMNS] = {0}; \
            lcd_spy_get_Frame(currentFrame); \
            for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
                for (int ith_row = 0; ith_row < ROWS; ith_row++) { \
                    LONGS_EQUAL_TEXT(num[ith_row][ith_col], currentFrame[ith_row][ith_col], "MISMATCH"); \
                } \
            } \
        } while (0)
  

};

/********/
/* ZERO */
/********/
TEST(GUITest, passing_null_as_write_function_causes_error)
{
    // init gui with null 
    gui_status_t initStatus = gui_init(NULL, zeroGui);
    LONGS_EQUAL(GUI_ERR,initStatus);
}

TEST(GUITest, passing_null_as_xml_causes_error)
{
    // init gui with null 
    gui_status_t initStatus = gui_init(lcd_spy_write, NULL);
    LONGS_EQUAL(GUI_ERR,initStatus);
}

TEST(GUITest, passing_in_inputs_causes_no_error)
{
    // init gui
    gui_status_t initStatus = gui_init(lcd_spy_write, zeroGui);
    LONGS_EQUAL(GUI_OK,initStatus);
}

TEST(GUITest, on_init_no_frame_has_been_written)
{
    // init gui 
    gui_init(lcd_spy_write, zeroGui);
    // Check spy state undefined 
    IS_LCD_EQUAL_TO(LCD_UNDEFINED);
}

TEST(GUITest, when_passed_a_xml_with_no_pages_the_page_count_is_0)
{
    // init gui 
    gui_init(lcd_spy_write, zeroGui);
    // Check spy state undefined 
    int16_t pageCount = gui_get_page_count();
    LONGS_EQUAL(0,pageCount);
}

TEST(GUITest, when_passed_a_xml_with_no_variables_the_variable_count_is_0)
{
    // init gui 
    gui_init(lcd_spy_write, zeroGui);
    // Check spy state undefined 
    int16_t varCount = gui_get_variable_count();
    LONGS_EQUAL(0,varCount);
}

/**
 * Zero ToDo:
 */

/********/
/* ONE  */
/********/
TEST(GUITest, when_passed_a_xml_two_pages_the_page_count_is_two)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Check page count
    int16_t pageCount = gui_get_page_count();
    LONGS_EQUAL(2,pageCount);
}

// after init the gui_get_page_position(0) can be called and will return the index when the start and end of page 0 is 
TEST(GUITest, gui_get_page_position_returns_page_0_start_and_end)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Check page count
    uint32_t startIndex = 0;
    uint32_t endIndex = 0;
    gui_get_page_position(0,&startIndex,&endIndex);

    CHECK_EQUAL('>',helloWorldGui[startIndex]);
    CHECK_EQUAL('<',helloWorldGui[endIndex]);

    LONGS_EQUAL(197,startIndex);
    LONGS_EQUAL(3780,endIndex);
}

TEST(GUITest, gui_get_page_position_returns_page_1_start_and_end)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Check page count
    uint32_t startIndex = 0;
    uint32_t endIndex = 0;
    gui_get_page_position(1,&startIndex,&endIndex);

    CHECK(helloWorldGui[startIndex] == '>');
    CHECK(helloWorldGui[endIndex] == '<');

    LONGS_EQUAL(3797,startIndex);
    LONGS_EQUAL(4107,endIndex);
}

// if gui_get_page_position is called on page that does not exist then error is returned 
TEST(GUITest, error_if_page_doesnt_exist)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // get status
    uint32_t startIndex = 0;
    uint32_t endIndex = 0;
    gui_status_t getStatus = gui_get_page_position(10,&startIndex,&endIndex);
    LONGS_EQUAL(GUI_ERR, getStatus);
}

// when passed a xml with one variables the variable count is set to 1
TEST(GUITest, one_xml_defined_var_increments_var_count_by_one)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Check page count 
    int16_t varCount = gui_get_variable_count();
    LONGS_EQUAL(1,varCount);
}

// can call gui_variable_create() and created an uint16 variable 
TEST(GUITest, gui_var_init_can_be_used_to_create_uint16_variables)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Create variable definition strings
    char lastName[64]  = "testVar"; 
    char lastValue[64] = "10"; 
    char lastType[64]  = "int32_t";  
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_OK, createStatus);
    // Check operation was successful 
    int32_t value = 0;
    gui_variable_status_t fetchStatus = gui_get_int32_var(lastName, &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    LONGS_EQUAL(10, value);
}
// after init the page index variable exists and is set to 0
TEST(GUITest, variable_exists_and_is_set_to_its_default)
{
    // init gui 
    gui_init(lcd_spy_write, singleVarGui);
    // Fetch variable value 
    int32_t value = 0; // set to non zero value 
    gui_variable_status_t fetchStatus = gui_get_int32_var("pageIndex", &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    LONGS_EQUAL(55, value);
}

// the page index variable can be changed using gui_variable_update("pageIndex",10)
TEST(GUITest, uint16_variables_can_be_updated)
{
    // init gui 
    gui_init(lcd_spy_write, singleVarGui);
    // Update Variable 
    gui_update_int32_var("pageIndex",1);
    // Fetch variable value 
    int32_t value = 0; // set to non zero value 
    gui_variable_status_t fetchStatus = gui_get_int32_var("pageIndex", &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    LONGS_EQUAL(1, value);
}

TEST(GUITest, feeding_a_garbled_type_causes_error)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Create variable definition strings
    char lastName[64]  = "testVar"; 
    char lastValue[64] = "10"; 
    char lastType[64]  = "uint6_t";  // Create garbled type
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_ERR, createStatus);
}

TEST(GUITest, feeding_in_a_value_with_name_greater_then_max_causes_error)
{
    // Create variable definition strings
    char lastName[64]  = "ghjopasd;lkfjas;ldkfjas;ldfkja;sldfkjas;dlfkjas"; 
    char lastValue[64] = "10"; 
    char lastType[64]  = "int32_t";  // Create garbled type
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_ERR, createStatus);
}

TEST(GUITest, if_initted_with_xml_with_a_page_with_no_closing_brace_then_error_is_thrown)
{
    // init gui 
    gui_status_t initStatus = gui_init(lcd_spy_write, noPageEndBrace);
    LONGS_EQUAL(GUI_INIT_PGE_BRACE, initStatus);
}
TEST(GUITest, if_initted_with_an_xml_with_only_a_page_closing_brace_then_error_is_thrown )
{
    // init gui 
    gui_status_t initStatus = gui_init(lcd_spy_write, noPageStrtBrace);
    LONGS_EQUAL(GUI_INIT_PGE_BRACE, initStatus);
}
TEST(GUITest, if_initted_with_xml_with_a_variable_with_no_closing_brace_then_error_is_thrown )
{
    // init gui 
    gui_status_t initStatus = gui_init(lcd_spy_write, noVarEndBrace);
    LONGS_EQUAL(GUI_INIT_VAR_BRACE, initStatus);
}
TEST(GUITest, if_initted_with_an_xml_with_only_a_variable_closing_brace_then_error_is_thrown  )
{
    // init gui 
    gui_status_t initStatus = gui_init(lcd_spy_write, noVarStrtBrace);
    LONGS_EQUAL(GUI_INIT_VAR_BRACE, initStatus);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ONE: BIT-MAP RENDER
//////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(GUITest, bitmaps_can_be_rendered_using_render_bitmap)
{
    // Get bitmap string 
    const char* strBitMapCopy = justABitmap;
    // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 99, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus = gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that bitmaps match 
    IS_BIT_MAP_EQUAL_BIT(beautifulBitMap,outputMap,0,0,32,32);
}

TEST(GUITest, if_no_start_bitMap_tag_is_found_returns_bitmap_error)
{
    // Get bitmap string 
    const char* strBitMapCopy = justABitmap_startBraceErr;
    // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 99, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus = gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, if_no_end_bitMap_tag_is_found_returns_bitmap_error)
{
    // Get bitmap string 
    const char* strBitMapCopy = justABitmap_closeBraceErr;
    // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 99, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus = gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, if_no_position_is_found_then_error_is_returned)
{
    // Get bitmap string 
    const char* strBitMapCopy = justABitmap_positionBraceErr;
    // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 99, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus = gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, if_no_size_is_found_then_error_is_returned)
{
    // Get bitmap string 
    const char* strBitMapCopy = justABitmap_positionBraceErr;
    // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 99, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus = gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, if_gui_render_bitmap_finds_a_non_bitmapable_chacter_then_returns_error)
{
    // Get bitmap string 
    const char* strBitMapCopy = justABitmap_randomChar;
    // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 99, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus = gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

// bitmaps can be located partially on screen using negitive or overflow positions
TEST(GUITest, bit_maps_can_be_renders_above_and_to_left_of_top_left_of_screen)
{
    // Get bitmap string 
    const char* strBitMapCopy = ABitmap_negitivePosition;
        // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 6, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus =  gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that partial bitmaps match 
    // PRINT_BIT_MAP(ROWS, COLUMNS, outputMap);
    IS_BIT_MAP_EQUAL_BIT(beautifulBitMap,outputMap,-12,-5,32,32);
}

TEST(GUITest, bit_maps_can_be_renders_below_and_to_right_of_screen)
{
    // Get bitmap string 
    const char* strBitMapCopy = ABitmap_postivePosition;
    // Create Empty 2D array
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 6, COLUMNS * ROWS * sizeof(uint8_t));
    // Call gui_render_bitmap 
    gui_status_t renderStatus =  gui_render_bitmap(outputMap,strBitMapCopy);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that partial bitmaps match 
    // PRINT_BIT_MAP(ROWS, COLUMNS, outputMap);
    IS_BIT_MAP_EQUAL_BIT(beautifulBitMap,outputMap,49,87,32,32);
}

TEST(GUITest, bit_maps_have_no_artifacts_when_written)
{
    // Fetch bitmap that it offset so a single one is in frame
    const char* strBitMapCopy = ABitmap_singleOne;
    // Render the bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // PRINT_BIT_MAP(ROWS, COLUMNS, outputMap);
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // PRINT_BIT_MAP(ROWS, COLUMNS, outputMap);
    gui_status_t renderStatus =  gui_render_bitmap(outputMap,strBitMapCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check the rest is the defualt value 
    // PRINT_BIT_MAP(ROWS, COLUMNS, outputMap);
    // Check that one is there
    IS_BIT_MAP_EQUAL_BIT(simpleBitMap,outputMap,62,100,2,2);
    IS_BIT_EQUAL_TO_UP_TO(0,outputMap,62,100);
}

/* <bitMap> rendering tests
* 
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ONE: TEXT RENDER 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(GUITest, text_render_does_not_return_error_when_xml_is_errorless)
{
     const char* strTextCopy = text_HelloWorld;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_no_text_starting_brace_found)
{
     const char* strTextCopy = text_HelloWorld_error_start_brace;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}


TEST(GUITest, text_render_returns_error_when_no_font_found)
{
    const char* strTextCopy = text_HelloWorld_error_no_font;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_no_font_size)
{
    const char* strTextCopy = text_HelloWorld_error_no_font_size;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_no_alignment)
{
    const char* strTextCopy = text_HelloWorld_error_no_alignment;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_no_position)
{
    const char* strTextCopy = text_HelloWorld_error_no_position;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_no_content)
{
    const char* strTextCopy = text_HelloWorld_error_no_content;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_font_does_not_exist)
{
    const char* strTextCopy = text_HelloWorld_error_font_does_not_exist;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_font_size_not_exist)
{
    const char* strTextCopy = text_HelloWorld_error_font_size_does_not_exist;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, text_render_returns_error_when_alignment_is_not_defined)
{
    const char* strTextCopy = text_HelloWorld_error_alignment_does_not_exist;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, get_char_width_returns_corect_char_width_for_c)
{
    // Get the width of x 
    uint8_t c_width =  gui_get_char_width(1,0, 'c');
    // Check that width matches expectation 
    LONGS_EQUAL(7,c_width);

}

TEST(GUITest, get_char_width_returns_corect_char_width_for_whole_glyph_set)
{
    char glyphs[96] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";
    int32_t widthIndex = 0;
    uint8_t widthArray[95] = {
    0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x03,   0x03,   0x07,   0x03,   0x0B,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x0B,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x0B,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x0B,   0x07,   0x07,   0x07,   0x07,   0x03,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x04,   0x0B,   0x03,   0x0E,   0x09,   0x07,   0x07,   0x07,   0x07,   0x09,   0x05,   0x05,   0x07,   0x07,   0x07,   0x07,   0x06,   0x06,   0x06,   0x06,   0x03,   0x03,   0x03,   0x03,   0x07,   0x06,   0x03,   0x03,   0x07,   0x06,   0x06,   0x07
    };


    for (const char* text = glyphs; *text != '\0'; ++text) {
        uint8_t c_width = gui_get_char_width(1, 0, *text);
        // Check that width matches expectation
        char str[64];
        snprintf(str, 64, "MISMATCH CHAR %c, %d != %d ",  *text,widthArray[widthIndex],c_width ); 
        LONGS_EQUAL_TEXT(widthArray[widthIndex], c_width,str);
        widthIndex +=1;
    }


}

TEST(GUITest, unknown_chacters_return_0_for_width)
{
   // Get the width of x 
    uint8_t c_width =  gui_get_char_width(1,0, 127);
    // Check that width matches expectation 
    LONGS_EQUAL(0,c_width);

}


TEST(GUITest, unknown_chacter_returns_error)
{
    // Create blank bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Write a character in top left corner 
    gui_status_t writeStatus = gui_write_char(1,0,0,0,outputMap,127,false);
    LONGS_EQUAL(GUI_ERR,writeStatus);
}



TEST(GUITest, chacter_can_be_written_to_bitmap)
{
    // Create blank bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Write a character in top left corner 
    gui_status_t writeStatus = gui_write_char(1,0,0,0,outputMap,'A',false);
    LONGS_EQUAL(GUI_OK,writeStatus);
    // Check that width matches expectation clear
    uint8_t (*layer)[14] = juipiter_fontMap[26];
    // PRINT_BIT_MAP(19,14,layer);
    // PRINT_BIT_MAP(19,14,outputMap);
    IS_BIT_MAP_EQUAL_BIT(layer,outputMap,0,0,14,19);
}

TEST(GUITest, question_can_be_written_to_bitmap)
{
    // Create blank bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Write a character in top left corner 
    gui_status_t writeStatus = gui_write_char(1,0,0,0,outputMap,'?',false);
    LONGS_EQUAL(GUI_OK,writeStatus);
    // Check that width matches expectation clear
    uint8_t (*layer)[14] = juipiter_fontMap[94];
    // PRINT_BIT_MAP(19,14,layer);
    // PRINT_BIT_MAP(19,14,outputMap);
    IS_BIT_MAP_EQUAL_BIT(layer,outputMap,0,0,14,19);
}

TEST(GUITest, every_glyf_can_be_written_to_bitmap)
{
    char glyphs[96] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";

    for(int i = 0; i<95; i++)
    {
        // Create blank bitmap 
        uint8_t outputMap[ROWS][COLUMNS];
        memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
        // Write a character in top left corner 
        gui_status_t writeStatus = gui_write_char(1,0,0,0,outputMap,glyphs[i],false);
        LONGS_EQUAL(GUI_OK,writeStatus);
        // Check that width matches expectation clear
        uint8_t (*layer)[14] = juipiter_fontMap[i];
        IS_BIT_MAP_EQUAL_BIT(layer,outputMap,0,0,14,19);
    }
}

TEST(GUITest, can_partially_render_chars_overflow)
{
    // Create blank bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Write a character in top left corner 
    gui_status_t writeStatus = gui_write_char(1,0,54,90,outputMap,'A',false);
    LONGS_EQUAL(GUI_OK,writeStatus);
    // Check that width matches expectation clear
    uint8_t (*layer)[14] = juipiter_fontMap[26];
    IS_BIT_MAP_EQUAL_BIT(layer,outputMap,54,90,14,19);
}

TEST(GUITest, can_partially_render_chars_underflow)
{
    // Create blank bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Write a character in top left corner 
    gui_status_t writeStatus = gui_write_char(1,0,-2,-2,outputMap,'A',false);
    LONGS_EQUAL(GUI_OK,writeStatus);
    // Check that width matches expectation clear
    uint8_t (*layer)[14] = juipiter_fontMap[26];
    IS_BIT_MAP_EQUAL_BIT(layer,outputMap,-2,-2,14,19);
}

TEST(GUITest, can_render_center_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // PRINT_BIT_MAP(64,102,helloWorld_19_juipeter); 
    // PRINT_BIT_MAP(64,102,outputMap);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter,outputMap,0,0,102,64);
}

TEST(GUITest, can_render_text_elements_left)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_left;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly

    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_left,outputMap,0,0,102,64);
}

TEST(GUITest, can_render_text_elements_right)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_right;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    // PRINT_BIT_MAP(64,102,outputMap);
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_right,outputMap,0,0,102,64);
}

TEST(GUITest, can_render_multiline_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_multiline;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_multiline,outputMap,0,0,102,64);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ONE: PAGE RENDER 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(GUITest, rendering_with_page_with_no_errors_does_not_return_error)
{
    // init gui clear
    gui_init(lcd_spy_write, helloWorldGui);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_OK, renderStatus);
}

TEST(GUITest, changing_to_page_that_does_not_exist_causes_error)
{
    // init gui clear
    gui_init(lcd_spy_write, helloWorldGui);
    // Change page number to be out of bounds
    gui_update_int32_var("pageIndex", 10);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, if_page_index_does_not_exist_then_error_is_returned)
{
    // init gui clear
    gui_init(lcd_spy_write, helloWorldGui_no_page_index);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, attempting_to_render_text_with_error_returns_error)
{
    // init gui clear
    gui_init(lcd_spy_write, helloWorldGui_text_error);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, attempting_to_render_bitmap_with_error_returns_error)
{
    // init gui clear
    gui_init(lcd_spy_write, helloWorldGui_bitmap_error);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, pages_with_bitmap_can_be_written_to_screen)
{
    // init gui clear
    gui_init(lcd_spy_write, helloWorldGui);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that bitmaps match 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    lcd_spy_get_Frame(outputMap);
    // PRINT_BIT_MAP(64,102,outputMap);
    IS_BIT_MAP_EQUAL_BIT(beautifulBitMap,outputMap,0,0,32,32);
}

TEST(GUITest, pages_with_text_can_be_written_to_screen)
{
    // init gui clear
    gui_init(lcd_spy_write, helloWorldGui);
    gui_update_int32_var("pageIndex", 1);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that bitmaps match 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    lcd_spy_get_Frame(outputMap);
    // PRINT_BIT_MAP(64,102,outputMap);
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter,outputMap,0,0,102,64);
}
/**
 * <page>
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ONE: OPERANDS 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(GUITest, passing_operand_with_no_error_returns_ok)
{
    const char* strTextCopy = operand_equal;
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
}

TEST(GUITest, if_missing_operand_tag_throws_error)
{
    const char* strTextCopy = operand_equal_missing_opening_flag;
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
    // Check value same as when created
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(10, value);
}

TEST(GUITest, if_missing_if_tag_throws_error)
{
    const char* strTextCopy = operand_equal_missing_if_tag;
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
    // Check value same as when created
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(10, value);
}

TEST(GUITest, if_missing_operation_tag_throws_error)
{
    const char* strTextCopy = operand_equal_missing_operation_tag;
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
    // Check value same as when created
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(10, value);
}

TEST(GUITest, if_missing_var_tag_throws_error)
{
    const char* strTextCopy = operand_equal_missing_var_tag;
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
    // Check value same as when created
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(10, value);
}

TEST(GUITest, if_missing_value_tag_throws_error)
{
    const char* strTextCopy = operand_equal_missing_value_tag;
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
    // Check value same as when created
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(10, value);
}

TEST(GUITest, if_var_dne_throws_error)
{
    const char* strTextCopy = operand_equal;
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
}

TEST(GUITest, comparing_two_vars_does_not_throw_error)
{
    const char* strTextCopy = operand_equal_two_var;
    // Perform Operation 
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    gui_create_var("test2","int32_t","10");
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
    
}

TEST(GUITest, if_statement_true_and_then_not_found_throws_error)
{
    const char* strTextCopy = operand_equal_no_then;
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
    // Check value same as when created
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(10, value);
}

TEST(GUITest, if_statement_true_and_then_is_found_operation_takes_place)
{
    const char* strTextCopy = operand_equal;
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
    // Check var now equals 2
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(2, value);
}

TEST(GUITest, if_statement_false_and_else_is_not_found__no_operation_takes_place)
{
    const char* strTextCopy = operand_equal_no_else;
    gui_create_var("test1","int32_t","9");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
    // Check var now equals 2
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(9, value);
}

TEST(GUITest, if_statement_false_and_else_is_found_operation_takes_place)
{
    const char* strTextCopy = operand_equal;
    gui_create_var("test1","int32_t","9");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
    // Check var now equals 2
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(6, value);
}

TEST(GUITest, if_statement_true_and_then_one_var_can_be_set_to_value_of_anoter)
{
    const char* strTextCopy = operand_equal_dual_var_out;
    gui_create_var("test1","int32_t","10");
    gui_create_var("test2","int32_t","50");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
    // Check var now equals 
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(50, value);
}

TEST(GUITest, if_statement_false_then_one_var_can_be_set_to_value_of_anoter)
{
    const char* strTextCopy = operand_equal_dual_var_out;
    gui_create_var("test1","int32_t","11");
    gui_create_var("test2","int32_t","60");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
    // Check var now equals 
    int32_t value = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(60, value);
}

TEST(GUITest, if_true_multiple_thens_can_take_place)
{
    const char* strTextCopy = operand_equal_many_thens;
    gui_create_var("test1","int32_t","10");
    gui_create_var("test2","int32_t","60");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, operationStatus);
    // Check var now equals 
    int32_t value = 0;
    int32_t value2 = 0;
    gui_get_int32_var("test1", &value);
    LONGS_EQUAL(60, value);
    gui_get_int32_var("test2", &value2);
    LONGS_EQUAL(22, value2);

}

TEST(GUITest, if_end_operand_tag_hit_loop_breaks)
{
    const char* strTextCopy = operand_equal_early_stop;
    // Create the var used in test case
    gui_create_var("test1","int32_t","10");
    // Perform Operation 
    gui_status_t operationStatus =  gui_execute_operand(strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_ERR, operationStatus);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// MANY: PAGES 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(GUITest, can_render_calbri_center_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter,outputMap,0,0,102,64);
}


TEST(GUITest, can_render_many_text_elements_per_page)
{
    // init gui clear
    gui_init(lcd_spy_write, advanced_gui);
    // Update to set the first frame 
    gui_status_t renderStatus = gui_update();
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that all text elements were rendered 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    lcd_spy_get_Frame(outputMap);
    // PRINT_BIT_MAP(64,102,outputMap);
    // PRINT_BIT_MAP(64,102,ready_19_juipeter_T_L_0_0);
    IS_BIT_REG_EQUAL_REG(ready_19_juipeter_T_L_0_0,outputMap,0,0,20,19);
    IS_BIT_REG_EQUAL_REG(service_due_19_juipeter_T_L_30_0,outputMap,30,0,102,19);
}
/**
 * <page>
 * - Additional options, Variable refresh rates/partial screen refreshes 
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// MANY: TEXT RENDER 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(GUITest, get_char_width_returns_corect_char_width_for_whole_glyph_set_sans)
{
    char glyphs[96] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";
    int32_t widthIndex = 0;
    uint8_t widthArray_size12[95] = {
    0x07,   0x08,   0x06,   0x08,   0x07,   0x06,   0x07,   0x08,   0x04,   0x05,   0x08,   0x04,   0x0C,   0x08,   0x07,   0x08,   0x08,   0x06,   0x06,   0x05,   0x08,   0x07,   0x0B,   0x07,   0x07,   0x06,   0x09,   0x08,   0x08,   0x09,   0x07,   0x07,   0x09,   0x09,   0x04,   0x05,   0x08,   0x07,   0x0B,   0x0A,   0x0A,   0x08,   0x0A,   0x08,   0x07,   0x07,   0x09,   0x08,   0x0C,   0x08,   0x08,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x07,   0x03,   0x07,   0x07,   0x03,   0x0B,   0x08,   0x07,   0x0B,   0x07,   0x09,   0x07,   0x04,   0x04,   0x04,   0x06,   0x07,   0x07,   0x04,   0x04,   0x05,   0x05,   0x07,   0x03,   0x03,   0x03,   0x05,   0x06,   0x03,   0x03,   0x05,   0x07,   0x07,   0x06
    };


    for (const char* text = glyphs; *text != '\0'; ++text) {
        uint8_t c_width = gui_get_char_width(0, 0, *text);
        // Check that width matches expectation
        char str[64];
        snprintf(str, 64, "MISMATCH CHAR %c, %d != %d ",  *text,widthArray_size12[widthIndex],c_width ); 
        LONGS_EQUAL_TEXT(widthArray_size12[widthIndex], c_width,str);
        widthIndex +=1;
    }
}


TEST(GUITest, every_glyf_can_be_written_to_bitmap_sans)
{
    char glyphs[96] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[]{}|;':\\\",./<>?";

    for(int i = 0; i<95; i++)
    {
        // Create blank bitmap 
        uint8_t outputMap[ROWS][COLUMNS];
        memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
        // Write a character in top left corner 
        gui_status_t writeStatus = gui_write_char(0,0,0,0,outputMap,glyphs[i],false);
        LONGS_EQUAL(GUI_OK,writeStatus);
        // Check that width matches expectation clear
        uint8_t (*layer)[12] = sans_fontMap_size12[i];
        // PRINT_BIT_MAP(12,12,outputMap);
        // PRINT_BIT_MAP(12,12,layer);
        IS_BIT_MAP_EQUAL_BIT(layer,outputMap,0,0,12,12);
    }
}
// Can use more then one font per gui
TEST(GUITest, can_render_center_text_new_font)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_sans;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    // PRINT_BIT_MAP(64,102,helloWorld_12_sans);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_12_sans,outputMap,0,0,102,64);
}

TEST(GUITest, if_no_verticle_alignment_returns_error)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_error_vert_alignment_does_not_exist;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    // PRINT_BIT_MAP(64,102,helloWorld_12_sans);
    LONGS_EQUAL(GUI_ERR, renderStatus);
}

TEST(GUITest, verticle_alignment_can_be_used_to_align_to_top)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_top;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_center_top,outputMap,0,0,102,64);
}

TEST(GUITest, verticle_alignment_can_be_used_to_align_to_bottom)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_bottom;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_center_bottom,outputMap,0,0,102,64);
}

TEST(GUITest, text_can_be_inverted)
{
    // Create blank bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Write a character in top left corner 
    gui_status_t writeStatus = gui_write_char(1,0,0,0,outputMap,'A',true);
    LONGS_EQUAL(GUI_OK,writeStatus);
    IS_BIT_MAP_EQUAL_BIT(invertedA_juipter_19,outputMap,0,0,7,19);
}


TEST(GUITest, hello_world_can_be_inverted)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_invert;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    // PRINT_BIT_MAP(64,102,helloWorld_19_juipeter_inverted);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_inverted,outputMap,0,0,84,19);
}

TEST(GUITest, variables_can_be_used_to_not_invert_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_var_invert;
    gui_create_var("invert","int32_t","0");
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_not_inverted,outputMap,0,0,84,19);
}

TEST(GUITest, variables_can_be_used_to_invert_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_var_invert;
    gui_create_var("invert","int32_t","1");
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_inverted,outputMap,0,0,84,19);
}

TEST(GUITest, numbers_can_be_printed_in_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_one;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    // PRINT_BIT_MAP(64,102,helloWorld_19_juipeter_one);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_one,outputMap,0,0,84,19);
}


// GUI parse tag
TEST(GUITest, passing_gui_parse_tag_an_errorless_xml_returns_ok)
{
    // Define string to test 
    const char * stringOfTag = "<content>\"one: \%d\",1</content>\n";
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL(GUI_OK, parseStatus);
}


TEST(GUITest, passing_a_string_that_does_not_contain_start_tag_returns_boolean_false)
{
    // Define string to test 
    const char * stringOfTag = "\"one: \%d\",1</content>\n";
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL(GUI_OK, parseStatus);
    LONGS_EQUAL(false, is_found);
}

TEST(GUITest, passing_a_string_that_does_contain_start_tag_returns_boolean_true)
{
    // Define string to test 
    const char * stringOfTag = "<content>\"one: \%d\",1</content>\n";
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL(GUI_OK, parseStatus);
    LONGS_EQUAL(true, is_found);
}


TEST(GUITest, if_end_tag_is_not_found_returns_error)
{
    // Define string to test 
    const char * stringOfTag = "<content>\"one: \%d\",1\n";
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL(GUI_ERR, parseStatus);
    LONGS_EQUAL(true, is_found);
}

TEST(GUITest, gui_parse_tag_returns_text_between_tags)
{
    // Define string to test 
    const char * stringOfTag = "<content>Its a string!</content>\n";
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
    STRCMP_EQUAL_TEXT("Its a string!", text, "TEXT MISMATCH");
}

TEST(GUITest, if_a_var_is_refrenced_in_string_that_dne_error_returned)
{
    // Define string to test 
    const char * stringOfTag = "<content>\"one: \%d\",<var>test</var></content>\n";
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL_TEXT(GUI_ERR, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
}

TEST(GUITest, if_a_var_is_refrenced_in_string_the_value_is_returned_in_str)
{
    // Define string to test 
    const char * stringOfTag = "<content>\"one: \%d\",<var>test</var></content>\n";
    gui_create_var("test","int32_t","1");
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
    STRCMP_EQUAL_TEXT("\"one: \%d\",1", text, "TEXT MISMATCH");
}
// Error is returned if end tag is greater then MAX_TAG_DATA length away
TEST(GUITest, error_returned_if_end_tag_is_more_then_max_len_away)
{
    // Create string longer then max data length 
    const char * stringOfTag = "<content>adfasdfasdfasdfasdfasdfasdfasdfsdafsdfsadfasdfasdfasdfasdfsadfasdffasdfsdaASASDasdSDAdsasadsda</content>\n";
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check for returned error 
    LONGS_EQUAL_TEXT(GUI_ERR, parseStatus,"STATUS CODE MISMATCH");
}
// Error is returned if end var tolken found before start  
TEST(GUITest, error_if_var_end_tolken_found_first)
{
    // Define string to test 
    const char * stringOfTag = "<content>\"one: \%d\",</var>test<var></content>\n";
    gui_create_var("test","int32_t","1");
    const char* tagName = "content";
    char text[MAX_TAG_DATA_LENGTH];
    bool is_found = false;
    // Call gui parse tag 
    gui_status_t parseStatus = gui_parse_tag_str(stringOfTag,tagName,text,&is_found);
    // Check there is no error
    LONGS_EQUAL_TEXT(GUI_ERR, parseStatus,"STATUS CODE MISMATCH");
}
// // Variables can be printed in text
TEST(GUITest, variables_can_be_printed_in_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_one_var;
    gui_create_var("test1","int32_t","1");
    // Create eclearmpty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    // PRINT_BIT_MAP(64,102,helloWorld_19_juipeter_one);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_one,outputMap,0,0,84,19);
}

TEST(GUITest, passing_gui_parse_tag_val_an_errorless_xml_returns_ok)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert>1</invert>\n";
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);

    LONGS_EQUAL(GUI_OK, parseStatus);
}

TEST(GUITest, passing_a_string_that_does_not_contain_start_tag_returns_boolean_false_var)
{
    // Create string to parse 
    const char* strTextCopy =  "1</invert>\n";
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(false, is_found, "BOOLEAN MISMATCH");
}

TEST(GUITest, passing_a_string_that_does_contain_start_tag_returns_boolean_true_var)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert>1</invert>\n";
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
}

TEST(GUITest, if_end_tag_is_not_found_returns_error_var)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert>1\n";
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_ERR, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
}

TEST(GUITest, gui_parse_tag_returns_value_between_tags)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert>1</invert>\n";
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
    LONGS_EQUAL_TEXT(1, value, "VALUE");
}

TEST(GUITest, if_a_var_is_refrenced_in_val_that_dne_error_returned_var)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert><var>test</var></invert>\n";
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_ERR, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
}

TEST(GUITest, if_a_var_is_refrenced_in_val_the_value_is_returned)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert><var>test</var></invert>\n";
    gui_create_var("test","int32_t","1");
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
    LONGS_EQUAL_TEXT(1, value, "VALUE");
}

TEST(GUITest, error_returned_if_end_tag_is_more_then_max_len_away_var)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert><var>test</var>                                                                                                                </invert>\n";
    gui_create_var("test","int32_t","1");
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_ERR, parseStatus,"STATUS CODE MISMATCH");

}

TEST(GUITest, error_if_var_end_tolken_found_first_var)
{
    // Create string to parse 
    const char* strTextCopy =  "<invert></var>test<var></invert>\n";
    gui_create_var("test","int32_t","1");
    // Call gui parse tag 
    int32_t value = 0;
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"invert",&value,1,&is_found);
    LONGS_EQUAL_TEXT(GUI_ERR, parseStatus,"STATUS CODE MISMATCH");
}

TEST(GUITest, can_be_used_to_return_two_values)
{
    // Create string to parse 
    const char* strTextCopy =  "<position>32,51</position>\n";
    gui_create_var("test","int32_t","1");
    // Call gui parse tag 
    int32_t value[2] = {0};
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"position",value,2,&is_found);
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
    // printf("%d\n", value[0]);
    // printf("%d\n", value[1]);
    LONGS_EQUAL_TEXT(32, value[0], "VALUE");
    LONGS_EQUAL_TEXT(51, value[1], "VALUE");
}

TEST(GUITest, can_be_used_to_return_one_vars)
{
    // Create string to parse 
    const char* strTextCopy =  "<position><var>test</var>,51</position>\n";
    gui_create_var("test","int32_t","99");
    // Call gui parse tag 
    int32_t value[2] = {0};
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"position",value,2,&is_found);
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
    LONGS_EQUAL_TEXT(99, value[0], "VALUE");
    LONGS_EQUAL_TEXT(51, value[1], "VALUE");
}

TEST(GUITest, can_be_used_to_return_two_vars)
{
    // Create string to parse 
    const char* strTextCopy =  "<position><var>test1</var>,<var>test2</var></position>\n";
    gui_create_var("test1","int32_t","99");
    gui_create_var("test2","int32_t","88");
    // Call gui parse tag 
    int32_t value[2] = {0};
    bool is_found = false;
    gui_status_t parseStatus = gui_parse_tag_val(strTextCopy,"position",value,2,&is_found);
    LONGS_EQUAL_TEXT(GUI_OK, parseStatus,"STATUS CODE MISMATCH");
    LONGS_EQUAL_TEXT(true, is_found, "BOOLEAN MISMATCH");
    LONGS_EQUAL_TEXT(99, value[0], "VALUE");
    LONGS_EQUAL_TEXT(88, value[1], "VALUE");
}


TEST(GUITest, text_position_can_be_set_using_variables_and_position_can_be_changed_)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_moveable;
    // Create variables 
    gui_create_var("xPos","int32_t","32");
    gui_create_var("yPos","int32_t","51");
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // PRINT_BIT_MAP(64,102,helloWorld_19_juipeter); 
    // PRINT_BIT_MAP(64,102,outputMap);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter,outputMap,0,0,102,64);
    // MOVING AND CHECKING AGAIN 
    gui_update_int32_var("xPos",13);
    gui_update_int32_var("yPos",-2);
    // Reset bitmap 
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check it matches 
    // PRINT_BIT_MAP(64,102,helloWorld_19_juipeter_moved_off); 
    // PRINT_BIT_MAP(64,102,outputMap);
    LONGS_EQUAL(GUI_OK, renderStatus);

    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_moved_off,outputMap,0,0,102,64);
}

TEST(GUITest, can_render_just_a_var)
{
    // just a var text 
    const char* strTextCopy = just_one_var;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render 
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    // PRINT_BIT_MAP(64,102,outputMap);
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check matches expectation 
    IS_BIT_MAP_EQUAL_BIT(one_19_juipeter_T_L_0_0,outputMap,0,0,102,64);

}
/**
 * <text>
 * - Can set default font at the start of the pages
 * - Can set default alighment at start of pages 
 * - Can set default vert alighment at start of pages 
 * - Can set default font size at start of pages 
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// MANY: HASHMAP 
//////////////////////////////////////////////////////////////////////////////////////////////////////////


TEST(GUITest, gui_var_init_does_not_throw_error_with_float)
{
    // Create variable definition strings
    char lastName[64]  = "testVar"; 
    char lastValue[64] = "10"; 
    char lastType[64]  = "float";  
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_OK, createStatus);
}

TEST(GUITest, after_creation_float_vars_can_be_queryed_without_error)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Create variable definition strings
    char lastName[64]  = "testVar"; 
    char lastValue[64] = "10.22"; 
    char lastType[64]  = "float";  
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_OK, createStatus);
    // Checking the value is default 
    float value = 0; 
    gui_variable_status_t fetchStatus = gui_get_float_var("testVar", &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
}


TEST(GUITest, after_creation_float_vars_can_be_queryed_and_return_value)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Create variable definition strings
    char lastName[64]  = "testVar"; 
    char lastValue[64] = "10.22"; 
    char lastType[64]  = "float";  
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_OK, createStatus);
    // Checking the value is default 
    float value = 0; 
    gui_variable_status_t fetchStatus = gui_get_float_var("testVar", &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    DOUBLES_EQUAL(10.22, value,.1);
    
}

TEST(GUITest, float_vars_can_be_updated)
{
    // init gui 
    gui_init(lcd_spy_write, helloWorldGui);
    // Create variable definition strings
    char lastName[64]  = "testVar"; 
    char lastValue[64] = "10.22"; 
    char lastType[64]  = "float";  
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_OK, createStatus);
    // Updsating var 
    gui_variable_status_t updateStatus = gui_update_float_var("testVar",-2.2);
    LONGS_EQUAL(GUI_VAR_OK, updateStatus);
    // Checking the value is default 
    float value=0;
    gui_variable_status_t fetchStatus = gui_get_float_var("testVar", &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    DOUBLES_EQUAL(-2.2, value,.1);
 
}

TEST(GUITest, floats_can_be_printed_in_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_one_pt_2;
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay 
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_one_pt_2,outputMap,0,0,102,64);
}

TEST(GUITest, float_vars_can_be_printed_in_text)
{
    // Fetch the xml text extract 
    const char* strTextCopy = text_HelloWorld_one_pt_2_var;
        // Call variable create 
    gui_variable_status_t createStatus = gui_create_var("test1","float","1.2");
    LONGS_EQUAL(GUI_VAR_OK, createStatus);
    // Create empty bitmap 
    uint8_t outputMap[ROWS][COLUMNS];
    memset(outputMap, 0, COLUMNS * ROWS * sizeof(uint8_t));
    // Render text
    gui_status_t renderStatus =  gui_render_text(outputMap,strTextCopy);
    // Check status is okay clear
    LONGS_EQUAL(GUI_OK, renderStatus);
    // Check that text rendered correctly 
    IS_BIT_MAP_EQUAL_BIT(helloWorld_19_juipeter_one_pt_2,outputMap,0,0,102,64);
}

/**
 * <variable>
 * - Hash table can handle collisions through Open Addressing
 * - Can't create two variables of same name 
 * - variables max and min can be set clear
 * make

 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// MANY: BIT-MAP RENDER 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * <bitMaps>
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// MANY: OPERANDS 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * - Instead of just setting the value equal to another can do things like increment or deincrement the 
 * value by one 
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// OTHER 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * - Make error codes more highly specific 
 * - if not pages brace exists then no pages are created 
 * - if a page exists outside the <pages> tag an error is thrown 
 * - if a variable exists outside the <variables> tag an error is thrown  
 * - Calling gui_update() when page number has not changed does not change the bitmap written to spy 
 * - add in logger output support for xml formating checks and feedback 
 */