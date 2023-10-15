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
 * - Passing in bitmap write function causes error 
 * - After init no bitmap has been written to spy
 * - 
 */

/**
 * One:
 * - After init the page number variable exists and is set to 0
 * - When gui_update() is called the page 0 "hello world\n" bitmap is written to the spy 
 * - After when gui_update is called a second time the next bitmap will then be written to 
 *   lcd 
 */

// Many