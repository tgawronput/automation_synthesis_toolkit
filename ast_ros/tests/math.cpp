#include <catch_ros/catch.hpp>
#include <ast/math.hpp>

TEST_CASE("QBody construction")
{
    ast::QBody q(1, 2, 3);
    REQUIRE(q.th() == 1);
}
