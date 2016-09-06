#include "MinA/common/Parameter.h"
using namespace std;

Parameter::Parameter()
{
    startingPoint = nullptr;
    boundaryLeft = nullptr;
    boundaryRight = nullptr;
}

bool Parameter::operator<(const Parameter& e) const
{
    return strcmp(getName().c_str(), e.getName().c_str()) < 0;
}

void Parameter::setName(string na)
{
    name = na;
}

string Parameter::getName() const
{
    return name;
}

void Parameter::setStartingPoint(double sp)
{
    startingPoint = std::allocate_shared<double>(alloc, sp);
}

void Parameter::setBoundaryLeft(double bl)
{
    boundaryLeft = std::allocate_shared<double>(alloc, bl);
}

void Parameter::setBoundaryRight(double br)
{
    boundaryRight = std::allocate_shared<double>(alloc, br);
}

std::shared_ptr<double> Parameter::getStartingPoint() const
{
    return startingPoint;
}

std::shared_ptr<double> Parameter::getBoundaryLeft() const
{
    return boundaryLeft;
}

std::shared_ptr<double> Parameter::getBoundaryRight() const
{
    return boundaryRight;
}
