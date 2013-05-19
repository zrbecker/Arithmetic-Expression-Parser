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

int eval(char string[]);
int getError();

#endif /* PARSER_H_ */
