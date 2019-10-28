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


/*! \file partition.inl
 *  \brief Inline file for partition.h.
 */

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/partition.h>
#include <hydra/detail/external/thrust/iterator/iterator_traits.h>
#include <hydra/detail/external/thrust/system/detail/generic/select_system.h>
#include <hydra/detail/external/thrust/system/detail/generic/partition.h>
#include <hydra/detail/external/thrust/system/detail/adl/partition.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename ForwardIterator,
         typename Predicate>
__hydra_host__ __hydra_device__
  ForwardIterator partition(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                            ForwardIterator first,
                            ForwardIterator last,
                            Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::partition;
  return partition(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, pred);
} // end partition()


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename ForwardIterator,
         typename InputIterator,
         typename Predicate>
__hydra_host__ __hydra_device__
  ForwardIterator partition(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                            ForwardIterator first,
                            ForwardIterator last,
                            InputIterator stencil,
                            Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::partition;
  return partition(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, stencil, pred);
} // end partition()


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename InputIterator,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
__hydra_host__ __hydra_device__
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    partition_copy(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                   InputIterator first,
                   InputIterator last,
                   OutputIterator1 out_true,
                   OutputIterator2 out_false,
                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::partition_copy;
  return partition_copy(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, out_true, out_false, pred);
} // end partition_copy()


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename InputIterator1,
         typename InputIterator2,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
__hydra_host__ __hydra_device__
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    partition_copy(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                   InputIterator1 first,
                   InputIterator1 last,
                   InputIterator2 stencil,
                   OutputIterator1 out_true,
                   OutputIterator2 out_false,
                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::partition_copy;
  return partition_copy(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, stencil, out_true, out_false, pred);
} // end partition_copy()


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename ForwardIterator,
         typename Predicate>
__hydra_host__ __hydra_device__
  ForwardIterator stable_partition(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                                   ForwardIterator first,
                                   ForwardIterator last,
                                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::stable_partition;
  return stable_partition(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, pred);
} // end stable_partition()


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename ForwardIterator,
         typename InputIterator,
         typename Predicate>
__hydra_host__ __hydra_device__
  ForwardIterator stable_partition(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                                   ForwardIterator first,
                                   ForwardIterator last,
                                   InputIterator stencil,
                                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::stable_partition;
  return stable_partition(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, stencil, pred);
} // end stable_partition()


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename InputIterator,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
__hydra_host__ __hydra_device__
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    stable_partition_copy(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                          InputIterator first,
                          InputIterator last,
                          OutputIterator1 out_true,
                          OutputIterator2 out_false,
                          Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::stable_partition_copy;
  return stable_partition_copy(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, out_true, out_false, pred);
} // end stable_partition_copy()


__thrust_exec_check_disable__
template<typename DerivedPolicy,
         typename InputIterator1,
         typename InputIterator2,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
__hydra_host__ __hydra_device__
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    stable_partition_copy(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                          InputIterator1 first,
                          InputIterator1 last,
                          InputIterator2 stencil,
                          OutputIterator1 out_true,
                          OutputIterator2 out_false,
                          Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::stable_partition_copy;
  return stable_partition_copy(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, stencil, out_true, out_false, pred);
} // end stable_partition_copy()


__thrust_exec_check_disable__
template<typename DerivedPolicy, typename ForwardIterator, typename Predicate>
__hydra_host__ __hydra_device__
  ForwardIterator partition_point(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                                  ForwardIterator first,
                                  ForwardIterator last,
                                  Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::partition_point;
  return partition_point(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, pred);
} // end partition_point()


__thrust_exec_check_disable__
template<typename DerivedPolicy, typename InputIterator, typename Predicate>
__hydra_host__ __hydra_device__
  bool is_partitioned(const HYDRA_EXTERNAL_NS::thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                      InputIterator first,
                      InputIterator last,
                      Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::is_partitioned;
  return is_partitioned(HYDRA_EXTERNAL_NS::thrust::detail::derived_cast(HYDRA_EXTERNAL_NS::thrust::detail::strip_const(exec)), first, last, pred);
} // end is_partitioned()


template<typename ForwardIterator,
         typename Predicate>
  ForwardIterator partition(ForwardIterator first,
                            ForwardIterator last,
                            Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<ForwardIterator>::type System;

  System system;

  return HYDRA_EXTERNAL_NS::thrust::partition(select_system(system), first, last, pred);
} // end partition()


