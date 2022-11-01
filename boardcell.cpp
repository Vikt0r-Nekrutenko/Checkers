#include "boardcell.hpp"
#include "gamemodel.hpp"

stf::sdb::DynamicFieldsAllocator BoardCell::_cellAllocator = stf::sdb::DynamicFieldsAllocator();

auto isOpponent = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& direction){
    return model->board[cursor.selectedCell.pos + direction]->color() == model->opponent()->color();
};

bool MovableObject::lMoveIsPossible(GameModel *model, const Cursor& cursor)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirectionL == cursor.selectableCell.pos)
        return true;
    return false;
}

bool MovableObject::rMoveIsPossible(GameModel *model, const Cursor& cursor)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirectionR == cursor.selectableCell.pos)
        return true;
    return false;
}

bool MovableObject::lAttackIsPossible(GameModel *model, const Cursor& cursor)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell() &&
       cursor.selectedCell.pos + attackDirectionL == cursor.selectableCell.pos &&
       isOpponent(model, cursor, moveDirectionL))
        return true;
    return false;
}

bool MovableObject::rAttackIsPossible(GameModel *model, const Cursor& cursor)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell() &&
       cursor.selectedCell.pos + attackDirectionR == cursor.selectableCell.pos &&
       isOpponent(model, cursor, moveDirectionR))
        return true;
    return false;
}

bool MovableObject::isFightAvailiable(GameModel *model, const Cursor& cursor) const
{
    try {
        BoardCell *lCellBehindOpponent = model->board[cursor.selectedCell.pos + attackDirectionL];
        BoardCell *rCellBehindOpponent = model->board[cursor.selectedCell.pos + attackDirectionR];

        if((isOpponent(model, cursor, moveDirectionL) && lCellBehindOpponent == GameBoard::emptyCell()) ||
           (isOpponent(model, cursor, moveDirectionR) && rCellBehindOpponent == GameBoard::emptyCell()))
            return true;
    } catch(const std::out_of_range& ex) {
        return false;
    }
    return false;
}

bool MovableObject::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    if(isFightAvailiable(model, cursor)) {
        if(rAttackIsPossible(model, cursor)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveDirectionR);
            return true;
        } else if(lAttackIsPossible(model, cursor)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveDirectionL);
            return true;
        }
    } else if(rMoveIsPossible(model, cursor) || lMoveIsPossible(model, cursor)) {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    }
    return false;
}
