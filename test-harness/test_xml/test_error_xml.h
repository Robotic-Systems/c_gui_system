#ifndef MYHEADER3_H
#define MYHEADER3_H

#define COLUMNS 102
#define ROWS    64



const char * noPageStrtBrace =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"        <name>0</name>\n"
"    </page>\n";

const char * noPageEndBrace =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"    <page>\n"
"        <name>0</name>\n";

const char * noVarStrtBrace =
"        <name>pageIndex</name>\n"
"        <type>int32_t</type>\n"
"        <value>0</value>\n"
"    </variable>\n";

const char * noVarEndBrace =
"    <variable>\n"
"        <name>pageIndex</name>\n"
"        <type>int32_t</type>\n"
"        <value>0</value>\n";


const char * justABitmap_startBraceErr =
" <size>32,32</size>\n"
" <position>0,0</position>"
" <data>\n"
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,\n" 
"   0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,\n" 
"   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,\n" 
"   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
" </data>"
"</bitMap>\n";

const char * justABitmap_closeBraceErr =
"<bitMap>\n"
" <size>32,32</size>\n"
" <position>0,0</position>"
" <data>\n"
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,\n" 
"   1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,\n" 
"   0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,\n" 
"   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,\n" 
"   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
"   0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,\n" 
" </data>";

const char * justABitmap_positionBraceErr =
"<bitMap>\n"
" <size>2,32</size>\n"
" 0,0</position>"
" <data>\n"
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
" </data>"
"</bitMap>\n";

const char * justABitmap_sizeBraceErr =
"<bitMap>\n"
" 2,32</size>\n"
" <position>0,0</position>"
" <data>\n"
"   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
" </data>"
"</bitMap>\n";

const char * justABitmap_randomChar =
"<bitMap>\n"
" <size>2,32</size>\n"
" <position>0,0</position>"
" <data>\n"
"   1, 1, A, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"   1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
" </data>"
"</bitMap>\n";

const char * text_HelloWorld_error_start_brace =
"    <font>jupiter</font>\n"
"    <font-size>19</font-size>\n"
"    <alignment>center</alignment>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char * text_HelloWorld_error_no_font =
"<text>\n"
"    <font-size>19</font-size>\n"
"    <alignment>center</alignment>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char * text_HelloWorld_error_no_font_size =
"<text>\n"
"    <font>jupiter</font>\n"
"    <alignment>center</alignment>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char * text_HelloWorld_error_no_alignment =
"<text>\n"
"    <font>jupiter</font>\n"
"    <font-size>19</font-size>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";
const char * text_HelloWorld_error_no_vert_alignment =
"<text>\n"
"    <font>jupiter</font>\n"
"    <font-size>19</font-size>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";
const char * text_HelloWorld_error_no_position =
"<text>\n"
"    <font>jupiter</font>\n"
"    <font-size>19</font-size>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <alignment>center</alignment>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char * text_HelloWorld_error_no_content =
"<text>\n"
"    <font>jupiter</font>\n"
"    <font-size>19</font-size>\n"
"    <alignment>center</alignment>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"</text>\n";

const char * text_HelloWorld_error_font_does_not_exist =
"<text>\n"
"    <font>spaceMan</font>\n"
"    <font-size>19</font-size>\n"
"    <alignment>center</alignment>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char * text_HelloWorld_error_font_size_does_not_exist =
"<text>\n"
"    <font>jupiter</font>\n"
"    <font-size>20</font-size>\n"
"    <alignment>center</alignment>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char * text_HelloWorld_error_alignment_does_not_exist =
"<text>\n"
"    <font>jupiter</font>\n"
"    <font-size>19</font-size>\n"
"    <alignment>backwards</alignment>\n"
"    <vert-alignment>center</vert-alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char * text_HelloWorld_error_vert_alignment_does_not_exist =
"<text>\n"
"    <font>jupiter</font>\n"
"    <font-size>19</font-size>\n"
"    <alignment>center</alignment>\n"
"    <position>32,51</position>\n"
"    <content>\"Hello, World!\n\"</content>\n"
"</text>\n";

const char* helloWorldGui_no_page_index = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<variables>\n"
"    <variable>\n"
"        <name>othertIndex</name>\n"
"        <type>int32_t</type>\n"
"        <value>0</value>\n"
"    </variable>\n"
"</variables>\n"
"<pages>\n" 
"    <page>\n"
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

const char* helloWorldGui_text_error= 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<variables>\n"
"    <variable>\n"
"        <name>pageIndex</name>\n"
"        <type>int32_t</type>\n"
"        <value>0</value>\n"
"    </variable>\n"
"</variables>\n"
"<pages>\n" 
"    <page>\n"
"        <name>1</name>\n"
"        <text>\n"
"            <font>jupiter</font>\n"
"            <font-size>19</font-size>\n"
"            <alignment>backwards</alignment>\n"
"            <vert-alignment>center</vert-alignment>\n"
"            <position>32,51</position>\n"
"            <content>\"Hello, World!\n\"</content>\n"
"        </text>\n"
"    </page>\n"
"</pages>";

const char* helloWorldGui_bitmap_error= 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<variables>\n"
"    <variable>\n"
"        <name>pageIndex</name>\n"
"        <type>int32_t</type>\n"
"        <value>0</value>\n"
"    </variable>\n"
"</variables>\n"
"<pages>\n" 
"    <page>\n"
"        <name>1</name>\n"
"        <bitMap>\n"
"         <size>2,32</size>\n"
"         <position>0,0</position>"
"         <data>\n"
"           1, 1, A, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"           1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\n" 
"         </data>"
"        </bitMap>\n"
"    </page>\n"
"</pages>";


