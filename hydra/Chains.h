/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 - 2020 Antonio Augusto Alves Junior
 *
 *   This file is part of Hydra Data Analysis Framework.
 *
 *   Hydra is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Hydra is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Hydra.  If not, see <http://www.gnu.org/licenses/>.
 *
 *---------------------------------------------------------------------------*/

/*
 * Chains.h
 *
 *  Created on: 07/10/2017
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef CHAINS_H_
#define CHAINS_H_

#include <utility>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <cassert>

#include <hydra/detail/Config.h>
#include <hydra/detail/BackendPolicy.h>
#include <hydra/Types.h>
#include <hydra/Containers.h>
#include <hydra/Decays.h>
#include <hydra/detail/utility/Utility_Tuple.h>
#include <hydra/detail/utility/Generic.h>
#include <hydra/detail/functors/FlagAcceptReject.h>
#include <hydra/Placeholders.h>
//thrust
#include <hydra/detail/external/hydra_thrust/tuple.h>
#include <hydra/detail/external/hydra_thrust/iterator/zip_iterator.h>
#include <hydra/detail/external/hydra_thrust/distance.h>

namespace hydra {

/**
 * \ingroup phsp
 */
template<typename... Decays>
class Chains;


/**
 * \ingroup phsp
 * \brief This  class provides memory storage for a chain of decays.
 *
 * Allocate memory for a chain of decays. Each decay needs be generated by a corresponding instance of hydra::PhaseSpace.
 * \tparam N number of particles in final states of each decay.
 * \tparam BACKEND
 */
template<size_t ...N, hydra::detail::Backend BACKEND>
class Chains<Decays<N, hydra::detail::BackendPolicy<BACKEND> >...> {

	constexpr const static size_t NDecays = sizeof...(N);
	typedef hydra::detail::BackendPolicy<BACKEND> system_t;

	typedef decltype(hydra::detail::make_index_sequence<sizeof...(N)> {}) indexing_type;

public:

	//direct iterators
	typedef hydra_thrust::tuple<typename
	Decays<N,hydra::detail::BackendPolicy<BACKEND> >::iterator...> iterator_tuple;

	typedef hydra_thrust::tuple<typename
	Decays<N,hydra::detail::BackendPolicy<BACKEND> >::const_iterator...> const_iterator_tuple;

	//reverse iterators
	typedef hydra_thrust::tuple<typename
	Decays<N,hydra::detail::BackendPolicy<BACKEND> >::reverse_iterator...> reverse_iterator_tuple;

	typedef hydra_thrust::tuple<typename
	Decays<N,hydra::detail::BackendPolicy<BACKEND> >::const_reverse_iterator...> const_reverse_iterator_tuple;

	//weights
	typedef typename system_t::template container<GReal_t> weights_type;
	typedef typename weights_type::iterator iterator_v;
	typedef typename weights_type::const_iterator const_iterator_v;
	typedef typename weights_type::iterator reverse_iterator_v;
	typedef typename weights_type::const_iterator const_reverse_iterator_v;

	//zipped iterators
	typedef hydra_thrust::zip_iterator<
	typename detail::tuple_cat_type<
	hydra_thrust::tuple<iterator_v>,
	iterator_tuple
	>::type
	> iterator;

	typedef hydra_thrust::zip_iterator<
	typename detail::tuple_cat_type<
	hydra_thrust::tuple<const_iterator_v>,
	const_iterator_tuple
	>::type
	> const_iterator;

	typedef hydra_thrust::zip_iterator<
	typename detail::tuple_cat_type<
	hydra_thrust::tuple<reverse_iterator_v>,
	reverse_iterator_tuple
	>::type
	> reverse_iterator;

	typedef hydra_thrust::zip_iterator<
	typename detail::tuple_cat_type<
	hydra_thrust::tuple<const_reverse_iterator_v>,
	const_reverse_iterator_tuple
	>::type
	> const_reverse_iterator;

	typedef typename iterator::value_type value_type;
	typedef typename iterator::reference reference_type;
	typedef typename hydra_thrust::iterator_traits<const_iterator>::reference const_reference_type;

