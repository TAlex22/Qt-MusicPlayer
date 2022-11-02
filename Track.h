#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track {
public:
    Track(const QString& name, const QString& location);

    QString getName() const ;

    QString getLocation() const ;

    void setName(const QString& name);

    void setLocation(const QString& location);

private:
    QString _name;
    QString _location;
};


#endif // TRACK_H
