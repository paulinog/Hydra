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
 * CompositeBase.h
 *
 *  Created on: 11/07/2016
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef COMPOSITE_BASE_H_
#define COMPOSITE_BASE_H_



#include <hydra/detail/Config.h>
#include <hydra/Types.h>
#include <hydra/detail/TypeTraits.h>
#include <hydra/detail/utility/Utility_Tuple.h>
#include <hydra/detail/base_functor.h>
#include <hydra/detail/Constant.h>
#include <hydra/Parameter.h>
#include <hydra/Placeholders.h>
#include <hydra/detail/TupleTraits.h>

namespace hydra {

namespace detail {

template<typename F0, typename F1, typename... Fs >
class  CompositeBase
{



public:

    typedef typename hydra::detail::merged_tuple<typename F1::argument_type>::type argument_type;
	typedef typename hydra_thrust::tuple<F0, F1, Fs...> functors_type;

	//tag
	typedef void hydra_composed_functor_type;

	CompositeBase()=delete;

	CompositeBase(F0 const& f0, F1 const& f1,  Fs const& ...fs):
		fFtorTuple(hydra_thrust::make_tuple(f0, f1, fs...)),
		fNorm(1.0)
	{ }

	__hydra_host__ __hydra_device__
	inline CompositeBase(CompositeBase<F0,F1,Fs...> const& other):
	fFtorTuple( other.GetFunctors() ),
	fNorm(other.GetNorm())
	{}

	__hydra_host__ __hydra_device__
	inline typename std::enable_if<
	std::is_copy_constructible<F0>::value &&
	std::is_copy_constructible<F1>::value &&
	detail::all_true<std::is_copy_constructible<Fs>::value...>::value,
		 CompositeBase<F0,F1,Fs...>&>::type
    operator=(CompositeBase<F0,F1,Fs...> const& other)
	{
		if(this==&other)return *this;

		fFtorTuple = other.GetFunctors();
		fNorm      = other.GetNorm();
		return *this;
	}

	inline void PrintRegisteredParameters()
	{
		HYDRA_CALLER ;
		HYDRA_MSG << "Registered parameters begin:\n" << HYDRA_ENDL;
		detail::print_parameters_in_tuple(fFtorTuple);
		HYDRA_MSG <<"Registered parameters end.\n" << HYDRA_ENDL;
		return;
	}

	inline void AddUserParameters(std::vector<hydra::Parameter*>& user_parameters )
	{
		detail::add_parameters_in_tuple(user_parameters, fFtorTuple );
	}


	inline void SetParameters(const std::vector<double>& parameters){

		detail::set_functors_in_tuple(fFtorTuple, parameters);
	}

	inline size_t  GetParametersKey(){

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		std::vector<double> _temp(_parameters.size());

		for(size_t i=0; i< _parameters.size(); i++)
			_temp[i]= *(_parameters[i]);

		size_t key = detail::hash_range(_temp.begin(), _temp.end() );

		return key;
	}

	inline size_t GetNumberOfParameters() const {

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		return _parameters.size();

	}

	template<typename Int,
	typename = typename std::enable_if<std::is_integral<Int>::value, void>::type>
	inline const hydra::Parameter& GetParameter(Int i) const {

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );


		return *(_parameters[i]);
	}

	inline const hydra::Parameter& GetParameter(const char* name) const {

		size_t i=0;

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		for(i=0; i< _parameters.size(); i++)
			if (strcmp(_parameters[i]->GetName(),name)==0) break;

		return *(_parameters[i]) ;
	}

	template<typename Int,
	typename = typename std::enable_if<std::is_integral<Int>::value, void>::type>
	inline hydra::Parameter& Parameter(Int i) {

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		return *(_parameters[i]);
	}

	inline hydra::Parameter& Parameter(const char* name) {

		size_t i=0;

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		for(i=0; i<_parameters.size(); i++)
			if (strcmp(_parameters[i]->GetName(),name)==0) break;

		return *(_parameters[i]) ;
	}


	template<typename Int,
	typename = typename std::enable_if<std::is_integral<Int>::value, void>::type>
	inline void SetParameter(Int i, hydra::Parameter const& value) {

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		*(_parameters[i])=value;
	}

	template<typename Int,
	typename = typename std::enable_if<std::is_integral<Int>::value, void>::type>
	inline void SetParameter(Int i, double value) {
		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		*(_parameters[i])=value;
	}

	inline	void SetParameter(const char* name, hydra::Parameter const& value) {

		size_t i=0;

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );

		for(i=0; i<_parameters.size(); i++)
			if (strcmp(_parameters[i]->GetName(),name)==0){
				*(_parameters[i])=value;
				break;
			}
	}

	inline	void SetParameter(const char* name, double value) {

		size_t i=0;

		std::vector<hydra::Parameter*> _parameters;
		detail::add_parameters_in_tuple(_parameters, fFtorTuple );


		for(i=0; i<_parameters.size(); i++)
			if (strcmp(_parameters[i]->GetName(),name)==0){
				*(_parameters[i])=value;
				break;
			}
	}

	__hydra_host__ __hydra_device__
	inline const functors_type& GetFunctors() const
	{
		return fFtorTuple;
	}

	template<unsigned int I>
	inline typename hydra_thrust::tuple_element<I,functors_type>::type&
	GetFunctor(hydra::placeholders::placeholder<I> const& )
	{return hydra_thrust::get<I>(fFtorTuple);}



	__hydra_host__ __hydra_device__  inline
	GReal_t GetNorm() const {
		return fNorm;
	}

	__hydra_host__ __hydra_device__  inline
	void SetNorm(GReal_t norm) {
		fNorm = norm;
	}


protected:

	functors_type fFtorTuple;

private:

	GReal_t fNorm;
};

}  // namespace detail

} // namespace hydra

#endif /* COMPOSE_H_ */
