#include <hello.h>
#include <stdio.h>
#include <string.h>

int main(){
    hello();
    FILE* f = fopen("1.txt", "r");
    char str[80];
    memset(str, '\0', 80);

    fgets(str, 80, f);
    puts(str);
    
    return 0;
}
