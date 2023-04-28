
#ifndef _PARSERPATH_H
#define _PARSERPATH_H

/**
 * Unallocates parsed information
 */
void parser_path_clear();

/**
 * Parses string separated by space into array of words
 *  @warning                    this function uses malloc, be sure to clear the parser afterwards
 * 
 *  @param string               String to be parsed
 */
void parse_path(char* string);


/**
 *  Get last parsed result
 * 
 *  @return                     Last parsed result
 */
char** get_parsed_path_result();

/**
 *  Get last parsed word count
 * 
 *  @return                     Last parsed word count
 */
int get_parsed_path_word_count();

#endif