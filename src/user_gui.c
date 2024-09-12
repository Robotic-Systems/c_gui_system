#include "user_gui.h"

// FONT INCLUSIONS
///////////////////
#include "font_juipter_.h"
#include "font_sans_bold_.h"
#include "font__5pts_.h"
// FONT TYPE MASTER LIST 
////////////////////////
font_list_t font_master_list[NUM_FONT_TYPES] = {
    {"sans",{12,18}, {widthArray_size12,widthArray_size18}, {&fontMap_size12[0][0][0],&fontMap_size18[0][0][0]}},
    {"jupiter",{19},{widthArray},{&fontMap[0][0][0]}},
    {"_5pt", {10,15,18}, {_5pts_widthArray_10,_5pts_widthArray_15, _5pts_widthArray_18},{&_5pts_fontMap_10[0][0][0], &_5pts_fontMap_15[0][0][0], &_5pts_fontMap_18[0][0][0]}},

};
