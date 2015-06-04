#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "hashqual-functions.h"

unsigned int RSHash(const char *str)
{
  unsigned int    b = 378551;
  unsigned int    a = 63689;
  unsigned int    hash = 0;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = hash * a + (*sptr);
    a = a * b;
  }
  return hash;
}

unsigned int JSHash(const char *str)
{
  unsigned int    hash = 1315423911;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash ^= ((hash << 5) + (*sptr) + (hash >> 2));
  }
  return hash;
}

/* P. J. Weinberger Hash Function */

unsigned int PJWHash(const char *str)
{
  const unsigned int BitsInUnsignedInt
  = (unsigned int) (sizeof(unsigned int) * 8);
  const unsigned int ThreeQuarters
  = (unsigned int) ((BitsInUnsignedInt * 3) / 4);
  const unsigned int OneEighth = (unsigned int) (BitsInUnsignedInt / 8);
  const unsigned int HighBits
  = (unsigned int) (0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
  unsigned int    hash = 0;
  unsigned int    test = 0;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = (hash << OneEighth) + (*sptr);
    if ((test = hash & HighBits) != 0)
    {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }
  return hash;
}

unsigned int ELFHash(const char *str)
{
  unsigned int    hash = 0;
  unsigned int    x = 0;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = (hash << 4) + (*sptr);
    if ((x = hash & 0xF0000000L) != 0)
    {
      hash ^= (x >> 24);
    }
    hash &= ~x;
  }
  return hash;
}

unsigned int BKDRHash(const char *str)
{
  unsigned int    seed = 131;    /* 31 131 1313 13131 131313 etc.. */
  unsigned int    hash = 0;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = (hash * seed) + (*sptr);
  }
  return hash;
}

unsigned int SDBMHash(const char *str)
{
  unsigned int    hash = 0;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = (*sptr) + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}

unsigned int DJBHash(const char *str)
{
  unsigned int hash = 5381;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = ((hash << 5) + hash) + (*sptr);
  }
  return hash;
}

unsigned int DEKHash(const char *str)
{
  unsigned int    hash = (unsigned int) strlen(str);
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = ((hash << 5) ^ (hash >> 27)) ^ (*sptr);
  }
  return hash;
}

unsigned int BPHash(const char *str)
{
  unsigned int    hash = 0;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash = hash << 7 ^ (*sptr);
  }
  return hash;
}

unsigned int FNVHash(const char *str)
{
  const unsigned int fnv_prime = 0x811C9DC5;
  unsigned int    hash = 0;
  const char     *sptr;

  for (sptr = str; *sptr != '\0'; sptr++)
  {
    hash *= fnv_prime;
    hash ^= (*sptr);
  }
  return hash;
}

unsigned int APHash(const char *str)
{
  unsigned int    hash = 0xAAAAAAAA;
  unsigned long   i;
  const char     *sptr;

  for (sptr = str, i = 0; *sptr != '\0'; sptr++, i++)
  {
    hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*sptr) * (hash >> 3))
                           : (~((hash << 11) + ((*sptr) ^ (hash >> 5))));
  }
  return hash;
}

#define MKHASHFUNENTRY(X)  {X, #X}

static Hashfunction hashfunction_tab[] = 
{
  MKHASHFUNENTRY(RSHash),
  MKHASHFUNENTRY(JSHash),
  MKHASHFUNENTRY(PJWHash),
  MKHASHFUNENTRY(ELFHash),
  MKHASHFUNENTRY(BKDRHash),
  MKHASHFUNENTRY(SDBMHash),
  MKHASHFUNENTRY(DJBHash),
  MKHASHFUNENTRY(DEKHash),
  MKHASHFUNENTRY(BPHash),
  MKHASHFUNENTRY(FNVHash),
  MKHASHFUNENTRY(APHash)
};

unsigned int hashfunction_number(void)
{
  return (unsigned int) sizeof hashfunction_tab/sizeof hashfunction_tab[0];
}

const Hashfunction *hashfunction_get(unsigned int num)
{
  assert(num < hashfunction_number());
  return hashfunction_tab + num;
}
