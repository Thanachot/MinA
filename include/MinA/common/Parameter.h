#ifndef PARAMETER_H
#define PARAMETER_H
#include <map>
#include <iostream>
#include <cstring>
#include <memory>

class Parameter {
  public:
    Parameter();
    bool operator<(const Parameter& e) const;
    void setName(std::string na);
    std::string getName() const;
    void setStartingPoint(double sp);
    void setBoundaryLeft(double bl);
    void setBoundaryRight(double br);
    std::shared_ptr<double> getStartingPoint() const;
    std::shared_ptr<double> getBoundaryLeft() const;
    std::shared_ptr<double> getBoundaryRight() const;

  private:
    std::string name;
    std::shared_ptr<double> startingPoint;
    std::shared_ptr<double> boundaryLeft;
    std::shared_ptr<double> boundaryRight;
    std::allocator<double> alloc;
};

#endif
