#include <math.h>
#include <vector>

#include "sample_computer_player.hpp"

namespace reversi {

CellPosition SampleComputerPlayer::thinkNextMove(const Board &board) {
  // choose the first legal move
  std::vector<CellPosition> legal_moves = board.getAllLegalMoves(getSide());
  return legal_moves[rand() % legal_moves.size()];
}

} // namespace reversi