#include "Cell.h"
#include "Player.h"


const QList<Cell::Direction> Cell::directions =
    QList<Cell::Direction>()
        << Cell::North
        << Cell::NorthEast
        << Cell::East
        << Cell::SouthEast
        << Cell::South
        << Cell::SouthWest
        << Cell::West
        << Cell::NorthWest;

Cell::Cell(QWidget *parent)
        : QPushButton(parent),
          m_row(-1), m_col(-1),
          m_playable(false),
          m_player(nullptr),
          m_state(Cell::Empty) {
    QObject::connect(this, SIGNAL(playableChanged(bool)), this, SLOT(updateCell()));
    QObject::connect(this, SIGNAL(playerChanged(Player*)), this, SLOT(updateCell()));
    QObject::connect(this, SIGNAL(stateChanged(Cell::State)), this, SLOT(updateCell()));

    this->updateCell();
}

Cell::~Cell() {
}

void Cell::setPlayable(bool playable) {
    if (m_playable != playable) {
        m_playable = playable;
        emit playableChanged(playable);
    }
}

void Cell::setPlayer(Player* player) {
    if (m_player != player) {
        m_player = player;
        this->setState(player != nullptr ? Cell::Used : Cell::Empty);
        emit playerChanged(player);
    }
}

void Cell::setState(Cell::State state) {
    if (m_state != state) {
        switch (state) {
            case Cell::Empty:
            case Cell::Blocked:

                Q_ASSERT(m_player == nullptr);
                break;
            case Cell::Used:
                Q_ASSERT(m_player != nullptr);
                break;


            default:
                Q_UNREACHABLE();
        }

        m_state = state;
        emit stateChanged(state);
    }
}

void Cell::reset() {
    m_playable = false;
    m_player = nullptr;
    m_state = Cell::Empty;

    this->updateCell();
}

void Cell::updateCell() {
    QPixmap pixmap;
    switch (m_state) {
        case Cell::Empty:
            break;
        case Cell::Blocked:
            pixmap = QPixmap(":/black");
            break;

           break;
        case Cell::Used:
            pixmap = m_player->pixmap();
            break;
        default:
            Q_UNREACHABLE();
    }

    // Set icon.
    this->setIcon(pixmap);

    // Update the stylesheet.
    this->setStyleSheet(this->styleSheet());
}
