#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    // Read 1 Byte //
    Byte operator[](u32 Address) const
    {
        return Data[Address];
    }
};

struct CPU
{
    Word PC;                   // Program counter
    Word SP;                   // Stack pointer

    Byte A, X, Y;              // Accumulator (A), X-Register (X), Y-Register (Y)

    // Processor status flags ===========================================================================
    Byte C : 1;                // Carry flag
    Byte Z : 1;                // Zero flag
    Byte I : 1;                // Interrupt disable flag
    Byte D : 1;                // Decimal flag
    Byte B : 1;                // The B flag
    Byte V : 1;                // Overflow flag
    Byte N : 1;                // Negative flag

    //      7  bit  0
    //      ---- ----
    //      NV1B DIZC
    //      |||| ||||
    //      |||| |||+- Carry
    //      |||| ||+-- Zero
    //      |||| |+--- Interrupt Disable
    //      |||| +---- Decimal
    //      |||+------ (No CPU effect; see: the B flag)
    //      ||+------- (No CPU effect; always pushed as 1)
    //      |+-------- Overflow
    //      +--------- Negative
    // ==================================================================================================
    void Reset(Mem& memory)
    {
        PC = 0xFFFC;                   // Initialize PC to Reset vector address
        SP = 0x0100;                   // Initialize SP
        C = Z = I = D = V = N = 0;     // Clear status flags
        A = X = Y = 0;

        memory.Initialize();

    }

    Byte FetchByte(u32& Cycles, Mem& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }
    
    void Execute(u32 Cycles, Mem& memory)
    {
        while(Cycles > 0)
        {
            Byte Ins = FetchByte(Cycles, memory);
        }
    }
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    cpu.Execute(2, mem);
    return 0;
}