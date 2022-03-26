#ifndef CATCHRESULT_H
#define CATCHRESULT_H

#include <QString>

/*
 * Class is used to store analysis data for later visualization
 */
class CatchResult {
public:
    CatchResult();
    void setName(QString name);
    void increaseCount();
    void increaseWeight(double weight);
    void increaseLength(double length);
    QString getName();
    int getCount();
    double getTotalWeight();
    double getTotalLength();
    double getAverageWeight();
    double getAverageLength();
private:
    QString name;
    int count;
    double totalWeight;
    double totalLength;
};

#endif // CATCHRESULT_H
