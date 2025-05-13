#include <stdio.h>
#include <t/parser.h>
#include <assert.h>
#include <inttypes.h>


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

void det_print_rpn(CC_Array* rpn) {
    size_t size = cc_array_size(rpn);
    for (size_t i = 0; i < size; i++) {
        token_t* tok;
        cc_array_get_at(rpn, i, ((void**)&tok));

        if (tok->type == TOKEN_TYPE_NUM) {
            printf("%zu - %" PRId64 "\n ",i, tok->sym);
        }
        else if (tok->type == TOKEN_TYPE_OP) {
            printf("%zu - %c \n", i, (char)tok->sym);
        }
    }
    printf("\n");
}

void run_rpn_conversion_tests() {
    // ���� 1: ������� ��������
    //{
    //    printf("Test 1: '3 + 4' -> ");
    //    CC_Array* tokens = tokenize("3 + 4");
    //    CC_Array* rpn = convert_rpn(tokens);

    //    const char* expected[] = { "3", "4", "+" };
    //    assert(cc_array_size(rpn) == 3);

    //    for (size_t i = 0; i < 3; i++) {
    //        token_t* tok;
    //        cc_array_get_at(rpn, i, &tok);

    //        if (tok->type == TOKEN_TYPE_NUM) {
    //            assert(tok->sym == atoi(expected[i]));
    //        }
    //        else if (tok->type == TOKEN_TYPE_OP) {
    //            assert((char)tok->sym == expected[i][0]);
    //        }
    //    }
    //    print_rpn(rpn);
    //    clear_token_arr(tokens);
    //    cc_array_destroy(rpn);
    //    printf("Passed\n");
    //}

    // ���� 2: ������������� �����
    {
        printf("Test 2: '-5 + 3 - (-2)' -> ");
        CC_Array* tokens = tokenize("-5 + 3 - (-2)");
        print_rpn(tokens);
        CC_Array* rpn = convert_rpn(tokens);
        det_print_rpn(rpn);
        const char* expected[] = { "-5", "3", "+", "-2", "-" };
        assert(cc_array_size(rpn) == 5);

        for (size_t i = 0; i < 5; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, ((void**)&tok));

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(tokens);
        cc_array_destroy(rpn);
        printf("Passed\n");
    }

    // ���� 3: ��������� ����������
    {
        printf("Test 3: '2 + 3 * 4' -> ");
        CC_Array* tokens = tokenize("2 + 3 * 4");
        CC_Array* rpn = convert_rpn(tokens);

        const char* expected[] = { "2", "3", "4", "*", "+" };
        assert(cc_array_size(rpn) == 5);

        for (size_t i = 0; i < 5; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, ((void**)&tok));

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(tokens);
        cc_array_destroy(rpn);
        printf("Passed\n");
    }

    // ���� 4: ������
    {
        printf("Test 4: '(2 + 3) * 4' -> ");
        CC_Array* tokens = tokenize("(2 + 3) * 4");
        CC_Array* rpn = convert_rpn(tokens);

        const char* expected[] = { "2", "3", "+", "4", "*" };
        assert(cc_array_size(rpn) == 5);

        for (size_t i = 0; i < 5; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, ((void**)&tok));

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(tokens);
        cc_array_destroy(rpn);
        printf("Passed\n");
    }

    // ���� 5: ����������� ���������
    {
        printf("Test 5: '3 + 4 * 2 / (1 - 5) + -2' -> ");
        CC_Array* tokens = tokenize("3 + 4 * 2 / (1 - 5) + -2");
        CC_Array* rpn = convert_rpn(tokens);

        const char* expected[] = { "3", "4", "2", "*", "1", "5", "-", "/", "+", "-2", "+" };
        assert(cc_array_size(rpn) == 11);

        for (size_t i = 0; i < 11; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, ((void**)&tok));

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(tokens);
        cc_array_destroy(rpn);
        printf("Passed\n");
    }

    // ���� 6: ������������� ������
    {
        printf("Test 6: '((2 + 3) * (4 - 1))' -> ");
        CC_Array* tokens = tokenize("((2 + 3) * (4 - 1))");
        CC_Array* rpn = convert_rpn(tokens);

        const char* expected[] = { "2", "3", "+", "4", "1", "-", "*" };
        assert(cc_array_size(rpn) == 7);

        for (size_t i = 0; i < 7; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, ((void**)&tok));

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(tokens);
        cc_array_destroy(rpn);
        printf("Passed\n");
    }

    printf("All RPN conversion tests passed successfully!\n");
}

int main() {
    run_rpn_conversion_tests();
    return 0;
}