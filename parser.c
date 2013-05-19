/*
 * parser.c
 *
 *  Created on: May 19, 2013
 *      Author: zrbecker
 */


#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


typedef PARSER_INT_TYPE (*op_func)(PARSER_INT_TYPE, PARSER_INT_TYPE);

static void clearError();
static void evalAssert(int isOk);

static op_func charToOp(char op);
static PARSER_INT_TYPE operatorAddition(PARSER_INT_TYPE a, PARSER_INT_TYPE b);
static PARSER_INT_TYPE operatorSubtraction(PARSER_INT_TYPE a, PARSER_INT_TYPE b);
static PARSER_INT_TYPE operatorMultiplication(PARSER_INT_TYPE a, PARSER_INT_TYPE b);
static PARSER_INT_TYPE operatorDivision(PARSER_INT_TYPE a, PARSER_INT_TYPE b);
static PARSER_INT_TYPE operatorExponent(PARSER_INT_TYPE a, PARSER_INT_TYPE b);

static PARSER_INT_TYPE evaluateExpression(char *start, char *end);
static PARSER_INT_TYPE evaluateOperatorExpression(char *start, char *end, char *ops);
static PARSER_INT_TYPE evaluateIntegerExpression(char *start, char *end);
static PARSER_INT_TYPE evaluateParanthesisExpression(char *start, char *end);
static PARSER_INT_TYPE evaluateNegativeExpression(char *start, char *end);

static char *strpbrkReverse(char *start, char *end, char *needles);


static PARSER_INT_TYPE error = PARSER_ERROR_NONE;


PARSER_INT_TYPE parserEval(char str[]) {
	clearError();
	return evaluateExpression(str, str + strlen(str));
}

int parserGetError() {
	return error;
}


static op_func charToOp(char op) {
	switch (op) {
	case '+': return operatorAddition;
	case '-': return operatorSubtraction;
	case '*': return operatorMultiplication;
	case '/': return operatorDivision;
	case '^': return operatorExponent;
	}
	assert(0);
	return NULL;
}

static PARSER_INT_TYPE operatorAddition(PARSER_INT_TYPE a, PARSER_INT_TYPE b) {
	return a + b;
}

static PARSER_INT_TYPE operatorSubtraction(PARSER_INT_TYPE a, PARSER_INT_TYPE b) {
	return a - b;
}

static PARSER_INT_TYPE operatorMultiplication(PARSER_INT_TYPE a, PARSER_INT_TYPE b) {
	return a * b;
}

static PARSER_INT_TYPE operatorDivision(PARSER_INT_TYPE a, PARSER_INT_TYPE b) {
	return a / b;
}

static PARSER_INT_TYPE operatorExponent(PARSER_INT_TYPE a, PARSER_INT_TYPE b) {
	return (PARSER_INT_TYPE)pow(a, b);
}

static PARSER_INT_TYPE evaluateExpression(char *start, char *end) {
	evalAssert(start < end);
	PARSER_INT_TYPE result;

	while (*start == ' ' && start <= end)
		start += 1;
	while (*(end - 1) == ' ' && start <= end)
		end -= 1;

	result = evaluateOperatorExpression(start, end, "+-");
	if (!error)
		return result;

	clearError();
	result = evaluateOperatorExpression(start, end, "*/");
	if (!error)
		return result;

	clearError();
	result = evaluateNegativeExpression(start, end);
	if (!error)
		return result;

	clearError();
	result = evaluateOperatorExpression(start, end, "^");
	if (!error)
		return result;

	clearError();
	result = evaluateParanthesisExpression(start, end);
	if (!error)
		return result;

	clearError();
	result = evaluateIntegerExpression(start, end);
	if (!error)
		return result;

	return 0;
}

static PARSER_INT_TYPE evaluateOperatorExpression(char *start, char *end, char *ops) {
	evalAssert(start < end);
	PARSER_INT_TYPE a, b;
	char *pos = strpbrkReverse(start, end, ops);
	evalAssert(pos != NULL);

	while (pos) {
		clearError();
		a = evaluateExpression(start, pos);
		if (!error) {
			b = evaluateExpression(pos + 1, end);
			if (!error)
				return charToOp(*pos)(a, b);
		}
		pos = strpbrkReverse(start, pos, ops);
	}

	return 0;
}

static PARSER_INT_TYPE evaluateIntegerExpression(char *start, char *end) {
	evalAssert(start < end);

	evalAssert(start < end);
	if (*start == '0') {
		evalAssert(end - start == 1);
		return 0;
	}

	evalAssert(*start >= '1' && *start <= '9');
	if (error)
		return 0;
	PARSER_INT_TYPE result =  *start - '0';
	start += 1;

	while (start < end) {
		evalAssert(*start >= '0' && *start <= '9');
		if (error)
			return 0;
		result = 10 * result + (*start - '0');
		start += 1;
	}

	return result;
}

static PARSER_INT_TYPE evaluateParanthesisExpression(char *start, char *end) {
	evalAssert(start < end);
	evalAssert(*start == '(');
	evalAssert(*(end - 1) == ')');
	if (error)
		return 0;
	return evaluateExpression(start + 1, end - 1);
}

static PARSER_INT_TYPE evaluateNegativeExpression(char *start, char *end) {
	evalAssert(start < end);
	evalAssert(*start == '-');
	if (error)
		return 0;
	return -evaluateExpression(start + 1, end);
}

static void clearError() {
	error = PARSER_ERROR_NONE;
}

static void evalAssert(int isOk) {
	if (!isOk)
		error = PARSER_ERROR_FAILED;
}

// Finds the last occurrence of any character in needles
// within the string between start and end
static char *strpbrkReverse(char *start, char *end, char *needles)
{
	const char * sp;
	char c;
	while (end > start) {
		c = *--end;
		for (sp = needles; *sp != 0; ++sp)
			if (*sp == c)
				return end;
	}
	return NULL;
}
