#pragma once

#include <Eigen/Dense>
#include <Eigen/Core>
#include <autodiff/forward.hpp>
#include <autodiff/forward/eigen.hpp>
#include <cmath>

namespace ast
{
typedef autodiff::dual Real;
typedef autodiff::VectorXdual Vec;

typedef double RealV;
typedef Eigen::MatrixXd Mat;
typedef Eigen::VectorXd VecV;


constexpr RealV epsilon = 0.001;


using namespace Eigen;
using namespace autodiff;

//struct QBody : public Vec3
//{
//  QBody() : Vec3()
//  {}

//  QBody(Real th, Real x, Real y) : Vec3(th, x, y)
//  {}

//  template<typename OtherDerived>
//  QBody(const Eigen::MatrixBase<OtherDerived>& other) : Vec3(other)
//  { }

//  template<typename OtherDerived>
//  QBody& operator=(const Eigen::MatrixBase<OtherDerived>& other)
//  {
//     this->Vec3::operator=(other);
//     return *this;
//  }

//  Real& th() { return this->data()[0]; }

//  const Real& th() const { return this->data()[0]; }

//  Real& theta() { return this->data()[0]; }

//  const Real& theta() const { return this->data()[0]; }

//  Real& x() { return this->data()[1]; }

//  const Real& x() const { return this->data()[1]; }

//  Real& y() { return this->data()[2]; }

//  const Real& y() const { return this->data()[2]; }
//};


inline Real sat(Real x, Real lim)
{
  if(x > lim)
    return lim;
  else if(x < -lim)
    return - lim;
  else
    return x;
}

inline Real clipToRange(Real x, Real rMin, Real rMax)
{
  if(x < rMin)
    return  rMin;
  else if (x > rMax)
    return rMax;
  else return x;
}

inline Real sq(Real x)
{
  return x*x;
}

inline Real cube(Real x)
{
  return x*x*x;
}

inline int sign(Real val)
{
  return (0 < val) - (val < 0);
}

template <typename T>
inline Real distance(const T& a , const T& b)
{
  return (b-a).norm();
}

inline RealV continuousAngle(RealV angle, RealV lastAngle)
{
    auto dAngle = fmod(angle, 2*M_PI) - fmod(lastAngle, 2*M_PI);

    if (dAngle > M_PI)
        return lastAngle + dAngle - 2.0 * M_PI;
    else if (dAngle < -M_PI)
        return lastAngle + dAngle + 2.0 * M_PI;
    else
        return lastAngle + dAngle;
}

inline Real discontinuousAngle(Real angle)
{
    return atan2(sin(angle), cos(angle));
}


}
