#pragma once

#include "player.hpp"

namespace reversi
{

class SampleComputerPlayer : public Player
{
public:
	SampleComputerPlayer(Disk own_disk) : Player(own_disk) {}

	virtual std::tuple<int, int> thinkNextMove(const Board& board) override;
};

}  // namespace reversi
