#include <iostream>
#include <lemon/test/test.hpp>

int j = 0;

test_(fs_create){

    std::cout << "hello world" << std::endl;
}


bench_(fs_create){

    for(int i = 0; i < N; i ++ )
    {
        j ++;
    }
}
