#include <t/parser.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <inttypes.h>
#include <sized/cc_array_sized.h>


static inline token_t* token_init(int64_t n, const token_type_t type)
{
    token_t* tok = calloc(1, sizeof(token_t));
    if (tok == NULL) return NULL;

    tok->type = type;
    tok->sym = n;

    return tok;
}
static inline void token_free(token_t* t)
{
    free(t);
}

uint8_t is_left_assoc(char op) {
    switch (op) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
        return true;  // Left-associative operators
    case '^':
        return false; // Right-associative (exponentiation)
    default:
        return true;  // Default (assuming left-associative if unknown)
    }
}

uint8_t get_prior(char c)
{
    switch (c)
    {
    case '+':
    case '-': return 1;
    case '*':
    case '/': 
    case '%': return 2;
    case '^': return 3;
    }
    return 0;
}


static void print_rpn(CC_Array* rpn) {
    size_t size = cc_array_size(rpn);
    for (size_t i = 0; i < size; i++) {
        token_t* tok;
        cc_array_get_at(rpn, i, ((void**)&tok));

        if (tok->type == TOKEN_TYPE_NUM) {
            printf("%" PRId64 " ", tok->sym);
        }
        else if (tok->type == TOKEN_TYPE_OP) {
            printf("%c ", (char)tok->sym);
        }
        else if (tok->type == TOKEN_TYPE_OPEN_BRACKET || tok->type == TOKEN_TYPE_CLOSED_BRACKET)
        {
            printf("%c ", (char)tok->sym);
        }
    }
    printf("\n");
}


void clear_token_arr(CC_Array* arr)
{
    size_t size = cc_array_size(arr);
    for (size_t i = 0; i < size - 1; i += 1)
    {
        token_t* tok;
        cc_array_get_at(arr, i, ((void**)&tok));
        token_free(tok);
    }

    cc_array_destroy(arr);
}

static inline void clear_stack(CC_Stack* stack)
{
    size_t size = cc_stack_size(stack);
    for (size_t i = 0; i < size; i += 1)
    {
        token_t* tok;
        cc_stack_pop(stack, ((void**)&tok));
        token_free(tok);
    }

    cc_stack_destroy(stack);
}

CC_Array* tokenize(const char* str)
{
    size_t slen = strlen(str);

    CC_Array* sout;
    if (cc_array_new(&sout) != CC_OK)
    {
        return NULL;
    }

    for (size_t i = 0; i < slen; i += 1)
    {
        char c = str[i];
        if (isblank(c)) continue;

        if (c == '-')
        {
            token_t* last;
            if (cc_array_get_last(sout, ((void**)&last)) == CC_OK)
            {
                if ((last->type == TOKEN_TYPE_OP || last->type == TOKEN_TYPE_OPEN_BRACKET || last->type == TOKEN_TYPE_CLOSED_BRACKET) && (i < slen && isdigit(str[i + 1])))
                {
                    CC_ArraySized* reader;
                    if (cc_array_sized_new(sizeof(char), &reader) != CC_OK)
                    {
                        clear_token_arr(sout);
                        return NULL;
                    }
                    i += 1;
                    c = str[i];
                    char minus = '-';
                    cc_array_sized_add(reader, &minus);
                    while (isdigit(c)) {
                        cc_array_sized_add(reader, &c);
                        i++;
                        c = str[i];
                    }
                    char null_term = 0x0;
                    cc_array_sized_add(reader, &null_term);

                    const char* buf  = (const char*)cc_array_sized_get_buffer(reader);

                    int64_t num = atoll(buf);

                    token_t* new_tok = token_init(num, TOKEN_TYPE_NUM);

                    cc_array_sized_destroy(reader);
                    if (new_tok == NULL)
                    {
                        clear_token_arr(sout);
                        return NULL;
                    }

                    cc_array_add(sout, new_tok);

                    i -= 1;
                    continue;
                }
            }
            else
            {
                if (i+1 < slen && isdigit(str[i + 1]))
                {
                    CC_ArraySized* reader;
                    if (cc_array_sized_new(sizeof(char), &reader) != CC_OK)
                    {
                        clear_token_arr(sout);
                        return NULL;
                    }
                    char minus = '-';
                    cc_array_sized_add(reader, &minus);
                    i += 1;
                    c = str[i];
                    while (isdigit(c)) {
                        cc_array_sized_add(reader, &c);
                        i++;
                        c = str[i];
                    }
                    char null_term = 0x0;
                    cc_array_sized_add(reader, &null_term);

                    const char* buf = (const char*)cc_array_sized_get_buffer(reader);

                    int64_t num = atoll(buf);

                    token_t* new_tok = token_init(num, TOKEN_TYPE_NUM);

                    cc_array_sized_destroy(reader);
                    if (new_tok == NULL)
                    {
                        clear_token_arr(sout);
                        return NULL;
                    }

                    cc_array_add(sout, new_tok);

                    i -= 1;
                    continue;
                }
            }
        }
        if (c == '(')
        {
            token_t* out = token_init(c, TOKEN_TYPE_OPEN_BRACKET);
            cc_array_add(sout, out);
        }
        else if (c == ')')
        {
            token_t* out = token_init(c, TOKEN_TYPE_CLOSED_BRACKET);
            cc_array_add(sout, out);
        }
        else if (ispunct(c))
        {
            token_t* out = token_init(c, TOKEN_TYPE_OP);
            cc_array_add(sout, out);
        }
        else
        {
            CC_ArraySized* reader;
            if (cc_array_sized_new(sizeof(char), &reader) != CC_OK)
            {
                clear_token_arr(sout);
                return NULL;
            }

            while (isdigit(c)) {
                cc_array_sized_add(reader, &c);
                i++;
                c = str[i];
            }
            
            char null_term = 0x0;
            cc_array_sized_add(reader, &null_term);
            const char* buf = (const char*)cc_array_sized_get_buffer(reader);
            int64_t num = atoll(buf);

            token_t* new_tok = token_init(num, TOKEN_TYPE_NUM);
            cc_array_sized_destroy(reader);
            if (new_tok == NULL)
            {
                clear_token_arr(sout);
                return NULL;
            }

            cc_array_add(sout, new_tok);

            i -= 1;
        }
    }


    return sout;
}


