#pragma once

#include <ast/core.hpp>
#include <ast/math.hpp>

namespace ast
{
    struct Integrator
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

}
