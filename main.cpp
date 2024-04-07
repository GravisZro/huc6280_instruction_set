#include <format>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string_view>
#include <cstring>
#include <regex>
#include <algorithm>

#include "build_instructions.h"
#include "post_processing.h"

using namespace std::literals;
using namespace std::string_view_literals;
// ----------------------------------------------------------------------------

extern char _binary_page_header_txt_start[];
extern char _binary_page_header_txt_end[];

static isa_property display_name = isa_property
{
  NMOS6502, "NMOS6502",
  WDC65C02, "WDC65C02",
  HuC6280, "HuC6280",
};

// ----------------------------------------------------------------------------

std::string fix_id(std::string data)
{
  data = std::regex_replace(data, std::regex("<var[^>]+>([^<]+)</var>", std::regex_constants::extended),
                            "\\1", std::regex_constants::format_sed);

  data = std::regex_replace(data, std::regex("[[:space:]]", std::regex_constants::extended),
                            "", std::regex_constants::format_sed);
  data = "code" + data;
  return data;
}

// ----------------------------------------------------------------------------

std::string build_flags(const flags& farr)
{
  constexpr const char* const flag_ids = "NVTBDIZC";

  std::string rval;
  int idx = 0;
  for(auto& val : farr)
  {
    switch(val.index())
    {
    case 0:
      rval.push_back('-');
      break;
    case 1:
      rval.push_back(std::get<int>(val) ? '1' : '0');
      break;
    case 2:
      rval.push_back(flag_ids[idx]);
    }
    idx++;
  }
  return rval;
}

std::string build_span_section (std::string_view word_title, std::string tag_title, std::string val)
{
  std::string rval;
  if(!val.empty())
  {
    rval.append("<span title=\"section\">")
        .append(word_title)
        .append("</span>\n")
        .append("<span title=\"")
        .append(tag_title)
        .append("\">")
        .append(val)
        .append("</span>\n");
  }
  return rval;
}


std::string regex_property_list(const isa_property& prop, const std::string& newtext)
{
  std::string r;
  for(const auto& val : prop)
    if(!val.empty())
      r += std::regex_replace(std::string(val), std::regex(std::begin(val), std::end(val), std::regex_constants::basic), newtext, std::regex_constants::format_sed);
  return r;
}


std::string build_isa_list (const mode_details& md)
{
  static isa_property names = isa_property
  {
    NMOS6502, "NMOS6502",
    WDC65C02, "WDC65C02",
    HuC6280, "HuC6280",
  };
  constexpr static const std::array<isa, isa_count> list = { NMOS6502,
                                                             WDC65C02,
                                                             HuC6280 };
  auto func = [](bool match, const std::string_view& prop) -> std::string
  {
    if(match)
      return std::string(" ").append(prop);
    return std::string();
  };
  std::string r;
  for(std::size_t pos = 0; pos < list.size(); ++pos)
    r += func(md.cpus & list[pos], names[list[pos]]);
  return r;
}

std::string build_isa_tagged_property_list (const mode_details& md, const isa_property& p)
{
  constexpr static const std::array<isa, isa_count> list = { NMOS6502,
                                                             WDC65C02,
                                                             HuC6280 };
  auto func = [](bool match, const std::string_view& prop) -> std::string
  {
    if(match && !prop.empty())
      return std::string("<var>").append(prop).append("</var>");
    return "<var></var>";
  };
  std::string r;
  for(std::size_t pos = 0; pos < list.size(); ++pos)
    r += func(md.cpus & list[pos], p[list[pos]]);
  return r;
}


