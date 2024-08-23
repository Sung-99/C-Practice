#include <QMessageBox>
#include <QSignalMapper>


#include "Pursuit.h"
#include "ui_Pursuit.h"
#include "Player.h"

Pursuit::Pursuit(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Pursuit),
      m_selected(nullptr) ,
      m_player(Player::player(Player::Red)) {
     ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            QString cellName = QString("cell%1%2").arg(row).arg(col);
            Cell* cell = this->findChild<Cell*>(cellName);
            Q_ASSERT(cell != nullptr);
            Q_ASSERT(cell->row() == row && cell->col() == col);

            m_board[row][col] = cell;

            int id = row * 7 + col;
            map->setMapping(cell, id);
            QObject::connect(cell, SIGNAL(clicked(bool)), map, SLOT(map()));
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    // When the turn ends, switch the player.
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));

    // Reset the game.
    this->reset();
    this->adjustSize();
    this->setFixedSize(this->size());

    this -> updatePlayable();
}

Pursuit::~Pursuit() {
    delete ui;
}

void Pursuit::play(int id) {
    Cell* cell = m_board[id / 7][id % 7];

     if (cell->isEmpty() && cell -> isPlayable()) {

        cell->setPlayer(m_player);

          emit turnEnded();
    }

      switch (cell->state()) {
        case Cell::Used:
          //if(cell -> player() == m_player){
                //this -> limpaPlayable();


           // cell -> setState(Cell::Selected);
            foreach (Cell::Direction dir, Cell::directions) {
                Cell* n = this->celulaJogavel(cell, dir);
                if (n != nullptr && n->isEmpty())
                    n->setPlayable(true);

                }

            //}

            break;


            default:

                break;//nothing
      }



}


void Pursuit::limpaPlayable() {
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 7; col++) {
            Cell* cell = m_board[row][col];
            if(cell -> isPlayable())
                cell->setState(Cell::Empty);



            }

        }
    }




void Pursuit::switchPlayer() {
    // Switch the player.
    m_player = m_player->other();

    // Finally, update the status bar.
    this->updateStatusBar();
}




void Pursuit::updatePlayable(){
    m_playables.clear();
    m_playables << m_board[1][2] << m_board[1][4]<< m_board[2][3];

    foreach(Cell* cell , m_playables){
        cell -> setPlayable(true);
    }




}


Cell* Pursuit::celulaJogavel(Cell* cell, Cell::Direction dir) const {
    if (cell == nullptr)
        return nullptr;

    int row, col;
    switch (dir) {
        case Cell::North:
            row = cell->row() - 1;
            col = cell->col();
            break;
        case Cell::NorthEast:
            row = cell->row() - 1;
            col = cell->col() + 1;
            break;
        case Cell::East:
            row = cell->row();
            col = cell->col() + 1;
            break;
        case Cell::SouthEast:
            row = cell->row() + 1;
            col = cell->col() + 1;
            break;
        case Cell::South:
            row = cell->row() + 1;
            col = cell->col();
            break;
        case Cell::SouthWest:
            row = cell->row() + 1;
            col = cell->col() - 1;
            break;
        case Cell::West:
            row = cell->row();
            col = cell->col() - 1;
            break;
        case Cell::NorthWest:
            row = cell->row() - 1;
            col = cell->col() - 1;
            break;
        default:
            Q_UNREACHABLE();
    }

    return ((row >= 0 && row < 7 && col >= 0 && col < 7) ?
                m_board[row][col] : nullptr);
}


void Pursuit::reset() {
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            Cell* cell = m_board[row][col];
            cell->reset();

            if ((row + col) % 2 == 0)
                cell->setState(Cell::Blocked);
        }
    }

    m_player = Player::player(Player::Red);

    m_board[0][3]->setState(Cell::Blocked);
    m_board[1][3]->setPlayer(m_player);
    m_board[5][3]->setPlayer(m_player->other());
    m_board[6][3]->setState(Cell::Blocked);
    this -> updatePlayable();
    this->updateStatusBar();

}

void Pursuit::showAbout() {
    QMessageBox::information(this, tr("About"),
        tr("Pursuit\n\nVitor Hugo Mota Soares -vitorhugomota99@hotmail.com"
           "\n\nGustavo de Carvalho Balmant -gu123balmant@gmail.com"));
}

void Pursuit::updateStatusBar() {
    ui->statusbar->showMessage(tr("Vez do %1")
        .arg(m_player->name()));
}
