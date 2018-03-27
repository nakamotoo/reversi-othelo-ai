#include <cassert>

#include "board.hpp"

namespace reversi
{

Disk flip(const Disk disk)
{
	switch (disk) {
	case Disk::BLACK:
		return Disk::WHITE;
	case Disk::WHITE:
		return Disk::BLACK;
	default:
		assert(false);
		return disk;
	}
}

std::ostream& operator<<(std::ostream& os, const Disk disk)
{
	switch (disk) {
	case Disk::NONE:
		os << "none";
		break;
	case Disk::BLACK:
		os << "black";
		break;
	case Disk::WHITE:
		os << "white";
		break;
	default:
		assert(false);
	}
	return os;
}

namespace
{

constexpr std::array<std::tuple<int, int>, 8> DIRECTIONS = {
    std::make_tuple(-1, -1),
    std::make_tuple(-1, 0),
    std::make_tuple(-1, 1),
    std::make_tuple(0, -1),
    std::make_tuple(0, 1),
    std::make_tuple(1, -1),
    std::make_tuple(1, 0),
    std::make_tuple(1, 1),
};

int countBoundedDisks(const Board& board, int x, int y, const Disk disk, const std::tuple<int, int> direction)
{
	assert(board.get(x, y) == Disk::NONE);
	int num_bounded_disks = 0;
	while (true) {
		x += std::get<0>(direction);
		y += std::get<1>(direction);
		if (x < 0 || x >= Board::WIDTH || y < 0 || y >= Board::HEIGHT) {
			return 0;
		}
		if (board.get(x, y) == Disk::NONE) {
			return 0;
		}
		if (board.get(x, y) == disk) {
			break;
		}
		assert(board.get(x, y) == flip(disk));
		++num_bounded_disks;
	}
	return num_bounded_disks;
}

bool canFlip(const Board& board, const int x, const int y, const Disk disk, const std::tuple<int, int> direction)
{
	return countBoundedDisks(board, x, y, disk, direction) > 0;
}

}  // namespace

Board::Board() : m_states()
{
	set(3, 3, Disk::WHITE);
	set(3, 4, Disk::BLACK);
	set(4, 3, Disk::BLACK);
	set(4, 4, Disk::WHITE);
}

Disk Board::get(const int x, const int y) const
{
	assert(0 <= x && x < WIDTH);
	assert(0 <= y && y < HEIGHT);
	return m_states[y][x];
}

void Board::set(const int x, const int y, const Disk disk)
{
	assert(0 <= x && x < WIDTH);
	assert(0 <= y && y < HEIGHT);
	assert(disk == Disk::NONE || disk == Disk::BLACK || disk == Disk::WHITE);
	m_states[y][x] = disk;
}

bool Board::isLegalMove(const int x, const int y, const Disk disk) const
{
	assert(disk == Disk::BLACK || disk == Disk::WHITE);
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return false;
	}
	if (get(x, y) != Disk::NONE) {
		return false;
	}
	for (auto&& direction : DIRECTIONS) {
		if (canFlip(*this, x, y, disk, direction)) {
			return true;
		}
	}
	return false;
}

std::vector<std::tuple<int, int>> Board::getAllLegalMoves(const Disk disk) const
{
	assert(disk == Disk::BLACK || disk == Disk::WHITE);
	std::vector<std::tuple<int, int>> legal_moves;
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (isLegalMove(x, y, disk)) {
				legal_moves.emplace_back(x, y);
			}
		}
	}
	return legal_moves;
}

void Board::placeDisk(const int x, const int y, const Disk disk)
{
	assert(disk == Disk::BLACK || disk == Disk::WHITE);
	assert(isLegalMove(x, y, disk));
	for (auto&& direction : DIRECTIONS) {
		int num_bounded_disks = countBoundedDisks(*this, x, y, disk, direction);
		int flip_x = x;
		int flip_y = y;
		for (int i = 0; i < num_bounded_disks; ++i) {
			flip_x += std::get<0>(direction);
			flip_y += std::get<1>(direction);
			set(flip_x, flip_y, disk);
		}
	}
	set(x, y, disk);
}

int Board::countDisks(Disk target) const
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
			Disk disk = board.get(x, y);
			switch (disk) {
			case Disk::NONE:
				os << '-';
				break;
			case Disk::BLACK:
				os << 'B';
				break;
			case Disk::WHITE:
				os << 'W';
				break;
			}
		}
	}
	return os;
}

}  // namespace reversi
