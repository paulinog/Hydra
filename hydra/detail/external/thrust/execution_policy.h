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

/*! \file thrust/execution_policy.h
 *  \brief Thrust execution policies.
 */

#pragma once

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/detail/execution_policy.h>
#include <hydra/detail/external/thrust/detail/execute_with_allocator.h>
#include <hydra/detail/external/thrust/detail/seq.h>

//! \cond

// #include the host system's execution_policy header
#define __HYDRA_THRUST_HOST_SYSTEM_EXECUTION_POLICY_HEADER <__HYDRA_THRUST_HOST_SYSTEM_ROOT/execution_policy.h>
#include __HYDRA_THRUST_HOST_SYSTEM_EXECUTION_POLICY_HEADER
#undef __HYDRA_THRUST_HOST_SYSTEM_EXECUTION_POLICY_HEADER

// #include the device system's execution_policy.h header
#define __HYDRA_THRUST_DEVICE_SYSTEM_EXECUTION_POLICY_HEADER <__HYDRA_THRUST_DEVICE_SYSTEM_ROOT/execution_policy.h>
#include __HYDRA_THRUST_DEVICE_SYSTEM_EXECUTION_POLICY_HEADER
#undef __HYDRA_THRUST_DEVICE_SYSTEM_EXECUTION_POLICY_HEADER

