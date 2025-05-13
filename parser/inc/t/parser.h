#pragma once
#include <stdint.h>
#include <cc_list.h>
#include <cc_stack.h>


typedef enum _token_type_t
{
    TOKEN_TYPE_NUM,
    TOKEN_TYPE_OP,
    TOKEN_TYPE_OPEN_BRACKET,
    TOKEN_TYPE_CLOSED_BRACKET,
} token_type_t;

typedef struct _token_t
{
    int64_t sym;
    uint8_t type;
} token_t;


void clear_token_arr(CC_Array* arr);

CC_Array* tokenize(const char* str);

//warning all pointers will be copied from arr!
CC_Array* convert_rpn(CC_Array* arr);
