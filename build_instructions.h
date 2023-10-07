#pragma once

#include <cstdint>
#include <string>
#include <list>
#include <optional>
#include <variant>
#include <tuple>
#include <array>

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
  None     = 0x0000,
  NMOS6502 = 0x0001,
  WDC65C02 = 0x0002,
  HuC6280  = 0x0004,
};

constexpr isa operator |(isa a, isa b)
  { return isa(uint16_t(a) | uint16_t(b)); }

constexpr const int isa_count = 3;

struct isa_property : std::array<std::string, isa_count>
{
  using parent = std::array<std::string, isa_count>;
  isa_property (void) { }

  template <typename... Args>
  isa_property (const isa f, const char* val, const Args&... args)
    : isa_property(args...)
  {
    for(std::size_t pos = 0; pos < parent::size(); ++pos)
      if(f & (1 << pos))
        parent::operator[](pos) = val;
  }

  std::string operator[] (uint16_t i) const
  {
    return parent::operator[](std::countr_zero(i));
  }
};

enum modes_t : uint32_t
{
  ZeroPage = 1,
  Implied,
  Absolute,
  Immediate,
  Accumulator,
  Relative,
  Block,
  Indirect,
  X_Indexed,
  Y_Indexed,
  Secondary,
};

constexpr modes_t operator |(modes_t a, modes_t b)
  { return modes_t((uint32_t(a) << 4) | uint32_t(b)); }

using snn_t = std::variant<std::nullptr_t, int, std::string>; // String Number or Null

#define EASY_STRING(type) \
  struct type : std::string { using std::string::operator+=; using std::string::operator=;}; \
  //operator


EASY_STRING(name)
EASY_STRING(mnemonic)
EASY_STRING(mnemonic_origin)
EASY_STRING(llvm_syntax)
EASY_STRING(pceas_syntax)
EASY_STRING(abstract)
EASY_STRING(description)
EASY_STRING(summary)
EASY_STRING(machine_code)
EASY_STRING(mode_string)

EASY_STRING(mode_id)
EASY_STRING(cycles)
EASY_STRING(note)
using flags = std::array<snn_t, 8>;

struct mode_details
{
  isa cpus;
  int opcode;
  int byte_count;
  snn_t cycle_count;
  modes_t mode_data;
  std::optional<int> mnemonic_fill_value = std::nullopt;
  abstract abstract_string = {};
  pceas_syntax pceas_syntax_string = {};
  llvm_syntax  llvm_syntax_string = {};
  machine_code machine = {};
  description description_string = {};
  summary summary_string = {};
  name name_string = {};
  mode_string address_mode_string = {};
};

struct instruction
{
  instruction(void) {}

  template <typename T, typename... Args>
  instruction(const T& a, const Args&... args)
    : instruction(args...)
  {
    std::get<T>(details) = a;
  }

  template <typename T>
  const T& data(void) const { return std::get<T>(details); }

  template <typename T>
  T& data(void) { return std::get<T>(details); }

  std::tuple<name,
              mnemonic,
              mnemonic_origin,
              description,
              summary,
              llvm_syntax,
              abstract,
              mode_details,
              std::list<mode_details>,
              note,
              flags> details =
  {
    name(),
    mnemonic(),
    mnemonic_origin(),
    description(),
    summary(),
    llvm_syntax(),
    abstract(),
    mode_details(),
    std::list<mode_details>(),
    note(),
    flags(),
  };
};

struct instructions : public std::list<instruction>
{
  template <typename... Args>
  instructions (const char* title, const Args&... args)
    : std::list<instruction>(std::initializer_list<instruction>{ args... })
  { section_title = title; }

  const char* section_title;
};

// ----------------------------------------------------------------------------

void build_insn_blocks(std::list<instructions>& insn_blocks);
