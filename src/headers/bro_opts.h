#ifndef __bro_headers_options_h
#define __bro_headers_options_h

#include "bro_bt.h"

/*  Struct that contains all the data parsed from the options. I know it might
 *  be skipped but it could be used for future improvements. It's easier to
 *  update a code without having to rewrite every function descriptor.
 *  bdaddr_t mac:   Variable that contains the 6byte encoding of the MAC
 *                  address chosen for the SPAM communication.
 */
typedef struct 
{
  bdaddr_t mac;
} bro_opts_t;

/*  Function that uses getopt to parse every option passed to the BROFist Server
 *  It should be called before any initialisation function, to get the MAC
 *  addres for the SPAM or any other data.
 *  bro_opts_t *so:     The struct that will contain every parsed option.
 *  int argc:           Sounds familiar, doesn't it? It's the argument count
 *                      passed to the main(). It is needed, alongside argv, by
 *                      getopt.
 *  char *const argv[]: Another familiar name. It's the array of strings passed
 *                      to the applications as arguments. Another thing required
 *                      by getopt.
 */
int bro_opts_parse (bro_opts_t *so, int argc, char * const argv[]);

#endif 