const char * operand_equal_missing_opening_flag=
" <if>\n"
"    <equal>\n"
"       <var>test1</var>\n"
"       <value>10</value>\n"
"    </equal>\n"
"    <then>\n"
"       <do>\"set-equal\"</do>\n"
"       <var>pageIndex</var>\n"
"       <value>2</value>\n"
"    </then>\n"
"    <else>\n"
"    <do>\"set-equal\"</do>\n"
"       <var>pageIndex</var>\n"
"       <value>6</value>\n"
"    </else>\n"
" </if>\n"
"</operand>\n";

const char * operand_equal_missing_if_tag=
"<operand>\n"
"    <operation>\"equal\"</operation>"
"    <var>test1</var>\n"
"    <value>10</value>\n"
" </if>\n"
" <then>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>6</value>\n"
" </else>\n"
"</operand>\n";

const char * operand_equal_missing_operation_tag=
"<operand>\n"
" <if>\n"
"    <var>test1</var>\n"
"    <value>10</value>\n"
" </if>\n"
" <then>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>6</value>\n"
" </else>\n"
"</operand>\n";

const char * operand_equal_missing_var_tag=
"<operand>\n"
" <if>\n"
"    <operation>\"equal\"</operation>"
"    <value>10</value>\n"
" </if>\n"
" <then>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>6</value>\n"
" </else>\n"
"</operand>\n";

const char * operand_equal_missing_value_tag=
"<operand>\n"
" <if>\n"
"    <operation>\"equal\"</operation>"
"    <var>test1</var>\n"
" </if>\n"
" <then>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>6</value>\n"
" </else>\n"
"</operand>\n";

const char * operand_equal_no_then=
"<operand>\n"
" <if>\n"
"    <operation>\"equal\"</operation>"
"    <var>test1</var>\n"
"    <value>10</value>\n"
" </if>\n"
" <else>\n"
"    <do>\"set-equal\"</do>\n"
"    <var>pageIndex</var>\n"
"    <value>6</value>\n"
" </else>\n"
"</operand>\n";

const char * operand_equal_un_sup_do_code=
"<operand>\n"
" <if>\n"
"    <operation>\"equal\"</operation>"
"    <var>test1</var>\n"
"    <value>10</value>\n"
" </if>\n"
" <then>\n"
"    <do>\"divide\"</do>\n"
"    <var>test1</var>\n"
"    <value>2</value>\n"
" </then>\n"
"</operand>\n";


const char * list_no_start_tag=
" <cursor><var>cursor</var></cursor>\n"
" <font>jupiter</font>\n"
" <font-size>19</font-size>\n"
" <position>32,51</position>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n"
"</list>\n";

const char * list_no_cursor_tag=
"<list>\n"
" <font>jupiter</font>\n"
" <font-size>19</font-size>\n"
" <position>32,51</position>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n"
"</list>\n";

const char * list_no_font_tag=
"<list>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font-size>19</font-size>\n"
" <position>32,51</position>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n"
"</list>\n";

const char * list_no_size_tag=
"<list>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font>jupiter</font>\n"
" <position>32,51</position>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n"
"</list>\n";

const char * list_no_position_tag=
"<list>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font>jupiter</font>\n"
" <font-size>19</font-size>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n"
"</list>\n";

const char * list_no_options_tag=
"<list>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font>jupiter</font>\n"
" <font-size>19</font-size>\n"
" <position>32,51</position>\n"
"</list>\n";

const char * list_no_end_list_tag=
"<list>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font>jupiter</font>\n"
" <font-size>19</font-size>\n"
" <position>32,51</position>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n";

const char * list_fake_font=
"<list>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font>spaceMan</font>\n"
" <font-size>19</font-size>\n"
" <position>32,51</position>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n"
"</list>\n";

const char * list_fake_size=
"<list>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font>jupiter</font>\n"
" <font-size>22</font-size>\n"
" <position>32,51</position>\n"
" <options>1. Set-up\n2. Fast Set\n 3. Slow Set\n</options>\n"
"</list>\n";

const char * enter_no_intEntry_tag=
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_cursor=
"<intEntry>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_font=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_size=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_position=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_digits_tag=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_max_tag=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_min_tag=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_variable_tag=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
"</intEntry>\n";

const char * enter_var_no_end_tag=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n";



const char * enter_var_fake_font=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>spaceMan</font>\n"
" <font-size>19</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_fake_font_size=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>jupiter</font>\n"
" <font-size>22</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_heading=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <incrementer><var>inc</var></incrementer>\n"
" <font>_5pt</font>\n"
" <font-size>15</font-size>\n"
" <position>0,0</position>\n"
" <digits>2</digits>\n"
" <max>10</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

const char * enter_var_no_incrementer=
"<intEntry>\n"
" <cursor><var>cursor</var></cursor>\n"
" <font>_5pt</font>\n"
" <font-size>10</font-size>\n"
" <position>0,0</position>\n"
" <heading>Set Pull Power</heading>\n"
" <digits>2</digits>\n"
" <max>20</max>\n"
" <min>0</min>\n"
" <variable><var>pull_power</var></variable>\n"
"</intEntry>\n";

#endif