#pragma once

#include "player.hpp"

namespace reversi
{

class HumanPlayer : public Player
{
public:
	HumanPlayer(Disk own_disk) : Player(own_disk) {}

	virtual std::tuple<int, int> thinkNextMove(const Board& board) override;
};

}  // namespace reversi
