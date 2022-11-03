#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "gameboard.hpp"
#include "cursor.hpp"

class GameModel : public stf::smv::BaseModel
{
public:
    GameModel();

    BoardCell *opponent() const;
    Cursor getCellUnderAttack();
    void placementAfterHandling();
    stf::smv::IView* put(stf::smv::IView *sender);
    stf::smv::IView* select(stf::smv::IView *sender);
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) final;

    GameBoard board = GameBoard();
    Cursor cursor = Cursor();
    BoardCell *player = GameBoard::whitePlayer();

    uint32_t exCount = 0ul;
    bool isSelect = true;
};

#endif // GAMEMODEL_HPP
