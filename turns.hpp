#ifndef TURNS_HPP
#define TURNS_HPP

#include <corecrt.h>
class GameModel;
class GameTurn
{
public:
    void* operator new(size_t size);

    virtual void turnHandler(GameModel *model) = 0;
};
class NothingTurn : public GameTurn
{
    void turnHandler(GameModel *) override {  }
};
class AttackTurn : public GameTurn
{
    void turnHandler(GameModel *model) override;
};
class MustBeAttackingTurn : public GameTurn
{
    void turnHandler(GameModel *) override { }
};
class SimpleAttackTurn : public GameTurn { };
class MultiplyAttackTurn : public GameTurn
{
    void turnHandler(GameModel *model) override;
};
class MoveTurn : public GameTurn
{
    void turnHandler(GameModel *model) override;
};

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
