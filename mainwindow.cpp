#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMediaPlayer>
#include <QKeyEvent>
#include <QAudioOutput>
#include <QFileDialog>

#include <algorithm>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->searchWindow->setPlaceholderText("Song, artist, album");

    setFixedSize(geometry().width(), geometry().height());

    _audioOutput = new QAudioOutput;
    _audioOutput->setVolume(0.5);

    _player = new QMediaPlayer(this);
    _player->setAudioOutput(_audioOutput);

    ui->seekBar->setMaximum(1000);
    ui->volumeBar->setValue(50);

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"));
    if (!files.empty()) {
        playlist.add(files);
    }

    ui->listWidget->addItems(playlist.getTrackNameList());
    ui->listWidget->setCurrentRow(_trackIndex);

    loadTrack();
    _player->play();

    connect(_player, SIGNAL(positionChanged(qint64)), this, SLOT(updateSeekBar(qint64)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::next() {
    if (_onRepeat) {
        _player->setPosition(0);
        _player->play();
    }
}

void MainWindow::loadTrack() {
    int idx = _trackIndex;

    if (_onShuffle) {
        idx = _shuffledIndexes[_trackIndex];
    }

    QString str = playlist.at(idx).getLocation();
    _player->setSource(QUrl::fromLocalFile(str));
}


void MainWindow::shufflePlaylist() {
    _shuffledIndexes.resize(playlist.getNumberOfTracks());
    std::iota(std::begin(_shuffledIndexes), std::end(_shuffledIndexes), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(std::begin(_shuffledIndexes), std::end(_shuffledIndexes), g);
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Space :
        {
            if (_player->playbackState() == QMediaPlayer::PlayingState) {
                _player->pause();
            } else {
                _player->play();
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
        int newVal = int((float) position / _player->duration() * 1000);
        if (newVal > 0) {
            ui->seekBar->setValue(newVal);
        }
    }

    if (_player->playbackState() == QMediaPlayer::StoppedState && position != 0 ) {
        _trackIndex++;
        loadTrack();
        _player->play();
    }
}

void MainWindow::on_play_clicked() {
    if (_player->playbackState() == QMediaPlayer::PlayingState) {
        QIcon icon(":/images/icons/play-button.png");
        ui->play->setIcon(icon);
        _player->pause();
    } else {
        QIcon icon2(":/images/icons/pause.png");
        ui->play->setIcon(icon2);
        _player->play();
   }
}

void MainWindow::on_volumeBar_valueChanged(int value) {
    _audioOutput->setVolume((float) value / 100);
}

void MainWindow::on_seekBar_sliderMoved(int position) {
    _player->setPosition(_player->duration() / 1000 * position);
}

void MainWindow::on_createPlaylist_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"));

    if (!files.empty()) {
        playlist.add(files);
    }
}

void MainWindow::on_repeatButton_stateChanged(int arg1) {
    _onRepeat = !_onRepeat;
}

void MainWindow::on_nextButton_clicked() {
    if (_onRepeat) {
        _player->setPosition(0);
        _player->play();
    } else {
        ++_trackIndex;

        if (_trackIndex >= ui->listWidget->count()) {
            _trackIndex = 0;
        }

        loadTrack();
        _player->play();
    }

    (!_onShuffle or _onRepeat) ? ui->listWidget->setCurrentRow(_trackIndex) : ui->listWidget->setCurrentRow(_shuffledIndexes[_trackIndex]);
}

void MainWindow::on_prevButton_clicked() {
    if (_onRepeat) {
        _player->setPosition(0);
        _player->play();
    } else {
        --_trackIndex;

        if (_trackIndex < 0) {
            _trackIndex = ui->listWidget->count() - 1;
        }

        loadTrack();
        _player->play();
    }
    (!_onShuffle or _onRepeat) ? ui->listWidget->setCurrentRow(_trackIndex) : ui->listWidget->setCurrentRow(_shuffledIndexes[_trackIndex]);
}

void MainWindow::on_shuffleButton_stateChanged(int arg1) {
    _onShuffle = !_onShuffle;
    if (_onShuffle) {
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

void MainWindow::on_listWidget_doubleClicked(const QModelIndex& index) {
    _trackIndex = ui->listWidget->currentRow();
    ui->listWidget->setCurrentRow(_trackIndex);
    loadTrack();
    _player->play();
}

