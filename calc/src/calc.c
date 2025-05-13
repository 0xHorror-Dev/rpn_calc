#include <t/calc.h>
#include <t/parser.h>

#include <cc_list.h>
#include <ctype.h>
#include <cc_stack.h>
#include <stdio.h>
#include "sized/cc_array_sized.h"


static inline void free_stack(CC_Stack* stack)
{
	size_t len = cc_stack_size(stack);
	for (size_t i = 0; i < len; i += 1)
	{
		num_t* val;
		cc_stack_pop(stack,(void**)&val);
		free(val);
	}

	cc_stack_destroy(stack);
}

num_t calc(CC_Array* rpn, uint8_t* err)
{
	size_t len = cc_array_size(rpn);
	CC_Stack* stack;
	if (cc_stack_new(&stack) != CC_OK)
	{
		*err = 1;
		return -1;
	}

	for (size_t i = 0; i < len; i += 1)
	{
		token_t* tok;
		if (cc_array_get_at(rpn, i, (void**)&tok) != CC_OK)
		{
			free_stack(stack);
			*err = 1;
			return -1;
		}

		switch (tok->type)
		{
		case TOKEN_TYPE_NUM:
			{
			num_t* num = calloc(1, sizeof(num_t));
				if (num == NULL)
				{
					free_stack(stack);
					*err = 1;
					return -1;
				}
				(*num) = tok->sym;
				if (cc_stack_push(stack, num) != CC_OK)
				{
					free_stack(stack);
					*err = 1;
					return -1;
				}
			}
			break;
		case TOKEN_TYPE_OP:
		{
			num_t* out1;
			num_t* out2;
			if (cc_stack_pop(stack, (void**)&out1) != CC_OK)
			{
				free_stack(stack);
				*err = 1;
				return -1;
			}
			if (cc_stack_pop(stack, (void**)&out2) != CC_OK)
			{
				free(out1);
				free_stack(stack);
				*err = 1;
				return -1;
			}

			switch ((char)tok->sym)
			{
			case '+':
				*out1 = (*out2) + (*out1);
				break;
			case '-':
				*out1 = (*out2) - (*out1);
				break;
			case '*':
				*out1 = (*out2) * (*out1);
				break;
			case '/':
				if (*out1 == 0) {
					free(out1);
					free(out2);
					free_stack(stack);
					*err = 1;
					return -1;
				}
				*out1 = *out2 / *out1;
				break;
			case '%':
				if (*out1 == 0) {
					free(out1);
					free(out2);
					free_stack(stack);
					*err = 1;
					return -1;
				}
				*out1 = (int64_t)(*out2) % (int64_t)(*out1);
				break;
			}

			if (cc_stack_push(stack, out1) != CC_OK)
			{
				free(out1);
				free(out2);
				free_stack(stack);
				*err = 1;
				return -1;
			}

			free(out2);

		}
			break;
		}
	}

	num_t* res = NULL;
	if (cc_stack_pop(stack, (void**)&res) != CC_OK)
	{
		free_stack(stack);
		*err = 1;
		return -1;
	}

	num_t tmp = *res;
	free(res);

	cc_stack_destroy(stack);

	return tmp;
}