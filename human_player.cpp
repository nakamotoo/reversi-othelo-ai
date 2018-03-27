#include <iostream>
#include <string>

#include "human_player.hpp"

namespace reversi
{

std::tuple<int, int> HumanPlayer::thinkNextMove(const Board& board)
{
	while (true) {
		std::cout << "Where? ";
		std::string position_str;
		std::cin >> position_str;
		if (position_str.size() != 2) {
			std::cout << "invalid format \"" << position_str << "\"\n"
			          << std::endl;
			continue;
		}
		char col = position_str.at(0);
		char row = position_str.at(1);
		// assume ascii character code
		if (col < 'a' || col > 'h' || row < '1' || row > '8') {
			std::cout << "invalid format \"" << position_str << "\"\n"
			          << std::endl;
			continue;
		}
		int x = col - 'a';
		int y = row - '1';
		if (board.isLegalMove(x, y, getSide())) {
			return std::make_tuple(x, y);
		}
		std::cout << position_str << " is not a legal move!\n"
		          << std::endl;
	}
}

}  // namespace reversi
