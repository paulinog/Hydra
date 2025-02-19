/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 - 2023 Antonio Augusto Alves Junior
 *
 *   This file is part of Hydra Data Analysis Framework.
 *
 *   Hydra is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Hydra is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Hydra.  If not, see <http://www.gnu.org/licenses/>.
 *
 *---------------------------------------------------------------------------*/

/*
 * TrapezoidalShape.h
 *
 *  Created on: 04/09/2018
 *      Author: Antonio Augusto Alves Junior
 *
 *  Updated on: Feb 21 2020
 *      Author: Davide Brundu
 *         Log: Update call interface
 */

#ifndef TRAPEZOIDALSHAPE_H_
#define TRAPEZOIDALSHAPE_H_



#include <hydra/detail/Config.h>
#include <hydra/detail/BackendPolicy.h>
#include <hydra/Types.h>
#include <hydra/Function.h>
#include <hydra/Pdf.h>
#include <hydra/Integrator.h>
#include <hydra/detail/utility/CheckValue.h>
#include <hydra/Parameter.h>
#include <hydra/Tuple.h>
#include <tuple>
#include <limits>
#include <stdexcept>
#include <cassert>
#include <utility>

namespace hydra {
/**
 * \ingroup common_functions
 * \class TrapezoidalShape
 *
 * In probability theory and statistics, the trapezoidal distribution is a continuous probability distribution the graph
 *  of whose probability density function resembles a trapezoid. Likewise, trapezoidal distributions also roughly resemble mesas or plateaus.
 *  Each trapezoidal distribution has a lower bound 'a' and an upper bound 'd'
 *  , where 'a < d', beyond which no values or events on the distribution can occur
 *   (i.e. beyond which the probability is always zero). In addition,
 *  there are two sharp bending points (non-differentiable discontinuities) within the probability distribution,
 *  which we will call 'b' and 'c', which occur between 'a' and 'd', such that 'a ≤ b ≤ c ≤ d'
 *  Parameters:
 * * a ( a < d )  - lower bound
 * * b ( a ≤ b < c ) - level start
 * * c ( b < c ≤ d ) - level end
 * * d ( c ≤ d )- upper bound
 */
template<typename ArgType, typename Signature=double(ArgType)>
class TrapezoidalShape:public BaseFunctor<TrapezoidalShape<ArgType>, Signature, 4>
{
	using BaseFunctor<TrapezoidalShape<ArgType>,  Signature, 4>::_par;

public:

	TrapezoidalShape() = delete;

	TrapezoidalShape(Parameter const& A, Parameter const& B, Parameter const& C , Parameter const& D ):
		BaseFunctor<TrapezoidalShape<ArgType>, Signature, 4>({A,B,C,D}) {}

	__hydra_host__ __hydra_device__
	TrapezoidalShape(TrapezoidalShape<ArgType> const& other):
		BaseFunctor<TrapezoidalShape<ArgType>, Signature, 4>(other) {}

	__hydra_host__ __hydra_device__
	inline TrapezoidalShape<ArgType>&
	operator=( TrapezoidalShape<ArgType> const& other)
	{
		if(this == &other) return *this;
		BaseFunctor<TrapezoidalShape<ArgType>, Signature, 4>::operator=(other);
		return *this;
	}


	__hydra_host__ __hydra_device__
	inline double Evaluate(ArgType x)  const	{

		return  CHECK_VALUE( trapezoid(x, _par[0], _par[1], _par[2], _par[3] ),"par[0]=%f par[1]=%f par[2]=%f par[3]=%f", _par[0] , _par[1] , _par[2], _par[3] ) ;
	}


private:

	__hydra_host__ __hydra_device__
	inline double trapezoid(const double x, const double a, const double b, const double c, const double d ) const {

		double slope_left   = (x < b &&  x>=a )*2.0*(x-a)/((b-a)*(d+c-a-b));
		double slope1_right = (x <= d && x>=c )*2.0*(d-x)/((d+c-a-b)*(d-c)) ;
		double plateau      = (x < c  && x >= b)*2.0/(d+c-a-b) ;

		return slope_left + plateau + slope1_right;
	}
};

template<typename ArgType>
class IntegrationFormula< TrapezoidalShape<ArgType>, 1>
{

protected:

	inline std::pair<GReal_t, GReal_t>
	EvalFormula( TrapezoidalShape<ArgType>const& functor, double LowerLimit, double UpperLimit )const
	{

		double a = functor[0];
		double b = functor[1];
		double c = functor[2];
		double d = functor[3];

		double r  =  (cdf(a, b, c, d, UpperLimit)
				    - cdf(a, b, c, d, LowerLimit) ) ;

		return std::make_pair( CHECK_VALUE(r, "par[0]=%f par[1]=%f par[2]=%f par[2]=%f", a, b, c, d ) , 0.0);

	}

private:

	double cdf( const double a, const double b, const double c,const double d, const double x ) const {

		if(x < a) return 0.0;

		else if(x >d ) return 1.0;

		else if((x >= a)&&(x<b))
		{

			double delta = ( x - a);

			return delta*delta/((b-a)*(d+c-a-b));
		}
		else if((x >= b)&&(x<c))
		{

			return (2.0*x -a -b)/(d+c-a-b);
		}
		else if((x >= c)&&(x<=d))
		{

			double delta = (d-x);

			return 1.0 - delta*delta/((d+c-a-b)*(b-c));
		}

		return 0.0;
	}

};

template<typename ArgType>
struct RngFormula< TrapezoidalShape<ArgType> >
{

	typedef ArgType value_type;

	__hydra_host__ __hydra_device__
	unsigned NCalls( TrapezoidalShape<ArgType>const&) const
	{
		return 1;
	}

	template< typename T>
	__hydra_host__ __hydra_device__
	unsigned NCalls( std::initializer_list<T>) const
	{
		return 1;
	}

	template<typename Engine>
	__hydra_host__ __hydra_device__
	inline value_type Generate(Engine& rng, TrapezoidalShape<ArgType>const& functor) const
	{
		double a = functor[0];
		double b = functor[1];
		double c = functor[2];
		double d = functor[3];

		double denominator = (d+c-a-b);
		double f1 = (b-a)/denominator;
		double f2 = (2*c-b-a)/denominator;

		double x= RngBase::uniform(rng);

        if( x<=f1 ) x = sqrt(denominator*(b-a)*x)+a;
        else if (  x>f1 && x <=f2) x = 0.5*(denominator*x + a + b);
        else x = d - sqrt((1-x)*denominator*(d-c));

		return static_cast<value_type>(x);
	}

	template<typename Engine, typename T>
	__hydra_host__ __hydra_device__
	inline value_type Generate(Engine& rng, std::initializer_list<T> pars) const
	{
		double a = pars[0];
		double b = pars[1];
		double c = pars[2];
		double d = pars[3];

		double denominator = (d+c-a-b);
		double f1 = (b-a)/denominator;
		double f2 = (2*c-b-a)/denominator;

		double x= RngBase::uniform(rng);

		if( x<=f1 ) x = sqrt(denominator*(b-a)*x)+a;
		else if (  x>f1 && x <=f2) x = 0.5*(denominator*x + a + b);
		else x = d - sqrt((1-x)*denominator*(d-c));

		return static_cast<value_type>(x);
	}



};

}


#endif /* TRAPEZOIDALSHAPE_H_ */
