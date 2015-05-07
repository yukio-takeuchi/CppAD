/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

bool acosh(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// 10 times machine epsilon
	double eps = 200. * std::numeric_limits<double>::epsilon();

	// domain space vector
	size_t n  = 1;
	double x0 = 0.5;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0]     = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// a temporary value
	AD<double> cosh_of_x0 = CppAD::cosh(ax[0]);

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	ay[0] = CppAD::acosh(cosh_of_x0);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// check value
	ok &= NearEqual(ay[0] , x0,  eps, eps);

	// forward computation of first partial w.r.t. x[0]
	CPPAD_TESTVECTOR(double) dx(n);
	CPPAD_TESTVECTOR(double) dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 1., eps, eps);

	// forward computation of higher order partials w.r.t. x[0]
	size_t n_order = 5;
	for(size_t order = 2; order < n_order; order++)
	{	dx[0] = 0.;
		dy    = f.Forward(order, dx);
		ok   &= NearEqual(dy[0], 0., eps, eps);
	}
	// reverse computation of derivatives
	CPPAD_TESTVECTOR(double)  w(m);
	CPPAD_TESTVECTOR(double) dw(n_order * n);
	w[0]  = 1.;
	dw    = f.Reverse(n_order, w);
	ok   &= NearEqual(dw[0], 1., eps, eps);
	for(size_t order = 1; order < n_order; order++)
		ok   &= NearEqual(dw[order * n + 0], 0., eps, eps);

	return ok;
}

// END C++
