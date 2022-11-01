#include "boardcell.hpp"

stf::sdb::DynamicFieldsAllocator BoardCell::_cellAllocator = stf::sdb::DynamicFieldsAllocator();

bool WChecker::onPlacementHandler(GameModel *, const Cursor &) { return true; }

bool WQueen::onPlacementHandler(GameModel *, const Cursor &) { return true; }

bool BChecker::onPlacementHandler(GameModel *, const Cursor &) { return true; }

bool BQueen::onPlacementHandler(GameModel *, const Cursor &) { return true; }

