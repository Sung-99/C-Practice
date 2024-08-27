#include <QMessageBox>
#include <QSignalMapper>

#include    <QDebug>
#include "Pursuit.h"
#include "ui_Pursuit.h"
#include "Player.h"

Pursuit::Pursuit(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Pursuit),
      m_player(Player::player(Player::Red)),
      m_phase( Pursuit::moveJogador),
      checkGameContinue(true){
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
    // Notify when the game is over and reset.
     //  QObject::connect(this, SIGNAL(gameOver()), this, SLOT(showGameOver()));
       QObject::connect(this, SIGNAL(gameOver()), this, SLOT(reset()));
    // Reset the game.
    this->reset();
    this->adjustSize();
    this->setFixedSize(this->size());


}

Pursuit::~Pursuit() {
    delete ui;
}

void Pursuit::setPhase(Pursuit::Phase phase) {
    if (m_phase != phase) {
        m_phase = phase;
        emit phaseChanged(phase);
    }
}





void Pursuit::play(int id) {
    Cell* cell = m_board[id / 7][id % 7];

      // qDebug() << " used ";
    switch (m_phase) {
            case Pursuit::moveJogador:
                moveplay(cell);
                break;
            case Pursuit::retiraBlack:
                retira(cell);
                break;
            default:
                Q_UNREACHABLE();
        }

}

void Pursuit::moveplay(Cell *cell){

    if (cell->isEmpty() && cell -> isPlayable()) {
      if(m_player == Player::player(Player::Red)){
        m_selectedRed -> setPlayer(nullptr);
        cell->setPlayer(m_player);
        m_selectedRed = cell;
      }
      if(m_player == Player::player(Player::Blue)){
        m_selectedBlue -> setPlayer(nullptr);
        cell->setPlayer(m_player);
        m_selectedBlue = cell;
      }
        this-> setPhase(Pursuit::retiraBlack);
        this -> MostraOsBlackPlayable(cell);

    }


}

void Pursuit::MostraOsBlackPlayable(Cell *cell){
    this->updateStatusBar();
    this -> limpaPlayable();
    if(cell -> isUsed() && cell -> hasPlayer()) {
            // qDebug() << " : " << m_selected -> row() << " : "<< m_selected -> col();

            foreach (Cell::Direction dir, Cell::directions) {
              Cell* n = this->celulaJogavelblack(cell, dir);
              if (n != nullptr){
                if( n->isBlocked()){
                    n->setPlayable(true);
                }
              }
            }
    }
}




void Pursuit::MostraOsEmptyPlayable(Cell *cell){

    if(cell -> isUsed() && cell -> hasPlayer()) {
            // qDebug() << " : " << m_selected -> row() << " : "<< m_selected -> col();

            int soma = 1;
         //   int check = false;
            foreach (Cell::Direction dir, Cell::directions) {
              Cell* n;
                soma = 1;
                do{

                    n = this->celulaJogavelEmpty(cell, dir, soma);
                    if(n->row() == 6 || n->col()==6 || n->row() ==0 || n->col()==0)break;

                    if(n -> isEmpty()){ soma++;}
                    else{ break;}


                        }while(n -> isEmpty() && n != nullptr);

                        do{



                     if(n -> isBlocked()){soma--;}
                    n = this->celulaJogavelEmpty(cell, dir,soma);

                             if(n->row() ==0 || n->col()==0 || n->row() == 6 || n->col()==6)break;


                        }while(n -> isBlocked());


                  if((n->isEmpty() && n != nullptr) && (n->row() != 6 && n->row() !=0 && n->col()!=6 && n->col()!=0))n->setPlayable(true);
                }


            }
    }

