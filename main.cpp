#include "Chip8_test_implement.hpp"
#include <iostream>
#include <print>


int main(){
    chip8 cpu; 
     std::cout<<cpu.PC<<"\n"; 
     std::println("Memory: {::x}",cpu.memory);
    return 0; 
}