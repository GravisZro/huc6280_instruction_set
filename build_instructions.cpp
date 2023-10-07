#include "build_instructions.h"

void build_insn_blocks (std::list<instructions>& insn_blocks)
{
  insn_blocks.assign(
        {
          instructions
          {
            "Branching Operations",
            instruction
            {
              mnemonic { "BRA" },
              mnemonic_origin { "_B_ranch _Always" },
              llvm_syntax { "" },
              abstract { "PC = PC + 2 + REL" },
              description { "A branch is always taken; no testing is done. A one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch." },
              summary { "Unconditionally branch to the address calculated from the operand.  The operand is treated as an 8-bit signed number, -128 to 127.  When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction.  For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { WDC65C02 | HuC6280, 0x80, 2, 4, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "JMP" },
              mnemonic_origin { "_Ju_m_p" },
              llvm_syntax { "" },
              description { "Transfer control to the address specified by the operand field. The program counter is loaded with the target address." },
              summary { "Transfers control(sets the program counter) to the effective address calculated from the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x4C, 3, 4, Absolute, std::nullopt, abstract { "PCL = $ll\nPCH = $hh" } },
                { NMOS6502 | WDC65C02 | HuC6280, 0x6C, 3, 7, Absolute | Indirect, std::nullopt, abstract { "PCL = [$hhll]\nPCH = [$hhll + 1]" } },
                { WDC65C02 | HuC6280, 0x7C, 3, 7, Absolute | X_Indexed | Indirect, std::nullopt, abstract { "PCL = [$hhll + X]\nPCH = [$hhll + X + 1]" } },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BVS" },
              mnemonic_origin { "_Branch on O_verflow _Set" },
              llvm_syntax { "" },
              abstract { "If V == 1: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The overflow flag V in the status register is tested. If it is set, a branch is taken; if it is clear, the instruction immediately following the two-byte BVS instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch . BVS is almost exclusively used to check that a two's complement arithmetic calculation has overflowed. Add +2 cycles if branch is taken." },
              summary { "If the Overflow Flag is 1, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x70, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BVC" },
              mnemonic_origin { "_Branch on O_verflow _Clear" },
              llvm_syntax { "" },
              abstract { "If V == 0: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The overflow flag V in the status register is tested. If it is clear, a branch is taken; if it is set, the instruction immediately following the two-byte BVC instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch . BVC is almost exclusively used to check that a two's complement arithmetic calculation has not overflowed. Add +2 cycles if branch is taken." },
              summary { "If the Overflow Flag is 0, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x50, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BCS" },
              mnemonic_origin { "_Branch on _Carry _Set" },
              llvm_syntax { "" },
              abstract { "If C == 1: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The carry flag in the status register is tested. If it is set, a branch is taken; if it is clear, the instruction immediately following the two-byte BCS instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch . Note that BCC determines if the result of a comparison is greater than or equal to; therefore, BCC is sometimes written as BGE (Branch if Greater than or Equal). Add +2 cycles if branch is taken." },
              summary { "If the Carry Flag is 1, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xB0, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BCC" },
              mnemonic_origin { "_Branch on _Carry _Clear" },
              llvm_syntax { "" },
              abstract { "If C == 0: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The carry flag in the status register is tested. If it is clear, a branch is taken; if it is set, the instruction immediately following the two-byte BCC instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch . Note that BCC determines if the result of a comparison is less than; therefore, BCC is sometimes written as BLT (Branch if Less Than). Add +2 cycles if branch is taken." },
              summary { "If the Carry Flag is 0, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch."},
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x90, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BEQ" },
              mnemonic_origin { "_Branch on _E_qual(/Zero Set)" },
              llvm_syntax { "" },
              abstract { "If Z == 1: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The zero flag in the status register is tested. If it is set, meaning that the last value tested (which affected the zero flag) was zero, a branch is taken; if it is clear, meaning the value tested was non-zero, the instruction immediately following the two-byte BEQ instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch. Add +2 cycles if branch is taken." },
              summary { "If the Zero Flag is 1, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xF0, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BNE" },
              mnemonic_origin { "_Branch on _Not _Equal(/Zero Clear)" },
              llvm_syntax { "" },
              abstract { "If Z == 0: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The zero flag in the status register is tested. If it is clear, meaning that the last value tested (which affected the zero flag) was zero, a branch is taken; if it is set, meaning the value tested was non-zero, the instruction immediately following the two-byte BNE instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch. Add +2 cycles if branch is taken." },
              summary { "If the Zero Flag is 0, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xD0, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BMI" },
              mnemonic_origin { "_Branch on _M_inus(/Negative Set)" },
              llvm_syntax { "" },
              abstract { "If N == 1: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The negative flag in the status register is tested. If it is set, meaning the high bit of the value which most recently affected the N flag was set, a branch is taken. Since numbers are often stored in two's complement, this instruction can be used to detect negative numbers. If it is clear, the instruction immediately following the two-byte BMI instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch. Add +2 cycles if branch is taken." },
              summary { "If the Negative Flag is 1, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x30, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BPL" },
              mnemonic_origin { "_Branch on _P_lus(/Negative Clear)" },
              llvm_syntax { "" },
              abstract { "If N == 0: PC = PC + 2 + REL\nElse: PC = PC + 2" },
              description { "The negative flag in the status register is tested. If it is clear, meaning the high bit of the value which most recently affected the N flag was cleared, a branch is taken. Since numbers are often stored in two's complement, this instruction can be used to detect positive numbers. If it is set, the instruction immediately following the two-byte BPL instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch . This opcode takes one extra cycle if the branch is taken, and another extra cycle if a page boundary is crossed in taking the branch." },
              summary { "If the Negative Flag is 0, branch to the address calculated from the operand. The operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x10, 2, { "2 (4 if branch taken)" }, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BBR#" },
              mnemonic_origin { "_Branch on _Bit _Reset _#n" },
              llvm_syntax { "" },
              abstract { "If MEM#n == 0: PC = PC + 3\nElse: PC = PC + 3 + REL" },
              description { "The #th bit value in zero page memory location ZZ is tested. If it is clear, a branch is taken; if it is set, the instruction immediately following the three-byte BBRi instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch. Add +2 cycles if branch is taken." },
              summary { "If Bit #n of the value at the effective address specified by the second operand is clear, branch to the address calculated from the second operand. The second operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed second operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x0F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 0 },
                { WDC65C02 | HuC6280, 0x1F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 1 },
                { WDC65C02 | HuC6280, 0x2F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 2 },
                { WDC65C02 | HuC6280, 0x3F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 3 },
                { WDC65C02 | HuC6280, 0x4F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 4 },
                { WDC65C02 | HuC6280, 0x5F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 5 },
                { WDC65C02 | HuC6280, 0x6F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 6 },
                { WDC65C02 | HuC6280, 0x7F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 7 },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "BBS#" },
              mnemonic_origin { "_Branch on _Bit _Set _#n" },
              llvm_syntax { "" },
              abstract { "If MEM#n == 1: PC = PC + 3 + REL\nElse: PC = PC + 3" },
              description { "The #th bit value in zero page memory location ZZ is tested. If it is set, a branch is taken; if it is clear, the instruction immediately following the three-byte BBSi instruction is executed. If the branch is taken, a one-byte signed displacement, fetched from the third byte of the instruction, is added to the program counter. Once the branch address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the branch. Add +2 cycles if branch is taken." },
              summary { "If Bit #n of the value at the effective address specified by the operand is set, branch to the address calculated from the second operand. The second operand is treated as an 8-bit signed number, -128 to 127. When calculating the branch address, the 8-bit signed second operand is added to the address of the byte immediately following the branch instruction. For example, a branch instruction with an operand of $00 will never branch." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x8F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 0 },
                { WDC65C02 | HuC6280, 0x9F, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 1 },
                { WDC65C02 | HuC6280, 0xAF, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 2 },
                { WDC65C02 | HuC6280, 0xBF, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 3 },
                { WDC65C02 | HuC6280, 0xCF, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 4 },
                { WDC65C02 | HuC6280, 0xDF, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 5 },
                { WDC65C02 | HuC6280, 0xEF, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 6 },
                { WDC65C02 | HuC6280, 0xFF, 3, { "6 (8 if branch taken)" }, ZeroPage | Secondary | Relative, 7 },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          },

          instructions
          {
            "Subroutine Operations",
            instruction
            {
              mnemonic { "BSR" },
              mnemonic_origin { "_Branch to _Sub_routine" },
              llvm_syntax { "" },
              abstract { "PC = PC + 1\n[SP] = PCH ;\tSP = SP - 1\n[SP] = PCL ;\tSP = SP - 1\nPC = PC + 2 + REL" },
              description { "Similar to the Jump to Subroutine (JSR) instruction, Branch to Subroutine allows execution of a subroutine. However, the offset is specified in relative mode instead of as an absolute address. This saves a byte, but takes one more clock cycle than JSR, so its use is discouraged. The current program counter is pushed onto the stack. A one-byte signed displacement, fetched from the second byte of the instruction, is added to the program counter. Once the subroutine address has been calculated, the result is loaded into the program counter, transferring control to that location. The allowable range of the displacement is -128 to +127 from the instruction immediately following the BSR." },
              summary { "The program counter (last byte of the BSR instruction) is pushed to stack and the CPU branches to the specified relative address." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { HuC6280, 0x44, 2, 8, Relative },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "JSR" },
              mnemonic_origin { "_Jump to _Sub_routine" },
              llvm_syntax { "" },
              abstract { "PC = PC + 2\n[SP] = PCH ;\tSP = SP - 1\n[SP] = PCL ;\tSP = SP - 1\nPC = $hhll" },
              description { "Transfer control to the subroutine at the location specified by the operand, after first pushing the current program counter value onto the stack as a return address. The value of the PC which is pushed onto the stack is the location of the last (third) byte of the JSR instruction, not the address of the next opcode." },
              summary { "Push the Program Counter onto the stack and set it to the address specified in the second operand." },
              note { "HuC6280: 256-byte and 8192-byte page-boundary crossing do not incur cycle penalties" },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x20, 3, 7, Absolute },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "RTS" },
              mnemonic_origin { "_Re_turn from _Subroutine" },
              llvm_syntax { "" },
              abstract { "SP = SP + 1 ;\tPCL = [SP]\nSP = SP + 1 ;\tPCH = [SP]\nPC = PC + 1" },
              description { "Pull the program counter from the stack, incrementing the 16-bit value by one before loading the program counter with it. The low byte of the program counter is pulled from the stack first, followed by the high byte." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x60, 1, 7, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          },
          instructions
          {
            "Interrupt Operations",
            instruction
            {
              mnemonic { "BRK" },
              mnemonic_origin { "_B_rea_k" },
              llvm_syntax { "INT 2" },
              abstract { "PC = PC + 2\n[SP] = PCH ;\tSP = SP - 1\n[SP] = PCL ;\tSP = SP - 1\n[SP] = P ;\tSP = SP - 1\nPCL = [$FFF6]\nPCH = [$FFF7]" },
              description { "Forces a software interrupt. BRK is unaffected by the I interrupt disable flag. Although BRK is a one-byte instruction, the program counter (which is pushed onto the stack by the instruction) is incremented by two; this lets you follow the break instruction with a one-byte signature byte indicating which break caused the interrupt. Be sure to pad BRK with a single byte to allow an RTI (return from interrupt) instruction to execute correctly. Multiple actions are invoked on a BRK. The program counter is incremented by 2. The high and low bytes of the program counter are pushed onto the stack in order, followed by the status register (P). The program counter is then loaded with the break vector stored at absolute address $00FFF6-$00FFF7. (Remember, the high byte is stored in $00FFF7 and the low byte is stored in $00FFF6.) The decimal flag D is cleared, and the I flag is set (to disable hardware IRQ interrupts) after a break is executed. Additionally, the Break Flag in the status register value pushed onto the stack is set." },
              summary { "Forces a software interrupt using IRQ2's vector. Contrary to IRQs, BRK will push the status flags register with bit 4('B' flag) set."},
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x00, 1, 8, Implied },
              flags { nullptr, nullptr, 0, 1, 0, 1, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "RTI" },
              mnemonic_origin { "_Re_turn from _Interrupt" },
              llvm_syntax { "" },
              abstract { "SP = SP + 1 ;\tP = [SP]\nSP = SP + 1 ;\tPCL = [SP]\nSP = SP + 1 ;\tPCH = [SP]" },
              description { "Pull the status register and the program counter from the stack in order. Normally used to return from an interrupt call (such as BRK), this instruction can also be used to pull the status register P, and the program counter low and high bytes from the stack into the P and program counter registers." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x40, 1, 7, Implied },
              flags { "[SP]:7", "[SP]:6", "[SP]:5", "[SP]:4", "[SP]:3", "[SP]:2", "[SP]:1", "[SP]:0" },
            },
          },

          instructions
          {
            "Data Transfer Operations",
            instruction
            {
              mnemonic { "LDA" },
              mnemonic_origin { "_Loa_d _Accumulator" },
              llvm_syntax { "MOV A, MEM" },
              abstract { "A = MEM" },
              description { "Load the accumulator with the data located at the effective address specified by the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xA9, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0xA5, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xB5, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0xB2, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0xA1, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0xB1, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xAD, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0xBD, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xB9, 3, 5, Absolute | Y_Indexed },
              },
              flags { "MEM:7", nullptr, 0, nullptr, nullptr, nullptr, "MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "LDX" },
              mnemonic_origin { "_Loa_d _X Register" },
              llvm_syntax { "MOV X, MEM" },
              abstract { "X = MEM" },
              description { "Load the X register with the data located at the effective address specified by the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xA2, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0xA6, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xB6, 2, 4, ZeroPage | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xAE, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0xBE, 3, 5, Absolute | Y_Indexed },
              },
              flags { "MEM:7", nullptr, 0, nullptr, nullptr, nullptr, "MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "LDY" },
              mnemonic_origin { "_Loa_d _Y Register" },
              llvm_syntax { "MOV Y, MEM" },
              abstract { "Y = MEM" },
              description { "Load the Y register with the data located at the effective address specified by the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xA0, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0xA4, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xB4, 2, 4, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xAC, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0xBC, 3, 5, Absolute | X_Indexed },
              },
              flags { "MEM:7", nullptr, 0, nullptr, nullptr, nullptr, "MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "ST0" },
              mnemonic_origin { "_S_tore HuC6270 _0" },
              llvm_syntax { "MOV [$001FE000], MEM" },
              abstract { "[$001FE000] = IMM" },
              description { "The immediate argument is stored in the HuC6270's address register. This command is equivalent to storing the immediate argument in $1FE000. The HuC6270 No. 0 register is also known as the HuC6270 Address/Status Register; more information is available in the HuC6270 summary. According to the Develo Book , this operation sets /CE7, A1, and A0 to logical LOW." },
              mode_details { HuC6280, 0x03, 2, 5, Immediate | Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "ST1" },
              mnemonic_origin { "_S_tore HuC6270 _1" },
              llvm_syntax { "MOV [$001FE002], MEM" },
              abstract { "[$001FE002] = IMM" },
              description { "The immediate argument is stored in the HuC6270's low data register. This command is equivalent to storing the immediate argument in $1FE002. The HuC6270 No. 1 register is also known as the HuC6270 Low Data Register; more information is available in the HuC6270 summary. According to the Develo Book , this operation sets /CE7 and A0 to logical LOW, while setting A1 to logical HIGH." },
              mode_details { HuC6280, 0x13, 2, 5, Immediate | Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "ST2" },
              mnemonic_origin { "_S_tore HuC6270 _2" },
              llvm_syntax { "MOV ST2, MEM" },
              abstract { "[$001FE003] = IMM" },
              description { "The immediate argument is stored in the HuC6270's high data register. This command is equivalent to storing the immediate argument in $1FE003. The HuC6270 No. 2 register is also known as the HuC6270 High Data Register; more information is available in the HuC6270 summary. According to the Develo Book , this operation sets /CE7 to logical LOW, while setting A0 and A1 to logical HIGH." },
              mode_details { HuC6280, 0x23, 2, 5, Immediate | Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "STA" },
              mnemonic_origin { "_S_tore _Accumulator" },
              llvm_syntax { "MOV MEM, A" },
              abstract { "MEM = A" },
              description { "Stores the value in the accumulator to the effective address specified by the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x85, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x95, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0x92, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x81, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x91, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x8D, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x9D, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x99, 3, 5, Absolute | Y_Indexed },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "STX" },
              mnemonic_origin { "_S_tore _X Register" },
              llvm_syntax { "MOV MEM, X" },
              abstract { "MEM = X" },
              description { "Store the value in the X register to the effective address specified by the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x86, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x96, 2, 4, ZeroPage | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x8E, 3, 5, Absolute },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "STY" },
              mnemonic_origin { "_S_tore _Y Register" },
              llvm_syntax { "MOV MEM, Y" },
              abstract { "MEM = Y" },
              description { "Store the value in the Y register to the effective address specified by the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x84, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x94, 2, 4, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x8C, 3, 5, Absolute },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "TSX" },
              mnemonic_origin { "_Transfer _Stack Pointer to _X Register (TSX)" },
              llvm_syntax { "MOV X, SP" },
              abstract { "X = SP" },
              description { "Transfer the value in the stack pointer SP to the X register. The value of the stack pointer is not changed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xBA, 1, 2, Implied },
              flags { "SP7", nullptr, 0, nullptr, nullptr, nullptr, "SP == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TXS" },
              mnemonic_origin { "_Transfer _X Register to _Stack Pointer" },
              llvm_syntax { "MOV SP, X" },
              abstract { "SP = X" },
              description { "Transfer the value in the X register to the stack pointer. The value of the X register is not changed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x9A, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "TXA" },
              mnemonic_origin { "_Transfer _X Register to _Accumulator" },
              llvm_syntax { "MOV A, X" },
              abstract { "A = X" },
              description { "Transfer the value in the X register to the accumulator. The value of the X register is not changed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x8A, 1, 2, Implied },
              flags { "X:7", nullptr, 0, nullptr, nullptr, nullptr, "X == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TAX" },
              mnemonic_origin { "_Transfer _Accumulator to _X Register" },
              llvm_syntax { "MOV X, A" },
              abstract { "X = A" },
              description { "Transfer the value in the accumulator to register X." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xAA, 1, 2, Implied },
              flags { "A:7", nullptr, 0, nullptr, nullptr, nullptr, "A == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TYA" },
              mnemonic_origin { "_Transfer _Y Register to _Accumulator" },
              llvm_syntax { "MOV A, Y" },
              abstract { "A = Y" },
              description { "Transfer the value in the Y register to the accumulator. The value of the Y register is not changed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x98, 1, 2, Implied },
              flags { "Y:7", nullptr, 0, nullptr, nullptr, nullptr, "Y == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TAY" },
              mnemonic_origin { "_Transfer _Accumulator to _Y Register" },
              llvm_syntax { "MOV Y, A" },
              abstract { "Y = A" },
              description { "Transfer the value in the accumulator to register Y." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xA8, 1, 2, Implied },
              flags { "A:7", nullptr, 0, nullptr, nullptr, nullptr, "A == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TAM" },
              mnemonic_origin { "_Transfer _Accumulator to _Mapping Register" },
              llvm_syntax { "MOV MPR(IMM), A" },
              abstract { "MPR(IMM) = A" },
              description { "Loads Memory Mapping Register i with the value in the accumulator. More about the MPR registers can be found in the Memory Mapping summary. It is possible to load more than one MPR at a time by setting more than one bit in the immediate argument to TAM." },
              note { "" },
              mode_details { HuC6280, 0x53, 2, 5, Immediate },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "TMA" },
              mnemonic_origin { "_Transfer _Mapping Register to _Accumulator" },
              llvm_syntax { "MOV A, MPR(IMM)" },
              abstract { "A = MPR(IMM)" },
              description { "Transfers the value in Memory Mapping Register #$nn to the accumulator. More information about the MPRs can be found on the Memory Mapping summary. Only one bit in the immediate argument can be set to 1." },
              note { "" },
              mode_details { HuC6280, 0x43, 2, 4, Immediate },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CLA" },
              mnemonic_origin { "_C_lear _Accumulator" },
              llvm_syntax { "MOV A, 0" },
              abstract { "A = 0" },
              description { "The accumulator is set to #$00." },
              mode_details { HuC6280, 0x62, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CLX" },
              mnemonic_origin { "_C_lear _X Register" },
              llvm_syntax { "MOV X, 0" },
              abstract { "X = 0" },
              description { "The X register is set to #$00." },
              mode_details { HuC6280, 0x82, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CLY" },
              mnemonic_origin { "_C_lear _Y Register" },
              llvm_syntax { "MOV Y, 0" },
              abstract { "Y = 0" },
              description { "The Y register is set to #$00." },
              mode_details { HuC6280, 0xC2, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "STZ" },
              mnemonic_origin { "_S_tore _Zero" },
              llvm_syntax { "MOV MEM, #$00" },
              abstract { "MEM = $00" },
              description { "Store the value 0x00 to the effective address specified by the operand. Very useful for initialising memory." },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x64, 2, 4, ZeroPage },
                { WDC65C02 | HuC6280, 0x74, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0x9C, 3, 5, Absolute },
                { WDC65C02 | HuC6280, 0x9E, 3, 5, Absolute | X_Indexed },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          },
          instructions
          {
            "Register Exchange Operations",
            instruction
            {
              mnemonic { "SXY" },
              mnemonic_origin { "_Swap _X and _Y Registers" },
              llvm_syntax { "XCHG X, Y" },
              abstract { "TEMP = X\nX = Y\nY = TEMP" },
              description { "Swaps the values stored in the X and Y registers." },
              mode_details { HuC6280, 0x02, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "SAX" },
              mnemonic_origin { "_Swap _Accumulator and _X Register" },
              llvm_syntax { "XCHG A, X" },
              abstract { "TEMP = A\nA = X\nX = TEMP" },
              description { "The values of the accumulator and the X Register are swapped." },
              mode_details { HuC6280, 0x22, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "SAY" },
              mnemonic_origin { "_Swap _Accumulator and _Y Register" },
              llvm_syntax { "XCHG A, Y" },
              abstract { "TEMP = A\nA = Y\nY = TEMP" },
              description { "The values of the accumulator and the Y Register are swapped." },
              mode_details { HuC6280, 0x42, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          },
          instructions
          {
            "Mass Data Transfer Operations",
            instruction
            {
              mnemonic { "TAI" },
              mnemonic_origin { "_Transfer _Alternate _Increment" },
              llvm_syntax { "" },
              abstract { "" },
              description { "Execute a memory move where the source address alternates between two addresses, and the destination address increments with each loop cycle. This is an extremely powerful instruction, mainly used for transferring data from the special video memory (e.g., backgrounds, etc.) to the main memory. Blocks interrupts from happening until finished. A/X/Y are pushed onto the stack during the transfer." },
              mode_details { HuC6280, 0xE3, 7, nullptr, Block },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "TIA" },
              mnemonic_origin { "_Transfer _Increment _Alternate" },
              llvm_syntax { "" },
              abstract { "" },
              description { "Execute a memory move where the source address increments, and the destination address alternates between two addresses with each loop cycle. This is an extremely powerful instruction, mainly used for transferring data to the special video memory (e.g., backgrounds, etc.) from the main memory. Blocks interrupts from happening until finished. A/X/Y are pushed onto the stack during the transfer." },
              mode_details { HuC6280, 0xE3, 7, nullptr, Block },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "TDD" },
              mnemonic_origin { "_Transfer _Decrement _Decrement" },
              llvm_syntax { "" },
              abstract { "" },
              description { "Execute a memory move where the source and destination addresses decrement with each loop cycle. This is an extremely powerful instruction, mainly used for copying and moving data around in main memory. Blocks interrupts from happening until finished. A/X/Y are pushed onto the stack during the transfer." },
              mode_details { HuC6280, 0xC3, 7, nullptr, Block },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "TIN" },
              mnemonic_origin { "_Transfer _Increment _None" },
              llvm_syntax { "" },
              abstract { "" },
              description { "Execute a memory move where the source address increments with each loop cycle. This is an extremely powerful instruction, mainly used for transferring data to byte wide ports (e.g., PSG, etc.) to the main memory. Blocks interrupts from happening until finished. A/X/Y are pushed onto the stack during the transfer." },
              mode_details { HuC6280, 0xD3, 7, nullptr, Block },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "TII" },
              mnemonic_origin { "_Transfer _Increment _Increment" },
              llvm_syntax { "" },
              abstract { "" },
              description { "Execute a memory move where the source and destination addresses increment with each loop cycle. This is an extremely powerful instruction, mainly used for copying and moving blocks of data around in main memory. Blocks interrupts from happening until finished. A/X/Y are pushed onto the stack during the transfer." },
              mode_details { HuC6280, 0xD3, 7, nullptr, Block },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          },
          instructions
          {
            "Status Flag Operations",
            instruction
            {
              mnemonic { "SED" },
              mnemonic_origin { "_S_et _Decimal Mode Flag" },
              llvm_syntax { "" },
              abstract { "D = 1" },
              description { "The decimal mode flag D in the status register is set to 1. This enables BCD arithmetic. Interrupt calls will save this setting, but clear it for inside the ISR. V flag is not affected for operations that work under Decimal mode." },
              summary { "Sets the Decimal Mode Flag to 1."},
              note {
R"(The Decimal Mode Flag is also known as D and P3.
When set, the ADC and SBC instructions will utilized Packed BCD arithmetic.)"
              },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xF8, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, 1, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CLD" },
              mnemonic_origin { "_C_lear _Decimal Flag" },
              llvm_syntax { "" },
              abstract { "D = 0" },
              description { "The decimal flag D in the status register is set to 0, returning the processor to binary arithmetic mode." },
              summary { "Resets the Decimal Mode Flag to 0."},
              note {
R"(The Decimal Mode Flag is also known as D and P3.
When reset, the ADC and SBC instructions will return to using binary arithmetic.)"
              },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xD8, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, 0, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "SEC" },
              mnemonic_origin { "_S_et _Carry Flag" },
              llvm_syntax { "" },
              abstract { "C = 1" },
              description { "The carry flag C in the status register is set to 1." },
              summary { "Sets the Carry Flag to 1." },
              note { "The Carry Flag is also known as C and P0." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x38, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, 1 },
            },
            instruction
            {
              mnemonic { "CLC" },
              mnemonic_origin { "_C_lear _Carry Flag" },
              llvm_syntax { "" },
              abstract { "C = 0" },
              description { "The carry flag C in the status register is set to 0." },
              summary { "Resets the Carry Flag, known also as C and P0, to 0." },
              note { "The Carry Flag is also known as C and P0." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x18, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, 0 },
            },
            instruction
            {
              mnemonic { "SEI" },
              mnemonic_origin { "_S_et _Interrupt Disable Flag" },
              llvm_syntax { "" },
              abstract { "I = 1" },
              description { "The interrupt disable flag I in the status register is set to 1. This disables hardware interrupt processing." },
              summary { "Sets the Interrupt Disable Flag to 1. This prevents hardware IRQs from being processed." },
              note { "The Interrupt Disable Flag is also known as I and P2.\nThis will only prevent IRQs after the next instruction is executed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x78, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, 1, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CLI" },
              mnemonic_origin { "_C_lear _Interrupt Disable Flag" },
              llvm_syntax { "" },
              abstract { "I = 0" },
              description { "The interrupt disable flag I in the status register is set to 0. This re-enables hardware interrupt (IRQ) processing." },
              summary { "Resets the Interrupt Disable Flag to 0. This enables hardware IRQ processing."},
              note { "The Interrupt Disable Flag is also known as I and P2.\nThis will only allow IRQs after the next instruction is executed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x58, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, 0, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "SET" },
              mnemonic_origin { "_S_et Memory _Transfer Flag" },
              llvm_syntax { "" },
              abstract { "T = 1" },
              description { "The T flag in the status register is set to 1. The T flag is called the \"Memory Operation Flag;\", when this flag is set all the instructions that normally use the A register act differently, I don't know exactly if all the instructions are affected but I'm sure for AND, EOR, OR & ADC. In place of using the A register the instruction use the memory location in ZP pointed by the X register, so for example if you use SET followed by ADC #10, the CPU will do ZP[X] = ZP[X] + 10. When the T flag is set, operations that work under the T flag will have a Read-Modify-Write +1 penalty." },
              summary { "Sets the Memory Transfer Flag to 1." },
              note { "The Memory Transfer Flag is also known as T and P5.\nThis instruction should only be used immediately before ADC, SBC, AND, ORA, or EOR." },
              mode_details { HuC6280, 0xF4, 1, 2, Implied },
              flags { nullptr, nullptr, 1, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CLV" },
              mnemonic_origin { "_C_lear O_verflow Flag" },
              llvm_syntax { "" },
              abstract { "V = 0" },
              description { "The overflow flag V in the status register is set to 0." },
              summary { "Sets the Overflow Flag to 0." },
              note {
R"("The Overflow Flag is also known as V and P6.

The Overflow Flag cannot be set directly.
* ADC and SBC may set the Overflow Flag.
* BIT, TRB, TSB, and TST will load M6 into the Overflow Flag.
* PLP and RTI will restore the Overflow Flag from stack)"
              },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xB8, 1, 2, Implied },
              flags { nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          },
          instructions
          {
            "Bit Operations",
            instruction
            {
              mnemonic { "SMB#" },
              mnemonic_origin { "_Set _Memory _Bit _#n" },
              llvm_syntax { "" },
              abstract { "MEM#n = 1" },
              description { "Set the specified bit in the zero page memory location specified in the operand. The bit to clear is specified by a number concatenated to the end of the mnemonic, resulting in 8 distinct Opcodes." },
              summary { "Reads the zero-page address specified by the operand, sets the bit #n, and then writes it back to the aforementioned address." },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x87, 2, 7, ZeroPage, 0 },
                { WDC65C02 | HuC6280, 0x97, 2, 7, ZeroPage, 1 },
                { WDC65C02 | HuC6280, 0xA7, 2, 7, ZeroPage, 2 },
                { WDC65C02 | HuC6280, 0xB7, 2, 7, ZeroPage, 3 },
                { WDC65C02 | HuC6280, 0xC7, 2, 7, ZeroPage, 4 },
                { WDC65C02 | HuC6280, 0xD7, 2, 7, ZeroPage, 5 },
                { WDC65C02 | HuC6280, 0xE7, 2, 7, ZeroPage, 6 },
                { WDC65C02 | HuC6280, 0xF7, 2, 7, ZeroPage, 7 },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "RMB#" },
              mnemonic_origin { "_Reset _Memory _Bit _#n" },
              llvm_syntax { "" },
              abstract { "MEM#n = 0" },
              description { "Clear the specified bit in the zero page memory location specified in the operand. The bit to clear is specified by a number concatenated to the end of the mnemonic, resulting in 8 distinct Opcodes." },
              summary { "Reads the zero-page address specified by the operand, resets(clears) the bit #n, and then writes it back to the aforementioned address." },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x07, 2, 7, ZeroPage, 0 },
                { WDC65C02 | HuC6280, 0x17, 2, 7, ZeroPage, 1 },
                { WDC65C02 | HuC6280, 0x27, 2, 7, ZeroPage, 2 },
                { WDC65C02 | HuC6280, 0x37, 2, 7, ZeroPage, 3 },
                { WDC65C02 | HuC6280, 0x47, 2, 7, ZeroPage, 4 },
                { WDC65C02 | HuC6280, 0x57, 2, 7, ZeroPage, 5 },
                { WDC65C02 | HuC6280, 0x67, 2, 7, ZeroPage, 6 },
                { WDC65C02 | HuC6280, 0x77, 2, 7, ZeroPage, 7 },
              },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          },

          instructions
          {
            "Stack Operations",
            instruction
            {
              mnemonic { "PHA" },
              mnemonic_origin { "_Pus_h _Accumulator" },
              llvm_syntax { "PUSH A" },
              abstract { "SP = SP + 1 ;\t[SP] = A" },
              description { "Push the accumulator onto the stack." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x48, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "PHP" },
              mnemonic_origin { "_Pus_h _Processor Status Register" },
              llvm_syntax { "PUSH P" },
              abstract { "SP = SP + 1 ;\t[SP] = P" },
              description { "Push the process status register P onto the stack." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x08, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "PHX" },
              mnemonic_origin { "_Pus_h _X Register" },
              llvm_syntax { "PUSH X" },
              abstract { "SP = SP + 1 ;\t[SP] = X" },
              description { "Push the X register onto the stack." },
              mode_details { WDC65C02 | HuC6280, 0xDA, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "PHY" },
              mnemonic_origin { "_Pus_h _Y Register" },
              llvm_syntax { "PUSH Y" },
              abstract { "SP = SP + 1 ;\t[SP] = Y" },
              description { "Push the Y register onto the stack." },
              mode_details { WDC65C02 | HuC6280, 0x5A, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "PLA" },
              mnemonic_origin { "_Pu_ll _Accumulator" },
              llvm_syntax { "POP A" },
              abstract { "A = [SP]\nSP = SP - 1" },
              description { "Pull the value on the top of the stack into the accumulator. The previous contents of the accumulator are destroyed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x68, 1, 4, Implied },
              flags { "[SP]:7", nullptr, 0, nullptr, nullptr, nullptr, "[SP] == 0", nullptr },
            },
            instruction
            {
              mnemonic { "PLP" },
              mnemonic_origin { "_Pu_ll _Processor Status Register" },
              llvm_syntax { "POP P" },
              abstract { "P = [SP]\nSP = SP - 1" },
              description { "Pull the value on the top of the stack into the processor status register P. The previous contents of the status register are destroyed." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x28, 1, 4, Implied },
              flags { "P7", "P6", "P5", "P4", "P3", "P2", "P1", "P0" },
            },
            instruction
            {
              mnemonic { "PLX" },
              mnemonic_origin { "_Pu_ll _X Register" },
              llvm_syntax { "POP X" },
              abstract { "X = [SP]\nSP = SP - 1" },
              description { "Pull the value on the top of the stack into the X register. The previous contents of the X register are destroyed." },
              mode_details { WDC65C02 | HuC6280, 0xFA, 1, 4, Implied },
              flags { "[SP]:7", nullptr, 0, nullptr, nullptr, nullptr, "[SP] == 0", nullptr },
            },
            instruction
            {
              mnemonic { "PLY" },
              mnemonic_origin { "_Pu_ll _Y Register" },
              llvm_syntax { "POP Y" },
              abstract { "Y = [SP]\nSP = SP - 1" },
              description { "Pull the value on the top of the stack into the Y register. The previous contents of the Y register are destroyed." },
              mode_details { WDC65C02 | HuC6280, 0x7A, 1, 4, Implied },
              flags { "[SP]:7", nullptr, 0, nullptr, nullptr, nullptr, "[SP] == 0", nullptr },
            },
          },
          instructions
          {
            "ALU Operations",
            instruction
            {
              mnemonic { "ADC" },
              mnemonic_origin { "_A_dd With _Carry" },
              llvm_syntax { "" },
              abstract { "If T == 0: A = A + MEM\nElse: MEM = A + MEM" },
              description { "Add the data located at the effective address specified by the operand to the contents of the accumulator. Add one to the result if the carry flag is set, and store the final result in the accumulator. This opcode takes one extra cycle to complete if the decimal mode flag D is set." },
              summary {
R"(Add the value specified by the operand, and 1 if the Carry Flag is set, to the value in the accumulator. If the result is too large to fit in the accumulator, the carry flag will be set, otherwise it will be cleared.
If the Decimal-mode CPU flag is set, an extra cycle will be taken(*confirmed with Immediate mode only, however, other addressing modes need to be tested, but should yield the same result).
The overflow flag is not affected by this instruction if in Decimal mode; otherwise, if bit 7 of the result != bit 7 of the accumulator before the operation, and bit 7 of the accumulator before the operation == bit 7 of the value specified by the operand, the overflow flag is set, otherwise it is cleared. In other words, if we were to treat the accumulator and value specified by the operand as two's complement numbers, in the range of -128 to 127, the overflow flag will be set if the end result is outside of this range(otherwise it will be cleared).
If T=1 (the previous instruction is SET) the zero-page byte specified by the X register is used instead of the A register.)"
              },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x69, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0x65, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x75, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0x72, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x61, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x71, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x6D, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x7D, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x79, 3, 5, Absolute | Y_Indexed },
              },
              flags { "A + MEM > 127", "V*", 0, nullptr, nullptr, nullptr, "(A == 0) && (MEM == 0)", "C" },
            },
            instruction
            {
              mnemonic { "SBC" },
              mnemonic_origin { "_Su_btract with _Carry" },
              llvm_syntax { "" },
              abstract { "If T == 0: A = A - MEM\nElse: MEM = A - MEM" },
              description { "Subtract the data located at the effective address specified by the operand to the contents of the accumulator. Subtract one more from the result if the carry flag is set, and store the final result in the accumulator. This opcode takes one extra cycle if the decimal mode flag D is set." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xE9, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0xE5, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xF5, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0xF2, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0xE1, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0xF1, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xED, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0xFD, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xF9, 3, 5, Absolute | Y_Indexed },
              },
              flags { "A - MEM > 127", "V*", 0, nullptr, nullptr, nullptr, "A == MEM", "A >= MEM" },
            },
            instruction
            {
              mnemonic { "AND" },
              mnemonic_origin { "Bitwise _A_N_D" },
              llvm_syntax { "" },
              abstract { "If T == 0: A = A & MEM\nElse: MEM = A & MEM" },
              description { "Bitwise AND the data located at the effective address specified by the operand with the contents of the accumulator. Each bit in the accumulator is ANDed with the corresponding bit in memory, with the result being stored in the respective accumulator bit." },
              summary { "Performs a bit-by-bit logical and on the accumulator with the value specified by the operand." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x29, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0x25, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x35, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0x32, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x21, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x31, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x2D, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x3D, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x39, 3, 5, Absolute | Y_Indexed },
              },
              flags { "A:7 & MEM:7", nullptr, 0, nullptr, nullptr, nullptr, "A & MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "ORA" },
              mnemonic_origin { "Bitwise _O_R _Accumulator" },
              llvm_syntax { "" },
              abstract { "If T == 0: A = A | MEM\nElse: MEM = A | MEM" },
              description { "Bitwise OR the data located at the effective address specified by the operand with the contents of the accumulator. Each bit in the accumulator is ORed with the corresponding bit in memory, with the result being stored in the respective accumulator bit." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x09, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0x05, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x15, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0x12, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x01, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x11, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x0D, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x1D, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x19, 3, 5, Absolute | Y_Indexed },
              },
              flags { "A:7 | MEM:7", nullptr, 0, nullptr, nullptr, nullptr, "A | MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "EOR" },
              mnemonic_origin { "Bitwise _Exclusive _O_R Accumulator" },
              llvm_syntax { "" },
              abstract { "If T == 0: A = A ^ MEM\nElse: MEM = A ^ MEM" },
              description { "Bitwise Exclusive OR the data located at the effective address specified by the operand with the contents of the accumulator. Each bit in the accumulator is XORed with the corresponding bit in memory, with the result being stored in the respective accumulator bit." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x49, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0x45, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x55, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0x52, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x41, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0x51, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x4D, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x5D, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x59, 3, 5, Absolute | Y_Indexed },
              },
              flags { "A ^ MEM > 127", nullptr, 0, nullptr, nullptr, nullptr, "A ^ MEM == 0", nullptr },
            },
          },
          instructions
          {
            "Bitwise Shift Operations",
            instruction
            {
              mnemonic { "ASL" },
              mnemonic_origin { "_Arithmetic _Left _Shift" },
              llvm_syntax { "" },
              abstract { "C = MEM:7\nMEM = MEM << 1" },
              description { "Shift the contents of the location specified by the operand left one bit. That is, bit one takes on the value originally found in bit zero, bit two takes the value originally in bit one, and so on; bit 7 is transferred into the carry flag; bit 0 is cleared. The arithmetic result of the operation is an unsigned multiplication by two." },
              summary { "Shifts the value at the location specified by the operand left by one bit, shifting in 0 to bit 0, and writes the result back to that location. Bit 7 of the value before the shift is copied to the Carry flag." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x06, 2, 6, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x16, 2, 6, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x0E, 3, 7, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x1E, 3, 7, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x0A, 1, 2, Accumulator },
              },
              flags { "MEM:6", nullptr, 0, nullptr, nullptr, nullptr, "MEM & 0b01111111 == 0", "MEM:7" },
            },
            instruction
            {
              mnemonic { "ROL" },
              mnemonic_origin { "_R_otate _Left Through Carry" },
              llvm_syntax { "" },
              abstract { "TEMPBIT = MEM:7\nMEM = MEM << 1\nMEM:0 = C\nC = TEMPBIT" },
              description { "Rotate the contents of the location specified by the operand left one bit. That is, bit one takes on the value originally found in bit zero, bit two takes the value originally in bit one, and so on; bit 0 takes on the value in the carry flag; bit 7 is transferred into the carry." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x26, 2, 6, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x36, 2, 6, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x2E, 3, 7, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x3E, 3, 7, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x2A, 1, 2, Accumulator },
              },
              flags { "MEM:6", nullptr, 0, nullptr, nullptr, nullptr, "(MEM & 0b01111111 == 0) && (C == 0)", "MEM:7" },
            },
            instruction
            {
              mnemonic { "LSR" },
              mnemonic_origin { "_Logical _Shift _Right" },
              llvm_syntax { "" },
              abstract { "C = MEM:0\nMEM = MEM >> 1" },
              description { "Logical shift the contents of the location specified by the operand right one bit. That is, bit zero takes on the value originally found in bit one, bit one takes the value originally in bit two, and so on; bit 7 is cleared; bit 0 is transferred into the carry flag. The arithmetic result of the operation is an unsigned division by two." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x46, 2, 6, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x56, 2, 6, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x4E, 3, 7, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x5E, 3, 7, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x4A, 1, 2, Accumulator },
              },
              flags { 0, nullptr, 0, nullptr, nullptr, nullptr, "MEM & 0b11111110 == 0", "MEM:0" },
            },
            instruction
            {
              mnemonic { "ROR" },
              mnemonic_origin { "_R_otate _Right Through Carry" },
              llvm_syntax { "" },
              abstract { "TEMPBIT = MEM:0\nMEM = MEM >> 1\nMEM:7 = C\nC = TEMPBIT" },
              description { "Rotate the contents of the location specified by the operand right one bit. That is, bit zero takes on the value originally found in bit one, bit one takes the value originally in bit two, and so on; bit 7 takes on the value in the carry flag; bit 0 is transferred into the carry." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0x66, 2, 6, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0x76, 2, 6, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x6E, 3, 7, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0x7E, 3, 7, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x6A, 1, 2, Accumulator },
              },
              flags { "C == 1", nullptr, 0, nullptr, nullptr, nullptr, "(MEM & 0b11111110 == 0) && (C == 0)", "MEM:0" },
            },
          },
          instructions
          {
            "Increment/Decrement Operations",
            instruction
            {
              mnemonic { "DEC" },
              mnemonic_origin { "_D_e_crement" },
              llvm_syntax { "DEC MEM" },
              abstract { "MEM = MEM - 1" },
              description { "Decrement by one the contents of the location specified by the operand (subtract one from the value). DEC neither affects nor is affected by the carry flag." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xC6, 2, 6, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xD6, 2, 6, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xCE, 3, 7, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0xDE, 3, 7, Absolute | X_Indexed },
                { WDC65C02 | HuC6280, 0x3A, 1, 2, Accumulator },
              },
              flags { "MEM - 1 > 127", nullptr, 0, nullptr, nullptr, nullptr, "MEM - 1 == 0", nullptr },
            },
            instruction
            {
              mnemonic { "DEX" },
              mnemonic_origin { "_D_ecrement _X" },
              llvm_syntax { "DEC X" },
              abstract { "X = X - 1" },
              description { "Decrement by one the contents of the X register (subtract one from the value). DEX neither affects nor is affected by the carry flag." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xCA, 1, 2, Implied },
              flags { "X - 1 > 127", nullptr, 0, nullptr, nullptr, nullptr, "X - 1 == 0", nullptr },
            },
            instruction
            {
              mnemonic { "DEY" },
              mnemonic_origin { "_D_ecrement _Y" },
              llvm_syntax { "DEC Y" },
              abstract { "Y = Y - 1" },
              description { "Decrement by one the contents of the Y register (subtract one from the value). DEY neither affects nor is affected by the carry flag." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0x88, 1, 2, Implied },
              flags { "Y - 1 > 127", nullptr, 0, nullptr, nullptr, nullptr, "Y - 1 == 0", nullptr },
            },
            instruction
            {
              mnemonic { "INC" },
              mnemonic_origin { "_I_n_crement" },
              llvm_syntax { "INC MEM" },
              abstract { "MEM = MEM + 1" },
              description { "Increments contents of the location specified by the operand (add one to the value). INC neither affects nor is affected by the carry flag." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xE6, 2, 6, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xF6, 2, 6, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xEE, 3, 7, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0xFE, 3, 7, Absolute | X_Indexed },
                { WDC65C02 | HuC6280, 0x1A, 1, 2, Accumulator },
              },
              flags { "MEM + 1 > 127", nullptr, 0, nullptr, nullptr, nullptr, "MEM + 1 == 0", nullptr },
            },
            instruction
            {
              mnemonic { "INX" },
              mnemonic_origin { "_I_ncrement _X" },
              llvm_syntax { "INC X" },
              abstract { "X = X + 1" },
              description { "Increment by one contents of the X register (add one to the value). INX neither affects nor is affected by the carry flag." },
              summary { "Increments the value in the X register by one." },
              note { "The Carry Flag is not used, nor is it modified." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xE8, 1, 2, Implied },
              flags { "X + 1 > 127", nullptr, 0, nullptr, nullptr, nullptr, "X + 1 == 0", nullptr },
            },
            instruction
            {
              mnemonic { "INY" },
              mnemonic_origin { "_I_ncrement _Y" },
              llvm_syntax { "INC Y" },
              abstract { "Y = Y + 1" },
              description { "Increment by one contents of the Y register (add one to the value). INY neither affects nor is affected by the carry flag." },
              summary { "Increments the value in the Y register by one." },
              note { "The Carry Flag is not used, nor is it modified." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xC8, 1, 2, Implied },
              flags { "Y + 1 > 127", nullptr, 0, nullptr, nullptr, nullptr, "Y + 1 == 0", nullptr },
            },
          },

          instructions
          {
            "Comparison Operations",
            instruction
            {
              mnemonic { "CMP" },
              mnemonic_origin { "_Co_m_pare Accumulator" },
              llvm_syntax { "CMP A, MEM" },
              abstract { "A - MEM (result discarded)" },
              description { "Subtract the data located at the effective address specified by the operand from the contents of the accumulator, setting the carry, zero, and negative flags based on the result, but without altering the contents of either the memory location or the accumulator. The comparison is of unsigned binary values only (decimal mode is ignored), and the result is not saved." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xC9, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0xC5, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xD5, 2, 4, ZeroPage | X_Indexed },
                { WDC65C02 | HuC6280, 0xD2, 2, 7, ZeroPage | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0xC1, 2, 7, ZeroPage | X_Indexed | Indirect },
                { NMOS6502 | WDC65C02 | HuC6280, 0xD1, 2, 7, ZeroPage | Indirect | Y_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xCD, 3, 5, Absolute },
                { NMOS6502 | WDC65C02 | HuC6280, 0xDD, 3, 5, Absolute | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0xD9, 3, 5, Absolute | Y_Indexed },
              },
              flags { "A - MEM > 127", nullptr, 0, nullptr, nullptr, nullptr, "A == MEM", "A >= MEM" },
            },
            instruction
            {
              mnemonic { "CPX" },
              mnemonic_origin { "_Com_pare _X Register" },
              llvm_syntax { "CMP X, MEM" },
              abstract { "X - MEM (result discarded)" },
              description { "Subtract the data located at the effective address specified by the operand from the contents of the X register, setting the carry, zero, and negative flags based on the result, but without altering the contents of either the memory location or the accumulator. The comparison is of unsigned binary values only (decimal mode is ignored), and the result is not saved." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xE0, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0xE4, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xEC, 3, 5, Absolute },
              },
              flags { "X - MEM > 127", nullptr, 0, nullptr, nullptr, nullptr, "X == MEM", "X >= MEM" },
            },
            instruction
            {
              mnemonic { "CPY" },
              mnemonic_origin { "_Com_pare _Y Register" },
              llvm_syntax { "CMP Y, MEM" },
              abstract { "Y - MEM (result discarded)" },
              description { "Subtract the data located at the effective address specified by the operand from the contents of the Y register, setting the carry, zero, and negative flags based on the result, but without altering the contents of either the memory location or the accumulator. The comparison is of unsigned binary values only (decimal mode is ignored), and the result is not saved." },
              std::list<mode_details>
              {
                { NMOS6502 | WDC65C02 | HuC6280, 0xC0, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0xC4, 2, 4, ZeroPage },
                { NMOS6502 | WDC65C02 | HuC6280, 0xCC, 3, 5, Absolute },
              },
              flags { "Y - MEM > 127", nullptr, 0, nullptr, nullptr, nullptr, "Y == MEM", "Y >= MEM" },
            },
          },
          instructions
          {
            "Bit Testing Operations",
            instruction
            {
              mnemonic { "BIT" },
              mnemonic_origin { "_B_it _Test" },
              llvm_syntax { "" },
              abstract { "A & MEM (result discarded)" },
              description { "BIT sets the status register flags based on the result of two different operations. First, it sets or clears the N flag to reflect the value of the high bit (bit 7) of the data located at the effective address specified by the operand, and sets or clears the V flag to reflect the contents of the next-to-highest bit (bit 6) of the data addressed. Second, it logically ANDs the data located at the effective address with the contents of the accumulator; it changes neither value, but sets the Z flag if the result is zero, or clears it if the result is non-zero." },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x89, 2, 2, Immediate },
                { NMOS6502 | WDC65C02 | HuC6280, 0x24, 2, 4, ZeroPage },
                { WDC65C02 | HuC6280, 0x34, 2, 4, ZeroPage | X_Indexed },
                { NMOS6502 | WDC65C02 | HuC6280, 0x2C, 3, 5, Absolute },
                { WDC65C02 | HuC6280, 0x3C, 3, 5, Absolute | X_Indexed },
              },
              flags { "MEM:7", "MEM:6", 0, nullptr, nullptr, nullptr, "A & MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TRB" },
              mnemonic_origin { "_Test and _Reset _Bits" },
              llvm_syntax { "" },
              abstract { "MEM = MEM & ~A" },
              description { "Logically AND together the complement of the value in the accumulator with the data at the effective address specified by the operand. Store the result at the memory location. This clears each bit for which the corresponding accumulator bit is set, making it an ideal opcode for masking data. N and V and Z are set as in the BIT opcode instruction. These flags are set based on the ANDing of the uncomplemented accumulator value with the memory value." },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x14, 2, 6, ZeroPage },
                { WDC65C02 | HuC6280, 0x1C, 3, 7, Absolute },
              },
              flags { "MEM:7", "MEM:6", 0, nullptr, nullptr, nullptr, "A & MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TSB" },
              mnemonic_origin { "Test and _Set _Bits" },
              llvm_syntax { "" },
              abstract { "MEM = MEM | ~A" },
              description { "Logically OR together the value in the accumulator with the data at the effective address specified by the operand. Store the result at the memory location. This sets each bit for which the corresponding accumulator bit is set, making it an ideal opcode for masking data. N and V and Z are set as in the BIT opcode instruction. These flags are set based on the ANDing of the accumulator value with the memory value." },
              std::list<mode_details>
              {
                { WDC65C02 | HuC6280, 0x04, 2, 6, ZeroPage },
                { WDC65C02 | HuC6280, 0x0C, 3, 7, Absolute },
              },
              flags { "A:7 | MEM:7", "A:6 | MEM:6", 0, nullptr, nullptr, nullptr, "A | MEM == 0", nullptr },
            },
            instruction
            {
              mnemonic { "TST" },
              mnemonic_origin { "_Te_s_t Memory Bits" },
              llvm_syntax { "" },
              abstract { "IMM & MEM (result discarded)" },
              description { "Logically AND together the immediate operand with the data at the effective address specified by the operand. This sets each bit for which the corresponding immediate argument bit is set, making it an ideal opcode for masking data. N and V and Z are set as in the BIT opcode instruction." },
              std::list<mode_details>
              {
                { HuC6280, 0x83, 3, 7, Immediate | Secondary | ZeroPage },
                { HuC6280, 0xA3, 3, 7, Immediate | Secondary | ZeroPage | X_Indexed },
                { HuC6280, 0x93, 4, 8, Immediate | Secondary | Absolute },
                { HuC6280, 0xB3, 4, 8, Immediate | Secondary | Absolute | X_Indexed },
              },
              flags { "MEM:7", "MEM:6", 0, nullptr, nullptr, nullptr, "IMM & MEM == 0", nullptr },
            },
          },
          instructions
          {
            "Miscellaneous Operations",
            instruction
            {
              mnemonic { "NOP" },
              mnemonic_origin { "_No _O_peration" },
              llvm_syntax { "NOP" },
              abstract { "PC = PC + 1" },
              description { "NOP performs no action, and is often used for timing loops or temporarily removing certain instructions." },
              mode_details { NMOS6502 | WDC65C02 | HuC6280, 0xEA, 1, 2, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CSH" },
              mnemonic_origin { "_Change _Speed _High" },
              llvm_syntax { "" },
              abstract { "Run CPU at 100% speed (7.15909 MHz)" },
              description { "Sets the HuC6280 to \"high speed,\" or normal speed mode. Used for switching the processor back into high-speed mode." },
              mode_details { HuC6280, 0xD4, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
            instruction
            {
              mnemonic { "CSL" },
              mnemonic_origin { "_Change _Speed _Low" },
              llvm_syntax { "" },
              abstract { "Run CPU at 25% speed (1.7897725 MHz)" },
              description { "Sets the HuC6280 to low speed. Need for accessing slow memory. The CD bios routines, and some hucards, use this for accessing BRAM." },
              mode_details { HuC6280, 0x54, 1, 3, Implied },
              flags { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr },
            },
          }
        });
}
