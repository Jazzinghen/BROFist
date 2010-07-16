#ifndef __bro_headers_fist_h
#define __bro_headers_fist_h

typedef struct {
    uint16_t size;
    uint8_t operation;
    union {
        int32_t int_data;
        float   real_data;
    } data;
} __attribute__((__packed__)) bro_fist_t;
