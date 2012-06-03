//-----------------------------------------------------------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2012 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//-----------------------------------------------------------------------------

// test for arithmetic operations on VectorizedArray

#include "../tests.h"
#include <iomanip>
#include <limits>

#include <deal.II/base/vectorization.h>


template <typename Number>
void test ()
{
  // since the number of array elements is system dependent, it is not a good
  // idea to print them to an output file. Instead, check the values manually
  VectorizedArray<Number> a, b, c;
  const unsigned int n_vectors = VectorizedArray<Number>::n_array_elements;
  a = Number(2.);
  b = Number(-1.);
  for (unsigned int i=0; i<n_vectors; ++i)
    c[i] = i;

  deallog << "Addition: ";
  VectorizedArray<Number> d = a + b;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == 1, ExcInternalError());
  deallog << "OK" << std::endl
	  << "Subtraction: ";
  VectorizedArray<Number> e = d - b;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (e[i] == a[i], ExcInternalError());
  deallog << "OK" << std::endl
	  << "Multiplication: ";
  d = a * c;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == a[i] * c[i], ExcInternalError());
  deallog << "OK" << std::endl
	  << "Division: ";
  e = d / a;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (e[i] == c[i], ExcInternalError());
  deallog << "OK" << std::endl
	  << "Multiplication scalar: ";
  a = 2. * a;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (a[i] == 4., ExcInternalError()); 
  deallog << "OK" << std::endl
	  << "Division scalar left: ";
  d = 1. / a;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == 0.25, ExcInternalError());
  deallog << "OK" << std::endl
	  << "Division scalar right: ";
  e = d / 0.25;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (e[i] == 1, ExcInternalError());
  deallog << "OK" << std::endl
	  << "Unary operator -: ";
  d = -c;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == -(Number)i, ExcInternalError());
  deallog << "OK" << std::endl
	  << "Unary operator +: ";
  d = c;
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == i, ExcInternalError());


  deallog << "OK" << std::endl
	  << "Square root: ";
  d = std::sqrt(c);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (std::fabs(d[i]-std::sqrt(Number(i)))<
	    std::numeric_limits<Number>::epsilon(),
	    ExcInternalError());

  deallog << "OK" << std::endl
	  << "Absolute value: ";
  d = -c;
  d = std::abs(d);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == Number(i), ExcInternalError());
  deallog << "OK" << std::endl
	  << "Maximum value: ";
  d = std::max(a, c);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == std::max(a[i], c[i]), ExcInternalError());
  deallog << "OK" << std::endl
	  << "Minimum value: ";
  d = std::min(0.5 * a + b, c);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (d[i] == std::min(Number(0.5 * a[i] + b[i]), c[i]), ExcInternalError());

  deallog << "OK" << std::endl
	  << "Sine: ";
  e = std::sin(b);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (std::fabs(e[i]-std::sin(b[i])) <
	    10.*std::numeric_limits<Number>::epsilon(),
	    ExcInternalError());
  deallog << "OK" << std::endl
	  << "Cosine: ";
  e = std::cos(c);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (std::fabs(e[i]-std::cos(c[i])) <
	    10.*std::numeric_limits<Number>::epsilon(),
	    ExcInternalError());
  deallog << "OK" << std::endl
	  << "Tangent: ";
  d = std::tan(e);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (std::fabs(d[i]-std::tan(e[i])) <
	    10.*std::numeric_limits<Number>::epsilon(),
	    ExcInternalError());
  deallog << "OK" << std::endl
	  << "Exponential: ";
  d = std::exp(c-a);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (std::fabs(d[i]-std::exp(c[i]-a[i])) <
	    10.*std::numeric_limits<Number>::epsilon(),
	    ExcInternalError());
  deallog << "OK" << std::endl
	  << "Logarithm: ";
  e = std::log(d);
  for (unsigned int i=0; i<n_vectors; ++i)
    Assert (std::fabs(e[i]-(c[i]-a[i])) <
	    10.*std::numeric_limits<Number>::epsilon(),
	    ExcInternalError());
  deallog << "OK" << std::endl;
}




int main()
{
  std::ofstream logfile("vectorization_04/output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  deallog.push("double");
  test<double> ();
  deallog.pop();
  deallog.push("float");
  test<float> ();
  deallog.pop();

				// test long double: in that case, the default
				// path of VectorizedArray is taken no matter
				// what was done for double or float
  deallog.push("long double");
  test<float> ();
  deallog.pop();
}