	//cast iterator
	template < typename Iterator, typename Arg, typename Functor>
	using __caster_iterator = hydra_thrust::transform_iterator<Functor,
			Iterator, typename std::result_of<Functor(Arg&)>::type >;


	typedef hydra_thrust::tuple< Decays<N,hydra::detail::BackendPolicy<BACKEND> >...> decays_type;
	/**
	 * @brief default constructor
	 */
	Chains() = default;

	/**
	 * @brief Constructor allocating memory for a given number of events.
	 * @param nevents number of events
	 */
	Chains(size_t n) {resize(n);}

	/**
	 * @brief Copy constructor
	 * @param other chain container defined at same back-end.
	 */
	Chains(Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND> >...>const& other):
	fDecays(other.__copy_decays()),
	fWeights(other.__copy_weights()) {}

	/**
	 * @brief Copy constructor
	 * @param other chain container defined in other back-end.
	 */
	template<hydra::detail::Backend BACKEND2>
	Chains(Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND2> >...>const& other)
	{
		this->resize(hydra_thrust::distance(other.begin(), other.end()));
		hydra_thrust::copy(other.begin(), other.end(), this->begin() );
	}

	/**
	 *@brief  Move constructor
	 * @param other
	 */
	Chains(Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND> >...>&& other):
	fDecays(other.__move_decays()),
	fWeights(other.__move_weights())
	{}

	/**
	 * @brief Assignment operator for chain container allocated in the same back-end
	 * @param other
	 * @return
	 */
	Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND> >...>&
	operator=(Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND> >...> const& other)
	{
		if(this==&other) return *this;
		this->fDecays = other.__copy_decays();
		this->fWeights = other.__copy_weights();

		return *this;
	}

	/**
	 * @brief Assignment operator for chain container allocated in a different back-end.
	 * @param other
	 * @return
	 */
	template<hydra::detail::Backend BACKEND2>
	Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND> >...>&
	operator=(Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND2> >...>const& other)
	{
		hydra_thrust::copy(other.begin(), other.end(), this->begin() );

		return *this;
	}

	/**
	 * @brief Move-assignment operator for chain container allocated in a same back-end.
	 * @param other
	 * @return
	 */
	Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND> >...>&
	operator=(Chains<Decays<N,hydra::detail::BackendPolicy<BACKEND> >...>&& other)
	{
		if(this==&other) return *this;
		this->fDecays = other.__move_decays();
		this->fWeights = other.__move_weights();
		return *this;
	}

	Range<iterator_v >
	GetWeights() {
		return hydra::make_range(this->fWeights.begin(), this->fWeights.end());
	}

	Range<const_iterator_v >
	GetWeights() const {
		return hydra::make_range(this->fWeights.begin(), this->fWeights.end());
	}

	template<unsigned int I>
    typename hydra_thrust::tuple_element<I, decays_type >::type&
	GetDecays(placeholders::placeholder<I> ) {
		return hydra_thrust::get<I>(this->fDecays);
	}

	template<unsigned int I>
	typename hydra_thrust::tuple_element<I, decays_type >::type const&
	GetDecays(placeholders::placeholder<I> ) const {
		return hydra_thrust::get<I>(this->fDecays);
	}

	/**
	 * @brief Iterator to begin of container range.
	 * @return iterator
	 */
	iterator begin() {return __begin();}

	/**
	 * @brief Iterator to end of container range.
	 * @return iterator
	 */
	iterator end() {return __end();}

	/*
	 * constant access iterators
	 */
	const_iterator begin() const {return __begin();}
	const_iterator end() const {return __end();}
	const_iterator cbegin() const {return __cbegin();}
	const_iterator cend() const {return __cend();}

	/**
	 * @brief Iterator to begin of container range.
	 * @return iterator
	 */
	reverse_iterator rbegin() {return __rbegin();}

	/**
	 * @brief Iterator to end of container range.
	 * @return iterator
	 */
	reverse_iterator rend() {return __rend();}

	/*
	 * constant access iterators
	 */
	const_reverse_iterator rbegin() const {return __rbegin();}
	const_reverse_iterator rend() const {return __rend();}
	const_reverse_iterator crbegin() const {return __crbegin();}
	const_reverse_iterator crend() const {return __crend();}

