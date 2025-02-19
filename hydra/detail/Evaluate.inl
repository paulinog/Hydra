/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 - 2023 Antonio Augusto Alves Junior
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
 * Evaluate.inl
 *
 *  Created on: 01/07/2017
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef EVALUATE_INL_
#define EVALUATE_INL_

namespace hydra {

template< hydra::detail::Backend BACKEND, typename Iterator, typename Functor >
auto eval(hydra::detail::BackendPolicy<BACKEND>, Functor const& functor, Iterator begin, Iterator end)
-> typename hydra::detail::BackendPolicy<BACKEND>::template container<typename Functor::return_type>
{

	typedef	typename hydra::detail::BackendPolicy<BACKEND>::template
			container<typename Functor::return_type> container;
	size_t size = hydra_thrust::distance(begin, end) ;
	container Table( size );

	//auto fBegin = hydra_thrust::make_zip_iterator(hydra_thrust::make_tuple(begin) );
	//auto fEnd   = hydra_thrust::make_zip_iterator(hydra_thrust::make_tuple(end)   );

	hydra_thrust::transform(begin, end ,  Table.begin(), functor );

	return std::move(Table);
}

template<hydra::detail::Backend BACKEND, typename Iterator, typename ...Functors>
auto eval(hydra::detail::BackendPolicy<BACKEND>, hydra_thrust::tuple<Functors...> const& functors, Iterator begin, Iterator end)
-> multivector<hydra_thrust::tuple<typename Functors::return_type ...> , hydra::detail::BackendPolicy<BACKEND>>
//-> multivector< typename hydra::detail::BackendPolicy<BACKEND>::template
//container<hydra_thrust::tuple<typename Functors::return_type ...> >>
{
	typedef multivector<hydra_thrust::tuple<typename Functors::return_type ...>,
			hydra::detail::BackendPolicy<BACKEND>> container;

	size_t size = hydra_thrust::distance(begin, end) ;
	container Table( size );


	hydra_thrust::transform(begin, end ,  Table.begin(),
			detail::process< hydra_thrust::tuple<typename Functors::return_type ...>,
			hydra_thrust::tuple<Functors...>>(functors) );

	return std::move(Table);
}

template<hydra::detail::Backend BACKEND, typename Functor, typename Iterator, typename ...Iterators>
auto eval(hydra::detail::BackendPolicy<BACKEND>, Functor const& functor, Iterator begin, Iterator end, Iterators... begins)
-> typename hydra::detail::BackendPolicy<BACKEND>::template
container<typename Functor::return_type>
{
	typedef typename hydra::detail::BackendPolicy<BACKEND>::template
			container<typename Functor::return_type> container;


	size_t size = hydra_thrust::distance(begin, end) ;
	container Table( size );

	auto fBegin = hydra_thrust::make_zip_iterator(hydra_thrust::make_tuple(begin, begins...) );
	auto fEnd   = hydra_thrust::make_zip_iterator(hydra_thrust::make_tuple(end  , (begins+size)...) );

	hydra_thrust::transform(begin, end,  Table.begin(), functor );

	return std::move(Table);
}

template<hydra::detail::Backend BACKEND, typename Iterator,  typename ...Iterators, typename ...Functors>
auto eval(hydra::detail::BackendPolicy<BACKEND>, hydra_thrust::tuple<Functors...> const& functors,
		Iterator begin, Iterator end, Iterators... begins)
-> multivector<hydra_thrust::tuple<typename Functors::return_type ...> , hydra::detail::BackendPolicy<BACKEND> >
//-> multivector< typename hydra::detail::BackendPolicy<BACKEND>::template
//container<hydra_thrust::tuple<typename Functors::return_type ...> >>
{

	typedef multivector<hydra_thrust::tuple<typename Functors::return_type ...>
	, hydra::detail::BackendPolicy<BACKEND> > container;

	size_t size = hydra_thrust::distance(begin, end) ;
	container Table( size );

	auto fBegin = hydra_thrust::make_zip_iterator(hydra_thrust::make_tuple(begin, begins...) );
	auto fEnd   = hydra_thrust::make_zip_iterator(hydra_thrust::make_tuple(end  , (begins+size)...) );

	hydra_thrust::transform(fBegin, fEnd ,  Table.begin(),
			detail::process< hydra_thrust::tuple<typename Functors::return_type ...>,
			hydra_thrust::tuple<Functors...>>(functors) );

	return std::move(Table);
}

}  // namespace hydra

#endif /* EVALUATE_INL_ */
