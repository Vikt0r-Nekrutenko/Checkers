#include "turns.hpp"
#include "boardcell.hpp"

void *GameTurn::operator new(size_t size)
{
    return BoardCell::_cellAllocator.allocate(size);
}

 TurnsCreator<NothingTurn> turns::nothingTurn = TurnsCreator<NothingTurn>();
 TurnsCreator<AttackTurn> turns::attackTurn = TurnsCreator<AttackTurn>();
 TurnsCreator<ReAttackTurn> turns::reattackTurn = TurnsCreator<ReAttackTurn>();
 TurnsCreator<MoveTurn> turns::moveTurn = TurnsCreator<MoveTurn>();