	/**
	 * @brief capacity.
	 * @return capacity in number of elements
	 */
	size_t capacity() const {return fWeights.capacity();}

	/**
	 * @brief resize container to N.
	 */
	void resize(size_t n) {__resize(n);}

	/**
	 * @brief size of the container in number of elements.
	 * @return
	 */
	size_t size() const {return fWeights.size();}

	/**
	 * @brief subscript operator.
	 * @param i
	 * @return reference to chain i
	 */
	reference_type operator[](size_t i)
	{	return this->begin()[i];}

	const reference_type operator[](size_t i) const
	{	return this->begin()[i];}

private:

	const weights_type& __copy_weights() const {return fWeights;}
	const decays_type& __copy_decays() const {return fDecays;}

	weights_type __move_weights() {return std::move(fWeights);}
	decays_type __move_decays() {return std::move(fDecays);}

	//_______________________________________________
	//resize

	template<size_t I>
	inline typename hydra_thrust::detail::enable_if<(I == NDecays), void >::type
	__resize_helper( size_t ) {}

	template<size_t I = 0>
	inline typename hydra_thrust::detail::enable_if<(I < NDecays), void >::type
	__resize_helper( size_t n)
	{
		hydra_thrust::get<I>(fDecays).resize(n);
		__resize_helper<I+1>(n);
	}

	void __resize( size_t n)
	{
		fWeights.resize(n);
		__resize_helper(n);
	}

	//_______________________________________________
	//clear

	template<size_t I>
	inline typename hydra_thrust::detail::enable_if<(I == NDecays), void >::type
	__clear_helper() {}

	template<size_t I = 0>
	inline typename hydra_thrust::detail::enable_if<(I < NDecays), void >::type
	__clear_helper()
	{
		std::get<I>(fDecays).clear();
		__clear_helper<I+1>();
	}

	void __clear() {fWeights.clear(); __clear_helper();}

	//_______________________________________________
	//shrink_to_fit

	template<size_t I>
	inline typename hydra_thrust::detail::enable_if<(I == NDecays), void >::type
	__shrink_to_fit_helper() {}

	template<size_t I = 0>
	inline typename hydra_thrust::detail::enable_if<(I < NDecays), void >::type
	__shrink_to_fit_helper()
	{
		std::get<I>(fDecays).shrink_to_fit();
		__shrink_to_fit_helper<I+1>();
	}

	void __shrink_to_fit() {fWeights.shrink_to_fit(); __shrink_to_fit_helper();}

	//_______________________________________________
	//reserve

	template<size_t I>
	inline typename hydra_thrust::detail::enable_if<(I == NDecays), void >::type
	__reserve_helper( size_t ) {}

	template<size_t I = 0>
	inline typename hydra_thrust::detail::enable_if<(I < NDecays), void >::type
	__reserve_helper( size_t n)
	{
		std::get<I>(fDecays).reserve(n);
		__reserve_helper<I+1>(n);
	}

	void __reserve( size_t n) {fWeights.reserve(n); __reserve_helper(n);}

	//__________________________________________
	// caster accessors
	template<typename Functor>
	inline __caster_iterator<iterator, value_type, Functor> __begin( Functor const& caster )
	{
		return __caster_iterator<iterator, value_type, Functor>(this->begin(), caster);
	}

	template<typename Functor>
	inline __caster_iterator<iterator, value_type, Functor> __end( Functor const& caster )
	{
		return __caster_iterator<iterator, value_type, Functor>(this->end(), caster);
	}

	template<typename Functor>
	inline __caster_iterator<reverse_iterator, value_type, Functor> __rbegin( Functor const& caster )
	{
		return __caster_iterator<reverse_iterator, value_type, Functor>(this->rbegin(), caster);
	}

	template<typename Functor>
	inline __caster_iterator<reverse_iterator, value_type, Functor> __rend( Functor const& caster )
	{
		return __caster_iterator<reverse_iterator, value_type, Functor>(this->end(), caster);
	}

