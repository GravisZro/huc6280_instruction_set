#include "post_processing.h"

#include "build_instructions.h"

#include <cassert>
#include <format>
#include <optional>
#include <variant>
#include <algorithm>
#include <regex>
#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>

using namespace std::literals;
using namespace std::string_view_literals;
using namespace std::literals::string_literals;

void modes_decoder(mnemonic op_mnemonic, mode_details& details)
{
  details.abstract_string = std::regex_replace(details.abstract_string,
                                           std::regex("IMM", std::regex_constants::extended),
                                           "$nn", std::regex_constants::format_sed);

  details.abstract_string = std::regex_replace(details.abstract_string,
                                           std::regex("REL", std::regex_constants::extended),
                                           "$rr", std::regex_constants::format_sed);
  if(details.mnemonic_fill_value)
  {
    uint8_t value = details.mnemonic_fill_value.value();

    details.abstract_string = std::regex_replace(details.abstract_string,
                                             std::regex("#n", std::regex_constants::extended),
                                             std::to_string(value), std::regex_constants::format_sed);

    details.name_string = std::regex_replace(details.name_string,
                                             std::regex("#n", std::regex_constants::extended),
                                             std::to_string(value), std::regex_constants::format_sed);


    details.name_string = std::regex_replace(details.name_string,
                                             std::regex("#n", std::regex_constants::extended),
                                             std::to_string(value), std::regex_constants::format_sed);


    details.summary_string = std::regex_replace(details.summary_string,
                                                std::regex("#n", std::regex_constants::extended),
                                                std::to_string(value), std::regex_constants::format_sed);

    op_mnemonic.pop_back();
    op_mnemonic.push_back('0' + value);
  }

  uint32_t data = details.mode_data;
  assert(data);
  while(!(data & 0xF0000000))
    data <<= 4;

  std::string first_mode;
  std::string mem_string;
  details.machine = std::format("{:02X}", details.opcode);
  int cycle_count = 1;
  int byte_count = 1;

  bool is_zero_page = false;
  bool is_absolute = false;
  while(data & 0xF0000000)
  {
    switch(data >> 28)
    {
    case ZeroPage:
      is_zero_page = true;
      details.address_mode_string += "Zero Page";
      //details.abstract_string.replace("MEM", "ZP($ZZ)");
      details.pceas_syntax_string += "$ZZ";
      details.machine += " ZZ";
      mem_string = "ZP8($ZZ)"; // mem string base
      cycle_count += 3;
      byte_count += 1;
      break;
    case Implied:
      details.address_mode_string += "Implied";
      break;
    case Absolute:
      is_absolute = true;
      details.address_mode_string += "Absolute";
      details.pceas_syntax_string += "$hhll";
      details.machine += " ll hh";
      mem_string = "$hhll"; // mem string base
      cycle_count += 4;
      byte_count += 2;
      break;
    case Immediate:
      details.address_mode_string += "Immediate";
      details.pceas_syntax_string += "#$nn";
      details.machine += " nn";
      mem_string = "$nn"; // mem string base
      cycle_count += 1;
      byte_count += 1;
      break;
    case Accumulator:
      details.address_mode_string += "Accumulator";
      details.pceas_syntax_string += "A";
      mem_string = "A"; // mem string base
      cycle_count += 1;
      break;
    case Relative:
      details.address_mode_string += "Relative";
      details.pceas_syntax_string += "$rr";
      details.machine += " rr";
      cycle_count += 1;
      byte_count += 1;
      break;
    case Block:
      details.address_mode_string = "Block";
      details.pceas_syntax_string = "$SHSL, $DHDL, $LHLL";
      details.machine += " SL SH DH DL LL HL";
      byte_count += 6;
      details.cycle_count = "17 + 6 * $LHLL";
      break;
    case Indirect:
      details.address_mode_string += "Indirect";
      cycle_count += 3;
      details.pceas_syntax_string.pop_back();
      details.pceas_syntax_string.pop_back();
      details.pceas_syntax_string = "(" + details.pceas_syntax_string + ")";
      // mem string
      if(mem_string == "ZP8($ZZ)")
        mem_string = "ZP16($ZZ)";
      mem_string = "[" + mem_string + "]";
      break;
    case X_Indexed:
      details.address_mode_string+= "X-Indexed";
      if(is_absolute || is_zero_page)
        details.pceas_syntax_string += "X";
      // mem string
      if(is_zero_page)
        mem_string = "[ZP16($ZZ + X)]";
      if(is_absolute)
        mem_string = "[$hhll + X]";
      break;
    case Y_Indexed:
      details.address_mode_string += "Y-Indexed";
      if(is_absolute || is_zero_page)
        details.pceas_syntax_string += "Y";
      // mem string
      if(is_zero_page)
        mem_string = "[ZP16($ZZ) + Y]";
      if(is_absolute)
        mem_string = "[$hhll + Y]";
      break;
    case Secondary:
      details.address_mode_string += " and ";
      details.pceas_syntax_string.pop_back();
      details.pceas_syntax_string.pop_back();
      first_mode = details.pceas_syntax_string;
      details.pceas_syntax_string.clear();
      break;
    }

    data <<= 4;
    details.address_mode_string += ", ";
    details.pceas_syntax_string += ", ";
  }
/*
  if(!mem_string.empty())
    details.abstract_string = std::regex_replace(details.abstract_string,
                                             std::regex("MEM", std::regex_constants::extended),
                                             mem_string, std::regex_constants::format_sed);
*/
  details.address_mode_string.pop_back();
  details.address_mode_string.pop_back();
  details.pceas_syntax_string.pop_back();
  details.pceas_syntax_string.pop_back();

  details.address_mode_string = std::regex_replace(details.address_mode_string,
                            std::regex(R"~(,  and ,)~", std::regex_constants::extended),
                            R"~( and)~", std::regex_constants::format_sed);

  details.pceas_syntax_string = std::regex_replace(details.pceas_syntax_string,
                            std::regex(R"~(, \))~", std::regex_constants::extended),
                            R"~())~", std::regex_constants::format_sed);

  if(!details.cycle_count.index())
    details.cycle_count = std::to_string(cycle_count);
  if(details.cycle_count.index() == 1 &&
     std::get<int>(details.cycle_count) != cycle_count)
  {
    std::cerr << "mismatch " << op_mnemonic << " : " << details.address_mode_string << std::endl
              << "expected " << std::get<int>(details.cycle_count) << " got " << cycle_count << std::endl;
  }

  assert(details.byte_count == byte_count);
  details.pceas_syntax_string = op_mnemonic + " " + first_mode + details.pceas_syntax_string;
}
/*
// isolate html symbols so that html can be inserted without being modified
static const std::array<std::pair<std::string_view, std::string_view>, 4> html_symbols =
{
  {
    { R"~(>)~", R"~(&gt;)~" }, // because we use the < character
    { R"~(<)~", R"~(&lt;)~" }, // because we use the > character
    { R"~(=)~", R"~(&equal;)~" }, // because we use the = character
    { R"~(/)~", R"~(&divide;)~" }, // because we use the = character
  }
};

static const std::array<std::pair<std::string_view, std::string_view>, 33> unicode_symbols =
{
  {
    { R"~(>)~", R"~(&gt;)~" }, // because we use the < character
    { R"~(<)~", R"~(&lt;)~" }, // because we use the > character
    { R"~(=)~", R"~(&equal;)~" }, // because we use the = character
    { R"~(/)~", R"~(&divide;)~" }, // because we use the = character
//    { R"~(→)~", R"~(<var title="store into (right)"></var>)~" },
    { R"~(←)~", R"~(<var title="assignment"></var>)~" },

    { R"~(&equal;)~", R"~(<var title="equality"></var>)~" },
    { R"~(≥)~", R"~(<var title="greater than or equal"></var>)~" },
    { R"~(≤)~", R"~(<var title="less than or equal"></var>)~" },
    { R"~(&lt;&lt;)~", R"~(<var title="shift bits left"></var>)~" },
    { R"~(&gt;&gt;)~", R"~(<var title="shift bits right"></var>)~" },
    { R"~(&gt;)~", R"~(<var title="greater than"></var>)~" },
    { R"~(&lt;)~", R"~(<var title="less than"></var>)~" },
    { R"~(∀)~", R"~(<var title="for all"></var>)~" },

    { R"~(−)~", R"~(<var title="subtract"></var>)~" },
    { R"~(+)~", R"~(<var title="add"></var>)~" },
    { R"~(×)~", R"~(<var title="multiply"></var>)~" },
    { R"~(÷)~", R"~(<var title="divide"></var>)~" },
    { R"~(&divide;)~", R"~(<var title="divide"></var>)~" },

    { R"~(&&)~", R"~(<var title="logical and"></var>)~" },
    { R"~(||)~", R"~(<var title="logical or"></var>)~" },
    { R"~(⊕)~", R"~(<var title="bitwise xor"></var>)~" },
    { R"~(~)~", R"~(<var title="bitwise not"></var>)~" },
    { R"~(∨)~", R"~(<var title="bitwise or"></var>)~" },
    { R"~(∧)~", R"~(<var title="bitwise and"></var>)~" },
  }
};
*/

