#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP


#include "fieldsallocator.hpp"
#include "renderer.hpp"
#include "turns.hpp"

class GameModel;
struct Cursor;

struct Direction
{
    stf::Vec2d move   {0,0};
    stf::Vec2d attack {0,0};
};

class BoardCell
{
public:
    void* operator new(size_t size);
    virtual uint8_t view() const;
    virtual stf::ColorTable color() const;

    virtual bool isTransformPossible(GameModel *);;
    virtual BoardCell* getTransformPiece();;
    BoardCell* transformation(GameModel *model);

    void takeNextTurn(GameModel *, const Cursor&);
    GameTurn *isNextTurnAreAttack(GameModel *, const Cursor&, const stf::Vec2d&, const stf::Vec2d&) const;
    GameTurn *isAttackTurnAvailiable(GameModel *, const stf::Vec2d&) const;

    GameTurn* isAttackTurnAvailiable(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;
    GameTurn* isAttackTurnAvailiable(GameModel *model, const stf::Vec2d& pos, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;

    GameTurn* isMultiAttackTurn(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;
    GameTurn* isAttackTurnPossible(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;

    GameTurn* moveIsPossible(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection) const;

    static stf::sdb::DynamicFieldsAllocator _cellAllocator;
    std::vector<Direction> directions;
};

class EmptyCell : public BoardCell
{
public:
    inline uint8_t view() const override
    {
        return 'e';
    }
};

#endif // BOARDCELL_HPP
