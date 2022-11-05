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

auto getTargetPos = [](const Selector& s1, const Selector& s2) -> stf::Vec2d {
    stf::Vec2d sub = s1.pos - s2.pos;
    stf::Vec2d pos = { sub.x / std::abs(sub.x),
                       sub.y / std::abs(sub.y) };
    return pos;
};

auto clearTargets = [](GameModel *model, const stf::Vec2d& t1, const stf::Vec2d& t2) {
    model->board.clear(t1);
    model->board.clear(t2);

    BoardCell *transformed = model->cursor.selectableCell.cell->transformation(model);
    model->board.place(model->cursor.selectableCell.pos, transformed);
};

void AttackTurn::turnHandler(GameModel *model)
{
    clearTargets(model, model->cursor.selectedCell.pos, model->cursor.selectedCell.pos + getTargetPos(model->cursor.selectableCell, model->cursor.selectedCell));

    model->cursor.reset();
    model->player = model->opponent();
}

void MultiplyAttackTurn::turnHandler(GameModel *model)
{
    clearTargets(model, model->cursor.selectedCell.pos, model->cursor.selectedCell.pos + getTargetPos(model->cursor.selectableCell, model->cursor.selectedCell));
    model->cursor.select(model->board.getSelectableCell(model->cursor));
}

void MoveTurn::turnHandler(GameModel *model)
{
    clearTargets(model, model->cursor.selectedCell.pos, model->cursor.selectedCell.pos);
    model->cursor.reset();
    model->player = model->opponent();
}
