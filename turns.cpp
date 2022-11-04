#include "turns.hpp"
#include "gamemodel.hpp"

void *GameTurn::operator new(size_t size)
{
    return BoardCell::_cellAllocator.allocate(size);
}

TurnsCreator<NothingTurn> turns::nothingTurn = TurnsCreator<NothingTurn>();
TurnsCreator<AttackTurn> turns::attackTurn = TurnsCreator<AttackTurn>();
TurnsCreator<SimpleAttackTurn> turns::simpleTurn = TurnsCreator<SimpleAttackTurn>();
TurnsCreator<MultiplyAttackTurn> turns::multiAttatckTurn = TurnsCreator<MultiplyAttackTurn>();
TurnsCreator<MustBeAttackingTurn> turns::mustBeAttackingTurn = TurnsCreator<MustBeAttackingTurn>();
TurnsCreator<MoveTurn> turns::moveTurn = TurnsCreator<MoveTurn>();

void AttackTurn::turnHandler(GameModel *model)
{
    model->board.clear(model->cursor.selectedCell.pos);
    stf::Vec2d sub = model->cursor.selectableCell.pos - model->cursor.selectedCell.pos;
    stf::Vec2d norm = { sub.x / std::abs(sub.x), sub.y / std::abs(sub.y) };
    model->board.clear(model->cursor.selectedCell.pos + norm);

    model->placementAfterHandling();
    model->cursor.reset();
    model->player = model->opponent();
    model->isSelect = true;
}

void MultiplyAttackTurn::turnHandler(GameModel *model)
{
    model->board.clear(model->cursor.selectedCell.pos);
    stf::Vec2d sub = model->cursor.selectableCell.pos - model->cursor.selectedCell.pos;
    stf::Vec2d norm = { sub.x / std::abs(sub.x), sub.y / std::abs(sub.y) };
    model->board.clear(model->cursor.selectedCell.pos + norm);

    model->placementAfterHandling();
    model->cursor.select(model->board.getSelectableCell(model->cursor));
}

void MoveTurn::turnHandler(GameModel *model)
{
    model->board.clear(model->cursor.selectedCell.pos);
    model->board.place(model->cursor.selectableCell.pos, model->cursor.selectableCell.cell);

    model->placementAfterHandling();
    model->cursor.reset();
    model->player = model->opponent();
    model->isSelect = true;
}
