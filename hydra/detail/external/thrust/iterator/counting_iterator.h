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


/*! \file thrust/iterator/counting_iterator.h
 *  \brief An iterator which returns an increasing incrementable value
 *         when dereferenced
 */

/*
 * Copyright David Abrahams 2003.
 * 
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying NOTICE file for the complete license)
 *
 * For more information, see http://www.boost.org
 */

#pragma once

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/iterator/iterator_adaptor.h>
#include <hydra/detail/external/thrust/iterator/iterator_facade.h>
#include <hydra/detail/external/thrust/iterator/iterator_categories.h>

// #include the details first
#include <hydra/detail/external/thrust/iterator/detail/counting_iterator.inl>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{

/*! \addtogroup iterators
 *  \{
 */

/*! \addtogroup fancyiterator Fancy Iterators
 *  \ingroup iterators
 *  \{
 */

/*! \p counting_iterator is an iterator which represents a pointer into a range
 *  of sequentially changing values. This iterator is useful for creating a range
 *  filled with a sequence without explicitly storing it in memory. Using
 *  \p counting_iterator saves memory capacity and bandwidth.
 *
 *  The following code snippet demonstrates how to create a \p counting_iterator whose
 *  \c value_type is \c int and which sequentially increments by \c 1.
 *
 *  \code
 *  #include <hydra/detail/external/thrust/iterator/counting_iterator.h>
 *  ...
 *  // create iterators
 *  HYDRA_EXTERNAL_NS::thrust::counting_iterator<int> first(10);
 *  HYDRA_EXTERNAL_NS::thrust::counting_iterator<int> last = first + 3;
 *   
 *  first[0]   // returns 10
 *  first[1]   // returns 11
 *  first[100] // returns 110
 *   
 *  // sum of [first, last)
 *  HYDRA_EXTERNAL_NS::thrust::reduce(first, last);   // returns 33 (i.e. 10 + 11 + 12)
 *   
 *  // initialize vector to [0,1,2,..]
 *  HYDRA_EXTERNAL_NS::thrust::counting_iterator<int> iter(0);
 *  HYDRA_EXTERNAL_NS::thrust::device_vector<int> vec(500);
 *  HYDRA_EXTERNAL_NS::thrust::copy(iter, iter + vec.size(), vec.begin());
 *  \endcode
 *
 *  This next example demonstrates how to use a \p counting_iterator with the
 *  \p HYDRA_EXTERNAL_NS::thrust::copy_if function to compute the indices of the non-zero elements
 *  of a \p device_vector. In this example, we use the \p make_counting_iterator
 *  function to avoid specifying the type of the \p counting_iterator.
 *
 *  \code
 *  #include <hydra/detail/external/thrust/iterator/counting_iterator.h>
 *  #include <hydra/detail/external/thrust/copy.h>
 *  #include <hydra/detail/external/thrust/functional.h>
 *  #include <hydra/detail/external/thrust/device_vector.h>
 *   
 *  int main()
 *  {
 *   // this example computes indices for all the nonzero values in a sequence
 *   
 *   // sequence of zero and nonzero values
 *   HYDRA_EXTERNAL_NS::thrust::device_vector<int> stencil(8);
 *   stencil[0] = 0;
 *   stencil[1] = 1;
 *   stencil[2] = 1;
 *   stencil[3] = 0;
 *   stencil[4] = 0;
 *   stencil[5] = 1;
 *   stencil[6] = 0;
 *   stencil[7] = 1;
 *   
 *   // storage for the nonzero indices
 *   HYDRA_EXTERNAL_NS::thrust::device_vector<int> indices(8);
 *   
 *   // compute indices of nonzero elements
 *   typedef HYDRA_EXTERNAL_NS::thrust::device_vector<int>::iterator IndexIterator;
 *   
 *   // use make_counting_iterator to define the sequence [0, 8)
 *   IndexIterator indices_end = HYDRA_EXTERNAL_NS::thrust::copy_if(HYDRA_EXTERNAL_NS::thrust::make_counting_iterator(0),
 *                                               HYDRA_EXTERNAL_NS::thrust::make_counting_iterator(8),
 *                                               stencil.begin(),
 *                                               indices.begin(),
 *                                               HYDRA_EXTERNAL_NS::thrust::identity<int>());
 *   // indices now contains [1,2,5,7]
 *   
 *   return 0;
 *  }
 *  \endcode
 *
 *  \see make_counting_iterator
 */
