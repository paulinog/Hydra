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
#include <hydra/detail/external/thrust/system/detail/generic/tag.h>
#include <hydra/detail/external/thrust/pair.h>
#include <hydra/detail/external/thrust/detail/pointer.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace detail
{
namespace generic
{


template<typename T, typename DerivedPolicy>
__hydra_host__ __hydra_device__
  HYDRA_EXTERNAL_NS::thrust::pair<HYDRA_EXTERNAL_NS::thrust::pointer<T,DerivedPolicy>, typename HYDRA_EXTERNAL_NS::thrust::pointer<T,DerivedPolicy>::difference_type>
    get_temporary_buffer(HYDRA_EXTERNAL_NS::thrust::execution_policy<DerivedPolicy> &exec, typename HYDRA_EXTERNAL_NS::thrust::pointer<T,DerivedPolicy>::difference_type n);


template<typename DerivedPolicy, typename Pointer>
__hydra_host__ __hydra_device__
  void return_temporary_buffer(HYDRA_EXTERNAL_NS::thrust::execution_policy<DerivedPolicy> &exec, Pointer p);


} // end generic
} // end detail
} // end system
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
HYDRA_EXTERNAL_NAMESPACE_END
#include <hydra/detail/external/thrust/system/detail/generic/temporary_buffer.inl>

