/*
 *  Copyright 2008-2018 NVIDIA Corporation
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

/*! \file thrust/system/omp/memory.h
 *  \brief Managing memory associated with Thrust's OpenMP system.
 */

#pragma once

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/system/omp/memory_resource.h>
#include <hydra/detail/external/thrust/memory.h>
#include <hydra/detail/external/thrust/detail/type_traits.h>
#include <hydra/detail/external/thrust/mr/allocator.h>
#include <ostream>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace omp
{

/*! Allocates an area of memory available to Thrust's <tt>omp</tt> system.
 *  \param n Number of bytes to allocate.
 *  \return A <tt>omp::pointer<void></tt> pointing to the beginning of the newly
 *          allocated memory. A null <tt>omp::pointer<void></tt> is returned if
 *          an error occurs.
 *  \note The <tt>omp::pointer<void></tt> returned by this function must be
 *        deallocated with \p omp::free.
 *  \see omp::free
 *  \see std::malloc
 */
inline pointer<void> malloc(std::size_t n);

/*! Allocates a typed area of memory available to Thrust's <tt>omp</tt> system.
 *  \param n Number of elements to allocate.
 *  \return A <tt>omp::pointer<T></tt> pointing to the beginning of the newly
 *          allocated memory. A null <tt>omp::pointer<T></tt> is returned if
 *          an error occurs.
 *  \note The <tt>omp::pointer<T></tt> returned by this function must be
 *        deallocated with \p omp::free.
 *  \see omp::free
 *  \see std::malloc
 */
template<typename T>
inline pointer<T> malloc(std::size_t n);

/*! Deallocates an area of memory previously allocated by <tt>omp::malloc</tt>.
 *  \param ptr A <tt>omp::pointer<void></tt> pointing to the beginning of an area
 *         of memory previously allocated with <tt>omp::malloc</tt>.
 *  \see omp::malloc
 *  \see std::free
 */
inline void free(pointer<void> ptr);

// XXX upon c++11
// template<typename T>
// using allocator = HYDRA_EXTERNAL_NS::thrust::mr::stateless_resource_allocator<T, memory_resource>;

/*! \p omp::allocator is the default allocator used by the \p omp system's containers such as
 *  <tt>omp::vector</tt> if no user-specified allocator is provided. \p omp::allocator allocates
 *  (deallocates) storage with \p omp::malloc (\p omp::free).
 */
template<typename T>
  struct allocator
    : HYDRA_EXTERNAL_NS::thrust::mr::stateless_resource_allocator<
        T,
        memory_resource
    >
{
private:
    typedef HYDRA_EXTERNAL_NS::thrust::mr::stateless_resource_allocator<
        T,
        memory_resource
    > base;

public:
  /*! The \p rebind metafunction provides the type of an \p allocator
   *  instantiated with another type.
   *
   *  \tparam U The other type to use for instantiation.
   */
  template<typename U>
    struct rebind
  {
    /*! The typedef \p other gives the type of the rebound \p allocator.
     */
    typedef allocator<U> other;
  };

  /*! No-argument constructor has no effect.
   */
  __hydra_host__ __hydra_device__
  inline allocator() {}

  /*! Copy constructor has no effect.
   */
  __hydra_host__ __hydra_device__
  inline allocator(const allocator & other) : base(other) {}

  /*! Constructor from other \p allocator has no effect.
   */
  template<typename U>
  __hydra_host__ __hydra_device__
  inline allocator(const allocator<U> & other) : base(other) {}

  /*! Destructor has no effect.
   */
  __hydra_host__ __hydra_device__
  inline ~allocator() {}
}; // end allocator

} // end omp

/*! \}
 */

} // end system

/*! \HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace HYDRA_EXTERNAL_NS::thrust::omp
 *  \brief \p HYDRA_EXTERNAL_NS::thrust::omp is a top-level alias for HYDRA_EXTERNAL_NS::thrust::system::omp.
 */
namespace omp
{

using HYDRA_EXTERNAL_NS::thrust::system::omp::malloc;
using HYDRA_EXTERNAL_NS::thrust::system::omp::free;
using HYDRA_EXTERNAL_NS::thrust::system::omp::allocator;

} // end omp

} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END
#include <hydra/detail/external/thrust/system/omp/detail/memory.inl>

