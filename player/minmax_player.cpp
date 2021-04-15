#include "minmax_player.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

namespace reversi {

CellPosition MinMaxPlayer::thinkNextMove(const Board &board) {
  // choose the first legal move
  // int size = max_node(getSide(), 0, 2);
  CellPosition c;
  std::cout << "initial c " << c << "\n" << std::endl;
  Side side = getSide();
  int v;
  CellPosition best_step;
  // std::tie(v, best_step) = max_node(board, side, 0, 3, -10000, +10000, c);
  std::tie(v, best_step) = negamax(board, side, 0, 6, -10000, +10000, c, side);
  printf("finished! %d\n", v);
  std::cout << "best move " << best_step << "\n" << std::endl;
  // return board.getAllLegalMoves(getSide()).front();
  return best_step;
}

std::tuple<int, CellPosition>
MinMaxPlayer::negamax(const Board &board, Side side, int depth, int max_depth,
                      int alpha, int beta, CellPosition first_step,
                      Side init_side) {
  int turn;
  if (side == init_side) {
    // printf("MAX NODE, depth = %d\n", depth);
    turn = 1;
  } else {
    // printf("MIN NODE, depth = %d\n", depth);
    turn = -1;
  }
  if (depth == max_depth)
    return std::make_tuple(board.eval(side) * turn, first_step);

  int best = -100000000;
  CellPosition best_step;

  std::vector<CellPosition> legal_moves = board.getAllLegalMoves(side);

  // printf("legal_size = %d\n ", legal_moves.size());

  if (legal_moves.size() == 0) {
    return std::make_tuple(board.eval(side) * turn, first_step);
  }

  for (int i = 0; i < legal_moves.size(); ++i) {
    CellPosition move = legal_moves[i];

    if (depth == 0)
      first_step = move;

    Board new_board = board;
    new_board.placeDisk(move, side);

    // std::cout << "\nmove = " << move << "\n" << std::endl;

    int v;
    CellPosition step;

    std::tie(v, step) =
        negamax(new_board, getOpponentSide(side), depth + 1, max_depth, -beta,
                -alpha, first_step, init_side);
    v = -v;
    if (v > best) {
      best = v;
      best_step = step;
    }
    if (alpha < best) {
      alpha = best;
    }
    if (best >= beta) {
      return std::make_tuple(best, best_step);
    }

    // printf("depth = %d, V = %d, best = %d, legalsize = %d \n", depth, v,
    // best,
    //        legal_moves.size());
  }
  return std::make_tuple(best, best_step);
}

std::tuple<int, CellPosition>
MinMaxPlayer::max_node(const Board &board, Side side, int depth, int max_depth,
                       int alpha, int beta, CellPosition first_step) {
  // printf("MAX, depth = %d,  max_depth = %d", depth, max_depth);
  // if (side == Side::BLACK) {
  //   printf("side = black");
  // } else {
  //   printf("side= white");
  // }
  // printf("max! depth: %d\n", depth);
  // std::cout << board << "\n" << std::endl;
  // printf("max ! depth: %d\n", depth);

  if (depth == max_depth)
    return std::make_tuple(board.eval(side), first_step);

  int best = -100000000;
  CellPosition best_step;

  std::vector<CellPosition> legal_moves = board.getAllLegalMoves(side);
  if (legal_moves.size() == 0) {
    return std::make_tuple(board.eval(side), first_step);
  }
  // printf("MAX legalsize = %d \n", legal_moves.size());
  for (int i = 0; i < legal_moves.size(); ++i) {
    CellPosition move = legal_moves[i];

    if (depth == 0)
      first_step = move;

    // std::cout << "\nmove = " << move << "firststep = " << first_step << "\n"
    //           << std::endl;
    Board new_board = board;
    new_board.placeDisk(move, side);
    int v;
    CellPosition step;

    std::tie(v, step) = min_node(new_board, getOpponentSide(side), depth + 1,
                                 max_depth, alpha, beta, first_step);

    // printf("MAX depth = %d, V = %d, best = %d ", depth, v, best);
    // std::cout << " MAX DEPHT = " << depth << " V = " << v << " BEST = " <<
    // best
    //           << "\n"
    //           << std::endl;

    if (v >= beta) {
      // printf("v = %d, beta = %d max edagari!!!", v, beta);
      return std::make_tuple(v, step);
    }

    if (v > best) {
      best = v;
      best_step = step;
      alpha = std::max(alpha, best);
    }
    printf("MAX depth = %d, V = %d, best = %d, legalsize = %d \n", depth, v,
           best, legal_moves.size());
  }
  return std::make_tuple(best, best_step);
}

std::tuple<int, CellPosition>
MinMaxPlayer::min_node(const Board &board, Side side, int depth, int max_depth,
                       int alpha, int beta, CellPosition first_step) {
  // printf("MIN, depth = %d,  max_depth = %d\n", depth, max_depth);
  // if (side == Side::BLACK) {
  //   printf("side = black\n");
  // } else {
  //   printf("side= white\n");
  // }

  // printf("min ! depth: %d\n", depth);
  // std::cout << board << "\n" << std::endl;
  // printf("min ! depth: %d\n", depth);

  if (depth == max_depth)
    return std::make_tuple(board.eval(side), first_step);

  int best = 100000000;
  CellPosition best_step;
  std::vector<CellPosition> legal_moves = board.getAllLegalMoves(side);
  if (legal_moves.size() == 0) {
    return std::make_tuple(board.eval(side), first_step);
  }
  // printf("MIN legalsize = %d \n", legal_moves.size());
  for (int i = 0; i < legal_moves.size(); ++i) {
    CellPosition move = legal_moves[i];
    // std::cout << "\nmove = " << move << "firststep = " << first_step << "\n"
    //           << std::endl;
    Board new_board = board;
    new_board.placeDisk(move, side);

    int v;
    CellPosition step;
    std::tie(v, step) = max_node(new_board, getOpponentSide(side), depth + 1,
                                 max_depth, alpha, beta, first_step);
    // std::cout << " MIN DEPHT = " << depth << " V = " << v << " BEST = " <<
    // best
    //           << "\n"
    //           << std::endl;

    if (v <= alpha) {
      // printf("v = %d, alpha = %d min edagari!!!", v, alpha);
      return std::make_tuple(v, step);
    }

    if (v < best) {
      best = v;
      best_step = step;
      beta = std::min(beta, best);
    }
    printf("MIN depth = %d, V = %d, best = %d, legalsize = %d \n", depth, v,
           best, legal_moves.size());
  }
  return std::make_tuple(best, best_step);
}
} // namespace reversi