// no regex
static const std::array<std::pair<std::string_view, std::string_view>, 22> typeable_symbols =
{
  {
// isolate symbols used in HTML so that HTML can be inserted without being modified
    { R"~(>)~", R"~(&gt;)~" },
    { R"~(<)~", R"~(&lt;)~" },
    { R"~(=)~", R"~(&equal;)~" },
    { R"~(/)~", R"~(&divide;)~" },

// C operations
// modification
    { R"~(&lt;&lt;)~", R"~(<var title="shift bits left"></var>)~" },
    { R"~(&gt;&gt;)~", R"~(<var title="shift bits right"></var>)~" },
    { R"~(*)~", R"~(<var title="multiply"></var>)~" },
    { R"~(-)~", R"~(<var title="subtract"></var>)~" },
    { R"~(+)~", R"~(<var title="add"></var>)~" },
    { R"~(&divide;)~", R"~(<var title="divide"></var>)~" },
// comparison
    { R"~(&equal;&equal;)~", R"~(<var title="equality"></var>)~" },
    { R"~(&gt;&equal;)~", R"~(<var title="greater than or equal"></var>)~" },
    { R"~(&lt;&equal;)~", R"~(<var title="less than or equal"></var>)~" },
    { R"~(&gt;)~", R"~(<var title="greater than"></var>)~" },
    { R"~(&lt;)~", R"~(<var title="less than"></var>)~" },
// assignment
    { R"~(&equal;)~", R"~(<var title="assignment"></var>)~" },
// logic
    { R"~(&&)~", R"~(<var title="logical and"></var>)~" },
    { R"~(||)~", R"~(<var title="logical or"></var>)~" },
// bitwise logic
    { R"~(&)~", R"~(<var title="bitwise and"></var>)~" },
    { R"~(|)~", R"~(<var title="bitwise or"></var>)~" },
    { R"~(^)~", R"~(<var title="bitwise xor"></var>)~" },
    { R"~(~)~", R"~(<var title="bitwise not"></var>)~" },
  }
};