Cell* Pursuit::celulaJogavelEmpty(Cell* cell, Cell::Direction dir,int soma) const {
    if (cell == nullptr)
        return nullptr;

    int row, col;
    switch (dir) {
        case Cell::North:

                row = cell->row()-soma;
                col = cell->col();
            break;
    case Cell::NorthEast:
        row = cell->row()-soma;
        col = cell->col()+soma;
        break;
    case Cell::East:
        row = cell->row();
        col = cell->col() + soma;
        break;
    case Cell::SouthEast:
        row = cell->row()+soma;
        col = cell->col()+soma;
        break;
    case Cell::South:
        row = cell->row()+soma;
        col = cell->col();
        break;
    case Cell::SouthWest:
        row = cell->row()+soma;
        col = cell->col()-soma;
        break;
    case Cell::West:
        row = cell->row();
        col = cell->col() - soma;
        break;
    case Cell::NorthWest:

        row = cell->row()-soma;
        col = cell->col()-soma;

        break;
        default:
            Q_UNREACHABLE();
    }


    if(col > 6)col=6;
    if(col < 0)col=0;

    if(row > 6)row=6;
    if(row < 0)row=0;
    //qDebug() << " row: " <<row << "col: "<<col;
    return ((row >= 0 && row < 7 && col >= 0 && col < 7) ?
                m_board[row][col] : nullptr);
}

void Pursuit::retira(Cell *cell){
   // qDebug() << "AAAAAAAAAAAAAAAAAAAAAA " ;

    if (cell -> isPlayable()) {
        cell -> setPlayable(false);
        cell -> setPlayer(m_player);


        if(m_player == Player::player(Player::Red)){
          cell -> setPlayable(false);
          m_selectedRed -> setPlayer(nullptr);
          cell->setPlayer(m_player);
          m_selectedRed = cell;
        }
        if(m_player == Player::player(Player::Blue)){
            cell -> setPlayable(false);
          m_selectedBlue -> setPlayer(nullptr);
          cell->setPlayer(m_player);
          m_selectedBlue = cell;
        }

        this -> limpaPlayable();

        emit turnEnded();
        contTurn++;

        this->updatePlayable();

        this-> setPhase(Pursuit::moveJogador);
        this->updateStatusBar();

        if(contTurn >2){
            if(m_player == Player::player(Player::Blue)){
                    MostraOsEmptyPlayable(m_selectedBlue);
            }
            if(m_player == Player::player(Player::Red)){
                MostraOsEmptyPlayable(m_selectedRed);
            }


        }
    }




}

void Pursuit::limpaPlayable() {
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 7; col++) {
            Cell* cell = m_board[row][col];
            if(cell -> isPlayable())
                cell->setPlayable(false);
            }

        }

    }




void Pursuit::switchPlayer() {
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 7; col++) {
            Cell* cell = m_board[row][col];
            if(cell -> state() == Cell::Blocked)

                checkGameContinue = true;
        }
    }


    if(checkGameContinue){
        m_player = m_player->other();

    }
    // Switch the player.


    // Finally, update the status bar.
   // this->updateStatusBar();
}

void Pursuit::updatePlayable(){
 m_playables.clear();

    if(contTurn == 1)m_playables << m_board[1][2] << m_board[1][4]<< m_board[2][3];
    if(contTurn == 2)m_playables << m_board[4][3] << m_board[5][2]<< m_board[5][4];
    //qDebug() << " : " << m_playables;
    foreach(Cell* cell , m_playables){
        cell -> setPlayable(true);

    }

}

Cell* Pursuit::celulaJogavelblack(Cell* cell, Cell::Direction dir) const {
    if (cell == nullptr)
        return nullptr;

    int row, col;
    switch (dir) {
        case Cell::North:

                row = cell->row()-1;
                col = cell->col();
            break;
    case Cell::NorthEast:
        row = cell->row()-1;
        col = cell->col()+1;
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
        row = cell->row()+1;
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
    if(col > 6)col=6;
    if(col < 0)col=0;

    if(row > 6)row=6;
    if(row < 0)row=0;
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
    m_selectedBlue= m_board[5][3];
    m_selectedRed = m_board[1][3];
    contTurn = 1;

    this -> updatePlayable();
    m_phase = Pursuit::moveJogador;
       this->updateStatusBar();
    checkGameContinue = false;

}

void Pursuit::showAbout() {
    QMessageBox::information(this, tr("About"),
        tr("Pursuit\n\nVitor Hugo Mota Soares -vitorhugomota99@hotmail.com"
           "\n\nGustavo de Carvalho Balmant -gu123balmant@gmail.com"));
}

void Pursuit::updateStatusBar() {
    QString phase(m_phase == Pursuit::moveJogador ? tr("MOVA") : tr("RETIRE"));
    ui->statusbar->showMessage(tr("Fase de %1: Vez do %2")
                               .arg(phase)
        .arg(m_player->name()));
}
