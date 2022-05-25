#pragma once

#include <cstdint>
#include <string>
#include <list>
#include <tuple>

#if __cplusplus < 202002L
template< class T >
constexpr int countr_zero( T x ) noexcept
{
  for(std::size_t i = 0; i < sizeof(T) * 8; ++i)
    if(x & (1 << i))
      return i;
  return sizeof (T) * 8;
}
#else
# include <bit>
#endif

enum isa : uint16_t
{
  SH_NONE = 0x0000,
  SH1     = 0x0001,
  SH2     = 0x0002,
  SH2E    = 0x0004,
  SH2A    = 0x0008,
  SH3     = 0x0010,
  SH3E    = 0x0020,
  SH4     = 0x0040,
  SH4A    = 0x0080,
  SH_DSP  = 0x0100,

  SH_ALL  = SH1 | SH2 | SH2E | SH2A | SH3 | SH3E | SH4 | SH4A | SH_DSP,
};

constexpr isa operator |(isa a, isa b)
  { return isa(uint16_t(a) | uint16_t(b)); }

struct isa_property : std::array<std::string, 9>
{
  using parent = std::array<std::string, 9>;
  isa_property (void) { }

  template <typename... Args>
  isa_property (const isa f, const char* val, const Args&... args)
    : isa_property(args...)
  {
    for(std::size_t pos = 0; pos < parent::size(); ++pos)
      if(f & (1 << pos))
        parent::operator[](pos) = val;
  }

  std::string operator[] (isa i) const
    { return parent::operator[](countr_zero(i)); }
};

struct group : public isa_property { using isa_property::isa_property; };
struct issue : public isa_property { using isa_property::isa_property; };
struct latency : public isa_property { using isa_property::isa_property; };


template<typename T, T default_value>
struct property_class
{
  constexpr property_class(const T& other) : value(other) {}
  constexpr property_class(void) : value(default_value) {}
  constexpr operator const T(void) const { return value; }
  constexpr T& operator =(const T& other) { return value = other; }
  T value;
};

enum document
{
  SH7750_PROG_DOC,
  SH1_2_PROG_DOC,
  SH1_2_DSP_DOC,
  SH2A_DOC,
  SHA4_CORE_DOC,
  SH4A_DOC,
};

struct document_details_t
{
  const std::string_view name;
  const std::string_view identifier;
  const std::string_view date;
  const std::string_view location;
};

constexpr std::array<document_details_t, 6> documents =
{
  {
    {
      "SuperH™ (SH) 32-Bit RISC MCU/MPU Series\nSH7750\nHigh-Performance RISC Engine\nProgramming Manual",
      "ADE-602-156A\nRev. 2.0",
      "1999/03/04",
      "https://ia802500.us.archive.org/9/items/manuallib-id-2595799/2595799.pdf",
    },
    {
      "SuperH RISC Engine\nSH-1/SH-2\nProgramming Manual",
      "",
      "1996/09/03",
      "https://antime.kapsi.fi/sega/files/h12p0.pdf",
    },
    {
      "Hitachi SuperH™ RISC Engine\nSH-1/SH-2/SH-DSP\nProgramming Manual",
      "ADE-602-063C\nRev. 4.0",
      "1999/13/05",
      "https://retrocdn.net/images/3/35/Hitachi_SuperH_Programming_Manual.pdf",
    },
    {
      "SH-2A, SH2A-FPU\nSoftware Manual\nUser’s Manual\nRenesas 32-Bit RISC\nMicrocomputer\nSuperH™ RISC Engine",
      "Rev. 3.00",
      "2005/07/08",
      "https://www.renesas.com/us/en/document/mah/sh-2a-sh2a-fpu-software-manual?language=en",
    },
    {
      "SH-4 CPU Core Architecture",
      "ADCS 7182230F",
      "2002/09/12",
      "https://www.st.com/resource/en/user_manual/cd00147165-sh-4-32-bit-cpu-core-architecture-stmicroelectronics.pdf",
    },
    {
      "SH-4A\nExtended Functions\nSoftware Manual",
      "Rev.2.00",
      "2013/01/18",
      "https://www.renesas.com/us/en/document/mat/sh-4a-extended-functions-software-manual?language=en",
    },
  }
};

struct citation_t
{
  document source;
  int page;
};

struct environment_t
{
  isa cpus;
  std::string property;
};

struct environments     : std::list<environment_t> {};
struct citations        : std::list<citation_t> {};

struct format           : std::string {};
struct abstract         : std::string {};
struct brief            : std::string {};
struct name             : std::string {};
struct restriction      : std::string {};
struct classification   : std::string {};
struct mnemonic         : std::string {};
struct mnemonic_origin  : std::string {};
struct code             : std::string {};
struct flags            : std::string {};
struct description      : std::string {};
struct note             : std::string {};
struct operation        : std::string {};
struct example          : std::string {};
struct exceptions       : std::string {};

/*
 TODO:
 add: BRK instruction
 add: FIPR,FTRV brief

  mnemonic { "FMOV.D" }, // ??
*/

struct insn
{
  insn(void) {}

  template <typename T, typename... Args>
  insn(const T& a, const Args&... args)
    : insn(args...)
  {
    std::get<T>(details) = a;
  }

  template <typename... Args>
  insn (const char* str_format, const Args&... args)
      : insn(args...)
  {
    std::get<format>(details) = format { str_format };
  }

  template <typename T>
  const T& data(void) const { return std::get<T>(details); }

  template <typename T>
  T& data(void) { return std::get<T>(details); }

  bool is_isa (isa i) const { return std::get<isa>(details) & i; }

  std::tuple<format, abstract, name, classification, brief, restriction, mnemonic, mnemonic_origin,
             citations, code, description, note, operation, example, exceptions,
             group, issue, latency, environments, flags, isa> details =
  {
    format(), abstract(), name(), classification(), brief(), restriction(), mnemonic(), mnemonic_origin(),
    citations(), code(), description(), note(), operation(), example(), exceptions(),
    group(), issue(), latency(), environments(), flags(), SH_NONE,
  };
};

struct insns : public std::list<insn>
{
  template <typename... Args>
  insns (const char* title, const Args&... args)
    : std::list<insn>({ args... })
  { section_title = title; }

  const char* section_title;
};

// ----------------------------------------------------------------------------

void build_insn_blocks(std::list<insns>& insn_blocks);
