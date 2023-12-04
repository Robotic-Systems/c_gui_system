c_gui_system
This is a simple 'xml' based GUI system for use in embedded projects. It's created to simplify the development path when creating GUIs that display data and for configuring menus. The GUI system is composed of the following components:

Variables
Pages
Text
Fonts
Bitmaps
Operands
Lists
An example of how to initialize the GUI system is as follows:

c
Copy code
// Create a simple hello world xml 
const char* helloWorldGui = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<variables>\n"
// ... (rest of the XML definition)
"</variables>\n"
"<pages>\n" 
// ... (rest of the XML definition)
"</pages>";

// Init GUI 
gui_status_t initStatus = gui_init(lcd_write_function, printf, guiXml);
The init function takes three inputs:

A function pointer that can print bitmaps to the write function
A function pointer that can be used to print log messages (printf can be used if no specific logger is available)
A string where your GUI XML is defined
Note that EVERY GUI requires the page index variable to be defined as a variable.

Variables
Variables are user-defined entities that can be displayed on the GUI, used to position text, or employed in operands. Variables are defined in the XML the user creates and are then stored in a hashmap when the GUI XML is parsed. Variables can have types int32_t or float; all other variable types are not supported. Variables are the first thing defined in the XML string.

To interact with a variable from outside the GUI driver, the following functions can be used:

c
Copy code
// For uint32_t variables 
gui_variable_status_t fetchStatus = gui_update_int32_var("pageIndex", 10);
gui_variable_status_t fetchStatus = gui_get_int32_var("pageIndex", &value);

// For float variables 
gui_variable_status_t updateStatus = gui_update_float_var("testVar",-2.2);
gui_variable_status_t fetchStatus = gui_get_float_var("testVar", &value);
To reference variables within the XML in operands and position tags, they must be enclosed in <var></var> tags as follows:

xml
Copy code
<text>
    <!-- Other text properties -->
    <position><var>xPos</var>,<var>yPos</var></position>
    <!-- Other text content -->
</text>
Pages
Pages are the primary way GUI information can be grouped for display. They can contain all other elements except variable declarations. To switch between pages, the 'pageIndex' can be used, or more complex behavior can be defined using operands.

Note:

When a page is being rendered, ONLY the XML defined between its <page></page> braces is being read through.
The page <name></name> is not saved or parsed; a page's index is set as the order in which they are defined.
Text
Text elements have many options associated with them:

<font></font>: Name of the font you wish to use. Note: To use a font, it must be added into user_gui.h. See Fonts heading for more details.
<font-size>: Size of font you wish to use.
<alignment>: Options include 'center', 'left', and 'right'.
<vert-alignment>: Options include 'top', 'bottom', and 'center'.
<position>: Where the text will be positioned.
<content>: The actual text content.
To display variables in text, follow this syntax:

xml
Copy code
<text>
    <!-- Other text properties -->
    <content>"one: %d",<var>test1</var></content>
    <!-- Other text content -->
</text>
This will display whatever is stored in 'test1' as text, following basic formatting rules similar to C printf.

Fonts
To add additional fonts, use a python tool called tff2bitmap.py. It converts valid .tff or .otf font files into a header file that can be added to the project for use. To use the newly created font, add it to the font_master_list in user_gui.c.

c
Copy code
#include "user_gui.h"
// FONT INCLUSIONS
///////////////////
#include "font_juipter_.h"
#include "font_sans_bold_.h"
// FONT TYPE MASTER LIST 
////////////////////////
font_list_t font_master_list[NUM_FONT_TYPES] = {
    {"sans",{12,18}, {widthArray_size12,widthArray_size18}, {&fontMap_size12[0][0][0],&fontMap_size18[0][0][0]}},
    {"jupiter",{19},{widthArray},{&fontMap[0][0][0]}}
};
The newly created header file should contain a width_array and fontMap that can be easily added. For finding fonts, a good resource can be found at 1001fonts.com.

Bitmaps
Bitmaps are another element that can be added to pages in the following XML format:

xml
Copy code
<page>
    <bitMap>
        <!-- Other bitmap properties -->
        <size>32,32</size>
        <position>0,0</position>
        <data>
            <!-- Bitmap data -->
        </data>
    </bitMap>
</page>
This XML creates a bitmap with a size of 32x32 and positions it at the top-left corner (0,0).

Operands
Operands take the following form:

xml
Copy code
<operand>
    <if>
        <!-- Condition -->
    </if>
    <then>
        <!-- Action if condition is true -->
    </then>
    <else>
        <!-- Action if condition is false -->
    </else>
</operand>
Operands can contain:

<operation>: Evaluations such as 'equal', 'less-than', or 'greater-than'.
<do>: Actions such as 'add', 'minus', or 'set-equal'.
Once a statement has been evaluated, every <then> (if true) or <else> (if false) found until the end of the operand tag will be operated on.
