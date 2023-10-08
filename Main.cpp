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

class Board
{
    private:
        int _width;
        BoardSquare::E* _squares;

    public:
        // constructor using Init list ( skips internal steps, is faster )
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
        virtual BoardSquare::E HasWon(Board& board) = 0;
};

class Game
{
    public:
        BoardSquare::E Run() {}
};

int main(){
    return 0;
}