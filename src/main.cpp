/*
author : lion
*/
#include <iostream>
#include <memory>
#include <run/src/run.h>

int main(){
auto run{std::make_unique<RUN::Run_t>()};

run->start();
    return 0 ;
}
