#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include <t/calc.h>
#include <t/parser.h>

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


void run_test(const char* expr, num_t expected, const char* test_name) {
    printf("Running test: %s\n", test_name);
    printf("Expression: %s\n", expr);

    CC_Array* tokens = tokenize(expr);
    assert(tokens != NULL && "Tokenization failed");
    CC_Array* rpn = convert_rpn(tokens);
    assert(rpn != NULL && "convertation failed");

    uint8_t err = 0;
    num_t res = calc(rpn, &err);

    printf("Expected: %" PRId64 ", got: %" PRId64 "\n", expected, res);
    //printf("Expected: %f, got: %f\n", expected, res);
    print_rpn(rpn);
    assert(!err && "Calculation error occurred");
    assert(res == expected && "Wrong calculation result");

    clear_token_arr(rpn);
    printf("Test passed!\n\n");
}

void run_error_test(const char* expr, const char* test_name) {
    printf("Running error test: %s\n", test_name);
    printf("Expression: %s\n", expr);

    CC_Array* tokens = tokenize(expr);
    assert(tokens != NULL && "Tokenization failed");
    CC_Array* rpn = convert_rpn(tokens);
    assert(rpn != NULL && "convertation failed");

    uint8_t err = 0;
    num_t res = calc(rpn, &err);

    assert(err && "Expected error but calculation succeeded");

    clear_token_arr(rpn);
    printf("Error test passed (got expected error)!\n\n");
}

int main(void) {
    // Basic arithmetic
    run_test("2 + 3", 5, "Simple addition");
    run_test("10 - 4", 6, "Simple subtraction");
    run_test("7 * 3", 21, "Simple multiplication");
    run_test("15 / 3", 5, "Simple division");
    run_test("17 % 5", 2, "Simple modulus");

    // Operator precedence
    run_test("2 + 3 * 4", 14, "Operator precedence 1");
    run_test("3 * 4 + 2", 14, "Operator precedence 2");
    run_test("2 + 3 * 4 - 5 / 2", 12, "Operator precedence 3");

    // Parentheses
    run_test("(2 + 3) * 4", 20, "Parentheses 1");
    run_test("3 * (4 + 5)", 27, "Parentheses 2");
    run_test("(2 + 3) * (4 - 5)", -5, "Parentheses 3");
    run_test("((2 + 3) * 4) - 5", 15, "Nested parentheses 1");
    run_test("1 + 2 * (3 + 4 * (5 + 6))", 95, "Nested parentheses 2");

    // Negative numbers
    run_test("-5 + 3", -2, "Negative numbers 1");
    run_test("5 * -3", -15, "Negative numbers 2");
    run_test("(-5 + 3) * -2", 4, "Negative numbers 3");

    // Division and modulus
    run_test("10 / 3", 3.333333, "Integer division");
    run_test("10 % 3", 1, "Modulus operation");
    run_test("5 * (3 + 4) / 2", 17, "Division with remainder");

    // Large numbers
    run_test("1000000 * 1000000", 1000000000000, "Large numbers multiplication");
    run_test("123456789 + 987654321", 1111111110, "Large numbers addition");

    // Edge cases
    run_test("0 + 0", 0, "Zero addition");
    run_test("5 * 0", 0, "Multiplication by zero");
    run_test("1", 1, "Single number");
    run_test("  42  ", 42, "Number with spaces");

    // Error cases
    run_error_test("2 / 0", "Division by zero");
    run_error_test("2 + ", "Incomplete expression");
    run_error_test("+ 2", "Invalid expression start");
    run_error_test("2 + + 3", "Double operator");

    printf("All tests passed successfully!\n");
    return 0;
}