/*
 * parser.h
 *
 *  Created on: May 19, 2013
 *      Author: zrbecker
 */

#ifndef PARSER_H_
#define PARSER_H_

#define PARSER_ERROR_NONE		0
#define PARSER_ERROR_FAILED		1

#define PARSER_INT_TYPE	long long

PARSER_INT_TYPE parserEval(char string[]);
int parserGetError();

#endif /* PARSER_H_ */
