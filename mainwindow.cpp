/* Tetris : My own version of tetris.
 *
 * Program author
 * Name: Matias Nurmi
 * Student number: 286031
 * UserID: nurmim
 * E-Mail: matias.nurmi@tuni.fi
 *
 * Notes: instructions in Finnish at instructions.txt
 *
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <iostream>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 100; // x coordinate
    int top_margin = 100; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is almost always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    // Add more initial settings and connect calls, when needed.

    // A non-singleshot timer fires every interval (1000) milliseconds,
    // which makes circle_move function to be called,
    // until the timer is stopped
    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::block_move_down);

    clock_.setSingleShot(false);
    connect(&clock_, &QTimer::timeout, this, &MainWindow::add_second);

    create_grid();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    if(event->key() == Qt::Key_A) {
        block_move_side("left");
    }

    if(event->key() == Qt::Key_D) {
        block_move_side("right");
    }

    if(event->key() == Qt::Key_S) {
        while(can_move()){
            block_move_down();
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    create_tetronom(distr(randomEng));
    ui->startButton->setDisabled(true);
    timer_.start(400);
    clock_.start(1000);

}

void MainWindow::create_tetronom(int Tetromino_kind)
{
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);

    //Making all small blocks first to left top corner
    //to get correct coordinates
    for (int i = 0; i < 4; i++){
        block_ = scene_->addRect(0,0,BLOCK_SIZE,BLOCK_SIZE,blackPen,redBrush);
        blocks_.push_back(block_);

    }
    //Horizontal
    if (Tetromino_kind == 0) {
        for (int i = 0; i < 4; i++){
            blocks_.at(i)->moveBy(7*BLOCK_SIZE,i*BLOCK_SIZE);
            blocks_.at(i)->setBrush(QBrush(Qt::blue));
        }
    }

    //Left corner
    if (Tetromino_kind == 1){
        blocks_.at(0)->moveBy(6*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(0)->setBrush(QBrush(Qt::gray));

        for (int i = 1; i < 4; i++){
            blocks_.at(i)->moveBy(i*BLOCK_SIZE+5*BLOCK_SIZE,1*BLOCK_SIZE);
            blocks_.at(i)->setBrush(QBrush(Qt::gray));
        }
    }

    //Right corner
    if (Tetromino_kind == 2){
        blocks_.at(0)->moveBy(8*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(0)->setBrush(QBrush(Qt::yellow));

        for (int i = 1; i < 4; i++){
            blocks_.at(i)->moveBy(i*BLOCK_SIZE+5*BLOCK_SIZE,1*BLOCK_SIZE);
            blocks_.at(i)->setBrush(QBrush(Qt::yellow));
        }
    }

    //Square
    if (Tetromino_kind == 3) {
        blocks_.at(0)->moveBy(7*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(1)->moveBy(6*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(2)->moveBy(7*BLOCK_SIZE,1*BLOCK_SIZE);
        blocks_.at(3)->moveBy(6*BLOCK_SIZE,1*BLOCK_SIZE);

        for (auto block : blocks_){
            block->setBrush(QBrush(Qt::magenta));
        }
    }

    // Right over
    if (Tetromino_kind == 4){
        blocks_.at(0)->moveBy(7*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(1)->moveBy(8*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(2)->moveBy(7*BLOCK_SIZE,1*BLOCK_SIZE);
        blocks_.at(3)->moveBy(6*BLOCK_SIZE,1*BLOCK_SIZE);

        for (auto block : blocks_){
            block->setBrush(QBrush(Qt::green));
        }
        return;
    }

    //Pyramide
    if (Tetromino_kind == 5){
        blocks_.at(3)->moveBy(7*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(3)->setBrush(QBrush(Qt::cyan));

        for (int i = 0; i < 3; i++){
            blocks_.at(i)->moveBy(i*BLOCK_SIZE+6*BLOCK_SIZE,1*BLOCK_SIZE);
            blocks_.at(i)->setBrush(QBrush(Qt::cyan));
        }
    }

    // Left over
    if (Tetromino_kind == 6){
        blocks_.at(0)->moveBy(6*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(1)->moveBy(5*BLOCK_SIZE,0*BLOCK_SIZE);
        blocks_.at(2)->moveBy(7*BLOCK_SIZE,1*BLOCK_SIZE);
        blocks_.at(3)->moveBy(6*BLOCK_SIZE,1*BLOCK_SIZE);
    }
}

void MainWindow::create_grid()
{
    std::vector<int> line;

    // One vertical line of the board.
    for (int x = 0; x <= BORDER_RIGHT/BLOCK_SIZE; ++x) {
        line.push_back(0);
    }

    // Make board by add vertical lines together.
    for (int y = 0; y <= BORDER_DOWN/BLOCK_SIZE; y++) {
        grid_.push_back(line);
    }
}

void MainWindow::add_to_grid()
{
    for(auto block : blocks_) {

        int current_y = block->y()/BLOCK_SIZE;
        int current_x = block->x()/BLOCK_SIZE;

        grid_.at(current_y).at(current_x) = 1;
    }
}

void MainWindow::block_move_side(std::string side)
{
    // If the new position is in the scene, the block can be moved there

    if (can_move_side(side)){
        for (auto block_ : blocks_) {

            // To move wanted direction
            if (side == "right") {
                block_->moveBy(BLOCK_SIZE, 0);
            } else {
                block_->moveBy(-BLOCK_SIZE, 0);
            }
        }
    }
}

bool MainWindow::can_move_side(std::string side)
{
    for (auto block_ : blocks_) {

        qreal current_y = block_->y();
        qreal current_x = block_->x();

        int ind_x = (block_->x()/BLOCK_SIZE);
        int ind_y = (block_->y()/BLOCK_SIZE);

        // New position of the block
        if (side == "right") {
            current_x += BLOCK_SIZE;
            ind_x += 1;
        } else {
            current_x -= BLOCK_SIZE;
            ind_x -= 1;
        }

        // Check the borders
        QRectF rect = scene_->sceneRect();
        if( not rect.contains(current_x, current_y)) {
            return false;
        }

        // Check the other blocks
        if (grid_.at(ind_y).at(ind_x) == 1) {
            return false;
        }
    }
    return true;
}

bool MainWindow::is_game_ended()
{
    for (int i = 5; i < 8; i++) {
        if (grid_.at(0).at(i) == 1){
            clock_.stop();
            return true;

        }
    }

    for (int i = 5; i < 8; i++) {
        if (grid_.at(1).at(i) == 1){
            clock_.stop();
            return true;

        }
    }
    return false;
}

void MainWindow::add_second()
{
    second_ += 1;

    if (second_ == 60) {
        second_ = 0;
        minute_ += 1;
    }

    ui->minLcdNumber->display(minute_);
    ui->secLcdNumber->display(second_);

}

void MainWindow::add_block_count()
{
    count_of_blocks_++;
    ui->blocksLcdNumber->display(count_of_blocks_);
}

void MainWindow::block_move_down()
{
    if (can_move()) {
        for (auto block_ : blocks_) {

            // If the new position is in the scene, the block can be moved there

            block_->moveBy(0, BLOCK_SIZE);

        }
    } else {

        // Preperation to get new block to top
        add_to_grid();
        blocks_.clear();
        add_block_count();

        if (is_game_ended()){
            ui->infoLabel->setText("GAME OVER");
            ui->infoLabel->setFrameStyle(1);
            return;
        }
        create_tetronom(distr(randomEng));
    }
}

bool MainWindow::can_move()
{
    for (auto block_ : blocks_) {

        qreal current_y = block_->y();
        qreal current_x = block_->x();

        int ind_x = (block_->x()/BLOCK_SIZE);
        int ind_y = (block_->y()/BLOCK_SIZE);

        // New position of the block
        current_y += BLOCK_SIZE;
        ind_y += 1;

        // Check the borders
        QRectF rect = scene_->sceneRect();
        if( not rect.contains(current_x, current_y)) {
            return false;
        }

        // Check the otger blocks
        if (grid_.at(ind_y).at(ind_x) == 1) {
            return false;
        }
    }

    return true;
}

void MainWindow::on_resetButton_clicked()
{
    // Reset every variable
    timer_.stop();
    clock_.stop();
    minute_ = 0;
    second_ = 0;
    count_of_blocks_ = 0;
    scene_->clear();
    blocks_.clear();

    // Reset the grid
    for (int x = 0; x <= BORDER_RIGHT/BLOCK_SIZE; ++x) {

        for (int y = 0; y <= BORDER_DOWN/BLOCK_SIZE; y++) {
            grid_.at(y).at(x) = 0;
        }
    }

    // Reset every display to zero
    ui->startButton->setEnabled(true);
    ui->minLcdNumber->display(0);
    ui->secLcdNumber->display(0);
    ui->blocksLcdNumber->display(0);
}
