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
#include <iostream>
int main(int argc, char** argv) {
    CalculatorParser parser;
    parser.parse(string(argv[1]));
    cout << parser.calculate(parser.getPostFix()) << endl;
    return 0;
}