#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "board.hpp"
#include "human_player.hpp"
#include "sample_computer_player.hpp"

int main(int argc, char* argv[])
{
	using namespace reversi;

	std::unordered_map<std::string, std::function<std::unique_ptr<Player>(Disk)>> player_types = {
	    {"human", [](Disk disk) { return std::make_unique<HumanPlayer>(disk); }},
	    {"sample", [](Disk disk) { return std::make_unique<SampleComputerPlayer>(disk); }},
	};

	auto make_player_from_arg = [&](Disk disk, const std::string& arg) {
		auto it = player_types.find(arg);
		if (it == player_types.end()) {
			std::cerr << arg << " is not a valid type!" << std::endl;
			std::exit(1);
		}
		return it->second(disk);
	};
	auto make_player = [&](Disk disk) {
		while (true) {
			std::cout << "Input the " << disk << " player type > ";
			std::string type;
			std::cin >> type;
			auto it = player_types.find(type);
			if (it == player_types.end()) {
				std::cout << type << " is not a valid type!" << std::endl;
				continue;
			}
			return it->second(disk);
		}
	};

	const auto black_player
	    = argc >= 3 ? make_player_from_arg(Disk::BLACK, argv[1]) : make_player(Disk::BLACK);
	const auto white_player
	    = argc >= 3 ? make_player_from_arg(Disk::WHITE, argv[2]) : make_player(Disk::WHITE);

	Board board;

	for (Disk turn = Disk::BLACK;; turn = flip(turn)) {
		std::cout << board << "\n"
		          << std::endl;

		if (board.countDisks(Disk::NONE) == 0) {
			// no empty square
			break;
		}
		auto legal_moves = board.getAllLegalMoves(turn);
		if (legal_moves.empty()) {
			// pass
			std::cout << "turn = " << turn << ", move = Pass\n"
			          << std::endl;
			continue;
		}

		auto& player = turn == Disk::BLACK ? *black_player : *white_player;

		auto[x, y] = player.thinkNextMove(board);
		if (!board.isLegalMove(x, y, turn)) {
			std::cout << "turn = " << turn << ", illegal move (x, y) = (" << x << ", " << y << ")\n"
			          << std::endl;
			break;
		}
		std::cout << "turn = " << turn << ", move = "
		          << static_cast<char>('a' + x) << static_cast<char>('1' + y) << "\n"
		          << std::endl;

		board.placeDisk(x, y, turn);
	}

	return 0;
}
