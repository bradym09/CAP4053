using GameAI.GamePlaying.Core;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace GameAI.GamePlaying
{
    public class StudentAI : Behavior
    {
        private int Evaluate(Board board)
        {
            int score = 0;
            
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    int value = board.GetTile(row, col);
                    if (row == 0 || row == 7)
                        value *= 10;
                    if (col == 0 || col == 7)
                        value *= 10;
                    score += value;
                }
            }

            if (board.IsTerminalState())
            {
                if (score > 0)
                    score += 10000;
                else if (score < 0)
                    score -= 10000;
            }

            return score;
        }

        private int getNextPlayer(int _color, Board board)
        {
            if (board.HasAnyValidMove(-1 * _color))
                return -1 * _color;
            return _color;
        }

        //Methods go here
        public StudentAI() 
        { 
        
        }

        public ComputerMove Run(int _color, Board _board, int _lookAheadDepth)
        {
            ComputerMove bestMove = null;
            
            //Compute moves
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    //Skip invalid moves
                    if (_board.IsValidMove(_color, row, col))
                    {
                        Board newState = new Board();
                        newState.Copy(_board);
                        ComputerMove move = new ComputerMove(row, col);

                        newState.MakeMove(_color, row, col);

                        //Make immediate move if terminal
                        if (newState.IsTerminalState() || _lookAheadDepth == 0)
                        {
                            move.rank = Evaluate(newState);
                        }
                        else
                        {
                            move.rank = Run(getNextPlayer(_color, newState), newState, _lookAheadDepth - 1).rank;
                        }


                        if (bestMove == null)
                            bestMove = move;

                        //Higher = better for white
                        else if (_color == 1)
                        {
                            if (move.rank > bestMove.rank)
                                bestMove = move;
                        }

                        //Lower = better for black
                        else if (_color == -1)
                        {
                            if (move.rank < bestMove.rank)
                                bestMove = move;
                        }
                    }
                }
            }
            return bestMove;
        }
    }
}
