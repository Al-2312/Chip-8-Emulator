#pragma once
#include <cstddef>
#include <cstdint> // Required for uint32_t
#include <array>
//#include <iterator>
//#include <memory>
//#include <span>
#include <algorithm>

constexpr uint8_t char_fonts[] = 
{0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static constexpr uint16_t screenWidth {64};
static constexpr uint16_t screenHeight {32};
static constexpr uint16_t resolution {screenHeight * screenWidth};

constexpr float clockSpeed {1760640.0f }; //1.76064 MHz
constexpr uint8_t Hz {60}; // I have no idea what type this should be
constexpr float cyclePerFrame {clockSpeed/ static_cast<float>(Hz) }; //2.934400e+04 cycle per frame


class chip8{
public:
    chip8(){
        PC=0x200; //we should load the program at 0x200 
        std::ranges::copy(std::begin(char_fonts), std::end(char_fonts), 
        std::begin(memory) + 0x050);

    };
    
    uint16_t I {0x0000};//index register 
    uint16_t PC {0x0000};
    std::array<uint16_t,16> stack{};
    // Should i make this dynamic 
    uint16_t opcode{};
    //bit manipulation with keypad
    std::array<uint8_t,16> keypad {};
    //16 8bit registers
    std::array<uint8_t,16> VREG {};
    //Timers
    uint8_t dt {0x00}; //delay timer
    uint8_t st {0x00}; //sound timer 
    uint8_t sp {0x00}; //STack ptr; 
    
    std::array<uint8_t,resolution> screen{}; //no 1 bit size  
    std::array<uint8_t, 4096> memory{};
    //when stackCounter is 15 reset to 0 to keep pushing
    std::size_t stackCounter {};  

    void I_0NNN(); void I_00E0(); void I_00EE();
    void I_1NNN(); void I_2NNN(); void I_3XNN();
    void I_4XNN(); void I_5XY0(); void I_6XNN();
    void I_7XNN(); void I_8XY0(); void I_8XY1(); 
    void I_8XY2(); void I_8XY3(); void I_8XY4(); 
    void I_8XY5(); void I_8XY6(); void I_8XY7(); 
    void I_8XYE(); void I_9XY0(); void I_ANNN();
    void I_BNNN(); void I_CXNN(); void I_DXYN();
    void I_EX9E(); void I_EXA1(); void I_FX07();
    void I_FX0A(); void I_FX15(); void I_FX18();
    void I_FX1E(); void I_FX29(); void I_FX33();
    void I_FX55(); void I_FX65();     

    uint8_t FETCH();
    uint8_t DECODE();
    uint8_t EXECUTE(); 
    void ResetStack();
};

