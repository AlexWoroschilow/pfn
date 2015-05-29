#ifndef FASTQ_PARSE_H
#define FASTQ_PARSE_H
#include <stdbool.h>

/* class to represent a single <FastQentry> */

typedef struct FastQentry FastQentry;

/* create a <FastQentry} object for <filename>. To generate an appropriate
   error message, the name of the program must be provided as first argument */
FastQentry *fastqentry_new(const char *progname,const char *filename);

/* Ask for next <FastQentry>. Returns <false>, if there is no more
   <FastQentry>. Return <true> if there is one which is referred to
   by <fastqentry>. */
bool fastqentry_next(FastQentry *fastqentry);

/* clear the contents of a <FastQentry>. */
void fastqentry_clear(FastQentry *fastqentry);

/* delete <FastQentry>-object. */
void fastqentry_delete(FastQentry *fastqentry);

/* show <FastQentry>-object. This is used mainly for testing. */
void fastqentry_show(const FastQentry *fastqentry);

/* the following functions return pointer to a buffer internal
   to the <FastQentry>-object. This buffer is used with each call
   to <fastqentry_next>. So if the sequence lines are to be stored, the
   user has to copy them. */

/* return header line from given <FastQentry>-object.*/
const char *fastqentry_headerline(const FastQentry *fastqentry);

/* return sequence line from given <FastQentry>-object.*/
const char *fastqentry_sequenceline(const FastQentry *fastqentry);

/* return quality value line from given <FastQentry>-object.*/
const char *fastqentry_qualityline(const FastQentry *fastqentry);

/* deliver length of line of sequenceline and qualityline of
   <FastQentry>-object.*/
unsigned long fastqentry_linelength(const FastQentry *fastqentry);

/* deliver the line number of the input file at which the current
   <FastQentry> starts. Can be used to generate meaningful error messages. */

unsigned long fastqentry_linenum(const FastQentry *fastqentry);

/* Assume that process_entry is a function to process the
   header, sequence and quality line of a <FastQentry>.

   Here is an example how to use the functions:

   FastQentry *fastqentry = fastqentry_new(progname,inputfilename);

   while (fastqentry_next(fastqentry))
   {
     process_entry(fastqentry_headerline(fastqentry),
                   fastqentry_sequenceline(fastqentry),
                   fastqentry_qualityline(fastqentry),
                   fastqentry_linelength(fastqentry));
     fastqentry_clear(fastqentry);
   }
   fastqentry_delete(fastqentry);

*/
#endif
