#include <stdio.h>
#include <t/parser.h>
#include <assert.h>
#include <inttypes.h>

void print_rpn(CC_Array* rpn) {
    size_t size = cc_array_size(rpn);
    for (size_t i = 0; i < size; i++) {
        token_t* tok;
        cc_array_get_at(rpn, i, &tok);

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

void run_tokenizer_tests() {
    // Тест 1: Простые числа и операторы
    //{
    //    printf("Test 1: '22 33 + 55 +' -> ");
    //    CC_Array* rpn = tokenize("22 + 33 + 55");

    //    const char* expected[] = { "22", "+", "33", "+", "55" };
    //    assert(cc_array_size(rpn) == 5);

    //    for (size_t i = 0; i < 5; i++) {
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
    //    clear_token_arr(rpn);
    //    printf("Passed\n");
    //}

    // Тест 2: Отрицательные числа
    {
        printf("Test 2: '-5 + 3 - (-2)' -> ");
        CC_Array* rpn = tokenize("-5 + 3 - (-2)");

        const char* expected[] = { "-5", "+", "3", "-", "(", "-2", ")" };
        print_rpn(rpn);
        assert(cc_array_size(rpn) == 7);
        for (size_t i = 0; i < 7; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, &tok);

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_OPEN_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_CLOSED_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(rpn);
        printf("Passed\n");
    }

    // Тест 3: Скобки и приоритеты операторов
    {
        printf("Test 3: '(2 + 3) * 4 - 5' -> ");
        CC_Array* rpn = tokenize("(2 + 3) * 4 - 5");

        const char* expected[] = { "(", "2", "+", "3", ")", "*", "4", "-", "5" };
        assert(cc_array_size(rpn) == 9);

        for (size_t i = 0; i < 9; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, &tok);

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_OPEN_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_CLOSED_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(rpn);
        printf("Passed\n");
    }

    // Тест 4: Множественные скобки и отрицательные числа
    {
        printf("Test 4: '-((3 + 4) * -5)' -> ");
        CC_Array* rpn = tokenize("-((3 + 4) * -5)");

        const char* expected[] = { "-", "(", "(", "3", "+", "4", ")", "*", "-5", ")" };
        assert(cc_array_size(rpn) == 10);

        for (size_t i = 0; i < 10; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, &tok);

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_OPEN_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_CLOSED_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(rpn);
        printf("Passed\n");
    }

    // Тест 5: Комбинация разных операторов
    {
        printf("Test 5: '2 * -3 + 4 / (-5 + 2)' -> ");
        CC_Array* rpn = tokenize("2 * -3 + 4 / (-5 + 2)");

        const char* expected[] = { "2", "*", "-3", "+", "4", "/", "(", "-5", "+", "2", ")" };
        assert(cc_array_size(rpn) == 11);

        for (size_t i = 0; i < 11; i++) {
            token_t* tok;
            cc_array_get_at(rpn, i, &tok);

            if (tok->type == TOKEN_TYPE_NUM) {
                assert(tok->sym == atoi(expected[i]));
            }
            else if (tok->type == TOKEN_TYPE_OP) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_OPEN_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
            else if (tok->type == TOKEN_TYPE_CLOSED_BRACKET) {
                assert((char)tok->sym == expected[i][0]);
            }
        }
        print_rpn(rpn);
        clear_token_arr(rpn);
        printf("Passed\n");
    }

    printf("All tokenizer tests passed successfully!\n");
}

int main() {
    run_tokenizer_tests();
    return 0;
}