// regex
static const std::array<std::pair<const char*, const char*>, 4> typeable_patterns =
{
  {
    // derefence
    { R"~(\[([^]]+)])~", R"~(<var title="dereferenced">\1</var>)~" },
    //{ R"~(\[([^\)]+)])~", R"~(<var title="Byte from Zero Page memory (constrained range 0x2000 - 0x20FF)">ZP(\1)</var>)~" },
    { R"~(ZP8\(([^\)]+)\))~", R"~(<var title="Byte from Zero Page memory (constrained range 0x2000 - 0x20FF)">ZP(\1)</var>)~" },
    { R"~(\[ZP16\(([^\)]+)\)])~", R"~(<var title="Byte at 16-bit address in Zero Page memory">[ZP(\1)]</var>)~" },
    { R"~(\[ZP16\(([^\)]+)\)([^\]]+)])~", R"~(<var title="Byte at 16-bit address (indexed) in Zero Page memory">[ZP(\1)\2]</var>)~" },
  }
};

static constexpr const std::array<std::pair<std::string_view, std::string_view>, 8> long_accronyms =
{
  {
    { R"~(ALU)~", R"~(<abbr title="Arithmetic Logic Unit">ALU</abbr>)~" },
    { R"~(CPU)~", R"~(<abbr title="Central Processing Unit">CPU</abbr>)~" },
    { R"~(LSB)~", R"~(<abbr title="Least Significant Bit">LSB</abbr>)~" },
    { R"~(MSB)~", R"~(<abbr title="Most Significant Bit">MSB</abbr>)~" },
    { R"~(PCH)~", R"~(<abbr title="Program Counter High Byte">PC<sub>H</sub></abbr>)~" },
    { R"~(PCL)~", R"~(<abbr title="Program Counter Low Byte">PC<sub>L</sub></abbr>)~" },
    { R"~(IRQ1)~", R"~(<abbr title="Interrupt Requestion 1">IRQ 1</abbr>)~" },
    { R"~(IRQ2)~", R"~(<abbr title="Interrupt Requestion 2">IRQ 2</abbr>)~" },
  }
};