int main (int argc, char** argv)
{
  std::cout << std::unitbuf; // enable automatic flushing
  std::cerr << std::unitbuf; // enable automatic flushing

  //std::cout << "data: " << _binary_page_header_txt_size << std::endl;

  std::ofstream fileOut;

  if(argc == 2)
  {
    std::cout << "output file: " << argv[1] << std::endl;
    fileOut.open(argv[1]);
    std::cout.rdbuf(fileOut.rdbuf());
  }

  std::string page_header(_binary_page_header_txt_start);
  /*
  {
    std::cerr << QUOTE(PROJECT_DIR) << std::endl;
    std::filesystem::current_path(QUOTE(PROJECT_DIR));
    std::ifstream page_header_file("page_header.txt");
    page_header = std::string((std::istreambuf_iterator<char>(page_header_file)), (std::istreambuf_iterator<char>())); // copy file contents
    page_header_file.close();;
  }
  */
  page_header.replace(page_header.find("__DATE__"), sizeof(R"(__DATE__)"), __DATE__);
  page_header.replace(page_header.find("__TIME__"), sizeof(R"(__TIME__)"), __TIME__);

  std::cout << page_header

  << regex_property_list(display_name, "\n  <input type=\"checkbox\" id=\"cb_&\" name=\"&\" checked /><label for=\"cb_&\">&</label>")

  << R"html(<br />
    <span id="table_header" class="summary)html" << regex_property_list(display_name, " &") << R"html(">
    <span>Compatibilty</span>
    <span>PCEAS Syntax</span>
    <span>Abstract</span>
    <span>Machine Code</span>
    <span>Status Flags</span>
    <span>Addressing Mode</span>
  </span>)html";

  try
  {
    std::list<instructions> insn_blocks;
    build_insn_blocks(insn_blocks);
    post_processing(insn_blocks);

    int id = 0;
    for (const auto& block : insn_blocks)
    {
      std::cout << "<span class=\"section_title\">" << block.section_title << "</span>" << std::endl;

      for (const auto& i : block)
      {
        for (const auto& md : i.data<std::list<mode_details>>())
        {
          std::cout << "<input name=\"instruction\" type=\"radio\" id=\"row" << id << "\" />" << std::endl;
          std::cout
              << "<label class=\"summary" << build_isa_list(md) << "\" for=\"row" << id << "\">" << std::endl
              << "<span class=\"cpu_grid\"><var></var><var></var><var></var></span>" << std::endl
              << "<span>" << md.pceas_syntax_string << "</span>" << std::endl
              << "<span>" << md.abstract_string << "</span>" << std::endl
              << "<span id=\"" << fix_id(md.machine) << "\" class=\"colorized\">" << md.machine << "</span>" << std::endl
              << "<span>" << build_flags(i.data<flags>()) << "</span>" << std::endl
              << "<span>" << md.address_mode_string << "</span>" << std::endl
  //            << "<span class=\"cycle_grid\">" << build_isa_tagged_property_list (i, i.data<group>()) << "</span>" << std::endl
  //            << "<span class=\"cycle_grid\">" << build_isa_tagged_property_list (i, i.data<issue>()) << "</span>" << std::endl
  //            << "<span class=\"cycle_grid\">" << build_isa_tagged_property_list (i, i.data<latency>()) << "</span>" << std::endl
              << "<span class=\"details\">" << std::endl;

  //        std::cout << build_environments (i.data<environments>());
  //        std::cout << build_citations (i.data<citations>());
          std::cout << build_span_section (md.name_string, "summary", md.description_string);
          std::cout << build_span_section ("Note", "note", i.data<note>());
  //        std::cout << build_span_section ("Operation", "operation", i.data<operation>());
  //        std::cout << build_span_section ("Example", "assembly", i.data<example>());
  //        std::cout << build_span_section ("Possible Exceptions", "list", i.data<exceptions>());

          std::cout << "</span>" << std::endl // close "details"
                    << "</label>" << std::endl;
          ++id;
        }
      }
    }

    std::cout << "</body>" << std::endl
              << "</html>" << std::endl;
  }
  catch (std::string message)
  {
    std::cerr << "exception caught: " << message << std::endl;
  }

  if(fileOut.is_open())
    fileOut.close();

  return 0;
}

