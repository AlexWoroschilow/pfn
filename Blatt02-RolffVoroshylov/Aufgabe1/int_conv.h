/* converts a string to a number using strtol(). Hard exits the program if the
   string is no valid number. Assumes base 0, that is it should handle
   hexadecimal and octal values.
   Returns the converted Integer. */

int get_int(const char *const number);

long get_long(const char *const number);
