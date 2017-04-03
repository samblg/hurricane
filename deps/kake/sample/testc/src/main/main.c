#include <hello.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
    hello();
    FILE* f = fopen(argv[1], "r");
    if ( f ) {
        char str[80];
        memset(str, '\0', 80);

        fgets(str, 80, f);
        puts(str);
    }
    
    return 0;
}
