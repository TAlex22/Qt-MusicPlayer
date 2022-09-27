
#include "Playlist.h"

#include <fstream>

std::string getNameFromLocation(const std::string& location) {
    return location.substr(location.find_last_of("/") + 1);
}

Playlist::Playlist() {
    std::ifstream fin("playlist.txt");

    if (fin) {
        std::string location;

        while(getline(fin, location)) {
            std::string name =  getNameFromLocation(location);
            tracks_.push_back(Track(name, location));
        }

    } else {
        std::invalid_argument("could not open a file");
    }

}

void Playlist::add(const QStringList& files) {
    for (size_t i = 0; i < files.size(); ++i) {
        std::string location = files[i].toStdString();
        std::string name = getNameFromLocation(location);
        tracks_.push_back(Track(name, location));
    }
}

void Playlist::remove(int index) {
    tracks_.erase(tracks_.begin() + index);
}

void Playlist::save() {
    std::ofstream fout("playlist.txt");

    if (fout) {
        for (const auto& track : tracks_) {
            fout << track.getLocation() << std::endl;
        }
    } else {
        throw std::invalid_argument("could not open a file");
    }
}

QStringList Playlist::getTrackNameList() const {
    QStringList list;

    for (const auto& track : tracks_) {
        QString name = QString::fromStdString(track.getName());
        list.push_back(name);
    }

    return list;
}

const Track& Playlist::at(int index) const {
    return tracks_[index];
}

int Playlist::getNumberOfTracks() const {
    return tracks_.size();
}