static constexpr std::array<std::pair<const char*, const char*>, 22> short_accronyms =
{
  {
    // { R"~((^|[^\<\>[:alnum:]\S])XXXXXX([^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="YYYYY">XXXXXX</abbr>\2)~" },

    { R"~((^|[^\<\>[:alnum:]\S])MEM:([:digit:])($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Memory, bit \2">MEM<sub>\2</sub></abbr>\3)~" },
    { R"~((^|[^\<\>[:alnum:]\S])A:([:digit:])($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Accumulator register, bit 0">A<sub>\2</sub></abbr>\3)~" },
    { R"~((^|[^\<\>[:alnum:]\S])Y:([:digit:])($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Y register, bit \2">Y<sub>\2</sub></abbr>\3)~" },
    { R"~((^|[^\<\>[:alnum:]\S])X:([:digit:])($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="X register, bit \2">X<sub>\2</sub></abbr>\3)~" },
    { R"~((^|[^\<\>[:alnum:]\S])SP:([:digit:])($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Stack pointer, bit \2">S<sub>\2</sub></abbr>\3)~" },
    { R"~((^|[^\<\>[:alnum:]\S])\[SP]:([:digit:])($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Top Stack byte, bit \2">*S<sub>\2</sub></abbr>\3)~" },
    { R"~((^|[^\<\>[:alnum:]\S])\[SP]($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Top Stack byte">*S</abbr>\2)~" },

//  { R"~(\[([^]]+)])~", R"~(\1<var title="dereferenced"></var>\2)~" },

    { R"~(([^\<\>"[:alnum:]\S])MMU($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Memory Management Unit">MMU</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])PC($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Program Counter">PC</abbr>\2)~" },

    { R"~((^|[^\<\>[:alnum:]\S])SP($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Stack Pointer">S</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])A($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Accumulator register">A</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])X($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="X register">X</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])Y($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Y register">Y</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])P($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Processor Status Register">P</abbr>\2)~" },

    { R"~((^|[^\<\>[:alnum:]\S])N($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Negative Flag">N</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])V($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Overflow Flag">V</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])T($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Memory Transfer Flag">T</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])B($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Break Flag">B</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])D($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Decimal Mode Flag">D</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])I($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Interrupt Disable Flag">I</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])Z($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Zero Flag">Z</abbr>\2)~" },
    { R"~((^|[^\<\>[:alnum:]\S])C($|[^\<\>[:alnum:]\S]))~", R"~(\1<abbr title="Carry Flag">C</abbr>\2)~" },
  }
};


template<typename T>
void replace_symbols(std::string& data, const T& symbols)
{
  if(!data.empty())
    for(const auto& spair : symbols)
    {
      auto pos = std::begin(data);
      while(pos = std::search(pos, std::end(data), std::begin(spair.first), std::end(spair.first)),
            pos != std::end(data))
      {
         pos = data.erase(pos, pos + spair.first.size());
         pos = data.insert(pos, std::begin(spair.second), std::end(spair.second));
         pos = std::next(pos, spair.second.size());
      }
    }
}

template<typename T>
void replace_patterns(std::string& data, const T& patterns)
{
  try
  {
    if(!data.empty())
      for(const auto& spair : patterns)
        data = std::regex_replace(data,
                                  std::regex(spair.first, std::regex_constants::extended),
                                  spair.second,
                                  std::regex_constants::format_sed);
  }
  catch(const std::regex_error& err)
  {
    std::cerr << err.what() << std::endl;
  }
}

