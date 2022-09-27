#ifndef TRACK_H
#define TRACK_H

#include <string>

class Track {
public:
    Track(const std::string& name, const std::string& location);

    std::string getName() const ;

    std::string getLocation() const ;

    void setName(const std::string& name);

    void setLocation(const std::string& location);

private:
    std::string name_;
    std::string location_;
};


#endif // TRACK_H
