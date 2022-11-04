#ifndef TURNS_HPP
#define TURNS_HPP

#include <corecrt.h>
class GameModel;
class GameTurn
{
public:
    void* operator new(size_t size);

    virtual void turnHandler(GameModel *) { }
};

class MustBeAttackingTurn : public GameTurn {};

class NothingTurn : public GameTurn {};

class AttackTurn : public GameTurn
{
    void turnHandler(GameModel *model) override;
};

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
    static TurnsCreator<MoveTurn>           moveTurn;
    static TurnsCreator<NothingTurn>        nothingTurn;
    static TurnsCreator<AttackTurn>         attackTurn;
    static TurnsCreator<MultiplyAttackTurn> multiAttatckTurn;
    static TurnsCreator<MustBeAttackingTurn>mustBeAttackingTurn;
};

#endif // TURNS_HPP
