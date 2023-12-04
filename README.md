tool for creating bitmaps: https://tools.withcode.uk/bitmap/

TODO:
- Explain diffrence between error and test xml's
- Explainer about what needs to be added to the gui user file 


# c_gui_system
This is a simple 'xml' based gui system for use in embedded projects. I was created in order to give a simplified development path when creating gui's that display data and for configering menus. The gui system is made up of the following componenets:
- [Variables](#Variables)
- [Pages](#Pages)
- [Text](#Text)
- [Fonts](#Fonts)
- [Bitmap's](#Bitmap's)
- [Operands](#Operands)
- [Lists](#Lists)
An example of how to initalise the gui system is as follows :

'''
// Create a simple hello world xml 
const char* helloWorldGui = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<variables>\n"
"    <variable>\n"
"        <name>pageIndex</name>\n"
"        <type>int32_t</type>\n"
"        <value>0</value>\n"
"    </variable>\n"
"</variables>\n"
"<pages>\n" 
<page>\n"
"        <name>1</name>\n"
"        <text>\n"
"            <font>jupiter</font>\n"
"            <font-size>19</font-size>\n"
"            <alignment>center</alignment>\n"
"            <vert-alignment>center</vert-alignment>\n"
"            <position>32,51</position>\n"
"            <content>\"Hello World!\"</content>\n"
"        </text>\n"
"    </page>\n"
"</pages>";

// Init GUI 
gui_status_t initStatus = gui_init(lcd_write_function, printf, guiXml);
'''C

The the init function takes three inputs,
 - A fucntion pointer that can print bitmaps to the write function 
 - A function pointer that can be used to print log messages, printf can just be used if you don't have a specific logger you want to use 
 - A string where your gui xml is defined 
Note that EVERY gui required the page index variable to be defined as a variable. 


## Variables 
Variables are user defined variables that can be displayed on the gui, used to position text or used in operands. Variables are defined in the xml the user created and are then when the gui xml is first parsed are then stored in a hashmap. Variables can have type int32_t or float all other variable tyupes are not supported. Variables are the first thing defined in the xml string as can be seen in the example above. 

To interact with a variable from outside the gui driver the following functions can be used:

'''
    // For uint32_t variables 
    gui_variable_status_t fetchStatus = gui_update_int32_var("pageIndex", 10);
    gui_variable_status_t fetchStatus = gui_get_int32_var("pageIndex", &value);

    // For float variables 
    gui_variable_status_t updateStatus = gui_update_float_var("testVar",-2.2);
    gui_variable_status_t fetchStatus = gui_get_float_var("testVar", &value);
'''C

To refrenve variables within the xml in operands and position tags they must be enclosed in <var></var> tags like follows:

'''
<text>\n
    <font>jupiter</font>\n
    <font-size>19</font-size>\n
    <alignment>center</alignment>\n
    <vert-alignment>center</vert-alignment>\n
    <position><var>xPos</var>,<var>yPos</var></position>\n
    <content>\"Hello World!\"</content>\n
</text>\n
'''xml

In this two uint32_t vars xPos and yPos can be used to update the texts position in between renders.

## Pages
Pages is the largest way gui information can be grouped for display, they can contain all the other elements except variable declerations. To switch between pages the 'pageIndex' can be used or more complex behaviour can be defined using operands. 

Note: When a page is being rendered ONLY the xml defined between its <page></page> braces is being read through.
Note: The page <name></name> is not saved or parsed, a pages index is set as the order in which they are defined 

### Text
Text elements have many options associated with them and they will be explained below:
 - <font></font> Name of the font that you wish to use, Note to use a font you must first make sure its added into user_gui.h see Fonts headig for more detial 
 - <font-size> Size of font you wish to use, Note the name and size must both be defined in user_gui.h for use 
 - <alignment> Has three options 'center', 'left' and 'right'. They do the same things as they do in word 
 - <vert-alignment> Verticle alignment, Has threee options 'top', 'bottom', 'center'. They do the same things as they do in word 
 - <position> Where the text will be positioned, if you have the alignment options top and left then this position will be the postion on the screen of the top left pixel ect ect.......
 - <content> What you actually want the text to be 
To display variables in text the following syntax can be followed:

'''
<text>
    <font>jupiter</font>
    <font-size>19</font-size>
    <alignment>left</alignment>
    <vert-alignment>top</vert-alignment>
    <position>0,0</position>
    <content>\"one: \%d\",<var>test1</var></content>
</text>
'''xml 

This will display whatever is stored in 'test1' as in text. The same bastic formatting rules as for c printf.

#### Fonts
To add additional fonts there is a python tool called tff2bitmap.py which when pointed to a valid .tff or .otf font file can create a header file that can be dragged into the project for use. To use the newly created front you first need to add the font to the font_master_list in user_gui.c which should look somthing like follows:

'''
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
'''C

If you look in your newly created headder file you should find a width_array and fontMap that can be easily added.
A good resouce for finding fonts can be found at the following:
https://www.1001fonts.com/carlito-font.html

### Bitmap's
Bitmaps are another element that can be added to pages, the xml to add a bitmap is as follows:

'''
<page>
<bitMap>
 <size>32,32</size>
 <position>0,0</position>
 <data>
   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
   1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
   1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 
   1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 
   1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 
   0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 
   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 
   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
   0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
   0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
   0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
   0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
 </data>
</bitMap>
</page>
'''xml 

This page when rendered would contain the following bitmap with its top left corner positioned at 0,0. The <size> tag is the dimesions of the bitmap.


### Operands
Operands take the form as follows:

'''
<operand>
 <if>
    <operation>\"equal\"</operation>
    <var>test1</var>
    <value>10</value>
 </if>
 <then>
    <do>\"add\"</do>
    <var>test1</var>
    <value>2</value>
 <else>
    <do>\"minus\"</do>
    <var>test1</var>
    <value>3</value>
 </else>\n"
</operand>\n";
'''xml

In this the var test1 is compared to 10 and if equal it will <then> "add" 2 to test 1, <else> it will minus 3 from test 1. Operands can be added to a page in any location and will be evaluated as soon ad they are hit. The tags associated with operands are as follows:
- <operation> The evaluation to take place, can have value \"equal\", \"less-than\" or \"greater-than\". 
- <do> Operation to be done if line is parsed, can have value \"add\", \"minus\" OR \"set-equal\"
Note that once a statement has been evaulated then every <then> (if true) or <else> (if false) found until the end operand tag is found will be operated on i.e 

'''
<operand>
 <if>
    <operation>\"equal\"</operation>
    <var>test1</var>
    <value>10</value>
 </if>
 <then>
    <do>\"set-equal\"</do>
    <var>test1</var>
    <var>test2</var>
 </then>
 <then>
    <do>\"set-equal\"</do>
    <var>test2</var>
    <value>22</value>
 </then>
</operand>
'''xml 

if the <if> statement is true then evey then test1 will be set equal to test2 and then test2 will be set equal to 22. 
