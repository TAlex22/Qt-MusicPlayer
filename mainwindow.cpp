#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include <algorithm>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->searchWindow->setPlaceholderText("Song, artist, album");


    player = new QMediaPlayer(this);

    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updateSeekBar(qint64)));

    this->setFixedSize(this->geometry().width(),this->geometry().height());

    audioOutput = new QAudioOutput;

    player->setAudioOutput(audioOutput);

    ui->seekBar->setMaximum(1000);
    ui->volumeBar->setValue(50);
    audioOutput->setVolume(0.5);

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"));
    if (!files.empty()) {
        playlist.add(files);
    }

    ui->listWidget->addItems(playlist.getTrackNameList());
    ui->listWidget->setCurrentRow(index);
    loadTrack();
    player->play();

}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::next() {
    if (repeat) {
        player->setPosition(0);
        player->play();
    }
}


void MainWindow::loadTrack() {
    int idx = index;

    if (shuffle) {
        idx = shuffledIndexes[index];
    }

    QString str = QString::fromStdString(playlist.at(idx).getLocation());
    player->setSource(QUrl::fromLocalFile(str));
}


void MainWindow::shufflePlaylist() {
    shuffledIndexes.resize(playlist.getNumberOfTracks());
    std::iota(std::begin(shuffledIndexes), std::end(shuffledIndexes), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(std::begin(shuffledIndexes), std::end(shuffledIndexes), g);
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Space :
        {
            if (player->playbackState() == QMediaPlayer::PlayingState) {
                player->pause();
            } else {
                player->play();
            }
            break;
        }
        default :
        {
            break;
        }
    }
}


void MainWindow::updateSeekBar(qint64 position) {
    if(!ui->seekBar->isSliderDown()) {
        int newVal = int((float) position / player->duration() * 1000);
        if (newVal > 0) {
            ui->seekBar->setValue(newVal);
        }
    }

    if (player->playbackState() == QMediaPlayer::StoppedState && position != 0 ) {
        index++;
        loadTrack();
        player->play();
    }
}


void MainWindow::on_play_clicked() {
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        QIcon icon(":/images/play-button.png");
        ui->play->setIcon(icon);
        player->pause();
    } else {
        QIcon icon2(":/images/icons/pause.png");
        ui->play->setIcon(icon2);
        player->play();
   }
}


void MainWindow::on_volumeBar_valueChanged(int value) {
    audioOutput->setVolume((float) value / 100);
}


void MainWindow::on_seekBar_sliderMoved(int position) {
    player->setPosition(player->duration() / 1000 * position);
}


void MainWindow::on_createPlaylist_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"));

        if (!files.empty()) {
            playlist.add(files);
            QStringList songs = playlist.getTrackNameList();

            for (size_t i = 0; i < songs.size(); ++i) {
                std::cout << songs[i].toStdString() << std::endl;
            }
        }
}


void MainWindow::on_repeatButton_stateChanged(int arg1) {
    repeat = !repeat;
}


void MainWindow::on_nextButton_clicked() {
    if (repeat) {
        player->setPosition(0);
        player->play();
    } else {
        ++index;

        if (index >= ui->listWidget->count()) {
            index = 0;
        }

        loadTrack();
        player->play();
    }

    (!shuffle or repeat) ? ui->listWidget->setCurrentRow(index) : ui->listWidget->setCurrentRow(shuffledIndexes[index]);
}


void MainWindow::on_prevButton_clicked() {
    if (repeat) {
        player->setPosition(0);
        player->play();
    } else {
        --index;

        if (index < 0) {
            index = ui->listWidget->count() - 1;
        }

        loadTrack();
        player->play();
    }
    (!shuffle or repeat) ? ui->listWidget->setCurrentRow(index) : ui->listWidget->setCurrentRow(shuffledIndexes[index]);
}



void MainWindow::on_shuffleButton_stateChanged(int arg1) {
    shuffle = !shuffle;
    if (shuffle) {
        shufflePlaylist();
    }
}


void MainWindow::on_searchButton_clicked() {
    ui->searchResults->clear();
    QString find = ui->searchWindow->text();
    auto tracklist = playlist.getTrackNameList();
    auto found = tracklist.filter(find, Qt::CaseInsensitive);

    if (!found.empty()) {
         ui->searchResults->addItems(found);
    } else {
         ui->searchResults->addItem("Not found");
    }

}


void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index) {
    this->index = ui->listWidget->currentRow();
    ui->listWidget->setCurrentRow(this->index);
    loadTrack();
    player->play();
}