std::string fix_name(std::string name)
{
  name = std::regex_replace(name,
                            std::regex("_([[:alnum:]]|#n)", std::regex_constants::extended),
                            R"~(<em>\1</em>)~", std::regex_constants::format_sed);
  constexpr auto to_remove = "</em><em>"sv;
  std::size_t pos = std::string::npos;
  while(pos = name.find(to_remove), pos != std::string::npos)
    name.erase(pos, to_remove.size());

  return name;
}


void post_processing(std::list<instructions>& insn_blocks)
{
  for(auto& block : insn_blocks)
  {
    for(auto& instruction : block)
    {

      std::string clean_name = instruction.data<mnemonic_origin>();
      std::size_t underscore_count = std::count_if(std::begin(clean_name), std::end(clean_name), [](char c) -> bool { return c == '_'; });
      if(underscore_count)
      {
        std::remove_if(std::begin(clean_name), std::end(clean_name), [](char c) -> bool { return c == '_'; });
        clean_name.resize(clean_name.size() - underscore_count); // remove_if doesn't resize the container. RUDE!
      }
      instruction.data<name>() = fix_name(instruction.data<mnemonic_origin>());


      if(instruction.data<std::list<mode_details>>().empty())
        instruction.data<std::list<mode_details>>().push_back(instruction.data<mode_details>());

      for(auto& mdetails : instruction.data<std::list<mode_details>>())
      {
        mdetails.description_string = instruction.data<description>();
        mdetails.summary_string = instruction.data<summary>();
        mdetails.name_string = instruction.data<name>();
        if(mdetails.abstract_string.empty())
          mdetails.abstract_string = instruction.data<abstract>();
        modes_decoder(instruction.data<mnemonic>(), mdetails);

        replace_symbols(mdetails.abstract_string, typeable_symbols);
        replace_patterns(mdetails.abstract_string, typeable_patterns);
        replace_symbols(mdetails.abstract_string, long_accronyms);
        replace_patterns(mdetails.abstract_string, short_accronyms);
      }


      for(auto& flag : instruction.data<flags>())
        if(flag.index() == 2)
          replace_symbols(std::get<std::string>(flag), typeable_symbols);

      fix_name(instruction.data<mnemonic_origin>());
    }
  }
}

#if 0
void fix_format(std::string& format, std::size_t fixed_width)
{
  std::string::iterator pos;
  do
  {
    pos = std::find(std::begin(format), std::end(format), '\t');
    if(pos != std::end(format))
    {
      std::size_t tabpos = std::distance(std::begin(format), pos);
      format.erase(tabpos, 1);
      if(tabpos > fixed_width)
        tabpos -= format.rfind('\n', tabpos) + 1;
      format.insert(pos, fixed_width - tabpos, ' ');
    }
  } while (pos != std::end(format));
}



void replace_string(std::string& haystack,
                    const std::string& needle,
                    const std::string& replacement)
{

  for(auto pos = std::begin(haystack);
      pos = std::search(pos, std::end(haystack), std::begin(needle), std::end(needle)), pos != std::end(haystack);)
  {
    pos = haystack.erase(pos, std::next(pos, needle.size()));
    pos = haystack.insert(pos, std::begin(replacement), std::end(replacement));
    pos = std::next(pos, replacement.size());
  }
}

void no_rogue_angle_brackets(std::string& data)
{
  replace_string(data, "<<"s, "&lt;&lt;"s);
  replace_string(data, ">>"s, "&gt;&gt;"s);

  // match anything EXCEPT:
  // <a* (a/abbr HTML tag)
  // <b* (b/blockquote HTML tag)
  // <c* (code/cite HTML tag)
  // <s* (span HTML tag)
  // <v* (var HTML tag)
  // <em* (em HTML tag)
  data = std::regex_replace(data, std::regex("<([defghijklmnopqrtuwxyz[:space:]=][^m])", std::regex_constants::extended), "\\&lt;\\1", std::regex_constants::format_sed);

  // match anything EXCEPT:
  // ">  (HTML tag with attribute)
  // />  (self-closing HTML tag)
  // *n> (span HTML tag)
  // *r> (var/br/abbr HTML tag)
  // *e> (cite/blockquote HTML tag)
  // *p> (p HTML tag)
  // *b> (b HTML tag)
  data = std::regex_replace(data, std::regex("([^nrepb\"[:space:]/])>", std::regex_constants::extended), "\\1\\&gt;", std::regex_constants::format_sed);
}

