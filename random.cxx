// random.cpp		A collection of Random number generators

//                      (c) Copyright 1995, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

// Updated to 2003 C++ standard by Shawn Waldon in 2014

static const char rcsid[] = "@(#)random.c++	1.10 11:34:07 6/9/95   EFC";

#include <cmath>
#include <ctime>

#include <random.hpp>
#include <errorstream.hpp>

RUniform::RUniform()
{
  seed(time(NULL));
}

/* uniform [a, b] random variate generator */
Real RUniform::number(const Real a, const Real b)
{
  if (a > b && ostr) {
    ErrorStream error("RUniform::number", *ostr);
    error.fatal() << "Argument Error: a (" << a << ") > b (" << b << ')'
                  << std::endl;
  }

  return (a + (b - a) * ranf());
}

IUniform::IUniform()
{
  seed(time(NULL));
}

/* random integer generator, uniform */
int IUniform::number(const int i, const int n)
{ /* return an integer in i, i+1, ... n */
  if (i > n && ostr) {
    ErrorStream error("IUniform::number", *ostr);
    error.fatal() << "Argument Error: i (" << i << "> n (" << n << ')' << std::endl;
  }

  return (int)(i + (rani() % (n - i + 1)));
}

Expntl::Expntl()
{
  seed(time(NULL));
}

/* negative exponential random variate generator */
Real Expntl::number(const Real x) { return (-x * log(ranf())); }

Erlang::Erlang()
{
  seed(time(NULL));
}

/* erlang random variate generator */
Real Erlang::number(const Real x, const Real s)
{
  int i, k;
  Real z;

  if (s > x && ostr) {
    ErrorStream error("Erlang::number", *ostr);
    error.fatal() << "Argument Error: s (" << s << " > x (" << x << ')' << std::endl;
  }

  z = x / s;
  k = (int)(z * z);
  for (i = 0, z = 1.0; i < k; i++) z *= ranf();
  return (-(x / k) * log(z));
}

Hyperx::Hyperx()
{
  seed(time(NULL));
}

/* hyperexponential random variate generator */
Real Hyperx::number(const Real x, const Real s)
{
  Real cv, z, p;

  if (s <= x && ostr) {
    ErrorStream error("Hyperx::number", *ostr);
    error.fatal() << "Argument Error: s (" << s << " not > x (" << x << ')'
                  << std::endl;
  }

  cv = s / x;
  z = cv * cv;
  p = 0.5 * (1.0 - sqrt((z - 1.0) / (z + 1.0)));
  z = (ranf() > p) ? (x / (1.0 - p)) : (x / p);

  return (-0.5 * z * log(ranf()));
}

Normal::Normal()
  : z2(0.0),
    use_z2(1)
{
  seed(time(NULL));
}

/* normal random variate generator */
Real Normal::number(const Real x,
                    const Real s) /* mean x, standard deviation s */
{
  Real v1, v2, w, z1;

  if (use_z2 && z2 != 0.0) /* use value from previous call */
  {
    z1 = z2;
    z2 = 0.0;
  } else {
    do {
      v1 = 2.0 * ranf() - 1.0;
      v2 = 2.0 * ranf() - 1.0;
      w = v1 * v1 + v2 * v2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    z1 = v1 * w;
    z2 = v2 * w;
  }

  return (x + z1 * s);
}
