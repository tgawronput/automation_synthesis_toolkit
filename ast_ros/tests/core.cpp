#include <catch_ros/catch.hpp>

#include <ast/core.hpp>
#include <ast/math.hpp>

struct ComplexType
{
  ast::Real x, y;
};

struct A
{
  int a = 10;
  ast::Out<int> data = "data";
  ast::Out<int> data2 = "data2";


  ast::In<ComplexType> complex;

  void update()
  {
    data() = ++a;
    data2() = -1;
  }
};

struct B
{
  ast::In<int> in = "data";
  ast::In<int> in2 = "data2";

  ast::Out<ComplexType> complexOut;

  int result = 0;

  void update()
  {
    result = in()*3;
    complexOut().x = 10;
  }

};


TEST_CASE("Named connections pairing")
{
  ast::Context ctx;
  auto a = ctx.build<A>();
  auto b = ctx.build<B>();
  a.update();
  b.update();

  REQUIRE(b.in2() == -1);
  REQUIRE(b.result == 33);
}

TEST_CASE("Typed connections pairing")
{
  ast::Context ctx;
  auto a = ctx.build<A>();
  auto b = ctx.build<B>();
  a.update();
  b.update();

  REQUIRE(a.complex().x == 10);
}