bool trim_endlines(std::string& val)
{
  auto is_endline = [](unsigned char c) -> bool { return c == '\n'; };
  auto is_whitespace = [](unsigned char c) -> bool { return c == '\n' || c == ' ' || c == '\t'; };
  if(!val.empty())
  {
    auto pos = std::find_if_not(std::begin(val), std::end(val), is_endline); // forward search
    val.erase(std::begin(val), pos);
  }
  if(!val.empty())
  {
    auto pos = std::find_if_not(std::rbegin(val), std::rend(val), is_whitespace); // backwards search (and erase whitespace)
    val.erase(pos.base(), std::end(val));
  }
  return !val.empty();
}

void format_assembly(std::string& data)
{
  constexpr std::string_view preformatted = "PREFORMATTED";
  if(!data.compare(0, preformatted.size(), preformatted))
    data.erase(0, preformatted.size());
  else
  {
    data = std::regex_replace(data, std::regex("H'([[:xdigit:]]+)", std::regex_constants::extended), "0x\\1", std::regex_constants::format_sed);
    data = std::regex_replace(data, std::regex("R([[:digit:]]{1,2})", std::regex_constants::extended), "r\\1", std::regex_constants::format_sed);
    replace_string(data, "After execution:", "After execution: ");

    enum captures : uint8_t
    {
      address = 0,
      label,
      mnemonic,
      directive,
      operand,
      comment,
      spacing,
    };

    typedef std::array<std::string, 7> asm_parts_t;

    asm_parts_t regexes =
    {
      "^[[:space:]]*([[:xdigit:]]{4,8})[[:space:]]*" ,
      "^[[:space:]]*([[:alpha:]_][[:alnum:]_]*:)",
      "^[[:space:]]*([[:alpha:]][[:alnum:]/\\.]+)",
      "^[[:space:]]*(\\.[[:alpha:]][[:alnum:]\\.]+)",
      "^[[:space:]]*([-+,#_@”“\\(\\)[:alnum:]]+)",
      "^[[:space:]]*;(.*)$",
      "^[[:space:]\\.]*$",
    };

    std::smatch matches;
    std::list<asm_parts_t> lines;

    auto pos = std::cbegin(data);
    while (pos != std::cend(data))
    {
      asm_parts_t line;
      auto line_pos = pos;
      auto eol = std::find(pos, std::cend(data), '\n');
      if(line_pos != eol)
      {
        for(uint8_t part = address; part <= spacing; ++part)
        {
          std::regex_search(line_pos, eol, matches, std::regex(regexes[part], std::regex_constants::extended), std::regex_constants::format_sed);
          if(!matches.empty())
          {
            line[part] = matches[1];
            line_pos = std::next(line_pos, matches[0].str().size());
          }
        }
      }
      if(!line[comment].empty())
        line[comment].insert(0, "! ");
      if(std::end(line) != std::find_if(std::begin(line), std::end(line), [](const std::string& str) -> bool { return !str.empty(); })) // if line is not empty
        lines.push_back(line);
      pos = std::next(eol);
    }

    data.clear(); // wipe existing string

    std::array<std::size_t, 7> indent = { { 0 } };

    for(const auto& line : lines)
      for(uint8_t part = address; part <= spacing; ++part)
        if(!line[part].empty())
          indent[part] = std::max(indent[part], line[part].size() + 1);

    for(auto& line : lines)
      if(!line[mnemonic].empty())
        std::transform(std::cbegin(line[mnemonic]),
                       std::cend(line[mnemonic]),
                       std::begin(line[mnemonic]),
                       [](unsigned char c) { return std::tolower(c); });

    indent[mnemonic] = indent[directive] = std::max(indent[mnemonic], indent[directive]);


    for(const auto& line : lines)
    {
      for(uint8_t part = address; part <= spacing; ++part)
      {
        if(!line[part].empty())
          data.append(line[part])
              .append(indent[part] - line[part].size(), ' ');
        else if(part != (line[directive].empty() ? directive : mnemonic)) // if directive then ignore mnemonic spacing.  if not directive then ignore directive spacing.
            data.append(indent[part], ' ');
      }
      data.push_back('\n');
    }
  }
}

