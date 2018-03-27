#pragma once

#include "player.hpp"

namespace reversi
{

class HumanPlayer : public Player
{
public:
	HumanPlayer(Side side) : Player(side) {}

	virtual std::tuple<int, int> thinkNextMove(const Board& board) override;
};

}  // namespace reversi