	// non-constant access
	//___________________________________________
	//begin
	template < size_t... I>
	iterator __begin_helper(detail::index_sequence<I...>) {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.begin(),hydra_thrust::get<I>(fDecays).begin() ...));
	}

	iterator __begin() {return __begin_helper( detail::make_index_sequence<NDecays> {});}

	//___________________________________________
	//end
	template < size_t... I>
	iterator __end_helper(detail::index_sequence<I...>) {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.end(),hydra_thrust::get<I>(fDecays).end()... ));
	}

	iterator __end() {return __end_helper( detail::make_index_sequence<NDecays> {});}
	//___________________________________________
	//rbegin
	template < size_t... I>
	reverse_iterator __rbegin_helper(detail::index_sequence<I...>) {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.rbegin(),hydra_thrust::get<I>(fDecays).rbegin()... ));
	}

	reverse_iterator __rbegin() {return __rbegin_helper( detail::make_index_sequence<NDecays> {});}
	//___________________________________________
	//rend
	template < size_t... I>
	reverse_iterator __rend_helper(detail::index_sequence<I...>) {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.rend(),hydra_thrust::get<I>(fDecays).rend() ...));
	}

	reverse_iterator __rend() {return __rend_helper( detail::make_index_sequence<NDecays> {});}

	// constant access
	//___________________________________________
	//begin
	template < size_t... I>
	const_iterator __begin_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.begin(),hydra_thrust::get<I>(fDecays).begin() ...));
	}

	const_iterator __begin() const {return __begin_helper( detail::make_index_sequence<NDecays> {});}

	//___________________________________________
	//end
	template < size_t... I>
	const_iterator __end_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.end(),hydra_thrust::get<I>(fDecays).end() ...));
	}

	const_iterator __end() const {return __end_helper( detail::make_index_sequence<NDecays> {});}
	//___________________________________________
	//rbegin
	template < size_t... I>
	const_reverse_iterator __rbegin_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.rbegin(),hydra_thrust::get<I>(fDecays).rbegin() ...));
	}

	const_reverse_iterator __rbegin() const {return __rbegin_helper( detail::make_index_sequence<NDecays> {});}
	//___________________________________________
	//rend
	template < size_t... I>
	const_reverse_iterator __rend_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.rend(),hydra_thrust::get<I>(fDecays).rend() ...));
	}

	const_reverse_iterator __rend() const {return __rend_helper( detail::make_index_sequence<NDecays> {});}

	//___________________________________________
	//begin
	template < size_t... I>
	const_iterator __cbegin_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.cbegin(),hydra_thrust::get<I>(fDecays).cbegin() ...));
	}

	const_iterator __cbegin() const {return __cbegin_helper( detail::make_index_sequence<NDecays> {});}

	//___________________________________________
	//end
	template < size_t... I>
	const_iterator __cend_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.cend(),hydra_thrust::get<I>(fDecays).cend() ...));
	}

	const_iterator __cend() const {return __cend_helper( detail::make_index_sequence<NDecays> {});}

	//___________________________________________
	//rbegin
	template < size_t... I>
	const_reverse_iterator __crbegin_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.crbegin(),hydra_thrust::get<I>(fDecays).crbegin()... ));
	}

	const_reverse_iterator __crbegin() const {return __crbegin_helper( detail::make_index_sequence<NDecays> {});}
	//___________________________________________
	//rend
	template < size_t... I>
	const_reverse_iterator __crend_helper(detail::index_sequence<I...>) const {

		return hydra_thrust::make_zip_iterator(
				hydra_thrust::make_tuple(fWeights.crend(),hydra_thrust::get<I>(fDecays).crend() ...));
	}

	const_reverse_iterator __crend() const {return __crend_helper( detail::make_index_sequence<NDecays> {});}

	//___________________________________________
	//

	decays_type fDecays;
	weights_type fWeights;

};

template<size_t ...N,hydra::detail::Backend BACKEND >
Chains< Decays<N, hydra::detail::BackendPolicy<BACKEND> >...>
make_chain( hydra::detail::BackendPolicy<BACKEND>, size_t entries )
{
	return Chains<Decays<N, hydra::detail::BackendPolicy<BACKEND> >...>(entries) ;
}

}  // namespace hydra

//#include <hydra/detail/Chains.inl>




#endif /* CHAINS_H_ */
