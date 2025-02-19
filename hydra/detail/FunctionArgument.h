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
 * FunctionArgument.h
 *
 *  Created on: Feb 11, 2020
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef FUNCTIONARGUMENT_H_
#define FUNCTIONARGUMENT_H_

#include <hydra/detail/Config.h>
#include <hydra/detail/ArgumentTraits.h>

namespace hydra {

namespace detail {

template<typename Derived, typename Type>
struct FunctionArgument
{
    typedef Type   value_type;
    typedef Derived name_type;
    typedef void function_argument_type ;

    FunctionArgument() = default;

    __hydra_host__ __hydra_device__
    explicit FunctionArgument(value_type x) :
     value(x)
     {}

    __hydra_host__ __hydra_device__
    explicit FunctionArgument(hydra_thrust::device_reference<value_type> x) :
     value(x)
     {}


    __hydra_host__ __hydra_device__
    explicit  FunctionArgument(FunctionArgument<name_type, value_type>const& other):
     value(other.Value())
     {}

    __hydra_host__ __hydra_device__
    explicit  FunctionArgument(hydra_thrust::device_reference<name_type>const& other)
     {
    	name_type a=other;
        value = a.Value();
     }

    __hydra_host__ __hydra_device__
    FunctionArgument<name_type, value_type>&
    operator=(FunctionArgument<name_type, value_type>const& other)
    {
        if(this==&other) return *this;
        value = other();
        return *this;
    }

    template<typename T>
    __hydra_host__ __hydra_device__
    typename  std::enable_if< std::is_convertible<T, value_type>::value && (!detail::is_function_argument<T>::value),
    FunctionArgument<name_type, value_type>&>::type
    operator=(T other)
    {
    	value = other;
    	return *this;
    }

    template<typename T>
    __hydra_host__ __hydra_device__
    typename  std::enable_if< std::is_convertible<T, value_type>::value && (!detail::is_function_argument<T>::value),
       FunctionArgument<name_type, value_type>&>::type
    operator=(hydra_thrust::device_reference<T> const& other)
    {
    	value = other;
    	return *this;
    }

    template<typename Derived2>
    __hydra_host__ __hydra_device__
    FunctionArgument(FunctionArgument<Derived2, value_type>const& other):
     value(other())
     {}


    template<typename Derived2>
    __hydra_host__ __hydra_device__
    FunctionArgument<name_type, value_type>&
    operator=(FunctionArgument<Derived2, value_type>const& other)
    {

        value = other();
        return *this;
    }

      __hydra_host__ __hydra_device__
      operator value_type() const { return value; }

      __hydra_host__ __hydra_device__
     operator  value_type&()  { return value; }

    __hydra_host__ __hydra_device__
    value_type operator()(void) const { return value; }

    __hydra_host__ __hydra_device__
    value_type Value(void) const { return value; }

    //=============================================================
    //Compound assignment operators
    //=============================================================

     __hydra_host__ __hydra_device__
    FunctionArgument<Derived, Type>&
    operator+=( Type other)
	{
        value+=other;
		return *this;
	}

    __hydra_host__ __hydra_device__
    FunctionArgument<Derived, Type>&
    operator-=( Type other)
	{
        value-=other;
		return *this;
	}

    __hydra_host__ __hydra_device__
    FunctionArgument<Derived, Type>&
    operator*=( Type other)
	{
        value*=other;
		return *this;
	}

    __hydra_host__ __hydra_device__
    FunctionArgument<Derived, Type>&
    operator/=( Type other)
	{
        value/=other;
		return *this;
	}

    __hydra_host__ __hydra_device__
    FunctionArgument<Derived, Type>&
    operator%=( Type other)
	{
        value%=other;
		return *this;
	}

    //=============================================================
    //Compound assignment operators
    //=============================================================

    template<typename Derived2, typename Type2>
    __hydra_host__ __hydra_device__
    typename std::enable_if<std::is_convertible<Type, Type2>::value,
    FunctionArgument<Derived, Type>&>::type
    operator+=( FunctionArgument<Derived2, Type2> const & other)
	{
        value+=other();
		return *this;
	}

