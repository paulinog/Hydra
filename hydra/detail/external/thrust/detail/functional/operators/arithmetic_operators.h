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

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/detail/functional/actor.h>
#include <hydra/detail/external/thrust/detail/functional/composite.h>
#include <hydra/detail/external/thrust/detail/functional/operators/operator_adaptors.h>
#include <hydra/detail/external/thrust/functional.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace detail
{
namespace functional
{

template<typename Eval>
__hydra_host__ __hydra_device__
actor<
  composite<
    unary_operator<HYDRA_EXTERNAL_NS::thrust::negate>,
    actor<Eval>
  >
>
__hydra_host__ __hydra_device__
operator-(const actor<Eval> &_1)
{
  return compose(unary_operator<HYDRA_EXTERNAL_NS::thrust::negate>(), _1);
} // end operator-()

// there's no standard unary_plus functional, so roll an ad hoc one here
template<typename T>
  struct unary_plus
    : public HYDRA_EXTERNAL_NS::thrust::unary_function<T,T>
{
  __hydra_host__ __hydra_device__ T operator()(const T &x) const {return +x;}
}; // end unary_plus

template<typename Eval>
__hydra_host__ __hydra_device__
actor<
  composite<
    unary_operator<unary_plus>,
    actor<Eval>
  >
>
operator+(const actor<Eval> &_1)
{
  return compose(unary_operator<unary_plus>(), _1);
} // end operator+()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::plus>,
    actor<T1>,
    typename as_actor<T2>::type
  >
>
operator+(const actor<T1> &_1, const T2 &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::plus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator+()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::plus>,
    typename as_actor<T1>::type,
    actor<T2>
  >
>
operator+(const T1 &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::plus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator+()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::plus>,
    actor<T1>,
    actor<T2>
  >
>
operator+(const actor<T1> &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::plus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator+()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::minus>,
    typename as_actor<T1>::type,
    actor<T2>
  >
>
operator-(const T1 &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::minus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator-()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::minus>,
    actor<T1>,
    typename as_actor<T2>::type
  >
>
operator-(const actor<T1> &_1, const T2 &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::minus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator-()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::minus>,
    actor<T1>,
    actor<T2>
  >
>
operator-(const actor<T1> &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::minus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator-()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::multiplies>,
    typename as_actor<T1>::type,
    actor<T2>
  >
>
operator*(const T1 &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::multiplies>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator*()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::multiplies>,
    actor<T1>,
    typename as_actor<T2>::type
  >
>
operator*(const actor<T1> &_1, const T2 &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::multiplies>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator*()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::multiplies>,
    actor<T1>,
    actor<T2>
  >
>
operator*(const actor<T1> &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::multiplies>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator*()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::divides>,
    actor<T1>,
    typename as_actor<T2>::type
  >
>
operator/(const actor<T1> &_1, const T2 &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::divides>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator/()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::divides>,
    typename as_actor<T1>::type,
    actor<T2>
  >
>
operator/(const T1 &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::divides>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator/()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::divides>,
    actor<T1>,
    actor<T2>
  >
>
operator/(const actor<T1> &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::divides>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator/()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::modulus>,
    actor<T1>,
    typename as_actor<T2>::type
  >
>
operator%(const actor<T1> &_1, const T2 &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::modulus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator%()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::modulus>,
    typename as_actor<T1>::type,
    actor<T2>
  >
>
operator%(const T1 &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::modulus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator%()

template<typename T1, typename T2>
__hydra_host__ __hydra_device__
actor<
  composite<
    binary_operator<HYDRA_EXTERNAL_NS::thrust::modulus>,
    actor<T1>,
    actor<T2>
  >
>
operator%(const actor<T1> &_1, const actor<T2> &_2)
{
  return compose(binary_operator<HYDRA_EXTERNAL_NS::thrust::modulus>(),
                 make_actor(_1),
                 make_actor(_2));
} // end operator%()

// there's no standard prefix_increment functional, so roll an ad hoc one here
template<typename T>
  struct prefix_increment
    : public HYDRA_EXTERNAL_NS::thrust::unary_function<T&,T&>
{
  __hydra_host__ __hydra_device__ T& operator()(T &x) const { return ++x; }
}; // end prefix_increment

template<typename Eval>
__hydra_host__ __hydra_device__
actor<
  composite<
    unary_operator<prefix_increment>,
    actor<Eval>
  >
>
operator++(const actor<Eval> &_1)
{
  return compose(unary_operator<prefix_increment>(), _1);
} // end operator++()

// there's no standard suffix_increment functional, so roll an ad hoc one here
template<typename T>
  struct suffix_increment
    : public HYDRA_EXTERNAL_NS::thrust::unary_function<T&,T>
{
  __hydra_host__ __hydra_device__ T operator()(T &x) const { return x++; }
}; // end suffix_increment

template<typename Eval>
__hydra_host__ __hydra_device__
actor<
  composite<
    unary_operator<suffix_increment>,
    actor<Eval>
  >
>
operator++(const actor<Eval> &_1, int)
{
  return compose(unary_operator<suffix_increment>(), _1);
} // end operator++()

// there's no standard prefix_decrement functional, so roll an ad hoc one here
template<typename T>
  struct prefix_decrement
    : public HYDRA_EXTERNAL_NS::thrust::unary_function<T&,T&>
{
  __hydra_host__ __hydra_device__ T& operator()(T &x) const { return --x; }
}; // end prefix_decrement

template<typename Eval>
__hydra_host__ __hydra_device__
actor<
  composite<
    unary_operator<prefix_decrement>,
    actor<Eval>
  >
>
operator--(const actor<Eval> &_1)
{
  return compose(unary_operator<prefix_decrement>(), _1);
} // end operator--()

// there's no standard suffix_decrement functional, so roll an ad hoc one here
template<typename T>
  struct suffix_decrement
    : public HYDRA_EXTERNAL_NS::thrust::unary_function<T&,T>
{
  __hydra_host__ __hydra_device__ T operator()(T &x) const { return x--; }
}; // end suffix_decrement

template<typename Eval>
__hydra_host__ __hydra_device__
actor<
  composite<
    unary_operator<suffix_decrement>,
    actor<Eval>
  >
>
operator--(const actor<Eval> &_1, int)
{
  return compose(unary_operator<suffix_decrement>(), _1);
} // end operator--()

} // end functional
} // end detail
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust


HYDRA_EXTERNAL_NAMESPACE_END

