#include "Track.h"

Track::Track(const QString& name, const QString& location) :
    _name{name}, _location{location} {
}

QString Track::getName() const {
    return _name;
}

QString Track::getLocation() const {
    return _location;
}

void Track::setName(const QString& name) {
    _name = name;
}

void Track::setLocation(const QString& location) {
    _location = location;
}
