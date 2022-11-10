#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <random>
#include <QGraphicsRectItem>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_startButton_clicked();

    void on_resetButton_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind {HORIZONTAL,
                         LEFT_CORNER,
                         RIGHT_CORNER,
                         SQUARE,
                         STEP_UP_RIGHT,
                         PYRAMID,
                         STEP_UP_LEFT,
                         NUMBER_OF_TETROMINOS};
    // From the enum values above, only the last one is needed in this template.
    // Recall from enum type that the value of the first enumerated value is 0,
    // the second is 1, and so on.
    // Therefore the value of the last one is 7 at the moment.
    // Remove those tetromino kinds above that you do not implement,
    // whereupon the value of NUMBER_OF_TETROMINOS changes, too.
    // You can also remove all the other values, if you do not need them,
    // but most probably you need a constant value for NUMBER_OF_TETROMINOS.


    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    // More constants, attibutes, and methods

    QGraphicsRectItem* block_;
    std::vector<QGraphicsRectItem*> blocks_;
    QTimer timer_; // for continuous moving
    QTimer clock_;
    const int BLOCK_SIZE = 16;

    int second_ = 0;
    int minute_ = 0;
    int count_of_blocks_ = 0;

    // For write down location of old blocks
    std::vector<std::vector<int>> grid_;

    /**
     * @brief create_tetronom
     * @param tetrominunsignedo_kind
     * create new_tetronome to top of game field with randowm shape
     */
    void create_tetronom(int Tetrominunsignedo_kind);

    /**
     * @brief block_move_down
     * moves block step down if possible
     */
    void block_move_down();

    /**
     * @brief can_move
     * @return true if move down is possible
     * check if moving block step down is possible
     */
    bool can_move();

    /**
     * @brief create_grid
     * create the table for old blocks
     */
    void create_grid();

    /**
     * @brief add_to_grid
     * add stopped block to table for knowing it's location
     */
    void add_to_grid();

    /**
     * @brief block_move_side
     * @param side
     * moves block to wanted side if possible
     */
    void block_move_side(std::string side);

    /**
     * @brief can_move_side
     * @param side
     * @return true if moving is possible
     * check if move to wanted side is possible
     */
    bool can_move_side(std::string side);

    /**
     * @brief create_tetronom
     * @return true if no free space
     * check if there is free space on top of game field
     * for making new block there
     */
    bool is_game_ended();

    /**
     * @brief add_second
     * add second to timer for clock
     */
    void add_second();

    /**
     * @brief add_block_count
     * sum of stopped blocks
     */
    void add_block_count();


};

#endif // MAINWINDOW_HH
