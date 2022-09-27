#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Track.h"
#include <QStringList>
#include <vector>

class Playlist {
public:
    Playlist();

    void add(const QStringList& files);

    void remove(int index);

    void save();

    const Track& at(int index) const;

    QStringList getTrackNameList() const;

    int getNumberOfTracks() const;

private:
    std::vector<Track> tracks_;
};


#endif // PLAYLIST_H
