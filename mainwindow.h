#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Playlist.h"

#include <QMainWindow>

class QMediaPlayer;
class QAudioOutput;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void on_play_clicked();

    void on_volumeBar_valueChanged(int value);

    void on_seekBar_sliderMoved(int position);

    void updateSeekBar(qint64);

    void on_createPlaylist_clicked();

    void on_repeatButton_stateChanged(int arg1);

    void on_nextButton_clicked();

    void on_prevButton_clicked();

    void on_shuffleButton_stateChanged(int arg1);

    void on_searchButton_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    Playlist playlist;
    QMediaPlayer* _player;
    QAudioOutput* _audioOutput;

    bool _onRepeat{false};
    bool _onShuffle{false};
    int _trackIndex{0};

    std::vector<unsigned short> _shuffledIndexes;

    void next();

    void loadTrack();

    void shufflePlaylist();

    void loadSongs();

protected:
    void keyPressEvent(QKeyEvent* parent);
};

#endif // MAINWINDOW_H