template<typename Incrementable,
         typename System = use_default,
         typename Traversal = use_default,
         typename Difference = use_default>
  class counting_iterator
    : public detail::counting_iterator_base<Incrementable, System, Traversal, Difference>::type
{
    /*! \cond
     */
    typedef typename detail::counting_iterator_base<Incrementable, System, Traversal, Difference>::type super_t;

    friend class HYDRA_EXTERNAL_NS::thrust::iterator_core_access;

  public:
    typedef typename super_t::reference       reference;
    typedef typename super_t::difference_type difference_type;

    /*! \endcond
     */

    /*! Null constructor initializes this \p counting_iterator's \c Incrementable
     *  counter using its null constructor.
     */
    __hydra_host__ __hydra_device__
    counting_iterator() {}

    /*! Copy constructor copies the value of another \p counting_iterator into a
     *  new \p counting_iterator.
     *
     *  \p rhs The \p counting_iterator to copy.
     */
    __hydra_host__ __hydra_device__
    counting_iterator(counting_iterator const &rhs):super_t(rhs.base()){}

    /*! Copy constructor copies the value of another counting_iterator 
     *  with related System type.
     *
     *  \param rhs The \p counting_iterator to copy.
     */
    template<typename OtherSystem>
    __hydra_host__ __hydra_device__
    counting_iterator(counting_iterator<Incrementable, OtherSystem, Traversal, Difference> const &rhs,
                      typename HYDRA_EXTERNAL_NS::thrust::detail::enable_if_convertible<
                        typename HYDRA_EXTERNAL_NS::thrust::iterator_system<counting_iterator<Incrementable,OtherSystem,Traversal,Difference> >::type,
                        typename HYDRA_EXTERNAL_NS::thrust::iterator_system<super_t>::type
                      >::type * = 0)
      : super_t(rhs.base()){}

    /*! This \c explicit constructor copies the value of an \c Incrementable
     *  into a new \p counting_iterator's \c Incrementable counter.
     *  
     *  \param x The initial value of the new \p counting_iterator's \c Incrementable
     *         counter.
     */
    __hydra_host__ __hydra_device__
    explicit counting_iterator(Incrementable x):super_t(x){}

    /*! \cond
     */
  private:
    __hydra_host__ __hydra_device__
    reference dereference() const
    {
      return this->base_reference();
    }

    // note that we implement equal specially for floating point counting_iterator
    template <typename OtherIncrementable, typename OtherSystem, typename OtherTraversal, typename OtherDifference>
    __hydra_host__ __hydra_device__
    bool equal(counting_iterator<OtherIncrementable, OtherSystem, OtherTraversal, OtherDifference> const& y) const
    {
      typedef HYDRA_EXTERNAL_NS::thrust::detail::counting_iterator_equal<difference_type,Incrementable,OtherIncrementable> e;
      return e::equal(this->base(), y.base());
    }

    template <class OtherIncrementable>
    __hydra_host__ __hydra_device__
    difference_type
    distance_to(counting_iterator<OtherIncrementable, System, Traversal, Difference> const& y) const
    {
      typedef typename
      HYDRA_EXTERNAL_NS::thrust::detail::eval_if<
        HYDRA_EXTERNAL_NS::thrust::detail::is_numeric<Incrementable>::value,
        HYDRA_EXTERNAL_NS::thrust::detail::identity_<HYDRA_EXTERNAL_NS::thrust::detail::number_distance<difference_type, Incrementable, OtherIncrementable> >,
        HYDRA_EXTERNAL_NS::thrust::detail::identity_<HYDRA_EXTERNAL_NS::thrust::detail::iterator_distance<difference_type, Incrementable, OtherIncrementable> >
      >::type d;

      return d::distance(this->base(), y.base());
    }

    /*! \endcond
     */
}; // end counting_iterator


/*! \p make_counting_iterator creates a \p counting_iterator
 *  using an initial value for its \c Incrementable counter.
 *
 *  \param x The initial value of the new \p counting_iterator's counter.
 *  \return A new \p counting_iterator whose counter has been initialized to \p x.
 */
template <typename Incrementable>
inline __hydra_host__ __hydra_device__
counting_iterator<Incrementable> make_counting_iterator(Incrementable x)
{
  return counting_iterator<Incrementable>(x);
}

/*! \} // end fancyiterators
 */

/*! \} // end iterators
 */

} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust


HYDRA_EXTERNAL_NAMESPACE_END

