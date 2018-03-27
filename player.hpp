#pragma once

#include <tuple>

#include "board.hpp"

namespace reversi
{

class Player
{
public:
	Player(Disk own_disk) : m_own_disk(own_disk) {}
	virtual ~Player() = default;

	Disk getOwnDisk() const
	{
		return m_own_disk;
	}

	virtual std::tuple<int, int> thinkNextMove(const Board& board) = 0;

private:
	Disk m_own_disk;
};

}  // namespace reversi