    template<typename Derived2, typename Type2>
    __hydra_host__ __hydra_device__
    typename std::enable_if<std::is_convertible<Type, Type2>::value,
    FunctionArgument<Derived, Type>&>::type
    operator-=( FunctionArgument<Derived2, Type2> const & other)
	{
        value-=other();
		return *this;
	}

    template<typename Derived2, typename Type2>
    __hydra_host__ __hydra_device__
    typename std::enable_if<std::is_convertible<Type, Type2>::value,
    FunctionArgument<Derived, Type>&>::type
    operator*=( FunctionArgument<Derived2, Type2> const & other)
	{
        value*=other();
		return *this;
	}

    template<typename Derived2, typename Type2>
    __hydra_host__ __hydra_device__
    typename std::enable_if<std::is_convertible<Type, Type2>::value,
    FunctionArgument<Derived, Type>&>::type
    operator/=( FunctionArgument<Derived2, Type2> const & other)
	{
        value/=other();
		return *this;
	}


    template<typename Derived2, typename Type2>
    __hydra_host__ __hydra_device__
    typename std::enable_if<std::is_convertible<Type, Type2>::value,
    FunctionArgument<Derived, Type>&>::type
    operator%=( FunctionArgument<Derived2, Type2> const & other)
	{
        value%=other();
		return *this;
	}

    private:

    value_type value;
};


}  // namespace detail



}  // namespace hydra

#define declarg(NAME, TYPE )                                           \
namespace hydra {	namespace arguments  { 							   \
																	   \
struct NAME : detail::FunctionArgument<NAME, TYPE>                     \
{                                                                      \
 typedef  detail::FunctionArgument<NAME, TYPE>  super_type;            \
                                                                       \
  NAME()=default;                                                      \
                                                                       \
  __hydra_host__ __hydra_device__	                                   \
  NAME( TYPE x):                                                       \
     super_type(x)                                                     \
     {}                                                                \
                                                                       \
  __hydra_host__ __hydra_device__	                                   \
  NAME( hydra_thrust::device_reference<TYPE> x):                       \
         super_type(x)                                                 \
         {}                                                            \
                                                                       \
 __hydra_host__ __hydra_device__	                                   \
 explicit  NAME( hydra_thrust::device_reference<TYPE>& x):             \
        super_type(x)                                                  \
       {}                                                              \
                                                                       \
  __hydra_host__ __hydra_device__                                      \
  NAME( NAME const& other):                                            \
    super_type(other)                                                  \
    {}                                                                 \
                                                                       \
    __hydra_host__ __hydra_device__                                    \
    explicit NAME( hydra_thrust::device_reference<TYPE> const& other): \
         super_type(other)                                             \
         {}                                                            \
                                                                       \
  __hydra_host__ __hydra_device__                                      \
  explicit NAME( hydra_thrust::device_reference<NAME> const& other):   \
       super_type(other)                                               \
       {}                                                              \
                                                                       \
  template<typename T,                                                 \
       typename = typename std::enable_if<                             \
        std::is_base_of< detail::FunctionArgument<T, TYPE>, T>::value, \
        void >::type >  											   \
__hydra_host__ __hydra_device__										   \
  NAME( T const& other):                                               \
    super_type(other)                                                  \
    {}                                                                 \
                                                                       \
    __hydra_host__ __hydra_device__	                                   \
  NAME& operator=(NAME const& other)                                   \
  {                                                                    \
        if(this==&other)                                               \
         return *this;                                                 \
                                                                       \
        super_type::operator=(other);                                  \
                                                                       \
        return *this;                                                  \
  }                                                                    \
                                                                       \
  template<typename T>                                                 \
  typename std::enable_if<                                             \
        std::is_base_of< detail::FunctionArgument<T, TYPE>, T>::value, \
        NAME& >::type                                                  \
  __hydra_host__ __hydra_device__                                      \
  operator=(T const& other)                                            \
  {                                                                    \
                                                                       \
        super_type::operator=(other);                                  \
                                                                       \
        return *this;                                                  \
  }                                                                    \
                                                                       \
};                                                                     \
                                                                       \
} /*namespace arguments*/ }/*namespace hydra*/                         \

