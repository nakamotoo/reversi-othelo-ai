#include <cstdlib>

#include "option_parser.hpp"

namespace reversi
{

namespace
{

bool findHelpOption(int argc, char* argv[])
{
	using namespace std::literals::string_literals;
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == "-h"s || argv[i] == "--help"s) {
			return true;
		}
	}
	return false;
}

void printSimpleUsage(std::ostream& os, const char* program_name)
{
	os << "usage: " << program_name << " [-h] BLACK_PLYAER_TYPE WHITE_PLAYER_TYPE" << std::endl;
}

std::unique_ptr<Player> makePlayerFromType(const TypeStrToPlayerMap& player_types, const std::string& type, const Side side)
{
	auto it = player_types.find(type);
	if (it == player_types.end()) {
		std::cerr << "\'" << type << "\' is not a valid player type!" << std::endl;
		std::exit(1);
	}
	return it->second(side);
}

}  // namespace

CommandLineParams parseCommandLineOptions(int argc, char* argv[], const TypeStrToPlayerMap& player_types)
{
	if (findHelpOption(argc, argv)) {
		printSimpleUsage(std::cout, argv[0]);
		std::cout << "\n"
		             "  [BLACK/WHITE]_PLAYER_TYPE      Player Style.\n"
		             "                                 Choose from:";
		for (auto&& type : player_types) {
			std::cout << " " << type.first;
		}
		std::cout << std::endl;
		std::exit(1);
	}
	if (argc < 3) {
		std::cerr << "too few arguments!" << std::endl;
		printSimpleUsage(std::cerr, argv[0]);
		std::cerr << "Try '" << argv[0] << " -h' for more information." << std::endl;
		std::exit(1);
	}
	if (argc > 3) {
		std::cerr << "too many arguments!" << std::endl;
		printSimpleUsage(std::cerr, argv[0]);
		std::cerr << "Try '" << argv[0] << " -h' for more information." << std::endl;
		std::exit(1);
	}
	CommandLineParams params;
	params.black_player = makePlayerFromType(player_types, argv[1], Side::BLACK);
	params.white_player = makePlayerFromType(player_types, argv[2], Side::WHITE);
	return params;
}

}  // namespace reversi
