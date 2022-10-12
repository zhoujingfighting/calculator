/**
 * @file main.cpp
 * @author zhou jing (254644528@qq.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "parser.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>

int main(void) {
    CalculatorParser parser;
    parser.parse("1+2");
    return 0;
}