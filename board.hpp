#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

namespace reversi
{

enum class Side : std::uint8_t
{
	BLACK,
	WHITE
};

Side getOpponentSide(Side side);

std::ostream& operator<<(std::ostream& os, Side side);

enum class CellState : std::uint8_t
{
	BLANK,
	BLACK,
	WHITE
};

CellState getOwnState(Side side);

class Board
{
public:
	static constexpr int WIDTH = 8;
	static constexpr int HEIGHT = 8;

	Board();

	CellState get(int x, int y) const;
	void set(int x, int y, CellState state);

	bool isLegalMove(int x, int y, Side side) const;
	std::vector<std::tuple<int, int>> getAllLegalMoves(Side side) const;

	void placeDisk(int x, int y, Side side);

	int count(CellState target) const;

private:
	std::array<std::array<CellState, WIDTH>, HEIGHT> m_states;
};

std::ostream& operator<<(std::ostream& os, const Board& board);

}  // namespace reversi