template<typename ForwardIterator,
         typename InputIterator,
         typename Predicate>
  ForwardIterator partition(ForwardIterator first,
                            ForwardIterator last,
                            InputIterator stencil,
                            Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<ForwardIterator>::type System1;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator>::type System2;

  System1 system1;
  System2 system2;

  return HYDRA_EXTERNAL_NS::thrust::partition(select_system(system1,system2), first, last, stencil, pred);
} // end partition()


template<typename ForwardIterator,
         typename Predicate>
  ForwardIterator stable_partition(ForwardIterator first,
                                   ForwardIterator last,
                                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<ForwardIterator>::type System;

  System system;

  return HYDRA_EXTERNAL_NS::thrust::stable_partition(select_system(system), first, last, pred);
} // end stable_partition()


template<typename ForwardIterator,
         typename InputIterator,
         typename Predicate>
  ForwardIterator stable_partition(ForwardIterator first,
                                   ForwardIterator last,
                                   InputIterator stencil,
                                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<ForwardIterator>::type System1;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator>::type   System2;

  System1 system1;
  System2 system2;

  return HYDRA_EXTERNAL_NS::thrust::stable_partition(select_system(system1,system2), first, last, stencil, pred);
} // end stable_partition()


template<typename InputIterator,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    partition_copy(InputIterator first,
                   InputIterator last,
                   OutputIterator1 out_true,
                   OutputIterator2 out_false,
                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator>::type   System1;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator1>::type System2;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator2>::type System3;

  System1 system1;
  System2 system2;
  System3 system3;

  return HYDRA_EXTERNAL_NS::thrust::partition_copy(select_system(system1,system2,system3), first, last, out_true, out_false, pred);
} // end partition_copy()


template<typename InputIterator1,
         typename InputIterator2,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    partition_copy(InputIterator1 first,
                   InputIterator1 last,
                   InputIterator2 stencil,
                   OutputIterator1 out_true,
                   OutputIterator2 out_false,
                   Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator1>::type  System1;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator1>::type  System2;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator1>::type System3;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator2>::type System4;

  System1 system1;
  System2 system2;
  System3 system3;
  System4 system4;

  return HYDRA_EXTERNAL_NS::thrust::partition_copy(select_system(system1,system2,system3,system4), first, last, stencil, out_true, out_false, pred);
} // end partition_copy()


template<typename InputIterator,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    stable_partition_copy(InputIterator first,
                          InputIterator last,
                          OutputIterator1 out_true,
                          OutputIterator2 out_false,
                          Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator>::type   System1;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator1>::type System2;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator2>::type System3;

  System1 system1;
  System2 system2;
  System3 system3;

  return HYDRA_EXTERNAL_NS::thrust::stable_partition_copy(select_system(system1,system2,system3), first, last, out_true, out_false, pred);
} // end stable_partition_copy()


template<typename InputIterator1,
         typename InputIterator2,
         typename OutputIterator1,
         typename OutputIterator2,
         typename Predicate>
  HYDRA_EXTERNAL_NS::thrust::pair<OutputIterator1,OutputIterator2>
    stable_partition_copy(InputIterator1 first,
                          InputIterator1 last,
                          InputIterator2 stencil,
                          OutputIterator1 out_true,
                          OutputIterator2 out_false,
                          Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator1>::type   System1;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator2>::type   System2;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator1>::type  System3;
  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<OutputIterator2>::type  System4;

  System1 system1;
  System2 system2;
  System3 system3;
  System4 system4;

  return HYDRA_EXTERNAL_NS::thrust::stable_partition_copy(select_system(system1,system2,system3,system4), first, last, stencil, out_true, out_false, pred);
} // end stable_partition_copy()


template<typename ForwardIterator, typename Predicate>
  ForwardIterator partition_point(ForwardIterator first,
                                  ForwardIterator last,
                                  Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<ForwardIterator>::type System;

  System system;

  return HYDRA_EXTERNAL_NS::thrust::partition_point(select_system(system), first, last, pred);
} // end partition_point()


template<typename InputIterator, typename Predicate>
  bool is_partitioned(InputIterator first,
                      InputIterator last,
                      Predicate pred)
{
  using HYDRA_EXTERNAL_NS::thrust::system::detail::generic::select_system;

  typedef typename HYDRA_EXTERNAL_NS::thrust::iterator_system<InputIterator>::type System;

  System system;

  return HYDRA_EXTERNAL_NS::thrust::is_partitioned(select_system(system), first, last, pred);
} // end is_partitioned()


} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END
