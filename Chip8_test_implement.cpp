#include "Chip8_test_implement.hpp"
#include <cstdint>


void chip8::ResetStack(){
    if(stackCounter <= 15)
        stackCounter = 0;
    else {
        ++stackCounter;
    }
};

void chip8::I_0NNN(){
    //Execute machine language routine
    // Will be unused,
};

void chip8::I_00E0(){
    //CLEAR SCREEN:
    std::ranges::fill( screen , 0);
};

void chip8::I_00EE(){
    //RETURN FROM SUBROUTINE:
    // I may need to +0xFF to go to next instuction
    PC = stack[stackCounter];
    ResetStack();
};

void chip8::I_1NNN(){
    //JUMP:
    PC = opcode & 0x0FFF;
};

void chip8::I_2NNN(){
    //EXECUTE SUBROUTINE:
    stack[stackCounter] = PC;
    ResetStack();
    PC = opcode & 0x0FFF;
};

void chip8::I_3XNN(){ 
    uint8_t val = opcode & 0x00FF;
    uint8_t index = (opcode & 0x0F00) >> 8;

    if(VREG[index] == val){
        // move by 2 bytes
        PC += 0x02;
    }
};

void chip8::I_4XNN(){
    uint8_t val = opcode & 0x00FF;
    uint8_t index = (opcode & 0x0F00) >> 8;

    if(VREG[index]  != val){
        // move by 2 bytes
        PC += 0x02;
    }
};

void chip8::I_5XY0(){
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    if(VREG[indexX] == VREG[indexY]){
        // move by 2 bytes
        PC += 0x02;
    }
};


void chip8::I_6XNN(){
    //LOAD
    uint8_t val = opcode & 0x00FF;
    uint8_t index = (opcode & 0x0F00) >> 8;
    VREG[index]= val;
};

void chip8::I_7XNN(){
    uint8_t val = opcode & 0x00FF;
    uint8_t index = (opcode & 0x0F00) >> 8;
    VREG[index] += val;
};

void chip8::I_8XY0(){
    //SET: VX is set to the value of VY    
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    VREG[indexX] = VREG[indexY];
};

void chip8::I_8XY1(){
    //Binary OR: VX is set to the bitwise OR of VX and VY. VY is not affected.
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    VREG[indexX] |= VREG[indexY];
};

void chip8::I_8XY2(){
    //Binary AND: VX is set to the bitwise AND of VX and VY. VY is not affected.
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    VREG[indexX] &= VREG[indexY];
};

void chip8::I_8XY3(){
    //Binary XOR: VX is set to the bitwise XOR of VX and VY. VY is not affected.
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    VREG[indexX] ^= VREG[indexY];
};

void chip8::I_8XY4(){
    //Add: VX is set to the value of VX plus the value of VY
    // this addition will affect the carry flag
    //If the result is larger than 255  the flag register VF is set to 1. 
    // If it doesn’t overflow, VF is set to 0.
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    if(VREG[indexX] > (VREG[indexX] + VREG[indexY])){
        VREG[0xF]=1;
    }
    else{
        VREG[0xF]=0;
    }
    VREG[indexX] += VREG[indexY];
};


// for both sub 
//if the minuend (the first operand) is larger than or equal to the subtrahend (second operand), 
// VF will be set to 1. If the subtrahend is larger, and we “underflow” the result, VF is set to 0
void chip8::I_8XY5(){
    //SUB sets VX to the result of VX - VY.
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    if( VREG[indexX] >= VREG[indexY]) {
        VREG[0xF]=1;
    }
    else if ( (VREG[indexY] > VREG[indexX]) && (VREG[indexX] - VREG[indexY] > VREG[indexX])) {
        VREG[0xF]=0;
    }


    VREG[indexX] = VREG[indexX] - VREG[indexY];
};

void chip8::I_8XY6(){
    //SHIFT RIGHT:
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    //CHIP-8 interpreter for the original COSMAC VIP
    VREG[indexX] = VREG[indexY];
    if((VREG[indexX] & 0x1) == 1){
        VREG[0xF] = 1;
    }
    else {
        VREG[0xF] = 0;
    }
    VREG[indexX] = VREG[indexX] >> 1;
};

void chip8::I_8XY7(){
    //SUB sets VX to the result of VY - VX.
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    if( VREG[indexY] >= VREG[indexX]) {
        VREG[0xF]=1;
    }
    else if ( (VREG[indexX] > VREG[indexY]) && (VREG[indexY] - VREG[indexX] > VREG[indexY])) {
        VREG[0xF]=0;
    }
    VREG[indexX] = VREG[indexY] - VREG[indexX];
};

void chip8::I_8XYE(){
    //SHIFT LEFT:
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    //CHIP-8 interpreter for the original COSMAC VIP
    VREG[indexX] = VREG[indexY];
    //due to warning we will create a temp variable 
    // too warning: bitwise comparison always evaluates to false
    //[-Wtautological-bitwise-compare]
    uint8_t checkFlag = VREG[indexX] & 0x8000;
    if( checkFlag == 1){
        VREG[0xF] = 1;
    }
    else {
        VREG[0xF] = 0;
    }
    VREG[indexX] = VREG[indexX] << 1;
};