void format_exceptions(std::string& data)
{
  for(auto pos = data.find('\n'); pos != std::string::npos; pos = data.find('\n', pos + "</var>\n  <var>"sv.size()))
    data.replace(pos, 1, "</var>\n  <var>");
  data.insert(0, "\n  <var>").append("</var>\n");
}

void format_code(std::string& data)
{
  constexpr auto operand_type = [](char c) -> char { return c == '1' ? '0' : c; };
  std::string result;
  uint8_t count = 0;
  char current = '\0';

  for(auto pos = std::begin(data); pos != std::end(data); pos = std::next(pos))
  {
    result.push_back(*pos);
    ++count;

    current = *pos;

    if(std::next(pos) == std::end(data) ||
       operand_type(current) != operand_type(*std::next(pos)))
    {
      auto total = std::count(std::begin(data), std::end(data), current); // find the total number of occurances in the whole string
      std::string tag = "<var title=\"";
      switch(operand_type(current))
      {
        case '0': tag.append("Opcode Identifier"); break;
        case '*': tag.append("Ignored"); break;
        case 'm': tag.append("Source Register"); break;
        case 'n': tag.append("Destination Register"); break;
        case 'i': tag.append("Unsigned Immediate Data"); break;
        case 's': tag.append("Signed Immediate Data"); break;
        case 'd': tag.append("Displacement"); break;

        case 'A': tag.append("A"); break;
        case 'D': tag.append("D"); break;
        case 'e': tag.append("Multiplier Source Register 1 (A1, X0, X1, Y0)"); break;
        case 'f': tag.append("Multiplier Source Register 2 (A1, X0, Y0, Y1)"); break;
        case 'g': tag.append("Multiplier Destination Register (A0, A1, MEM0, MEM1)"); break;
        case 'x': tag.append("ALU Source Register 1 (A0, A1, X0, X1)"); break;
        case 'y': tag.append("ALU Source Register 2 (MEM0, MEM1, Y0, Y1)"); break;
        case 'u': tag.append("ALU Destination Register (A0, A1, X0, Y0)"); break;
        case 'z': tag.append("ALU Destination Register (A0, A1, MEM0, MEM1, X0, X1, Y0, Y1)"); break;

        default: throw("unknown operand type: '"s + current + "'");
      }

      switch(current)
      {
        case 'm':
        case 'n':
          tag.append(" (R0 - R")
           .append(std::to_string((1 << total) - 1))
           .push_back(')');
          break;
        case 'i':
        case 's':
        case 'd':
          tag.append(" (")
           .append(std::to_string(total))
           .append(" bits)");
          break;
        case 'e':
        case 'f':
        case 'g':
        case 'x':
        case 'y':
        case 'u':
          if(total != 2)
            throw("Expected two bits for '"s + current + "' register type. Found: " + std::to_string(total));
          break;

        case 'z':
          if(total != 4)
            throw("Expected four bits for 'z' register type. Found: "s + std::to_string(total));
          break;
      }
      tag.append("\">");

      result.insert(std::prev(std::end(result), count), std::begin(tag), std::end(tag));
      result.append("</var>");
      count = 0;
    }
  }

  data.assign(result);
}

