/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Library              *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the most current *
 * version of the Common Public License.                                  *
 * http://www.opensource.org/licenses/cpl1.0.php                          *
 *                                                                        *
 **************************************************************************
*/

#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

typedef struct
{
  unsigned int (*func)(const char *);
  const char *name;
} Hashfunction;

unsigned int RSHash  (const char *str);
unsigned int JSHash  (const char *str);
unsigned int PJWHash (const char *str);
unsigned int ELFHash (const char *str);
unsigned int BKDRHash(const char *str);
unsigned int SDBMHash(const char *str);
unsigned int DJBHash (const char *str);
unsigned int DEKHash (const char *str);
unsigned int BPHash  (const char *str);
unsigned int FNVHash (const char *str);
unsigned int APHash  (const char *str);

unsigned int hashfunction_number(void);
const Hashfunction *hashfunction_get(unsigned int num);

#endif
