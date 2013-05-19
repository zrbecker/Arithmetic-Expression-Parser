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


typedef int (*op_func)(int, int);

void clearError();
void evalAssert(int isOk);

op_func charToOp(char op);
int operatorAddition(int a, int b);
int operatorSubtraction(int a, int b);
int operatorMultiplication(int a, int b);
int operatorDivision(int a, int b);
int operatorExponent(int a, int b);

int evaluateExpression(char *start, char *end);
int evaluateOperatorExpression(char *start, char *end, char *ops);
int evaluateIntegerExpression(char *start, char *end);
int evaluateParanthesisExpression(char *start, char *end);
int evaluateNegativeExpression(char *start, char *end);

void trimWhiteSpace(char **start, char **end);
char *strpbrkReverse(char *start, char *end, char *needles);


static int error = PARSER_ERROR_NONE;


int eval(char str[]) {
	clearError();
	return evaluateExpression(str, str + strlen(str));
}

op_func charToOp(char op) {
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

int operatorAddition(int a, int b) {
	return a + b;
}

int operatorSubtraction(int a, int b) {
	return a - b;
}

int operatorMultiplication(int a, int b) {
	return a * b;
}

int operatorDivision(int a, int b) {
	return a / b;
}

int operatorExponent(int a, int b) {
	return (int)pow(a, b);
}

int evaluateExpression(char *start, char *end) {
	evalAssert(start < end);
	int result;

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

int evaluateOperatorExpression(char *start, char *end, char *ops) {
	evalAssert(start < end);
	int a, b;
	char *pos = strpbrkReverse(start, end, ops);
	evalAssert(pos != NULL && pos < end);

	while (pos && pos < end) {
		clearError();
		a = evaluateExpression(start, pos);
		if (!error) {
			clearError();
			b = evaluateExpression(pos + 1, end);
			if (!error)
				return charToOp(*pos)(a, b);
		}
		pos = strpbrkReverse(start, pos, ops);
	}

	return 0;
}

int evaluateIntegerExpression(char *start, char *end) {
	evalAssert(start < end);

	evalAssert(start < end);
	if (*start == '0') {
		evalAssert(end - start == 1);
		return 0;
	}

	evalAssert(*start >= '1' && *start <= '9');
	if (error)
		return 0;
	int result =  *start - '0';
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

int evaluateParanthesisExpression(char *start, char *end) {
	evalAssert(start < end);
	evalAssert(*start == '(');
	evalAssert(*(end - 1) == ')');
	if (error)
		return 0;
	return evaluateExpression(start + 1, end - 1);
}

int evaluateNegativeExpression(char *start, char *end) {
	evalAssert(start < end);
	evalAssert(*start == '-');
	if (error)
		return 0;
	return -evaluateExpression(start + 1, end);
}

int getError() {
	return error;
}

void clearError() {
	error = PARSER_ERROR_NONE;
}

void evalAssert(int isOk) {
	if (!isOk)
		error = PARSER_ERROR_FAILED;
}

void trimWhiteSpace(char **start, char **end) {
	while ((**start == ' ' || **start == '\t' || **start == '\r' || **start == '\n') && *start <= *end)
		*start += 1;
	while ((**(end - 1) == ' ' || **(end - 1) == '\t' || **(end - 1) == '\r' || **(end - 1) == '\n') && *start <= *end)
		*end -= 1;
}

// Finds the last occurrence of any character in needles
// within the string between start and end
char *strpbrkReverse(char *start, char *end, char *needles)
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



























