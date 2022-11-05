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

auto clearTargets = [](GameModel *model, const stf::Vec2d& t1, const stf::Vec2d& t2) {
    model->board.clear(t1);
    model->board.clear(t2);

    BoardCell *transformed = model->cursor.selectableCell.cell->transformation(model);
    model->board.place(model->cursor.selectableCell.pos, transformed);
};

void AttackTurn::turnHandler(GameModel *model, const stf::Vec2d& targetPos)
{
    clearTargets(model, model->cursor.selectedCell.pos, model->cursor.selectedCell.pos + targetPos);

    model->cursor.reset();
    model->player = model->opponent();
}

void MultiplyAttackTurn::turnHandler(GameModel *model, const stf::Vec2d& targetPos)
{
    clearTargets(model, model->cursor.selectedCell.pos, model->cursor.selectedCell.pos + targetPos);
    model->cursor.select(model->board.getSelectableCell(model->cursor));
}

void MoveTurn::turnHandler(GameModel *model, const stf::Vec2d& targetPos)
{
    clearTargets(model, model->cursor.selectedCell.pos, model->cursor.selectedCell.pos);
    model->cursor.reset();
    model->player = model->opponent();
}
