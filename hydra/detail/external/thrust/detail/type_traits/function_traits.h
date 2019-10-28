/*
 *  Copyright 2008-2013 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include <hydra/detail/external/thrust/detail/type_traits.h>
#include <hydra/detail/external/thrust/detail/type_traits/has_nested_type.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{

// forward definitions for is_commutative
template <typename T> struct plus;
template <typename T> struct multiplies;
template <typename T> struct minimum;
template <typename T> struct maximum;
template <typename T> struct logical_or;
template <typename T> struct logical_and;
template <typename T> struct bit_or;
template <typename T> struct bit_and;
template <typename T> struct bit_xor;

namespace detail
{


// some metafunctions which check for the nested types of the adaptable functions

__HYDRA_THRUST_DEFINE_HAS_NESTED_TYPE(has_result_type, result_type)

__HYDRA_THRUST_DEFINE_HAS_NESTED_TYPE(has_argument_type, argument_type)

__HYDRA_THRUST_DEFINE_HAS_NESTED_TYPE(has_first_argument_type, first_argument_type)

__HYDRA_THRUST_DEFINE_HAS_NESTED_TYPE(has_second_argument_type, second_argument_type)


template<typename AdaptableBinaryFunction>
  struct result_type
{
  typedef typename AdaptableBinaryFunction::result_type type;
};


template<typename T>
  struct is_adaptable_unary_function
    : HYDRA_EXTERNAL_NS::thrust::detail::and_<
        has_result_type<T>,
        has_argument_type<T>
      >
{};


template<typename T>
  struct is_adaptable_binary_function
    : HYDRA_EXTERNAL_NS::thrust::detail::and_<
        has_result_type<T>,
        HYDRA_EXTERNAL_NS::thrust::detail::and_<
          has_first_argument_type<T>,
          has_second_argument_type<T>
        >
      >
{};


template<typename BinaryFunction>
  struct is_commutative
    : public HYDRA_EXTERNAL_NS::thrust::detail::false_type
{};

template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::plus<T>        > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::multiplies<T>  > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::minimum<T>     > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::maximum<T>     > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::logical_or<T>  > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::logical_and<T> > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::bit_or<T>      > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::bit_and<T>     > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};
template<typename T> struct is_commutative< typename HYDRA_EXTERNAL_NS::thrust::bit_xor<T>     > : public HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<T> {};

} // end namespace detail
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust


HYDRA_EXTERNAL_NAMESPACE_END