//! \endcond

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{


/*! \cond
 */


namespace detail
{


typedef HYDRA_EXTERNAL_NS::thrust::system::__HYDRA_THRUST_HOST_SYSTEM_NAMESPACE::detail::par_t host_t;


typedef HYDRA_EXTERNAL_NS::thrust::system::__HYDRA_THRUST_DEVICE_SYSTEM_NAMESPACE::detail::par_t device_t;


} // end detail


/*! \endcond
 */


/*! \addtogroup execution_policies Parallel Execution Policies
 *  \{
 */


// define execution_policy for the purpose of Doxygenating it
// it is actually defined elsewhere
#if 0
/*! \p execution_policy is the base class for all Thrust parallel execution policies
 *  like \p HYDRA_EXTERNAL_NS::thrust::host, \p HYDRA_EXTERNAL_NS::thrust::device, and each backend system's tag type.
 *
 *  Custom user-defined backends should derive a policy from this type in order to
 *  interoperate with Thrust algorithm dispatch.
 *
 *  The following code snippet demonstrates how to derive a standalone custom execution policy
 *  from \p HYDRA_EXTERNAL_NS::thrust::execution_policy to implement a backend which only implements \p for_each:
 *
 *  \code
 *  #include <hydra/detail/external/thrust/execution_policy.h>
 *  #include <iostream>
 *
 *  // define a type derived from HYDRA_EXTERNAL_NS::thrust::execution_policy to distinguish our custom execution policy:
 *  struct my_policy : HYDRA_EXTERNAL_NS::thrust::execution_policy<my_policy> {};
 *
 *  // overload for_each on my_policy
 *  template<typename Iterator, typename Function>
 *  Iterator for_each(my_policy, Iterator first, Iterator last, Function f)
 *  {
 *    std::cout << "Hello, world from for_each(my_policy)!" << std::endl;
 *
 *    for(; first < last; ++first)
 *    {
 *      f(*first);
 *    }
 *
 *    return first;
 *  }
 *
 *  struct ignore_argument
 *  {
 *    void operator()(int) {}
 *  };
 *
 *  int main()
 *  {
 *    int data[4];
 *
 *    // dispatch HYDRA_EXTERNAL_NS::thrust::for_each using our custom policy:
 *    my_policy exec;
 *    HYDRA_EXTERNAL_NS::thrust::for_each(exec, data, data + 4, ignore_argument());
 *
 *    // can't dispatch HYDRA_EXTERNAL_NS::thrust::transform because no overload exists for my_policy:
 *    //HYDRA_EXTERNAL_NS::thrust::transform(exec, data, data, + 4, data, HYDRA_EXTERNAL_NS::thrust::identity<int>()); // error!
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  \see host_execution_policy
 *  \see device_execution_policy
 */
template<typename DerivedPolicy>
struct execution_policy : HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy>
{};
#endif


/*! \p host_execution_policy is the base class for all Thrust parallel execution policies
 *  which are derived from Thrust's default host backend system configured with the \p HYDRA_THRUST_HOST_SYSTEM
 *  macro.
 *
 *  Custom user-defined backends which wish to inherit the functionality of Thrust's host backend system
 *  should derive a policy from this type in order to interoperate with Thrust algorithm dispatch.
 *
 *  The following code snippet demonstrates how to derive a standalone custom execution policy from
 *  \p HYDRA_EXTERNAL_NS::thrust::host_execution_policy to implement a backend which specializes \p for_each while inheriting
 *  the behavior of every other algorithm from the host system:
 *
 *  \code
 *  #include <hydra/detail/external/thrust/execution_policy.h>
 *  #include <iostream>
 *
 *  // define a type derived from HYDRA_EXTERNAL_NS::thrust::host_execution_policy to distinguish our custom execution policy:
 *  struct my_policy : HYDRA_EXTERNAL_NS::thrust::host_execution_policy<my_policy> {};
 *
 *  // overload for_each on my_policy
 *  template<typename Iterator, typename Function>
 *  Iterator for_each(my_policy, Iterator first, Iterator last, Function f)
 *  {
 *    std::cout << "Hello, world from for_each(my_policy)!" << std::endl;
 *
 *    for(; first < last; ++first)
 *    {
 *      f(*first);
 *    }
 *
 *    return first;
 *  }
 *
 *  struct ignore_argument
 *  {
 *    void operator()(int) {}
 *  };
 *
 *  int main()
 *  {
 *    int data[4];
 *
 *    // dispatch HYDRA_EXTERNAL_NS::thrust::for_each using our custom policy:
 *    my_policy exec;
 *    HYDRA_EXTERNAL_NS::thrust::for_each(exec, data, data + 4, ignore_argument());
 *
 *    // dispatch HYDRA_EXTERNAL_NS::thrust::transform whose behavior our policy inherits
 *    HYDRA_EXTERNAL_NS::thrust::transform(exec, data, data, + 4, data, HYDRA_EXTERNAL_NS::thrust::identity<int>());
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  \see execution_policy
 *  \see device_execution_policy
 */
template<typename DerivedPolicy>
  struct host_execution_policy
    : HYDRA_EXTERNAL_NS::thrust::system::__HYDRA_THRUST_HOST_SYSTEM_NAMESPACE::execution_policy<DerivedPolicy>
{};


/*! \p device_execution_policy is the base class for all Thrust parallel execution policies
 *  which are derived from Thrust's default device backend system configured with the \p HYDRA_THRUST_DEVICE_SYSTEM
 *  macro.
 *
 *  Custom user-defined backends which wish to inherit the functionality of Thrust's device backend system
 *  should derive a policy from this type in order to interoperate with Thrust algorithm dispatch.
 *
 *  The following code snippet demonstrates how to derive a standalone custom execution policy from
 *  \p HYDRA_EXTERNAL_NS::thrust::device_execution_policy to implement a backend which specializes \p for_each while inheriting
 *  the behavior of every other algorithm from the device system:
 *
 *  \code
 *  #include <hydra/detail/external/thrust/execution_policy.h>
 *  #include <iostream>
 *
 *  // define a type derived from HYDRA_EXTERNAL_NS::thrust::device_execution_policy to distinguish our custom execution policy:
 *  struct my_policy : HYDRA_EXTERNAL_NS::thrust::device_execution_policy<my_policy> {};
 *
 *  // overload for_each on my_policy
 *  template<typename Iterator, typename Function>
 *  Iterator for_each(my_policy, Iterator first, Iterator last, Function f)
 *  {
 *    std::cout << "Hello, world from for_each(my_policy)!" << std::endl;
 *
 *    for(; first < last; ++first)
 *    {
 *      f(*first);
 *    }
 *
 *    return first;
 *  }
 *
 *  struct ignore_argument
 *  {
 *    void operator()(int) {}
 *  };
 *
 *  int main()
 *  {
 *    int data[4];
 *
 *    // dispatch HYDRA_EXTERNAL_NS::thrust::for_each using our custom policy:
 *    my_policy exec;
 *    HYDRA_EXTERNAL_NS::thrust::for_each(exec, data, data + 4, ignore_argument());
 *
 *    // dispatch HYDRA_EXTERNAL_NS::thrust::transform whose behavior our policy inherits
 *    HYDRA_EXTERNAL_NS::thrust::transform(exec, data, data, + 4, data, HYDRA_EXTERNAL_NS::thrust::identity<int>());
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  \see execution_policy
 *  \see host_execution_policy
 */
template<typename DerivedPolicy>
  struct device_execution_policy
    : HYDRA_EXTERNAL_NS::thrust::system::__HYDRA_THRUST_DEVICE_SYSTEM_NAMESPACE::execution_policy<DerivedPolicy>
{};


/*! \p HYDRA_EXTERNAL_NS::thrust::host is the default parallel execution policy associated with Thrust's host backend system
 *  configured by the \p HYDRA_THRUST_HOST_SYSTEM macro.
 *
 *  Instead of relying on implicit algorithm dispatch through iterator system tags, users may directly target
 *  algorithm dispatch at Thrust's host system by providing \p HYDRA_EXTERNAL_NS::thrust::host as an algorithm parameter.
 *
 *  Explicit dispatch can be useful in avoiding the introduction of data copies into containers such as
 *  \p HYDRA_EXTERNAL_NS::thrust::host_vector.
 *
 *  Note that even though \p HYDRA_EXTERNAL_NS::thrust::host targets the host CPU, it is a parallel execution policy. That is,
 *  the order that an algorithm invokes functors or dereferences iterators is not defined.
 *
 *  The type of \p HYDRA_EXTERNAL_NS::thrust::host is implementation-defined.
 *
 *  The following code snippet demonstrates how to use \p HYDRA_EXTERNAL_NS::thrust::host to explicitly dispatch an invocation
 *  of \p HYDRA_EXTERNAL_NS::thrust::for_each to the host backend system:
 *
 *  \code
 *  #include <hydra/detail/external/thrust/for_each.h>
 *  #include <hydra/detail/external/thrust/execution_policy.h>
 *  #include <cstdio>
 *
 *  struct printf_functor
 *  {
 *    __hydra_host__ __hydra_device__
 *    void operator()(int x)
 *    {
 *      printf("%d\n", x);
 *    }
 *  };
 *  ...
 *  int vec(3);
 *  vec[0] = 0; vec[1] = 1; vec[2] = 2;
 *
 *  HYDRA_EXTERNAL_NS::thrust::for_each(HYDRA_EXTERNAL_NS::thrust::host, vec.begin(), vec.end(), printf_functor());
 *
 *  // 0 1 2 is printed to standard output in some unspecified order
 *  \endcode
 *
 *  \see host_execution_policy
 *  \see HYDRA_EXTERNAL_NS::thrust::device
 */
static const detail::host_t host;


/*! \p HYDRA_EXTERNAL_NS::thrust::device is the default parallel execution policy associated with Thrust's device backend system
 *  configured by the \p HYDRA_THRUST_DEVICE_SYSTEM macro.
 *
 *  Instead of relying on implicit algorithm dispatch through iterator system tags, users may directly target
 *  algorithm dispatch at Thrust's device system by providing \p HYDRA_EXTERNAL_NS::thrust::device as an algorithm parameter.
 *
 *  Explicit dispatch can be useful in avoiding the introduction of data copies into containers such as
 *  \p HYDRA_EXTERNAL_NS::thrust::device_vector or to avoid wrapping e.g. raw pointers allocated by the CUDA API with types
 *  such as \p HYDRA_EXTERNAL_NS::thrust::device_ptr.
 *
 *  The user must take care to guarantee that the iterators provided to an algorithm are compatible with
 *  the device backend system. For example, raw pointers allocated by <tt>std::malloc</tt> typically
 *  cannot be dereferenced by a GPU. For this reason, raw pointers allocated by host APIs should not be mixed
 *  with a \p HYDRA_EXTERNAL_NS::thrust::device algorithm invocation when the device backend is CUDA.
 *
 *  The type of \p HYDRA_EXTERNAL_NS::thrust::device is implementation-defined.
 *
 *  The following code snippet demonstrates how to use \p HYDRA_EXTERNAL_NS::thrust::device to explicitly dispatch an invocation
 *  of \p HYDRA_EXTERNAL_NS::thrust::for_each to the device backend system:
 *
 *  \code
 *  #include <hydra/detail/external/thrust/for_each.h>
 *  #include <hydra/detail/external/thrust/device_vector.h>
 *  #include <hydra/detail/external/thrust/execution_policy.h>
 *  #include <cstdio>
 *
 *  struct printf_functor
 *  {
 *    __hydra_host__ __hydra_device__
 *    void operator()(int x)
 *    {
 *      printf("%d\n", x);
 *    }
 *  };
 *  ...
 *  HYDRA_EXTERNAL_NS::thrust::device_vector<int> vec(3);
 *  vec[0] = 0; vec[1] = 1; vec[2] = 2;
 *
 *  HYDRA_EXTERNAL_NS::thrust::for_each(HYDRA_EXTERNAL_NS::thrust::device, vec.begin(), vec.end(), printf_functor());
 *
 *  // 0 1 2 is printed to standard output in some unspecified order
 *  \endcode
 *
 *  \see host_execution_policy
 *  \see HYDRA_EXTERNAL_NS::thrust::device
 */
#ifdef __CUDA_ARCH__
static const __device__ detail::device_t device;
#else
static const detail::device_t device;
#endif


// define seq for the purpose of Doxygenating it
// it is actually defined elsewhere
#if 0
/*! \p HYDRA_EXTERNAL_NS::thrust::seq is an execution policy which requires an algorithm invocation to execute sequentially
 *  in the current thread. It can not be configured by a compile-time macro.
 *
 *  The type of \p HYDRA_EXTERNAL_NS::thrust::seq is implementation-defined.
 *
 *  The following code snippet demonstrates how to use \p HYDRA_EXTERNAL_NS::thrust::seq to explicitly execute an invocation
 *  of \p HYDRA_EXTERNAL_NS::thrust::for_each sequentially:
 *
 *  \code
 *  #include <hydra/detail/external/thrust/for_each.h>
 *  #include <hydra/detail/external/thrust/execution_policy.h>
 *  #include <vector>
 *  #include <cstdio>
 *
 *  struct printf_functor
 *  {
 *    __hydra_host__ __hydra_device__
 *    void operator()(int x)
 *    {
 *      printf("%d\n", x);
 *    }
 *  };
 *  ...
 *  std::vector<int> vec(3);
 *  vec[0] = 0; vec[1] = 1; vec[2] = 2;
 *
 *  HYDRA_EXTERNAL_NS::thrust::for_each(HYDRA_EXTERNAL_NS::thrust::seq, vec.begin(), vec.end(), printf_functor());
 *
 *  // 0 1 2 is printed to standard output in sequential order
 *  \endcode
 *
 *  \see HYDRA_EXTERNAL_NS::thrust::host
 *  \see HYDRA_EXTERNAL_NS::thrust::device
 */
static const detail::seq_t seq;
#endif


/*! \}
 */


} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust


HYDRA_EXTERNAL_NAMESPACE_END

