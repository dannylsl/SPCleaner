/**
 * Macro for ANSI Escape Code
 */

#ifndef __COLORPRINT_H
#define __COLORPRINT_H

#define BLACK   "\033[30;1m"
#define RED     "\033[31;1m"
#define GREEN   "\033[32;1m"
#define YELLOW  "\033[33;1m"
#define BLUE    "\033[34;1m"
#define MAGENTA "\033[35;1m"
#define CYAN    "\033[36;1m"
#define WHITE   "\033[37;1m"
#define NC      "\033[0m"

#define BLACK_TEXT(x) "\033[30;1m" x "\033[0m"
#define RED_TEXT(x) "\033[31;1m" x "\033[0m"
#define GREEN_TEXT(x) "\033[32;1m" x "\033[0m"
#define YELLOW_TEXT(x) "\033[33;1m" x "\033[0m"
#define BLUE_TEXT(x) "\033[34;1m" x "\033[0m"
#define MAGENTA_TEXT(x) "\033[35;1m" x "\033[0m"
#define CYAN_TEXT(x) "\033[36;1m" x "\033[0m"
#define WHITE_TEXT(x) "\033[37;1m" x "\033[0m"

#endif
