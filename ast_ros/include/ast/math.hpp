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


}
