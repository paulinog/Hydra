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

#pragma once

#include <hydra/detail/external/thrust/iterator/detail/normal_iterator.h>
#include <hydra/detail/external/thrust/detail/execution_policy.h>
#include <hydra/detail/external/thrust/detail/allocator/allocator_traits.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{

namespace detail
{

struct copy_allocator_t {};

// XXX parameter T is redundant with parameter Alloc
template<typename T, typename Alloc>
  class contiguous_storage
{
  private:
    typedef HYDRA_EXTERNAL_NS::thrust::detail::allocator_traits<Alloc> alloc_traits;

  public:
    typedef Alloc                                      allocator_type;
    typedef T                                          value_type;
    typedef typename alloc_traits::pointer             pointer;
    typedef typename alloc_traits::const_pointer       const_pointer;
    typedef typename alloc_traits::size_type           size_type;
    typedef typename alloc_traits::difference_type     difference_type;

    // XXX we should bring reference & const_reference into allocator_traits
    //     at the moment, it's unclear how -- we have nothing analogous to
    //     rebind_pointer for references
    //     we either need to add reference_traits or extend the existing
    //     pointer_traits to support wrapped references
    typedef typename Alloc::reference                  reference;
    typedef typename Alloc::const_reference            const_reference;

    typedef HYDRA_EXTERNAL_NS::thrust::detail::normal_iterator<pointer>       iterator;
    typedef HYDRA_EXTERNAL_NS::thrust::detail::normal_iterator<const_pointer> const_iterator;

    __thrust_exec_check_disable__
    __hydra_host__ __hydra_device__
    explicit contiguous_storage(const allocator_type &alloc = allocator_type());

    __thrust_exec_check_disable__
    __hydra_host__ __hydra_device__
    explicit contiguous_storage(size_type n, const allocator_type &alloc = allocator_type());

    __thrust_exec_check_disable__
    __hydra_host__ __hydra_device__
    explicit contiguous_storage(copy_allocator_t, const contiguous_storage &other);

    __thrust_exec_check_disable__
    __hydra_host__ __hydra_device__
    explicit contiguous_storage(copy_allocator_t, const contiguous_storage &other, size_type n);

    __thrust_exec_check_disable__
    __hydra_host__ __hydra_device__
    ~contiguous_storage();

    __hydra_host__ __hydra_device__
    size_type size() const;

    __hydra_host__ __hydra_device__
    size_type max_size() const;

    __hydra_host__ __hydra_device__
    pointer data();

    __hydra_host__ __hydra_device__
    const_pointer data() const;

    __hydra_host__ __hydra_device__
    iterator begin();

    __hydra_host__ __hydra_device__
    const_iterator begin() const;

    __hydra_host__ __hydra_device__
    iterator end();

    __hydra_host__ __hydra_device__
    const_iterator end() const;

    __hydra_host__ __hydra_device__
    reference operator[](size_type n);

    __hydra_host__ __hydra_device__
    const_reference operator[](size_type n) const;

    __hydra_host__ __hydra_device__
    allocator_type get_allocator() const;

    // note that allocate does *not* automatically call deallocate
    __hydra_host__ __hydra_device__
    void allocate(size_type n);

    __hydra_host__ __hydra_device__
    void deallocate();

    __hydra_host__ __hydra_device__
    void swap(contiguous_storage &x);

    __hydra_host__ __hydra_device__
    void default_construct_n(iterator first, size_type n);

    __hydra_host__ __hydra_device__
    void uninitialized_fill_n(iterator first, size_type n, const value_type &value);

    template<typename InputIterator>
    __hydra_host__ __hydra_device__
    iterator uninitialized_copy(InputIterator first, InputIterator last, iterator result);

    template<typename System, typename InputIterator>
    __hydra_host__ __hydra_device__
    iterator uninitialized_copy(HYDRA_EXTERNAL_NS::thrust::execution_policy<System> &from_system,
                                InputIterator first,
                                InputIterator last,
                                iterator result);

    template<typename InputIterator, typename Size>
    __hydra_host__ __hydra_device__
    iterator uninitialized_copy_n(InputIterator first, Size n, iterator result);

    template<typename System, typename InputIterator, typename Size>
    __hydra_host__ __hydra_device__
    iterator uninitialized_copy_n(HYDRA_EXTERNAL_NS::thrust::execution_policy<System> &from_system,
                                  InputIterator first,
                                  Size n,
                                  iterator result);

    __hydra_host__ __hydra_device__
    void destroy(iterator first, iterator last);

    __hydra_host__ __hydra_device__
    void deallocate_on_allocator_mismatch(const contiguous_storage &other);

    __hydra_host__ __hydra_device__
    void destroy_on_allocator_mismatch(const contiguous_storage &other,
        iterator first, iterator last);

    __hydra_host__ __hydra_device__
    void set_allocator(const allocator_type &alloc);

    __hydra_host__ __hydra_device__
    bool is_allocator_not_equal(const allocator_type &alloc) const;

    __hydra_host__ __hydra_device__
    bool is_allocator_not_equal(const contiguous_storage &other) const;

    __hydra_host__ __hydra_device__
    void propagate_allocator(const contiguous_storage &other);

#if __cplusplus >= 201103L
    __hydra_host__ __hydra_device__
    void propagate_allocator(contiguous_storage &other);

    // allow move assignment for a sane implementation of allocator propagation
    // on move assignment
    __hydra_host__ __hydra_device__
    contiguous_storage &operator=(contiguous_storage &&other);
#endif

  private:
    // XXX we could inherit from this to take advantage of empty base class optimization
    allocator_type m_allocator;

    iterator m_begin;

    size_type m_size;

    // disallow assignment
    contiguous_storage &operator=(const contiguous_storage &x);

    __hydra_host__ __hydra_device__
    void swap_allocators(true_type, const allocator_type &);

    __hydra_host__ __hydra_device__
    void swap_allocators(false_type, allocator_type &);

    __hydra_host__ __hydra_device__
    bool is_allocator_not_equal_dispatch(true_type, const allocator_type &) const;

    __hydra_host__ __hydra_device__
    bool is_allocator_not_equal_dispatch(false_type, const allocator_type &) const;

    __hydra_host__ __hydra_device__
    void deallocate_on_allocator_mismatch_dispatch(true_type, const contiguous_storage &other);

    __hydra_host__ __hydra_device__
    void deallocate_on_allocator_mismatch_dispatch(false_type, const contiguous_storage &other);

    __hydra_host__ __hydra_device__
    void destroy_on_allocator_mismatch_dispatch(true_type, const contiguous_storage &other,
        iterator first, iterator last);

    __hydra_host__ __hydra_device__
    void destroy_on_allocator_mismatch_dispatch(false_type, const contiguous_storage &other,
        iterator first, iterator last);

    __hydra_host__ __hydra_device__
    void propagate_allocator_dispatch(true_type, const contiguous_storage &other);

    __hydra_host__ __hydra_device__
    void propagate_allocator_dispatch(false_type, const contiguous_storage &other);

#if __cplusplus >= 201103L
    __hydra_host__ __hydra_device__
    void propagate_allocator_dispatch(true_type, contiguous_storage &other);

    __hydra_host__ __hydra_device__
    void propagate_allocator_dispatch(false_type, contiguous_storage &other);
#endif
}; // end contiguous_storage

} // end detail

template<typename T, typename Alloc>
__hydra_host__ __hydra_device__
void swap(detail::contiguous_storage<T,Alloc> &lhs, detail::contiguous_storage<T,Alloc> &rhs);

} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END

#include <hydra/detail/external/thrust/detail/contiguous_storage.inl>

