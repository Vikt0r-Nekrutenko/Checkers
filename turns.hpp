#ifndef TURNS_HPP
#define TURNS_HPP

#include "vec2d.hpp"
#include <corecrt.h>
class GameModel;
class BoardCell;
class GameTurn
{
public:
    void* operator new(size_t size);

    virtual void turnHandler(GameModel *, const stf::Vec2d&) { return; }
};

class MustBeAttackingTurn : public GameTurn {};

class NothingTurn : public GameTurn {};

class TargetsClearingTurn : public GameTurn
{
public:
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
};

class AttackTurn : public TargetsClearingTurn
{
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
};

class MultiplyAttackTurn : public TargetsClearingTurn
{
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
};
class MoveTurn : public TargetsClearingTurn
{
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
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
