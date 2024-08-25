#ifndef PURSUIT_H
#define PURSUIT_H

#include "Cell.h"
#include <QMainWindow>

class Cell;
class Player;

QT_BEGIN_NAMESPACE
namespace Ui { class Pursuit; }
QT_END_NAMESPACE

class Pursuit : public QMainWindow {
    Q_OBJECT

public:
    Pursuit(QWidget *parent = nullptr);
    virtual ~Pursuit();

    enum Phase {
            retiraBlack,
            moveJogador
        };


    Pursuit::Phase phase() const { return m_phase; }

signals:
    void turnEnded();
     void phaseChanged(Pursuit::Phase phase);
      void gameOver();
private:
    Ui::Pursuit *ui;
    Player* m_player;
    Cell* m_board[7][7];
    QList<Cell*> m_playables;
    Phase m_phase;
     Cell* m_selected;
    bool checkGameContinue;

    void limpaPlayable(void);

    void moveplay(Cell* cell);
     void retira(Cell* cell);

    Cell* celulaJogavel(Cell* cell, Cell::Direction dir) const;
    Cell* findSelectable(Cell* cell) const;
private slots:
    void play(int id);
    void switchPlayer();
    void reset();
    void updatePlayable();
    void showAbout();
    void updateStatusBar();
    void setPhase(Pursuit::Phase phase);

};

#endif // PURSUIT_H
