#include <stdio.h>
#include <stdlib.h>

namespace m6502
{
	using Byte = unsigned char;
	using Word = unsigned short;

	using u32 = unsigned int;

    struct Mem;
    struct StatusFlags;
    struct CPU;
}

struct m6502::Mem
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

    /* Read 1 Byte */
    Byte operator[](u32 Address) const
    {
        return Data[Address];
    }

    /* Write 1 Byte */
    Byte& operator[](u32 Address)
    {
        return Data[Address];
    }
};

struct m6502::StatusFlags
{
    // Processor status flags ===========================================================================
    Byte C : 1;                // Carry flag
    Byte Z : 1;                // Zero flag
    Byte I : 1;                // Interrupt disable flag
    Byte D : 1;                // Decimal flag
    Byte B : 1;                // The B flag
    Byte Unused = 1;
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
};

struct m6502::CPU
{
    Word PC;                   // Program counter
    Word SP;                   // Stack pointer

    Byte A, X, Y;              // Accumulator (A), X-Register (X), Y-Register (Y)

    StatusFlags Flags;

    void Reset(Mem& memory)
    {
        PC = 0xFFFC;                   // Initialize PC to Reset vector address
        SP = 0x0100;                   // Initialize SP
        Flags.C = Flags.Z = Flags.I = Flags.D = Flags.V = Flags.N = 0;     // Clear status flags
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

    Word FetchWord(u32& Cycles, Mem& memory)
    {
        // 6502 is little endian i.e first byte is LSB.
        Word Data = memory[PC];
        PC++;
        Cycles--;

        Data |= (memory[PC] << 8);
        PC++;
        Cycles--;

        return Data;
    }

    Byte ReadByte(u32& Cycles, Byte Address, Mem& memory)
    {
        /* Program Counter in not incremented because this function
        is not executing any code but only reading memory*/
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    /* Write 1 Word */
    void WriteWord(Word Value, u32 Address, u32& Cycles, Mem& memory)
    {
        // LSB goes first and then MSB
        memory[Address] = Value & 0xFF;
        Cycles--;

        memory[Address + 1] = Value >> 8;
        Cycles--;
    }

    void LDASetStatus()
    {
        Flags.Z = (A == 0);
        Flags.N = (A & 0b10000000) > 0;
    }

    void Execute(u32 Cycles, Mem& memory);

    // opcodes
    static constexpr Byte
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_JSR = 0x20;
};