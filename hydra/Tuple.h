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
 * Tuples.h
 *
 *  Created on: 10/07/2017
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef TUPLES_H_
#define TUPLES_H_

#include <hydra/detail/Config.h>
#include <hydra/detail/external/hydra_thrust/tuple.h>
#include <hydra/detail/external/hydra_thrust/detail/tuple/tuple.h>
#include <hydra/detail/utility/Generic.h>
#include <utility>

namespace hydra {
//---- type alias -----------------------

/*! \p tuple template is an alias to the variadic version of hydra_thrust::tuple
 *  and that can be instantiated with a indefinite number of arguments.
 *  Each template argument specifies the type of element in the \p tuple.
 *  Consequently, tuples are heterogeneous, fixed-size collections of values. An
 *  instantiation of \p tuple with two arguments is similar to an instantiation
 *  of \p pair with the same two arguments. Individual elements of a \p tuple may
 *  be accessed with the \p get function.
 *
 *  \tparam TN The type of the <tt>N</tt> \c tuple element.
 *
 *  The following code snippet demonstrates how to create a new \p tuple object
 *  and inspect and modify the value of its elements.
 *
 *  \code
 *  #include <hydra/tuple.h>
 *  #include <iostream>
 *  ...
 *  // create a tuple containing an int, a float, and a string
 *  hydra::tuple<int, float, const char*> t(13, 0.1f, "hydra");
 *
 *  // individual members are accessed with the free function get
 *  std::cout << "The first element's value is " << hydra::get<0>(t) << std::endl;
 *
 *  // or the member function get
 *  std::cout << "The second element's value is " << t.get<1>() << std::endl;
 *
 *  // we can also modify elements with the same function
 *  hydra::get<0>(t) += 10;
 *  \endcode
 */
template<typename... T> using tuple = hydra_thrust::tuple<T...>;

/*! \p pair  template is an alias to the hydra_thrust::pair structure.
 *
 *  \tparam T1 The type of \p pair's first object type.  There are no
 *          requirements on the type of \p T1. <tt>T1</tt>'s type is
 *          provided by <tt>pair::first_type</tt>.
 *
 *  \tparam T2 The type of \p pair's second object type.  There are no
 *          requirements on the type of \p T2. <tt>T2</tt>'s type is
 *          provided by <tt>pair::second_type</tt>.
 */
template<typename T1,typename T2> using pair  = hydra_thrust::pair<T1,T2>;

/*! The \p get function returns a \c const reference to a \p tuple element of
 *  interest.
 *
 *  \param t A reference to a \p tuple of interest.
 *  \return A \c const reference to \p t's <tt>N</tt>th element.
 *
 *  \tparam N The index of the element of interest.
 *
 *  The following code snippet demonstrates how to use \p get to print
 *  the value of a \p tuple element.
 *
 *  \code
 *  #include <hydra/Tuple.h>
 *  #include <iostream>
 *  ...
 *  hydra::tuple<int, const char *> t(13, "hydra");
 *
 *  std::cout << "The 1st value of t is " << hydra::get<0>(t) << std::endl;
 *  \endcode
 */

/*! This metafunction returns the type of a
 *  \p tuple's <tt>N</tt>th element.
 *
 *  \tparam N This parameter selects the element of interest.
 *  \tparam T A \c tuple type of interest.
 *
 */
template<int N, class T> using tuple_element = hydra_thrust::tuple_element<N,T>;

/*! This metafunction returns the number of elements
 *  of a \p tuple type of interest.
 *
 *  \tparam T A \c tuple type of interest.
 *
 */
template<class T> using tuple_size = hydra_thrust::tuple_size<T>;


/*
 * ----------- function "alias"
 */

template<int I, int N, typename T>
__hydra_host__ __hydra_device__
inline T get( T(&array)[N]) {

	return array[I];
}

template<int I,  typename T>
__hydra_host__ __hydra_device__
inline T get( T* array) {

	return array[I];
}



/*! The \p get function returns a reference to a \p tuple element of
 *  interest.
 *
 *  \param t A reference to a \p tuple of interest.
 *  \return A reference to \p t's <tt>N</tt>th element.
 *
 *  \tparam N The index of the element of interest.
 *
 *  The following code snippet demonstrates how to use \p get to print
 *  the value of a \p tuple element.
 *
 *  \code
 *  #include <hydra/Tuple.h>
 *  #include <iostream>
 *  ...
 *  hydra::tuple<int, const char *> t(13, "hydra");
 *
 *  std::cout << "The 1st value of t is " << hydra::get<0>(t) << std::endl;
 *  \endcode
 *
 */

//=====================================
//const hydra_thrust::tuple<T...>&

template<typename Type, typename ...T>
__hydra_host__ __hydra_device__
inline Type& get( hydra_thrust::tuple<T...> const& t)
{
	return hydra_thrust::get<Type>(t);
}

template<int N, typename ...T>
__hydra_host__ __hydra_device__
inline const typename hydra_thrust::tuple_element<N,hydra_thrust::tuple<T...>>::type &
get( hydra_thrust::tuple<T...> const& t)
{
	return hydra_thrust::get<N>(t);
}

//=====================================
//hydra_thrust::tuple<T...>&

template<typename Type, typename ...T>
__hydra_host__ __hydra_device__
inline Type&  get( hydra_thrust::tuple<T...>& t)
{
	return hydra_thrust::get<Type>(t);
}

template<int N, typename ...T>
__hydra_host__ __hydra_device__
inline typename hydra_thrust::tuple_element<N,hydra_thrust::tuple<T...>>::type &
get( hydra_thrust::tuple<T...>& t)
{
	return hydra_thrust::get<N>(t);
}

//=====================================
//hydra_thrust::tuple<T...>&&

template<typename Type, typename ...T>
__hydra_host__ __hydra_device__
inline Type&& get( hydra_thrust::tuple<T...>&& t)
{
	return hydra_thrust::get<Type>(std::forward<hydra_thrust::tuple<T...>>(t));
}

template<int N, typename ...T>
__hydra_host__ __hydra_device__
inline typename hydra_thrust::tuple_element<N,hydra_thrust::tuple<T...>>::type &&
get( hydra_thrust::tuple<T...>&& t)
{
	return hydra_thrust::get<N>(std::forward<hydra_thrust::tuple<T...>>(t));
}



//=====================================
// hydra_thrust::pair<T1,T2>

template<int N, typename T1,  typename T2>
__hydra_host__ __hydra_device__
inline typename hydra_thrust::tuple_element<N,hydra_thrust::pair<T1,T2>>::type &
get( hydra_thrust::pair<T1,T2>& t)
{
	return hydra_thrust::get<N>(t);
}

template<int N, typename T1,  typename T2>
__hydra_host__ __hydra_device__
inline const typename hydra_thrust::tuple_element<N,hydra_thrust::pair<T1,T2>>::type &
get( hydra_thrust::pair<T1,T2> const& t)
{
	return hydra_thrust::get<N>(t);
}


template<int N, typename T1,  typename T2>
__hydra_host__ __hydra_device__
inline typename hydra_thrust::tuple_element<N,hydra_thrust::pair<T1,T2>>::type &&
get( hydra_thrust::pair<T1,T2> && t)
{
	return hydra_thrust::get<N>(std::forward<hydra_thrust::pair<T1,T2>>(t));
}

//=============================================================

/*! This version of \p make_tuple creates a new \c tuple object from a list of
 *  objects.
 *
 *  \param T The first object to copy from.
 *  \return A \p tuple object with members which are copies of \p t.
 *
 */
template<class ...T>
__hydra_host__ __hydra_device__
inline auto make_tuple(T&&... t)
-> decltype(hydra_thrust::make_tuple( std::forward<T>(t)...))
{
	return hydra_thrust::make_tuple(std::forward<T>(t)...);
}

//======================================================
/*! This version of \p make_pair creates a new \c pair object from a list of
 *  objects.
 *
 *  \param T1 The first object to copy from.
 *  \param T2 The second object to copy from.
 *  \return A \p pair object with members which are copies of \p t.
 *
 */

template<class T1, class T2 >
__hydra_host__ __hydra_device__
inline auto make_pair( T1&& t1, T2&& t2 )
-> decltype(hydra_thrust::make_pair( std::forward<T1>(t1), std::forward<T2>(t2) ))
{
	return hydra_thrust::make_pair(std::forward<T1>(t1), std::forward<T2>(t2));
}

//======================================================
/*! This version of \p tie creates a new \c tuple whose elements are
 *  references which refers to this function's arguments.
 *
 *  \param t The objects to reference.
 *  \return A \p tuple object with members which are references to \p t.
 */

template<class ...T>
__hydra_host__ __hydra_device__
inline auto tie(T& ...t)
-> decltype(hydra_thrust::tie(t...))
{
	return hydra_thrust::tie(t...);
}

//======================================================
/*!Constructs a tuple of references to the arguments in args suitable
 * for forwarding as an argument to a function.
 * The tuple has rvalue reference data members when rvalues
 * are used as arguments, and otherwise has lvalue reference data members.
 */
template<class ...T>
__hydra_host__ __hydra_device__
inline auto forward_as_tuple(T&& ...t)
-> decltype(hydra_thrust::forward_as_tuple(std::forward<T>(t)...))
{
	return hydra_thrust::forward_as_tuple(std::forward<T>(t)...);
}



}//namespace hydra
#endif /* TUPLES_H_ */
