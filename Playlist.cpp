
#include "Playlist.h"
#include "Track.h"

#include <fstream>

QString getNameFromLocation(const std::string& location) {
    return QString::fromStdString(location.substr(location.find_last_of("/") + 1));
}

Playlist::Playlist() {
    std::ifstream fin("playlist.txt");

    if (fin) {
        std::string location;

        while(getline(fin, location)) {
            QString name = getNameFromLocation(location);
            _tracks.push_back(Track(name, QString::fromStdString(location)));
        }

    } else {
        std::invalid_argument("could not open a file");
    }

}

void Playlist::add(const QStringList& files) {
    for (size_t i = 0; i < files.size(); ++i) {
        QString location = files[i];
        QString name = getNameFromLocation(location.toStdString());
        _tracks.push_back(Track(name, location));
    }
}

void Playlist::remove(int index) {
    _tracks.erase(_tracks.begin() + index);
}

void Playlist::save() {
    std::ofstream fout("playlist.txt");

    if (fout) {
        for (const auto& track : _tracks) {
            fout << track.getLocation().toStdString() << std::endl;
        }
    } else {
        throw std::invalid_argument("could not open a file");
    }
}

QStringList Playlist::getTrackNameList() const {
    QStringList list;

    for (const auto& track : _tracks) {
        QString name = track.getName();
        list.push_back(name);
    }

    return list;
}

const Track& Playlist::at(int index) const {
    return _tracks[index];
}

int Playlist::getNumberOfTracks() const {
    return _tracks.size();
}
