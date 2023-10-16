#include "CppUTest/TestHarness.h"

extern "C" 
{
    #include "../spies/lcd_spy.h"
    #include "gui.h"
    #include "../test_xml/test_xml.h"
}


TEST_GROUP(GUITest) 
{
    void setup()
    {
        lcd_spy_init();
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
    // parse for pages 
    gui_parse_for_pages(); // !!!NOTE this already called during the init, however it is brought into the test just for explicitness
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

    LONGS_EQUAL(210,startIndex);
    LONGS_EQUAL(3729,endIndex);
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

    LONGS_EQUAL(3746,startIndex);
    LONGS_EQUAL(3932,endIndex);
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

/**
 * One:
 * - if initted with xml with a page with no closing brace then error is thrown
 * - if initted with an xml with only a page closing brace then error is thrown 
 * 
 * - when passed a xml with one variables the variable count is set to 1
 * - after init the page index variable exists and is set to 0
 * - the page index variable can be changed using gui_variable_update("pageIndex",10)
 * 
 * 
 * - When gui_update() is called the page 0 bitmap is written to the spy 
 * - Calling gui_update() when page number has not changed does not change the bitmap written to spy 
 * - Changing page number to a page that does not exist then calling gui_update() sets error and the spy page does not change
 * - User defines are defined in an untracked file 
 */

// Many