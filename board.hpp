#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

namespace reversi
{

enum class Disk : std::uint8_t
{
	NONE,
	BLACK,
	WHITE
};

Disk flip(Disk disk);

std::ostream& operator<<(std::ostream& os, Disk disk);

class Board
{
public:
	static constexpr int WIDTH = 8;
	static constexpr int HEIGHT = 8;

	Board();

	Disk get(int x, int y) const;
	void set(int x, int y, Disk disk);

	bool isLegalMove(int x, int y, Disk disk) const;
	std::vector<std::tuple<int, int>> getAllLegalMoves(Disk disk) const;

	void placeDisk(int x, int y, Disk disk);

	int countDisks(Disk target) const;

private:
	std::array<std::array<Disk, WIDTH>, HEIGHT> m_states;
};

std::ostream& operator<<(std::ostream& os, const Board& board);

}  // namespace reversi
