/** @file:      main.cpp
 *  @brief:     Main cpp file
 *  @date:      2015年03月06日 18时20分50秒
 *  @author:    kinuxroot, kinuxroot@163.com
 *  @group:     LessOS Development Group
 *  @version:   1.0
 *  @note:
 *
 */

#include "hello/hello.h"
#include <iostream>

int main(int argc, char** argv){
    hello();
    if ( argc > 1 ) {
        std::cout << argv[1] << std::endl;
    }

    return 0;
}
