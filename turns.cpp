#include "turns.hpp"
#include "gamemodel.hpp"

void *GameTurn::operator new(size_t size)
{
    return BoardCell::_cellAllocator.allocate(size);
}

TurnsCreator<MoveTurn>              turns::moveTurn = TurnsCreator<MoveTurn>();
TurnsCreator<AttackTurn>            turns::attackTurn = TurnsCreator<AttackTurn>();
TurnsCreator<NothingTurn>           turns::nothingTurn = TurnsCreator<NothingTurn>();
TurnsCreator<MultiplyAttackTurn>    turns::multiAttatckTurn = TurnsCreator<MultiplyAttackTurn>();
TurnsCreator<MustBeAttackingTurn>   turns::mustBeAttackingTurn = TurnsCreator<MustBeAttackingTurn>();

void AttackTurn::turnHandler(GameModel *model, const stf::Vec2d& targetPos)
{
    TargetsClearingTurn::turnHandler(model, model->cursor.selectedCell.pos + targetPos);

    model->cursor.reset();
    model->player = model->opponent();
}

void MultiplyAttackTurn::turnHandler(GameModel *model, const stf::Vec2d& targetPos)
{
    TargetsClearingTurn::turnHandler(model, model->cursor.selectedCell.pos + targetPos);
    model->cursor.select(model->board.getSelectableCell(model->cursor));
}

void MoveTurn::turnHandler(GameModel *model, const stf::Vec2d& targetPos)
{

    TargetsClearingTurn::turnHandler(model, targetPos);
    model->cursor.reset();
    model->player = model->opponent();
}

void TargetsClearingTurn::turnHandler(GameModel *model, const stf::Vec2d &targetPos)
{
    model->board.clear(model->cursor.selectedCell.pos);
    model->board.clear(targetPos);

    BoardCell *transformed = model->cursor.selectableCell.cell->transformation(model);
    model->board.place(model->cursor.selectableCell.pos, transformed);
}
