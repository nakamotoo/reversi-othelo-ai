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

	std::unordered_map<std::string, std::function<std::unique_ptr<Player>(Side)>> player_types = {
	    {"human", [](Side side) { return std::make_unique<HumanPlayer>(side); }},
	    {"sample", [](Side side) { return std::make_unique<SampleComputerPlayer>(side); }},
	};

	auto make_player_from_arg = [&](Side side, const std::string& arg) {
		auto it = player_types.find(arg);
		if (it == player_types.end()) {
			std::cerr << arg << " is not a valid type!" << std::endl;
			std::exit(1);
		}
		return it->second(side);
	};
	auto make_player = [&](Side side) {
		while (true) {
			std::cout << "Input the " << side << " player type > ";
			std::string type;
			std::cin >> type;
			auto it = player_types.find(type);
			if (it == player_types.end()) {
				std::cout << type << " is not a valid type!" << std::endl;
				continue;
			}
			return it->second(side);
		}
	};

	const auto black_player
	    = argc >= 3 ? make_player_from_arg(Side::BLACK, argv[1]) : make_player(Side::BLACK);
	const auto white_player
	    = argc >= 3 ? make_player_from_arg(Side::WHITE, argv[2]) : make_player(Side::WHITE);

	Board board;

	for (Side turn = Side::BLACK;; turn = getOpponentSide(turn)) {
		std::cout << board << "\n"
		          << std::endl;

		if (board.count(CellState::BLANK) == 0) {
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

		auto& player = turn == Side::BLACK ? *black_player : *white_player;

		int x, y;
		std::tie(x, y) = player.thinkNextMove(board);
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
