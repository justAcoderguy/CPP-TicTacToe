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
                case BoardSquare::E::X: return 'X'; break;
                case BoardSquare::E::O: return 'O'; break;
            }
        }

        char GetSquareChar (int index, BoardSquare::E square) 
        {
            switch(square)
            {
                case BoardSquare::E::X: return 'X'; break;
                case BoardSquare::E::O: return 'O'; break;
            }

            return index + '0';
        }
};

class NullRuleEngine : public IRuleEngine
{
    int GetIndexForXY(int x, int y, int width)
    {
        return width * x + y;
    }

    public:
        virtual WinningPlayer::E GetWinningPlayer(Board& Board)
        {
            // Checking for wins horizontally
            for (auto x = 0; x < Board.Getwidth(); x++)
            {
                BoardSquare::E typeOfSquare;

                int firstIndex = GetIndexForXY(x, 0, Board.Getwidth());
                typeOfSquare = Board.GetSquare(firstIndex);
                if (typeOfSquare == BoardSquare::E::Empty)
                    continue;

                WinningPlayer::E winningPlayer = typeOfSquare == BoardSquare::E::X ? WinningPlayer::E::X : WinningPlayer::E::O;

                for (auto y = 1; y < Board.Getwidth(); y++)
                {
                    if (typeOfSquare != Board.GetSquare(GetIndexForXY(x, y, Board.Getwidth())))
                    {
                        winningPlayer = WinningPlayer::E::None;
                        break;
                    }
                } 

                if (winningPlayer != WinningPlayer::E::None)
                    return winningPlayer;
            }
            
            // Checking if all boxes are filled and no winner was found
            bool foundEmpty = false;
            for (auto i = 0; i < Board.GetTotalSquares(); i++)
            {
                if (Board.GetSquare(i) == BoardSquare::E::Empty)
                {
                    foundEmpty = true;
                    break;
                }
            }
            if (!foundEmpty)
                return WinningPlayer::E::stalemate;    

            return WinningPlayer::E::None;
        }
};

int main(){
    Board board(3);
    NullRuleEngine ruleEngine;

    Game game(board, ruleEngine);
    WinningPlayer::E winner = game.Run();

    switch (winner)
    {
        case WinningPlayer::stalemate: cout << "Its a tie!\n"; break;
        case WinningPlayer::O: cout << "Congratulations player O, you have won!\n"; break;
        case WinningPlayer::X: cout << "Congratulations player X, you have won!\n"; break;
        default: cout << "Something unexpected happened. Sorry\n";
    }

    return 0;
}