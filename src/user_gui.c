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
