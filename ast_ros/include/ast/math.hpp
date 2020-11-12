#pragma once

#include <Eigen/Dense>
#include <autodiff/forward.hpp>


namespace ast
{

using namespace Eigen;

typedef autodiff::dual Real;

using namespace Eigen;

constexpr Real epsilon = 0.001;

typedef MatrixXd Mat;
typedef VectorXd Vec;

struct QBody : public Vec3
{
  QBody() : Vec3()
  {}

  QBody(Real th, Real x, Real y) : Vec3(th, x, y)
  {}

  template<typename OtherDerived>
  QBody(const Eigen::MatrixBase<OtherDerived>& other) : Vec3(other)
  { }

  template<typename OtherDerived>
  QBody& operator=(const Eigen::MatrixBase<OtherDerived>& other)
  {
     this->Vec3::operator=(other);
     return *this;
  }

  Real& th() { return this->data()[0]; }

  const Real& th() const { return this->data()[0]; }

  Real& theta() { return this->data()[0]; }

  const Real& theta() const { return this->data()[0]; }

  Real& x() { return this->data()[1]; }

  const Real& x() const { return this->data()[1]; }

  Real& y() { return this->data()[2]; }

  const Real& y() const { return this->data()[2]; }
};


inline Real indexLookupTable(Real* table, Real val, Real offset = 0.5, Real resolution = 0.01)
{
  auto index = (int)floor((val - offset)/resolution);
  auto weight = fmod(val - offset, resolution)/resolution;
  return (1.0-weight)*table[index] + weight*table[index + 1];
}

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

inline int randomInRange(Real lb, Real ub)
{
  return lb + ((Real)std::rand()/(Real)RAND_MAX) * (ub - lb);
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

inline Real continuousAngle(Real angle, Real lastAngle)
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

class Angle
{
    Real angle;

public:
    Angle() : angle(0)
    {
    }

    void Reset()
    {
        angle = 0;
    }

    Real operator()() const { return angle; }

    operator Real() { return angle; }

    Real Discontinuous() { return discontinuousAngle(angle); }

    Real Update(Real newAngle)
    {
        angle = continuousAngle(newAngle, angle);
        return angle;
    }
};


template <typename T>
class MedianFilter
{
    std::vector<T> history;
    int windowSize_ = 0;

    T median(std::vector<T> scores)
    {
      size_t size = scores.size();

      if (size == 0)
      {
        return 0.0;
      }
      else
      {
        sort(scores.begin(), scores.end());
        if (size % 2 == 0)
        {
          return (scores[size / 2 - 1] + scores[size / 2]) / 2;
        }
        else
        {
          return scores[size / 2];
        }
      }
    }

public:
    MedianFilter(int windowSize) : windowSize_(windowSize)
    {
        history.reserve(windowSize_);
    }

    void clear()
    {
        history.clear();
    }

    T filter(T value)
    {
        if(history.size() >= windowSize_)
            history.erase(history.begin());

        history.push_back(value);
        return median(history);
    }
};

//toparametrizedline from eigen
inline Real minDistance(Vec2 A, Vec2 B, const Vec2& E)
{
    auto AB = B - A;
    auto BE = B - E;
    auto AE = E - A;
    ;

    // Variables to store dot product
    Real AB_BE, AB_AE;

    AB_BE = AB.dot(BE);
    AB_AE = AB.dot(AE);

    double reqAns = 0;

    if (AB_BE > 0)
    {
        // Finding the magnitude
        auto d = E - B;
        reqAns = sqrt(sq(d.x()) + sq(d.y()));
    }
    else if (AB_AE < 0)
    {
        auto d = E- A;
        reqAns = sqrt(sq(d.x()) + sq(d.y()));
    }
    else
    {

        // Finding the perpendicular distance
        Real x1 = AB.x();
        Real y1 = AB.y();
        Real x2 = AE.x();
        Real y2 = AE.y();
        Real mod = sqrt(x1 * x1 + y1 * y1);
        reqAns = abs(x1 * y2 - y1 * x2) / mod;
    }

    return reqAns;
}

}
