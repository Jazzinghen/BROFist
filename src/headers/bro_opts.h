#ifndef __bro_headers_options_h
#define __bro_headers_options_h

typedef struct {
  bdaddr_t mac;
} bro_opts_t;


int opts_parse (bro_opts_t *so, int argc, char * const argv[])