namespace hydra {

namespace arguments  {

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()+  std::declval<Arg2>() )>::type
operator+( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()+
			 std::declval<Arg2>() ) return_type;

	return a1.Value() + a2;
}

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()+  std::declval<Arg2>() )>::type
operator+(Arg2 const& a2, Arg1 const& a1 ) {

	typedef decltype(std::declval<typename  Arg1::value_type>()+
			 std::declval<Arg2>() ) return_type;

	return a1.Value() + a2;
}


template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument_pack<Arg1, Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()+
		 std::declval<typename  Arg2::value_type>() )>::type
operator+( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()+
			 std::declval<typename  Arg2::value_type>() ) return_type;

	return a1.Value() + a2.Value();
}

//---------------

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()- std::declval<Arg2>() )>::type
operator-( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()-
			 std::declval<Arg2>() ) return_type;

	return a1.Value() - a2;
}

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()-  std::declval<Arg2>() )>::type
operator-(Arg2 const& a2, Arg1 const& a1 ) {

	typedef decltype(std::declval<typename  Arg1::value_type>()-
			 std::declval<Arg2>() ) return_type;

	return a1.Value() - a2;
}

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument_pack<Arg1, Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()-
		 std::declval<typename  Arg2::value_type>() )>::type
operator-( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()-
			 std::declval<typename  Arg2::value_type>() ) return_type;

	return a1.Value() - a2.Value();
}


//----------------

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()* std::declval<Arg2>() )>::type
operator*( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()-
			 std::declval<Arg2>() ) return_type;

	return a1.Value() * a2;
}

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()*  std::declval<Arg2>() )>::type
operator*(Arg2 const& a2, Arg1 const& a1 ) {

	typedef decltype(std::declval<typename  Arg1::value_type>()-
			 std::declval<Arg2>() ) return_type;

	return a1.Value() * a2;
}

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument_pack<Arg1, Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()*
		 std::declval<typename  Arg2::value_type>() )>::type
operator*( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()*
			 std::declval<typename  Arg2::value_type>() ) return_type;

	return a1.Value() * a2.Value();
}

//----------------

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>() / std::declval<Arg2>() )>::type
operator/( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()-
			 std::declval<Arg2>() ) return_type;

	return a1.Value() / a2;
}

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument<Arg1>::value && !detail::is_function_argument<Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>() / std::declval<Arg2>() )>::type
operator/(Arg2 const& a2, Arg1 const& a1 ) {

	typedef decltype(std::declval<typename  Arg1::value_type>()-
			 std::declval<Arg2>() ) return_type;

	return a1.Value() / a2;
}

template<typename Arg1, typename Arg2>
__hydra_host__ __hydra_device__
typename std::enable_if<detail::is_function_argument_pack<Arg1, Arg2>::value,
decltype(std::declval<typename  Arg1::value_type>()/
		 std::declval<typename  Arg2::value_type>() )>::type
operator/( Arg1 const& a1, Arg2 const& a2) {

	typedef decltype(std::declval<typename  Arg1::value_type>()/
			 std::declval<typename  Arg2::value_type>() ) return_type;

	return a1.Value() / a2.Value();
}



template<typename Arg>
inline typename std::enable_if<detail::is_function_argument<Arg>::value, std::ostream&>::type
operator<<(std::ostream& s, const hydra_thrust::device_reference<Arg>& a){
	s <<  typename Arg::name_type(a).Value();
	return s;
}

template<typename Arg>
inline typename std::enable_if<detail::is_function_argument<Arg>::value, std::ostream&>::type
operator<<(std::ostream& s, const Arg& a){
	s <<  a.Value();
	return s;
}


} //namespace arguments
}//namespace hydra

#endif /* FUNCTIONARGUMENT_H_ */