void post_processing(std::list<insns>& insn_blocks)
{
  struct instruction_info_t
  {
    std::string mnemonic_regex;
    std::string name;
    std::string classification;
    std::list<environment_t> environments;
    std::list<citation_t> citations;
  };



  for(instruction_info_t& info : name_data)
  {
    std::transform(std::begin(info.mnemonic_regex), std::end(info.mnemonic_regex), std::begin(info.mnemonic_regex),                   [](char c){ return std::tolower(c); }); // convert to lowercase
    info.mnemonic_regex.insert(0, 1, '^'); // add regex string start
    info.mnemonic_regex.append("[\\S]*"); // add "not whitespace" matching to end

    for(auto& cite : info.citations)
      cite.instruction_sets = documents[cite.source].instruction_sets; // fix default value for citations
  }

  for(insns& block : insn_blocks)
  {
    for(insn& instruction : block)
    {
      auto& fmt = instruction.data<format>();
      auto& n = instruction.data<name>();
      auto& e = instruction.data<environments>();
      for(const instruction_info_t& info : name_data)
      {
        try
        {
          std::smatch match;
          if(std::regex_search(fmt, match, std::regex(info.mnemonic_regex, std::regex_constants::extended)) &&
             (n.empty() || n == info.name) &&
             e == environments { info.environments })
          {
            if(n.empty())
            {
              n = { info.name };
              if(match.size() >= 1)
              {
                std::string replacement;
                for(char c : match[1].str())
                {
                  replacement.push_back('_');
                  replacement.push_back(c);
                }
                replace_string(n, "$"s, replacement);
              }
            }

            instruction.data<mnemonic>() = { match.str() };
            instruction.data<citations>() = { info.citations };
            instruction.data<classification>() = { info.classification };

            break;
          }
        }
        catch(const std::regex_error& err)
        {
          std::cerr << err.what() << std::endl;
        }
      }
    }
  }

  auto fix_images = [](std::string& data, const std::string& title)
    { replace_string(data, "<img src=", "<img alt=\""s + title + "\" class=\"image_filter\" src="s); };

  for(insns& block : insn_blocks)
  {
    for(insn& instruction : block)
    {
      for(std::size_t pos = 0; pos < 8; ++pos)
      {
        isa current_isa = isa(1 << pos);
        isa i_set = instruction.data<isa>();
        const isa_property& i = instruction.data<issue>();
        const isa_property& l = instruction.data<latency>();
        if(uint16_t(i_set) & uint16_t(current_isa) ||
           !i.operator[](current_isa).empty() ||
           !l.operator[](current_isa).empty())
        {
          if(! ((uint16_t(i_set) & uint16_t(current_isa))) ||
             i.operator[](current_isa).empty() ||
             l.operator[](current_isa).empty())
          {
            std::cerr << instruction.data<opcode>()
                      << " - current isa: " << std::hex << std::setw(3) << current_isa
                      << " - isa: " << std::hex << std::setw(3) << (i_set & current_isa)
                      << " - issue: '" << i.operator[](current_isa) << "'"
                      << " - latency: '" << l.operator[](current_isa) << "'"
                      << std::endl;
          }
        }
      }

      std::string clean_name = instruction.data<name>();
      std::size_t underscore_count = std::count_if(std::begin(clean_name), std::end(clean_name), [](char c) -> bool { return c == '_'; });
      if(underscore_count)
      {
        std::remove_if(std::begin(clean_name), std::end(clean_name), [](char c) -> bool { return c == '_'; });
        clean_name.resize(clean_name.size() - underscore_count); // remove_if doesn't resize the container. RUDE!
      }

      fix_format(instruction.data<format>(), 10); // replace tabs with spaces
      fix_images(instruction.data<note>(), clean_name);
      fix_images(instruction.data<description>(), clean_name);

      replace_symbols(instruction.data<abstract>(), typeable_symbols);
      replace_patterns(instruction.data<abstract>(), typeable_patterns);
      replace_symbols(instruction.data<brief>(), unicode_symbols);
      replace_symbols(instruction.data<flags>(), typeable_symbols);

      replace_symbols(instruction.data<description>(), long_accronyms);
      replace_patterns(instruction.data<description>(), short_accronyms);

      replace_symbols(instruction.data<note>(), long_accronyms);
      replace_patterns(instruction.data<note>(), short_accronyms);

      format_code(instruction.data<opcode>());
      format_assembly(instruction.data<example>());

      if(trim_endlines(instruction.data<exceptions>()))
        format_exceptions(instruction.data<exceptions>());

      no_rogue_angle_brackets(instruction.data<brief>());
      no_rogue_angle_brackets(instruction.data<operation>());

      trim_endlines(instruction.data<brief>());
      trim_endlines(instruction.data<description>());
      trim_endlines(instruction.data<note>());
      trim_endlines(instruction.data<operation>());
      trim_endlines(instruction.data<example>());

      fix_name(instruction.data<name>());
    }
  }
}
#endif
