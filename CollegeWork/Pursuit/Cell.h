#ifndef CELL_H
#define CELL_H

#include <QPushButton>

class Player;

class Cell : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow)
    Q_PROPERTY(int col READ col WRITE setCol)
    Q_PROPERTY(bool playable READ isPlayable WRITE setPlayable NOTIFY playableChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(Cell::State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool empty READ isEmpty)
    Q_PROPERTY(bool blocked READ isBlocked)
    Q_PROPERTY(bool used READ isUsed)

public:
    enum State {
        Empty,
        Blocked,
        Used,
        Selected
    };
    Q_ENUM(State)

    enum Direction {
            North,
            NorthEast,
            East,
            SouthEast,
            South,
            SouthWest,
            West,
            NorthWest
    };
    Q_ENUM(Direction)


    static const QList<Cell::Direction> directions;

    explicit Cell(QWidget *parent = nullptr);
    virtual ~Cell();

    int row() const { return m_row; }
    void setRow(int row) { m_row = row; }

    int col() const { return m_col; }
    void setCol(int col) { m_col = col; }

    bool isPlayable() const { return m_playable; }

    Player* player() const { return m_player; }
    bool hasPlayer() const { return m_player != nullptr; }

    Cell::State state() const { return m_state; }
    bool isEmpty() const { return m_state == Cell::Empty; }
    bool isBlocked() const { return m_state == Cell::Blocked; }
    bool isUsed() const { return m_state == Cell::Used; }
    bool isSelected() const { return m_state == State::Selected; }
public slots:
    void setPlayable(bool playable);
    void setPlayer(Player* player);
    void setState(Cell::State state);
    void reset();

signals:
    void playableChanged(bool playable);
    void playerChanged(Player* player);
    void stateChanged(Cell::State state);

private:
    int m_row, m_col;
    bool m_playable;
    Player* m_player;
    Cell::State m_state;

private slots:
    void updateCell();

};

#endif // CELL_H
