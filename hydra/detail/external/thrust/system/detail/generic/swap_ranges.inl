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
#include <hydra/detail/external/thrust/system/detail/generic/swap_ranges.h>
#include <hydra/detail/external/thrust/tuple.h>
#include <hydra/detail/external/thrust/iterator/zip_iterator.h>
#include <hydra/detail/external/thrust/detail/internal_functional.h>
#include <hydra/detail/external/thrust/for_each.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace detail
{
namespace generic
{
namespace detail
{


// XXX define this here rather than in internal_functional.h
// to avoid circular dependence between swap.h & internal_functional.h
struct swap_pair_elements
{
  template <typename Tuple>
  __hydra_host__ __hydra_device__
  void operator()(Tuple t)
  {
    // use unqualified swap to allow ADL to catch any user-defined swap
    using HYDRA_EXTERNAL_NS::thrust::swap;
    swap(HYDRA_EXTERNAL_NS::thrust::get<0>(t), HYDRA_EXTERNAL_NS::thrust::get<1>(t));
  }
}; // end swap_pair_elements


} // end detail


template<typename DerivedPolicy,
         typename ForwardIterator1,
         typename ForwardIterator2>
__hydra_host__ __hydra_device__
  ForwardIterator2 swap_ranges(HYDRA_EXTERNAL_NS::thrust::execution_policy<DerivedPolicy> &exec,
                               ForwardIterator1 first1,
                               ForwardIterator1 last1,
                               ForwardIterator2 first2)
{
  typedef HYDRA_EXTERNAL_NS::thrust::tuple<ForwardIterator1,ForwardIterator2> IteratorTuple;
  typedef HYDRA_EXTERNAL_NS::thrust::zip_iterator<IteratorTuple>              ZipIterator;

  ZipIterator result = HYDRA_EXTERNAL_NS::thrust::for_each(exec,
                                        HYDRA_EXTERNAL_NS::thrust::make_zip_iterator(HYDRA_EXTERNAL_NS::thrust::make_tuple(first1, first2)),
                                        HYDRA_EXTERNAL_NS::thrust::make_zip_iterator(HYDRA_EXTERNAL_NS::thrust::make_tuple(last1,  first2)),
                                        detail::swap_pair_elements());
  return HYDRA_EXTERNAL_NS::thrust::get<1>(result.get_iterator_tuple());
} // end swap_ranges()


} // end generic
} // end detail
} // end system
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END
