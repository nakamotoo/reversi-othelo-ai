#pragma once

#include "player.hpp"

namespace reversi
{

class SampleComputerPlayer : public Player
{
public:
	SampleComputerPlayer(Side side) : Player(side) {}

	virtual std::tuple<int, int> thinkNextMove(const Board& board) override;
};

}  // namespace reversi
