#pragma once

#include <Eigen/Dense>
#include <Eigen/Core>
#include <autodiff/forward.hpp>
#include <autodiff/forward/eigen.hpp>
#include <cmath>

namespace ast
{

using namespace Eigen;
using namespace std;

typedef double RealV;
typedef autodiff::dual Real;
typedef autodiff::VectorXdual Vec;
typedef MatrixXd Mat;


constexpr double epsilon = 0.001;



struct QBody
{
  Vec q;

  QBody() : q(3)
  {
      q<<0,0,0;
  }

  QBody(Real th, Real x, Real y) : q(3)
  {
      q<<th,x,y;
  }

  template<typename OtherDerived>
  QBody(const Eigen::MatrixBase<OtherDerived>& other) : q(other)
  { }

  template<typename OtherDerived>
  QBody& operator=(const Eigen::MatrixBase<OtherDerived>& other)
  {
     q = other;
     return *this;
  }

  Real& th() { return q[0]; }

  const Real& th() const { return q[0]; }

  Real& theta() { return q[0]; }

  const Real& theta() const { return q[0]; }

  Real& x() { return q[1]; }

  const Real& x() const { return q[1]; }

  Real& y() { return q[2]; }

  const Real& y() const { return q[2]; }


};




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

inline Real isSaturated(Real x, Real lim)
{
  return (x >= lim) || (x <= -lim);
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



//toparametrizedline from eigen
//inline Real minDistance(Vec2 A, Vec2 B, const Vec2& E)
//{
//    auto AB = B - A;
//    auto BE = B - E;
//    auto AE = E - A;
//    ;

//    // Variables to store dot product
//    Real AB_BE, AB_AE;

//    AB_BE = AB.dot(BE);
//    AB_AE = AB.dot(AE);

//    double reqAns = 0;

//    if (AB_BE > 0)
//    {
//        // Finding the magnitude
//        auto d = E - B;
//        reqAns = sqrt(sq(d.x()) + sq(d.y()));
//    }
//    else if (AB_AE < 0)
//    {
//        auto d = E- A;
//        reqAns = sqrt(sq(d.x()) + sq(d.y()));
//    }
//    else
//    {

//        // Finding the perpendicular distance
//        Real x1 = AB.x();
//        Real y1 = AB.y();
//        Real x2 = AE.x();
//        Real y2 = AE.y();
//        Real mod = sqrt(x1 * x1 + y1 * y1);
//        reqAns = abs(x1 * y2 - y1 * x2) / mod;
//    }

//    return reqAns;
//}

}
