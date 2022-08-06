/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 - 2022 Antonio Augusto Alves Junior
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
 * ComplexToRealFFTW.h
 *
 *  Created on: 16/11/2018
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef COMPLEXTOREALFFTW_H_
#define COMPLEXTOREALFFTW_H_

#include <hydra/detail/Config.h>
#include <hydra/detail/BackendPolicy.h>
#include <hydra/Types.h>
#include <hydra/detail/Iterable_traits.h>
#include <hydra/Range.h>
#include <hydra/Tuple.h>
#include <hydra/Complex.h>

#include <cassert>
#include <memory>
#include <utility>
#include <stdexcept>
#include <type_traits>

//FFTW3
#include <fftw3.h>

//Hydra wrappers
#include<hydra/detail/fftw/WrappersFFTW.h>
#include<hydra/detail/fftw/BaseFFTW.h>

namespace hydra {


template<typename T,
typename InputType   = typename std::conditional< std::is_same<double,T>::value, hydra::complex<double>, hydra::complex<float> >::type,
typename OutputType  = T,
typename PlanType    = detail::fftw::_Planner >
class ComplexToRealFFTW: public BaseFFTW<InputType, OutputType, PlanType >
{

public:

	ComplexToRealFFTW()=delete;

	ComplexToRealFFTW(int logical_size, unsigned flags=FFTW_ESTIMATE):
		BaseFFTW<InputType, OutputType, PlanType >(logical_size/2 +1, logical_size ,flags )
	{}

	ComplexToRealFFTW(ComplexToRealFFTW<T,InputType, OutputType, PlanType>&& other):
		BaseFFTW<InputType, OutputType, PlanType >(
				std::forward<BaseFFTW<InputType, OutputType, PlanType >&&>(other))
	{}

	ComplexToRealFFTW<T,InputType, OutputType, PlanType>&
	operator=(ComplexToRealFFTW<T,InputType, OutputType, PlanType>&& other)
	{
		if(this ==&other) return *this;

		BaseFFTW<InputType, OutputType, PlanType >::operator=(other);

		return *this;
	}

	void SetSize(int logical_size){
			this->Reset(logical_size/2+1, logical_size );
	}

	~ComplexToRealFFTW(){ }

};

}  // namespace hydra

#endif /* COMPLEXTOREALFFTW_H_ */
