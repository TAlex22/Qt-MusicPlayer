#include "Track.h"

Track::Track(const std::string& name, const std::string& location) :
    name_(name), location_(location) {
}

std::string Track::getName() const {
    return name_;
}

std::string Track::getLocation() const {
    return location_;
}

void Track::setName(const std::string& name) {
    name_ = name;
}

void Track::setLocation(const std::string& location) {
    location_ = location;
}
