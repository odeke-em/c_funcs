#ifndef _STR_ESCAPE_H
#define _STR_ESCAPE_H

// strEscape escapes every character in a character set `charset`
// replacing each occurrance in `orig` in with \\<character>
char *strEscape(const char *orig, const char *seq);

#endif  // _STR_ESCAPE_H

