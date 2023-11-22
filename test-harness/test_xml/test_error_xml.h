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
"       <var>pageIndex</var>\n"
"       <value>2</value>\n"
"    </then>\n"
"    <else>\n"
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
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
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
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
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
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
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
"    <var>pageIndex</var>\n"
"    <value>2</value>\n"
" </then>\n"
" <else>\n"
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
"    <var>pageIndex</var>\n"
"    <value>6</value>\n"
" </else>\n"
"</operand>\n";
#endif