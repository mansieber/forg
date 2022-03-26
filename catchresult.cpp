#include "catchresult.h"

CatchResult::CatchResult()
{
    name = "";
    count = 0;
    totalLength = 0.0;
    totalWeight = 0.0;
}

void CatchResult::setName(QString name)
{
    this->name = name;
}

void CatchResult::increaseCount()
{
    count++;
}

void CatchResult::increaseLength(double length)
{
    totalLength += length;
}

void CatchResult::increaseWeight(double weight)
{
    totalWeight += weight;
}

int CatchResult::getCount()
{
    return count;
}

QString CatchResult::getName()
{
    return name;
}

double CatchResult::getTotalLength()
{
    return totalLength;
}

double CatchResult::getTotalWeight()
{
    return totalWeight;
}

double CatchResult::getAverageLength()
{
    return totalLength / ( double ) count;
}

double CatchResult::getAverageWeight()
{
    return totalWeight / ( double ) count;
}
