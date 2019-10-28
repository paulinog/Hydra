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

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/system/detail/generic/count.h>
#include <hydra/detail/external/thrust/transform_reduce.h>
#include <hydra/detail/external/thrust/detail/internal_functional.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace detail
{
namespace generic
{


template <typename InputType, typename Predicate, typename CountType>
struct count_if_transform
{
  __hydra_host__ __hydra_device__ 
  count_if_transform(Predicate _pred) : pred(_pred){}

  __thrust_exec_check_disable__
  __hydra_host__ __hydra_device__
  CountType operator()(const InputType& val)
  {
    if(pred(val))
      return 1;
    else
      return 0;
  } // end operator()

  Predicate pred;
}; // end count_if_transform


template <typename DerivedPolicy, typename InputIterator, typename EqualityComparable>
__hydra_host__ __hydra_device__
typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<InputIterator>::difference_type
count(HYDRA_EXTERNAL_NS::thrust::execution_policy<DerivedPolicy> &exec, InputIterator first, InputIterator last, const EqualityComparable& value)
{
  // XXX use placeholder expression here
  return HYDRA_EXTERNAL_NS::thrust::count_if(exec, first, last, HYDRA_EXTERNAL_NS::thrust::detail::equal_to_value<EqualityComparable>(value));
} // end count()


template <typename DerivedPolicy, typename InputIterator, typename Predicate>
__hydra_host__ __hydra_device__
typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<InputIterator>::difference_type
count_if(HYDRA_EXTERNAL_NS::thrust::execution_policy<DerivedPolicy> &exec, InputIterator first, InputIterator last, Predicate pred)
{
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<InputIterator>::value_type InputType;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<InputIterator>::difference_type CountType;
  
  HYDRA_EXTERNAL_NS::thrust::system::detail::generic::count_if_transform<InputType, Predicate, CountType> unary_op(pred);
  HYDRA_EXTERNAL_NS::thrust::plus<CountType> binary_op;
  return HYDRA_EXTERNAL_NS::thrust::transform_reduce(exec, first, last, unary_op, CountType(0), binary_op);
} // end count_if()


} // end generic
} // end detail
} // end system
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END
