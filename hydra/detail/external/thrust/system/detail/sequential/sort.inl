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


#include <hydra/detail/external/thrust/reverse.h>
#include <hydra/detail/external/thrust/detail/type_traits.h>
#include <hydra/detail/external/thrust/iterator/iterator_traits.h>
#include <hydra/detail/external/thrust/system/detail/sequential/stable_merge_sort.h>
#include <hydra/detail/external/thrust/system/detail/sequential/stable_primitive_sort.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{
namespace sort_detail
{


////////////////////
// Primitive Sort //
////////////////////


template<typename KeyType, typename Compare>
struct needs_reverse
  : HYDRA_EXTERNAL_NS::thrust::detail::integral_constant<
      bool,
      HYDRA_EXTERNAL_NS::thrust::detail::is_same<Compare, typename HYDRA_EXTERNAL_NS::thrust::greater<KeyType> >::value
    >
{};


template<typename DerivedPolicy,
         typename RandomAccessIterator,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
void stable_sort(sequential::execution_policy<DerivedPolicy> &exec,
                 RandomAccessIterator first,
                 RandomAccessIterator last,
                 StrictWeakOrdering,
                 HYDRA_EXTERNAL_NS::thrust::detail::true_type)
{
  HYDRA_EXTERNAL_NS::thrust::system::detail::sequential::stable_primitive_sort(exec, first, last);
        
  // if comp is greater<T> then reverse the keys
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<RandomAccessIterator>::value_type KeyType;

  if(needs_reverse<KeyType,StrictWeakOrdering>::value)
  {
    HYDRA_EXTERNAL_NS::thrust::reverse(exec, first, last);
  }
}


template<typename DerivedPolicy,
         typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
void stable_sort_by_key(sequential::execution_policy<DerivedPolicy> &exec,
                        RandomAccessIterator1 first1,
                        RandomAccessIterator1 last1,
                        RandomAccessIterator2 first2,
                        StrictWeakOrdering,
                        HYDRA_EXTERNAL_NS::thrust::detail::true_type)
{
  // if comp is greater<T> then reverse the keys and values
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<RandomAccessIterator1>::value_type KeyType;

  // note, we also have to reverse the (unordered) input to preserve stability
  if(needs_reverse<KeyType,StrictWeakOrdering>::value)
  {
    HYDRA_EXTERNAL_NS::thrust::reverse(exec, first1,  last1);
    HYDRA_EXTERNAL_NS::thrust::reverse(exec, first2, first2 + (last1 - first1));
  }

  HYDRA_EXTERNAL_NS::thrust::system::detail::sequential::stable_primitive_sort_by_key(exec, first1, last1, first2);

  if(needs_reverse<KeyType,StrictWeakOrdering>::value)
  {
    HYDRA_EXTERNAL_NS::thrust::reverse(exec, first1,  last1);
    HYDRA_EXTERNAL_NS::thrust::reverse(exec, first2, first2 + (last1 - first1));
  }
}


////////////////
// Merge Sort //
////////////////


template<typename DerivedPolicy,
         typename RandomAccessIterator,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
void stable_sort(sequential::execution_policy<DerivedPolicy> &exec,
                 RandomAccessIterator first,
                 RandomAccessIterator last,
                 StrictWeakOrdering comp,
                 HYDRA_EXTERNAL_NS::thrust::detail::false_type)
{
  HYDRA_EXTERNAL_NS::thrust::system::detail::sequential::stable_merge_sort(exec, first, last, comp);
}


template<typename DerivedPolicy,
         typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
void stable_sort_by_key(sequential::execution_policy<DerivedPolicy> &exec,
                        RandomAccessIterator1 first1,
                        RandomAccessIterator1 last1,
                        RandomAccessIterator2 first2,
                        StrictWeakOrdering comp,
                        HYDRA_EXTERNAL_NS::thrust::detail::false_type)
{
  HYDRA_EXTERNAL_NS::thrust::system::detail::sequential::stable_merge_sort_by_key(exec, first1, last1, first2, comp);
}


template<typename KeyType, typename Compare>
struct use_primitive_sort
  : HYDRA_EXTERNAL_NS::thrust::detail::and_<
      HYDRA_EXTERNAL_NS::thrust::detail::is_arithmetic<KeyType>,
      HYDRA_EXTERNAL_NS::thrust::detail::or_<
        HYDRA_EXTERNAL_NS::thrust::detail::is_same<Compare, HYDRA_EXTERNAL_NS::thrust::less<KeyType> >,
        HYDRA_EXTERNAL_NS::thrust::detail::is_same<Compare, HYDRA_EXTERNAL_NS::thrust::greater<KeyType> >
      >
    >
{};


} // end namespace sort_detail


template<typename DerivedPolicy,
         typename RandomAccessIterator,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
void stable_sort(sequential::execution_policy<DerivedPolicy> &exec,
                 RandomAccessIterator first,
                 RandomAccessIterator last,
                 StrictWeakOrdering comp)
{

  // the compilation time of stable_primitive_sort is too expensive to use within a single CUDA thread
#ifndef __CUDA_ARCH__
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<RandomAccessIterator>::value_type KeyType;
  sort_detail::use_primitive_sort<KeyType,StrictWeakOrdering> use_primitive_sort;
#else
  HYDRA_EXTERNAL_NS::thrust::detail::false_type use_primitive_sort;
#endif

  sort_detail::stable_sort(exec, first, last, comp, use_primitive_sort);
}


template<typename DerivedPolicy,
         typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering>
__hydra_host__ __hydra_device__
void stable_sort_by_key(sequential::execution_policy<DerivedPolicy> &exec,
                        RandomAccessIterator1 first1,
                        RandomAccessIterator1 last1,
                        RandomAccessIterator2 first2,
                        StrictWeakOrdering comp)
{

  // the compilation time of stable_primitive_sort_by_key is too expensive to use within a single CUDA thread
#ifndef __CUDA_ARCH__
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_traits<RandomAccessIterator1>::value_type KeyType;
  sort_detail::use_primitive_sort<KeyType,StrictWeakOrdering> use_primitive_sort;
#else
  HYDRA_EXTERNAL_NS::thrust::detail::false_type use_primitive_sort;
#endif

  sort_detail::stable_sort_by_key(exec, first1, last1, first2, comp, use_primitive_sort);
}


} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
HYDRA_EXTERNAL_NAMESPACE_END
