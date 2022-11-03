#ifndef TURNS_HPP
#define TURNS_HPP

#include <corecrt.h>
class GameTurn
{
public:
    void* operator new(size_t size);
};
class NothingTurn : public GameTurn { };
class AttackTurn : public GameTurn { };
class MustBeAttackingTurn : public GameTurn { };
class SimpleAttackTurn : public GameTurn { };
class MultiplyAttackTurn : public GameTurn { };
class MoveTurn : public GameTurn { };

template<typename T> class TurnsCreator {
public:
    T *operator ()()
    {
        return turn = turn == nullptr ? new T() : turn;
    }

    T *turn = nullptr;
};

class turns
{
public:
    static TurnsCreator<NothingTurn> nothingTurn;
    static TurnsCreator<AttackTurn> attackTurn;
    static TurnsCreator<SimpleAttackTurn> simpleTurn;
    static TurnsCreator<MultiplyAttackTurn> multiplyTurn;
    static TurnsCreator<MustBeAttackingTurn> mustBeAttackingTurn;
    static TurnsCreator<MoveTurn> moveTurn;
};

#endif // TURNS_HPP
