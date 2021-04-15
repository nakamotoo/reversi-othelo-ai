#pragma once
#include "board.hpp"
#include "player.hpp"
#include <cstdlib>

namespace reversi {

class MinMaxPlayer : public Player {
public:
  MinMaxPlayer(Side side) : Player(side) {}

  virtual CellPosition thinkNextMove(const Board &board) override;
  std::tuple<int, CellPosition> max_node(const Board &board, Side side,
                                         int depth, int max_depth, int alpha,
                                         int beta, CellPosition first_step);
  std::tuple<int, CellPosition> min_node(const Board &board, Side side,
                                         int depth, int max_depth, int alpha,
                                         int beta, CellPosition first_step);
  std::tuple<int, CellPosition> negamax(const Board &board, Side side,
                                        int depth, int max_depth, int alpha,
                                        int beta, CellPosition first_step,
                                        Side init_side);
};

} // namespace reversi