CC_Array* convert_rpn(CC_Array* arr)
{
    CC_Stack* stack;
    CC_Array* sout;
    size_t slen = cc_array_size(arr);

    if (cc_array_new(&sout) != CC_OK)
    {
        return NULL;
    }

    if (cc_stack_new(&stack) != CC_OK)
    {
        cc_array_destroy(sout);
        return NULL;
    }

    for (size_t i = 0; i < slen; i += 1)
    {
        token_t* token;
        cc_array_get_at(arr, i, ((void**)&token));

        switch (token->type)
        {
        case TOKEN_TYPE_OP:
        {
            token_t* prev_op;
            while (cc_stack_peek(stack, ((void**)&prev_op)) == CC_OK&& prev_op->sym != '(' &&  (get_prior(prev_op->sym) > get_prior(token->sym) || (get_prior(prev_op->sym) == get_prior(token->sym) && is_left_assoc(token->sym))))
            {
                if (cc_stack_pop(stack, ((void**)&prev_op)) != CC_OK)
                    break;
                
                if (cc_array_add(sout, prev_op) != CC_OK)
                {
                    cc_stack_destroy(stack);
                    cc_array_destroy(sout);
                    return NULL;
                }
            }

            if (cc_stack_push(stack, token) != CC_OK)
            {
                clear_stack(stack);
                clear_token_arr(sout);
                return NULL;
            }
        }
            break;
        case TOKEN_TYPE_NUM:
        {
            if (cc_array_add(sout, token) != CC_OK)
            {
                cc_stack_destroy(stack);
                cc_array_destroy(sout);
                return NULL;
            }
        }
            break;
        case TOKEN_TYPE_OPEN_BRACKET:
            if (cc_stack_push(stack, token) != CC_OK)
            {
                cc_stack_destroy(stack);
                cc_array_destroy(sout);
                return NULL;
            }
            break;
        case TOKEN_TYPE_CLOSED_BRACKET:
        {
            token_t* out = NULL;
            if (cc_stack_pop(stack, ((void**)&out)) == CC_OK)
            {
                while (out->type != TOKEN_TYPE_OPEN_BRACKET)
                {
                    if (cc_array_add(sout, out) != CC_OK)
                    {
                        cc_stack_destroy(stack);
                        cc_array_destroy(sout);
                        return NULL;
                    }
                    if (cc_stack_pop(stack, ((void**)&out)) == CC_ERR_VALUE_NOT_FOUND) break;
                    if (out->type == TOKEN_TYPE_OPEN_BRACKET) break;
                }
                //print_rpn(sout);
            }
        }

            break;
        }

    }

    token_t* out = NULL;
    int64_t size = cc_stack_size(stack);
    while (size > 0)
    {
        if (cc_stack_pop(stack, ((void**)&out)) != CC_OK)
        {
            cc_stack_destroy(stack);
            cc_array_destroy(sout);
            return NULL;
        }
        cc_array_add(sout, out);
        size -= 1;
    }

    cc_stack_destroy(stack);
    return sout;
}

