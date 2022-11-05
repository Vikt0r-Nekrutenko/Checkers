#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP


#include "fieldsallocator.hpp"
#include "renderer.hpp"
#include "turns.hpp"

class GameModel;
struct Cursor;

class BoardCell
{
public:
    void* operator new(size_t size)
    {
        return _cellAllocator.allocate(size);
    }
    virtual uint8_t view() const { return 0; }
    virtual bool isTransformPossible(GameModel *) { return false; };
    virtual BoardCell* getTransformPiece() { return this; };
    virtual stf::ColorTable color() const { return stf::ColorTable::Default; }
    virtual GameTurn* takeNextTurn(GameModel *, const Cursor&) { return turns::nothingTurn(); }
    virtual GameTurn* isNextTurnAreAttack(GameModel *, const Cursor&, const stf::Vec2d&, const stf::Vec2d&) const { return turns::nothingTurn(); }
    virtual GameTurn* isAttackTurnAvailiable(GameModel *, const stf::Vec2d&) const { return turns::nothingTurn(); }

    BoardCell* transformation(GameModel *model);
    GameTurn* isAttackTurnAvailiable(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;
    GameTurn* isAttackTurnAvailiable(GameModel *model, const stf::Vec2d& pos, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;
    GameTurn* isMultiAttackTurn(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;
    GameTurn* isAttackTurnPossible(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const;
    GameTurn* moveIsPossible(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection) const;

    static stf::sdb::DynamicFieldsAllocator _cellAllocator;
};

class EmptyCell : public BoardCell
{
public:
    uint8_t view() const override
    {
        return 'e';
    }
};

#endif // BOARDCELL_HPP
