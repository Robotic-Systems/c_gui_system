/**
 * Tests for font file 
 * - need to add a user_fonts folder that will add the inscope fonts to the lookup table 
 * - calling font_get_letter(&width,&height,&pointer,"nil","0","a") returns error
 * - calling font_get_letter(&width,&height,&pointer,"calibri","12","a") returns pointer to a 2D array containting
 * the letter bitmap and also the letters correct width and height
 * 
 */