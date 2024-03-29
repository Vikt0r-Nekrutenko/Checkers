#ifndef TURNS_HPP
#define TURNS_HPP

#include "vec2d.hpp"
#include <corecrt.h>
#include <string>
class GameModel;
class BoardCell;
class GameTurn
{
public:
    void* operator new(size_t size);

    virtual void turnHandler(GameModel *, const stf::Vec2d&) { return; }
    virtual std::string log() const { return "Nothing turn."; };
};

class MustBeAttackingTurn : public GameTurn
{
    std::string log() const override { return "Turn must be attack!"; };
};

class NothingTurn : public GameTurn { };

class TargetsClearingTurn : public GameTurn
{
public:
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
    std::string log() const override { return "Targets cleared!"; };
};

class AttackTurn : public TargetsClearingTurn
{
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
    std::string log() const override { return "Attacked!"; };
};

class MultiplyAttackTurn : public TargetsClearingTurn
{
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
    std::string log() const override { return "Multiply attack turn."; };
};
class MoveTurn : public TargetsClearingTurn
{
    void turnHandler(GameModel *model, const stf::Vec2d &targetPos) override;
    std::string log() const override { return "Moved."; };
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
