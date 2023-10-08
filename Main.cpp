#include <iostream>
#include <string>

using namespace std;

struct BoardSquare
{
    // Placing the enum inside this struct limits its scope.
    // So we can perform actions like BoardSquare::E instead of it being global
    enum E
    {
        X,
        O,
        Empty
    };
};

struct WinningPlayer
{
    enum E
    {
        X,
        O,
        stalemate, 
        None
    };
};

class Board
{
    private:
        int _width;
        BoardSquare::E* _squares;

    public:
        // constructor using Init list ( skips internal steps; is faster )
        // Takes in width and assigns it to local _width
        Board(int width) : _width(width)
        {
            _squares = new BoardSquare::E[width * width];
            for ( auto i = 0; i < GetTotalSquares(); i++ ){
                _squares[i] = BoardSquare::E::Empty;
            } 
        }

        ~Board()
        {
            delete[] _squares;
        }

        int Getwidth() { return _width; }
        int GetTotalSquares() { return _width * _width; }

        BoardSquare::E GetSquare(int index) 
        {
            return _squares[index];
        }
        void Setsquare(int index, BoardSquare::E square) 
        {
            _squares[index] = square;
        }
};

class IRuleEngine
{
    // This is an interface
    public:
        // Dynamic Polymorphism - its an abstract method since its a pure virtual function
        virtual WinningPlayer::E GetWinningPlayer(Board& board) = 0;
};

class Game
{
    private:
        Board& _board;
        IRuleEngine& _ruleEngine;
    public:
        Game(Board& board, IRuleEngine& ruleEngine) : _board(board), _ruleEngine(ruleEngine) {}

        WinningPlayer::E Run() 
        {
            WinningPlayer::E winningPlayer;
            BoardSquare::E currentPlayer = BoardSquare::E::X;
            // While no one has won
            while((winningPlayer = _ruleEngine.GetWinningPlayer(_board)) == WinningPlayer::E::None)
            {
                Render();
                cout << "\n";

                int input;
                cout << "Move for " << GetPlayerChar(currentPlayer) << ": ";
                cin >> input;
                cin.ignore();

                input -= 1;
                // Validating input 
                if (input < 0 || input >= _board.GetTotalSquares() || _board.GetSquare(input) != BoardSquare::E::Empty)
                {
                    cout << "invalid Move!\n";
                    continue;
                }

                _board.Setsquare(input, currentPlayer);
                currentPlayer = currentPlayer == BoardSquare::E::X ? BoardSquare::E::O : BoardSquare::E::X;
            }

            return winningPlayer;
        }

    private:
        void Render() 
        {
            for (auto i = 1; i <= _board.GetTotalSquares(); i++)
            {
                // index 1 based as for the user the board should start from '1'
                cout << GetSquareChar(i, _board.GetSquare(i-1)) << " ";
                if ((i % _board.Getwidth()) == 0)
                    cout << "\n"; 
            }
        }

        char GetPlayerChar (BoardSquare::E player)
        {
            switch (player)
            {
                case BoardSquare::E::X: return 'X';
                case BoardSquare::E::O: return 'O';
            }
        }

        char GetSquareChar (int index, BoardSquare::E square) 
        {
            switch(square)
            {
                case BoardSquare::E::X: return 'X';
                case BoardSquare::E::O: return 'O';
            }

            return index + '0';
        }
};

int main(){
    return 0;
}