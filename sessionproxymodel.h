#ifndef SESSIONPROXYMODEL_H
#define SESSIONPROXYMODEL_H

#include <QIdentityProxyModel>

class SessionProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    enum {
        SessionId = 0,
        SessionStartTime = 1,
        SessionEndTime = 2,
        SessionLocationId = 3,
        SessionWeather = 4,
        SessionHighTime = 5,
        SessionAirPressure = 6,
        SessionPressureChange = 7,
        SessionAirTemperature = 8,
        SessionWaterTemperature = 9,
        SessionWindStrength = 10,
        SessionWindDirection = 11,
        SessionRating = 12,
        SessionDescription = 13,
        SessionCatches = 14
    };
    explicit SessionProxyModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // SESSIONPROXYMODEL_H
