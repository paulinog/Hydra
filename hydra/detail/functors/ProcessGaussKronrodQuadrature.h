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
 * ProcessGaussKronrodQuadrature.h
 *
 *  Created on: 02/02/2017
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef PROCESSGAUSSKRONRODQUADRATURE_H_
#define PROCESSGAUSSKRONRODQUADRATURE_H_

#include <hydra/detail/Config.h>
#include <hydra/Types.h>


namespace hydra {

struct GaussKronrodCall
{
	__hydra_host__ __hydra_device__ inline
	GaussKronrodCall():
		fGaussCall(0),
		fGaussKronrodCall(0)
	{}

	__hydra_host__ __hydra_device__ inline
	GaussKronrodCall(GReal_t gaussCall, GReal_t gaussKronrodCall):
		fGaussCall( gaussCall ),
		fGaussKronrodCall(gaussKronrodCall)
	{}
	__hydra_host__ __hydra_device__ inline
	GaussKronrodCall(GaussKronrodCall const& other):
		fGaussCall(other.fGaussCall),
		fGaussKronrodCall(other.fGaussKronrodCall ){}

	__hydra_host__ __hydra_device__ inline
	GaussKronrodCall& operator=(GaussKronrodCall const& other)
	{
		if( this == &other) return *this;

		fGaussCall = other.fGaussCall;
		fGaussKronrodCall = other.fGaussKronrodCall ;
		return *this;
	}

	GReal_t fGaussCall;
	GReal_t fGaussKronrodCall;

};

template <typename FUNCTOR>
struct GaussKronrodUnary
{
	GaussKronrodUnary()=delete;

	GaussKronrodUnary(FUNCTOR const& functor):
	fFunctor(functor)
	{}

	__hydra_host__ __hydra_device__ inline
	GaussKronrodUnary(GaussKronrodUnary<FUNCTOR> const& other ):
	fFunctor(other.fFunctor)
	{}

	__hydra_host__ __hydra_device__ inline
	GaussKronrodUnary& operator=(GaussKronrodUnary<FUNCTOR> const& other )
	{
		if( this== &other) return *this;

		fFunctor=other.fFunctor;
		return *this;
	}

	template<typename T>
	__hydra_host__ __hydra_device__ inline
	GaussKronrodCall operator()(T row)
	{
		GReal_t abscissa_X_P               = hydra_thrust::get<0>(row);
		GReal_t abscissa_X_M               = hydra_thrust::get<1>(row);
		GReal_t abscissa_Weight          = hydra_thrust::get<2>(row);
		GReal_t rule_GaussKronrod_Weight = hydra_thrust::get<3>(row);
		GReal_t rule_Gauss_Weight        = hydra_thrust::get<4>(row);

		GaussKronrodCall result;

		GReal_t function_call    = abscissa_Weight*(fFunctor(abscissa_X_M) + fFunctor(abscissa_X_P) ) ;

		result.fGaussCall        = function_call*rule_Gauss_Weight;
		result.fGaussKronrodCall = function_call*rule_GaussKronrod_Weight;

		return result;
	}

	FUNCTOR fFunctor;

};


struct GaussKronrodBinary: public hydra_thrust::binary_function<GaussKronrodCall const&,
		GaussKronrodCall const&, GaussKronrodCall>
{
	 __hydra_host__ __hydra_device__ inline
	 GaussKronrodCall  operator()( GaussKronrodCall const& x, GaussKronrodCall const& y)
	 {
		 GaussKronrodCall result;

		 result.fGaussCall         =  x.fGaussCall + y.fGaussCall;
		 result.fGaussKronrodCall  =  x.fGaussKronrodCall + y.fGaussKronrodCall;

		 return result;
	 }


};


}  // namespace hydra



#endif /* PROCESSGAUSSKRONRODQUADRATURE_H_ */
