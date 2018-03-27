#include "sample_computer_player.hpp"

namespace reversi
{

std::tuple<int, int> SampleComputerPlayer::thinkNextMove(const Board& board)
{
	// choose the first legal move
	return board.getAllLegalMoves(getOwnDisk()).front();
}

}  // namespace reversi