void chip8::I_9XY0(){
    uint8_t indexY = (opcode & 0x00F0) >> 4;
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    if(VREG[indexX] != VREG[indexY]){
        // move by 2 bytes
        PC += 0x02;
    }
}

void chip8::I_ANNN(){
    //STORE:
    //Set Index
    I = opcode & 0x0FFF;
};

void chip8::I_BNNN(){
    //JUMP W OFFSET:
    PC = (opcode & 0x0FFF) + VREG[0];
};

void chip8::I_CXNN(){
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    uint8_t randNum = static_cast<uint8_t>( rand() );
    VREG[indexX] = randNum && (opcode & 0xFF);
};

void chip8::I_DXYN(){
uint8_t indexX = (opcode & 0x0F00) >> 8;
uint8_t indexY = (opcode & 0x00F0) >> 4;
uint8_t N = (opcode & 0x00F);
// display is 64x32; 
uint8_t coorX = VREG[indexX] & 0x3f;
uint8_t coorY = VREG[indexY] & 0x1f;

//uint16_t address = I;

VREG[0x0F]=0;

for(auto i{0uz}; i<N ;i++){
    uint8_t charByte = memory[I+i];
    uint16_t flatten2D = ( (coorX + 1) + ((0x40) * (coorY + i)) ) - 1;
    if(flatten2D >= resolution){
        break;
    }
    //I dont really want to do two for loop
    // but it  is only 8 bits so not so bad
    // could also unroll the loop manually 
    for(auto j{0uz}; j < 8 ; j++){
        uint16_t increment_flatten = flatten2D+j;

        if( (coorX + j) >= 0x40 ){  
            break;
        }
        
        if(screen[increment_flatten] == 1 && ( (charByte >> (7-j)) & 0x01) == 1){
            VREG[0x0F] = 1;
        }
        screen[increment_flatten] ^= ( (charByte >> (7-j)) & 0x01); 
    }
}

};

void chip8::I_EX9E(){
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    if( keypad[ VREG[indexX] & 0x0F ] >= 1){
        PC += 0x02; 
    }
};

void chip8::I_EXA1(){
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    if( keypad[ VREG[indexX] & 0x0F ] == 0){
        PC += 0x02; 
    }
};

void chip8::I_FX07(){
    //Set VX to Delay TImer
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    VREG[indexX] = dt;
};


void chip8::I_FX0A(){
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    for(auto i{0uz};i<=0xF;i++){
        if(keypad[i]>0){
            VREG[indexX]=i;
            break;
        }
        if(i == 0xF && keypad[i] == 0){
            PC -= 0x2;
        }
    }

};

void chip8::I_FX15(){
    //Set Delay Timer to Vx
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    dt = VREG[indexX];
};


void chip8::I_FX18(){
    //Set Sound Timer to Vx
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    st = VREG[indexX];
};


void chip8::I_FX1E(){
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    I += VREG[indexX];

    if(I > 0x0FFF){
        VREG[0x0F] = 1;
    }
};

void chip8::I_FX29(){
    //Set I to address of sprite data of value in VX
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    // starting address of char fonts in memory
    uint8_t fonts_address= 0x50;

    uint8_t grabCharX = VREG[indexX] & 0x0F; // using toviasvl.github blog the original COSMAC VIP took only 
    // the last 4 bits of VX so it will be in range 0-F I will be following this   


    I = (fonts_address + (grabCharX*5)) & 0x0FFF; //realisticly no need for 0x0FFF but want to add safeguard 
};

void chip8::I_FX33(){
    //Binary-coded decimal conversion   
    uint8_t indexX = (opcode & 0x0F00) >> 8;
    uint8_t val = VREG[indexX];

        memory[ (I+2) % 0xFFF] = val % 10;
        val /=10;
        memory[ (I+1) % 0xFFF] = val % 10;
        val /=10;
        memory[I % 0xFFF] = val % 10;
        val /=10;
};

void chip8::I_FX55(){  
    //Due to this function being different on orignal COSMAC VIP and modern games
    //can use preprocessor directive for now? could have a potential code to change the what type of chip8 are we using. 
    // but not in the goal for this project 
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    #ifdef MODERN
        // We will use a temp value and keep I the same 
        uint16_t temp = I;
        for(auto i{0uz}; i<=indexX;++i){
            memory[ temp % 0xFFF ] = VREG[i];
            temp++;
        }
    #else
        // This will be OG COSMAC VIP incremented the I register so that I= I + X + 1 at the end
        for(auto i{0uz}; i<=indexX;++i){
            memory[ I % 0xFFF ] = VREG[i];
            I++;
        }
    #endif

};

void chip8::I_FX65(){
    uint8_t indexX = (opcode & 0x0F00) >> 8;

    #ifdef MODERN
        // We will use a temp value and keep I the same 
        uint16_t temp = I;
        for(auto i{0uz}; i<=indexX;++i){
            VREG[i] = memory[ temp % 0xFFF ] ;
            temp++;
        }
    #else
        // This will be OG COSMAC VIP incremented the I register so that I= I + X + 1 at the end
        for(auto i{0uz}; i<=indexX;++i){
            VREG[i] = memory[ I % 0xFFF ] ;
            I++;
        }
    #endif    
};