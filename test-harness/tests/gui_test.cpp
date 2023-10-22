#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../spies/lcd_spy.h"
    #include "gui.h"
    #include "../test_xml/test_xml.h"
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
        uint8_t currentFrame[COLUMNS][ROWS] = {0}; \
            lcd_spy_get_Frame(currentFrame); \
            for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
                for (int ith_row = 0; ith_row < ROWS; ith_row++) { \
                    LONGS_EQUAL_TEXT(num, currentFrame[ith_col][ith_row], "MISMATCH"); \
                } \
            } \
        } while (0)

    #define IS_BIT_MAP_EQUAL_BIT(num1, num2) do { \
        for (int ith_row = 0; ith_row < ROWS; ith_row++) { \
            for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
                char str[64]; \
                snprintf(str, 64, "MISMATCH ON Row: %d, Col: %d", ith_row, ith_col); \
                LONGS_EQUAL_TEXT(num1[ith_col][ith_row], num2[ith_col][ith_row], str); \
            } \
        } \
    } while (0)

    #define PRINT_BIT_MAP(rows, cols, bitMap) do { \
        printf("\n");\
        for (int ith_row = 0; ith_row < ROWS; ith_row++) { \
            for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
                printf("%d, ",bitMap[ith_col][ith_row]); \
            } \
            printf("\n");\
        } \
    } while (0)

    #define IS_LCD_EQUAL_BIT(num) do { \
        uint8_t currentFrame[COLUMNS][ROWS] = {0}; \
            lcd_spy_get_Frame(currentFrame); \
            for (int ith_col = 0; ith_col < COLUMNS; ith_col++) { \
                for (int ith_row = 0; ith_row < ROWS; ith_row++) { \
                    LONGS_EQUAL_TEXT(num[ith_col][ith_row], currentFrame[ith_col][ith_row], "MISMATCH"); \
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

    CHECK(helloWorldGui[startIndex] == '>');
    CHECK(helloWorldGui[endIndex] == '<');

    LONGS_EQUAL(198,startIndex);
    LONGS_EQUAL(3918,endIndex);
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

    LONGS_EQUAL(3935,startIndex);
    LONGS_EQUAL(4192,endIndex);
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
    char lastType[64]  = "uint16_t";  
    // Call variable create 
    gui_variable_status_t createStatus = gui_create_var(lastName,lastType,lastValue);
    LONGS_EQUAL(GUI_VAR_OK, createStatus);
    // Check operation was successful 
    uint16_t value = 0;
    gui_variable_status_t fetchStatus = gui_get_uint16_var(lastName, &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    LONGS_EQUAL(10, value);
}
// after init the page index variable exists and is set to 0
TEST(GUITest, variable_exists_and_is_set_to_its_default)
{
    // init gui 
    gui_init(lcd_spy_write, singleVarGui);
    // Fetch variable value 
    uint16_t value = 0; // set to non zero value 
    gui_variable_status_t fetchStatus = gui_get_uint16_var("pageIndex", &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    LONGS_EQUAL(55, value);
}

// the page index variable can be changed using gui_variable_update("pageIndex",10)
TEST(GUITest, uint16_variables_can_be_updated)
{
    // init gui 
    gui_init(lcd_spy_write, singleVarGui);
    // Update Variable 
    gui_update_uint16_var("pageIndex",10);
    // Fetch variable value 
    uint16_t value = 0; // set to non zero value 
    gui_variable_status_t fetchStatus = gui_get_uint16_var("pageIndex", &value);
    LONGS_EQUAL(GUI_VAR_OK, fetchStatus);
    LONGS_EQUAL(10, value);
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
    char lastType[64]  = "uint16_t";  // Create garbled type
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
    uint8_t outputMap[COLUMNS][ROWS] = {99};
    // Call gui_render_bitmap 
    gui_render_bitmap(outputMap,strBitMapCopy);
    // Check that bitmaps match 
    PRINT_BIT_MAP(ROWS,COLUMNS,outputMap);
    IS_BIT_MAP_EQUAL_BIT(beautifulBitMap,outputMap);
}

/* <bitMap> rendering tests
 * - if no <bitMap> tag is found returns error
 * - if no <\bitmap> tag is found then gui_render_bitmap returns an error
 * - if no <position> is found then error is returned 
 * - if no <size> is found then error is returned 
 * - if gui_render_bitmap finds a non-bitmapable chacter then returns error 
 * - bitmaps position can be changed
 * - bitmaps can be located partially on screen using negitive or overflow positions
 * - position can be set using variables
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ONE: TEXT RENDER 
//////////////////////////////////////////////////////////////////////////////////////////////////////////


/* <text> rendering tests
 * - CHANGE FORMATING OF STRINGS TO STANDARD C STYLE, XML FORMAT MAKES LIFE HARDER
 * - gui system rendurs text default size at specified position 
 * - gui system can render horizontially centered text 
 * - gui system can render vertically centered text 
 * - gui system can render horz and vert centered text 
 * - gui system can render diffrent sized text on same page 
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ONE: PAGE RENDER 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// TEST(GUITest, pages_can_be_written_to_screen)
// {
//     // init gui 
//     gui_init(lcd_spy_write, helloWorldGui);
//     // Update to set the first frame 
//     gui_update();
//     IS_LCD_EQUAL_BIT(beautifulBitMap);
// }

/**
 * <page>
 * - Changing page number to a page that does not exist then calling gui_update() sets error and the spy page does not change
 * - User defines are defined in an untracked file 
 * - If gui is initilaise with no pageIndex var then error occurs 
 * - Additional options, Variable refresh rates/partial screen refreshes 
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ONE: OPERANDS 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * - operand, will be restricted to:
 *  <if>
 *      <var>b_variable<value>true</value></var>
 *  <then>
 *      <var>pageIndex<value>10</value></var>
 *  <else>
 *          
 *  </if>
 * if b_variable is true then set pageIndex to 10, will be able to add else condition 
 * 
/*







// Many
/**
 * ToDo:
 * - Hash table can handle collisions through Open Addressing
 * - Can use int16_t as variables 
 * - Can use floats as variables 
 * - Can use uint32_t as variables 
 * - Can use int32_t as variables 
 * - Can't create two variables of same name 
 * - add in logger output support 
 */




/**
 * Pedantic checks 
 * - if not pages brace exists then no pages are created 
 * - if a page exists outside the <pages> tag an error is thrown 
 * - if a variable exists outside the <variables> tag an error is thrown  
 * - Calling gui_update() when page number has not changed does not change the bitmap written to spy 
 */