//CC_Array* tokenize(const char* str)
//{
//    size_t slen = strlen(str);
//
//    CC_Stack* stack;
//    if (cc_stack_new(&stack) != CC_OK)
//    {
//        return NULL;
//    }
//
//    CC_Array* sout;
//    if (cc_array_new(&sout) != CC_OK)
//    {
//        cc_stack_destroy(stack);
//        return NULL;
//    }
//
//    for (size_t i = 0; i < slen; i += 1)
//    {
//        char c = str[i];
//        if (isblank(c)) continue;
//
//
//        if (c == '(')
//        {
//            token_t* new_tok = token_init(0x0, TOKEN_TYPE_OPEN_BRACKET);
//            if (cc_stack_push(stack, new_tok) != CC_OK)
//            {
//                clear_stack(stack);
//                clear_token_arr(sout);
//                return NULL;
//            }
//        }
//        else if (c == ')')
//        {
//            token_t* out = NULL;
//            if (cc_stack_pop(stack, &out) == CC_OK)
//            {
//                while (out->type != TOKEN_TYPE_OPEN_BRACKET)
//                {
//                    if (cc_array_add(sout, out) != CC_OK)
//                    {
//                        clear_stack(stack);
//                        clear_token_arr(sout);
//                        return NULL;
//                    }
//                    if (cc_stack_pop(stack, &out) == CC_ERR_VALUE_NOT_FOUND) break;
//                    if (out->type == TOKEN_TYPE_OPEN_BRACKET) break;
//                }
//                free(out);
//                print_rpn(sout);
//            }
//        }
//        else if (ispunct(c))
//        {
//            token_t* out;
//
//            while (cc_stack_peek(stack, &out) == CC_OK &&
//                out->sym != '(' &&  
//                (
//                    get_prior(out->sym) > get_prior(c) ||
//                    (get_prior(out->sym) == get_prior(c) && is_left_assoc(c))
//                   )
//                )
//            {
//                if (cc_stack_pop(stack, &out) != CC_OK)
//                    break;
//
//                if (cc_array_add(sout, out) != CC_OK)
//                {
//                    clear_stack(stack);
//                    clear_token_arr(sout);
//                    return NULL;
//                }
//            }
//
//
//            token_t* new_tok = token_init(c, TOKEN_TYPE_OP);
//            if (new_tok == NULL)
//            {
//                clear_stack(stack);
//                clear_token_arr(sout);
//                return NULL;
//            }
//
//            if (cc_stack_push(stack, new_tok) != CC_OK)
//            {
//                clear_stack(stack);
//                clear_token_arr(sout);
//                return NULL;
//            }
//
//        }
//        else
//        {
//            //size_t j = i;
//            CC_ArraySized* reader;
//            if (cc_array_sized_new(sizeof(char), &reader) != CC_OK)
//            {
//                clear_stack(stack);
//                clear_token_arr(sout);
//                return NULL;
//            }
//
//            bool negative = false;
//            if (negative) {
//                char minus = '-';
//                cc_array_sized_add(reader, &minus);
//            }
//
//            while (isdigit(c)) {
//                cc_array_sized_add(reader, &c);
//                i++;
//                c = str[i];
//            }
//
//
//            char null_term = 0x0;
//            cc_array_sized_add(reader, &null_term);
//
//            //while (str[j] != ' ')
//            //{
//            //    char c = str[j];
//            //    cc_array_sized_add(reader, &c);
//            //    j += 1;
//            //    c = str[j];
//            //    if (str[j] == ' ')break;
//            //    if (str[j] == 0x0)break;
//            //    if (str[j] == '(')break;
//            //    if (str[j] == ')')break;
//            //    if (isblank(str[i]))break;
//            //    if (ispunct(str[j]))break;
//            //}
//            
//            const char* buf = cc_array_sized_get_buffer(reader);
//            int64_t num = atoll(buf);
//
//            token_t* new_tok = token_init(num, TOKEN_TYPE_NUM);
//            cc_array_sized_destroy(reader);
//            if (new_tok == NULL)
//            {
//                clear_stack(stack);
//                clear_token_arr(sout);
//                return NULL;
//            }
//
//            cc_array_add(sout, new_tok);
//
//            i -= 1;
//            //i = j - 1;
//            continue;
//        }
//    }
//
//    token_t* out = NULL;
//    int64_t size = cc_stack_size(stack);
//    while (size > 0)
//    {
//        if (cc_stack_pop(stack, &out) != CC_OK)
//        {
//            clear_stack(stack);
//            clear_token_arr(sout);
//            return NULL;
//        }
//        cc_array_add(sout, out);
//        size -= 1;
//    }
//
//    cc_stack_destroy(stack);
//
//    return sout;
//}

