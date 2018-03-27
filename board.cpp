#include <cassert>

#include "board.hpp"

namespace reversi
{

Side getOpponentSide(const Side side)
{
	switch (side) {
	case Side::BLACK:
		return Side::WHITE;
	case Side::WHITE:
		return Side::BLACK;
	default:
		assert(false);
		return side;
	}
}

std::ostream& operator<<(std::ostream& os, const Side side)
{
	switch (side) {
	case Side::BLACK:
		os << "black";
		break;
	case Side::WHITE:
		os << "white";
		break;
	default:
		assert(false);
	}
	return os;
}

CellState getOwnState(Side side)
{
	switch (side) {
	case Side::BLACK:
		return CellState::BLACK;
	case Side::WHITE:
		return CellState::WHITE;
	default:
		assert(false);
		return CellState::EMPTY;
	}
}

namespace
{

constexpr std::array<std::tuple<int, int>, 8> DIRECTIONS = {{
    std::make_tuple(-1, -1),
    std::make_tuple(-1, 0),
    std::make_tuple(-1, 1),
    std::make_tuple(0, -1),
    std::make_tuple(0, 1),
    std::make_tuple(1, -1),
    std::make_tuple(1, 0),
    std::make_tuple(1, 1),
}};

int countBoundedDisks(const Board& board, int x, int y, const Side side, const std::tuple<int, int> direction)
{
	assert(board.get(x, y) == CellState::EMPTY);
	int num_bounded_disks = 0;
	while (true) {
		x += std::get<0>(direction);
		y += std::get<1>(direction);
		if (x < 0 || x >= Board::WIDTH || y < 0 || y >= Board::HEIGHT) {
			return 0;
		}
		if (board.get(x, y) == CellState::EMPTY) {
			return 0;
		}
		if (board.get(x, y) == getOwnState(side)) {
			break;
		}
		assert(board.get(x, y) == getOwnState(getOpponentSide(side)));
		++num_bounded_disks;
	}
	return num_bounded_disks;
}

bool canFlip(const Board& board, const int x, const int y, const Side side, const std::tuple<int, int> direction)
{
	return countBoundedDisks(board, x, y, side, direction) > 0;
}

}  // namespace

Board::Board() : m_states()
{
	set(3, 3, CellState::WHITE);
	set(3, 4, CellState::BLACK);
	set(4, 3, CellState::BLACK);
	set(4, 4, CellState::WHITE);
}

CellState Board::get(const int x, const int y) const
{
	assert(0 <= x && x < WIDTH);
	assert(0 <= y && y < HEIGHT);
	return m_states[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
}

void Board::set(const int x, const int y, const CellState state)
{
	assert(0 <= x && x < WIDTH);
	assert(0 <= y && y < HEIGHT);
	m_states[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = state;
}

bool Board::isLegalMove(const int x, const int y, const Side side) const
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return false;
	}
	if (get(x, y) != CellState::EMPTY) {
		return false;
	}
	for (auto&& direction : DIRECTIONS) {
		if (canFlip(*this, x, y, side, direction)) {
			return true;
		}
	}
	return false;
}

std::vector<std::tuple<int, int>> Board::getAllLegalMoves(const Side side) const
{
	std::vector<std::tuple<int, int>> legal_moves;
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (isLegalMove(x, y, side)) {
				legal_moves.emplace_back(x, y);
			}
		}
	}
	return legal_moves;
}

void Board::placeDisk(const int x, const int y, const Side side)
{
	assert(isLegalMove(x, y, side));
	for (auto&& direction : DIRECTIONS) {
		int num_bounded_disks = countBoundedDisks(*this, x, y, side, direction);
		int flip_x = x;
		int flip_y = y;
		for (int i = 0; i < num_bounded_disks; ++i) {
			flip_x += std::get<0>(direction);
			flip_y += std::get<1>(direction);
			set(flip_x, flip_y, getOwnState(side));
		}
	}
	set(x, y, getOwnState(side));
}

int Board::count(const CellState target) const
{
	int count = 0;
	for (auto&& row : m_states) {
		for (auto&& disk : row) {
			if (disk == target) {
				++count;
			}
		}
	}
	return count;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
	os << "  a b c d e f g h";
	for (int y = 0; y < Board::HEIGHT; ++y) {
		os << '\n'
		   << (y + 1);
		for (int x = 0; x < Board::WIDTH; ++x) {
			os << ' ';
			CellState state = board.get(x, y);
			switch (state) {
			case CellState::EMPTY:
				os << '-';
				break;
			case CellState::BLACK:
				os << 'B';
				break;
			case CellState::WHITE:
				os << 'W';
				break;
			}
		}
	}
	return os;
}

}  // namespace reversi
