#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "board.hpp"
#include "human_player.hpp"
#include "option_parser.hpp"
#include "sample_computer_player.hpp"

int main(int argc, char* argv[])
{
	using namespace reversi;

	const std::unordered_map<std::string, std::function<std::unique_ptr<Player>(Side)>> player_types = {
	    {"human", [](Side side) { return std::make_unique<HumanPlayer>(side); }},
	    {"sample", [](Side side) { return std::make_unique<SampleComputerPlayer>(side); }},
	};

	auto command_line_params = parseCommandLineOptions(argc, argv, player_types);

	Board board;

	for (Side turn = Side::BLACK;; turn = getOpponentSide(turn)) {
		std::cout << board << "\n"
		          << std::endl;

		if (board.count(CellState::EMPTY) == 0) {
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

		auto& turn_player = turn == Side::BLACK
		                        ? *command_line_params.black_player
		                        : *command_line_params.white_player;

		int x, y;
		std::tie(x, y) = turn_player.thinkNextMove(board);
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
