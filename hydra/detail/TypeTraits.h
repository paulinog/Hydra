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
 * TypeTraits.h
 *
 *  Created on: 09/06/2016
 *      Author: Antonio Augusto Alves Junior
 */
/**
 * \file
 * \ingroup generic
 */
#ifndef TYPETRAITS_H_
#define TYPETRAITS_H_


#include <hydra/detail/Config.h>


#include <type_traits>
#include <limits>

#include <hydra/detail/external/hydra_thrust/complex.h>
#include <hydra/detail/external/hydra_thrust/detail/type_traits.h>
#include <hydra/detail/external/hydra_thrust/execution_policy.h>
#include <hydra/detail/external/hydra_thrust/iterator/iterator_categories.h>
#include <hydra/detail/external/hydra_thrust/iterator/detail/is_iterator_category.h>
#include <hydra/detail/external/hydra_thrust/iterator/iterator_traits.h>
#include <hydra/detail/external/hydra_thrust/device_reference.h>
#include <hydra/detail/external/hydra_thrust/detail/raw_reference_cast.h>
#include <hydra/detail/external/hydra_thrust/type_traits/void_t.h>
namespace std {

		template<class T, class U>
		struct common_type<hydra_thrust::complex<T>, hydra_thrust::complex<U> > {
			typedef hydra_thrust::complex<typename common_type<T, U>::type > type;
		};

		template<class T, class U>
		struct common_type<T, hydra_thrust::complex<U> > {
			typedef hydra_thrust::complex<typename common_type<T, U>::type> type;
		};

		template<class T, class U>
		struct common_type<hydra_thrust::complex<U>, T > {
			typedef hydra_thrust::complex<typename common_type<T, U>::type > type;
		};
}


namespace hydra {

	namespace detail {
	//this type trait should be defined in thrust
	//I will suggest if one day
	template <typename T, typename = void>
	struct is_iterator : std::false_type { };

	template <typename T>
	struct is_iterator<T,
	hydra_thrust::void_t<
	           typename  std::enable_if<std::is_default_constructible<T>::value, void>::type, //default constructible,
	           typename  std::enable_if<std::is_copy_constructible<T>::value, void>::type,    //copy constructible,
	           typename  std::enable_if<std::is_destructible<T>::value, void>::type,          //destructible,
	           decltype(std::declval<T&>()[0]),                      // offset dereference operator ([]),
	           decltype(std::declval<T&>()-std::declval<T&>()),      // Supports arithmetic operator -
	           decltype(++std::declval<T&>()),                       // incrementable,
	           decltype(*std::declval<T&>()),                        // dereferencable,
	           decltype(std::declval<T&>() == std::declval<T&>()),  // comparable
	           decltype(std::declval<T&>() != std::declval<T&>())>>  // comparable
	    : std::true_type { };

	//this type trait should be defined in thrust
	//I will suggest if one day
	template<typename T>
	struct is_device_reference: std::false_type
	{
	//	typedef T type;
	};

	template<typename T>
	struct is_device_reference<hydra_thrust::device_reference<T>>: std::true_type
	{
	//	typedef T type;
	};

    template<typename T>
    struct remove_device_reference
    {
    		typedef T type;
    };

    template<typename T>
    struct remove_device_reference<hydra_thrust::device_reference<T>>
    {
    	typedef T type;
    };


    template<typename T>
    struct remove_device_reference<T&>
    {
    	typedef T type;
    };



	//----------------------
	template<bool Condition, template<typename ...> class T1, template<typename ...> class T2>
	struct if_then_else_tt;

	template<template<typename ...> class T1, template<typename ...> class T2>
	struct if_then_else_tt<true, T1, T2>
	{
		template<typename ...T>
		using type=T1<T...>;
	};

	template<template<typename ...> class T1, template<typename ...> class T2>
	struct if_then_else_tt<false, T1, T2>
	{
		template<typename ...T>
		using type=T2<T...>;
	};


	//----------------------

	template<bool C, typename T1, typename T2>
	class if_then_else;

	template<typename T1, typename T2>
	class if_then_else<true, T1, T2>
	{
	public:
		typedef T1 type;
	};

	template<typename T1, typename T2>
	class if_then_else<false, T1, T2>
	{
	public:
		typedef T2 type;
	};
		enum {kInvalidNumber = -111};

	template<typename T>
	struct TypeTraits
	{

		typedef T type;
		__hydra_host__  __hydra_device__ inline static type zero(){ return type(0.0) ;}
		__hydra_host__  __hydra_device__ inline static type one(){ return type(1.0) ;}
		__hydra_host__  __hydra_device__ inline static type invalid(){ return std::numeric_limits<T>::quiet_NaN() ;}

	};

	template<typename T>
	struct TypeTraits<hydra_thrust::complex<T>>
	{

		typedef hydra_thrust::complex<T> type;
		__hydra_host__  __hydra_device__ inline static type zero(){ return type(0.0,0.0) ;}
		__hydra_host__  __hydra_device__ inline static type one(){ return type(1.0, 0.0) ;}
		__hydra_host__  __hydra_device__ inline static type invalid(){ return  std::numeric_limits<T>::quiet_NaN()  ;}

	};


		//----------------------
		template< class... T >
		using common_type_t = typename std::common_type<T...>::type;



	 template <typename T>
		struct function_traits
			: public function_traits<decltype(&T::operator())>
		{};

		template <typename ClassType, typename ReturnType, typename... Args>
			struct function_traits<ReturnType(ClassType::*)(Args...) const>
			{
				enum { argument_count = sizeof...(Args) };

				typedef ReturnType return_type;
				typedef hydra_thrust::tuple<Args...> args_type;

				template <size_t i>
				struct arg
				{
					typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
				 };
			};


		template <typename ClassType, typename ReturnType, typename... Args>
			struct function_traits<ReturnType(ClassType::*)(Args&...)>
			{
				enum { argument_count = sizeof...(Args) };

				typedef ReturnType return_type;
				typedef hydra_thrust::tuple<Args&...> args_type;

				template <size_t i>
				struct arg
				{
					typedef typename std::tuple_element<i, std::tuple<Args&...>>::type type;
				 };
			};

		// check if given type is specialization of a certain template REF

		template<typename T, template<typename...> class REF>
		struct is_specialization : std::false_type {};

		template<template<typename...> class REF, typename... Args>
		struct is_specialization<REF<Args...>, REF>: std::true_type {};


}

}

#endif /* TYPETRAITS_H_ */
