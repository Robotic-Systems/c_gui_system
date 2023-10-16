#include "CppUTest/TestHarness.h"

extern "C" 
{

}


TEST_GROUP(GUITest) 
{
    void setup()
    {

    }

    void teardown()
    {
    }

  

};

// Zero
TEST(GUITest, passing_in_no_xml_causes_error);
{
    //
    CHECK(1);
}

/**
 * Zero ToDo:
 * - Passing in no xml causes error 
 * - Passing in no bitmap write function causes error 
 * - After init no bitmap has been written to spy
 * - 
 */

/**
 * One:
 * - After init the page number variable exists and is set to 0
 * - When gui_update() is called the page 0 bitmap is written to the spy 
 * - Calling gui_update() when page number has not changed does not change the bitmap written to spy 
 * - Changing page number to a page that does not exist then calling gui_update() sets error and the spy page does not change
 * - 
 */

// Many