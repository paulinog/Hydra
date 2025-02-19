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
 * FCN1.inl
 *
 *  Created on: 07/03/2020
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef FCN1_INL_
#define FCN1_INL_

#include <hydra/detail/EstimatorTraits.h>

namespace hydra {


/**
 * \ingroup fit
 * \tparam Estimator estimator base class
 * \tparam Iterator iterator pointing to the data
 * \tparam Iterators more iterators pointing to weights, cache etc.
 */
template< template<typename ...> class Estimator, typename PDF, typename Iterator, typename ...Iterators>
class FCN<Estimator<PDF,Iterator,Iterators...>, true>: public ROOT::Minuit2::FCNBase
{

	typedef Estimator<PDF,Iterator,Iterators...> estimator_type;


public:

	typedef hydra_thrust::zip_iterator<hydra_thrust::tuple<Iterators...>> witerator;
	typedef hydra_thrust::zip_iterator<hydra_thrust::tuple<Iterator,Iterators...>> iterator;

    FCN() = delete;

	FCN(PDF const& pdf, Iterator begin, Iterator end, Iterators ...begins):
		fPDF(pdf),
		fErrorDef(0.5),
		fBegin(begin),
		fEnd(end),
		fWBegin(hydra_thrust::make_zip_iterator( hydra_thrust::make_tuple(begins...))),
		fWEnd(hydra_thrust::make_zip_iterator(hydra_thrust::make_tuple((begins + hydra_thrust::distance(begin, end))...))),
		fFCNCache(std::unordered_map<size_t, GReal_t>()),
		fFCNMaxValue(std::numeric_limits<GReal_t>::min())
	{
		auto weights_begin = hydra_thrust::make_zip_iterator( hydra_thrust::make_tuple( begins...));
		auto weights_end   = hydra_thrust::make_zip_iterator( hydra_thrust::make_tuple( (begins + hydra_thrust::distance(begin, end))...));

		typedef typename  hydra_thrust::iterator_traits<decltype(weights_begin)>::value_type arg_type;

		fDataSize = hydra_thrust::transform_reduce(weights_begin, weights_end,
				detail::FCNWeightsReducerUnary<arg_type>() , 0.0, hydra_thrust::plus<double>());

		LoadFCNParameters();
	}


	FCN(FCN<estimator_type, true> const& other):
		ROOT::Minuit2::FCNBase(other),
		fPDF(other.GetPDF()),
		fBegin(other.GetBegin()),
		fEnd(other.GetEnd()),
		fWBegin(other.GetWBegin()),
		fWEnd(other.GetWEnd()),
		fDataSize(other.GetDataSize()),
		fErrorDef(other.GetErrorDef()),
		fUserParameters(other.GetParameters()),
		fFCNCache(other.GetFcnCache()),
		fFCNMaxValue(other.GetFcnMaxValue())
	{
		LoadFCNParameters();
	}

	FCN<estimator_type, true>& operator=(FCN<estimator_type, true> const& other){

		if( this==&other ) return this;

		ROOT::Minuit2::FCNBase::operator=(other);
		fPDF   = other.GetPDF();
		fBegin = other.GetBegin();
		fEnd   = other.GetEnd();
		fDataSize = other.GetDataSize();
		fWBegin = other.GetWBegin();
		fWEnd   = other.GetWEnd();
		fErrorDef = other.GetErrorDef();
		fUserParameters = other.GetParameters();
		fFCNCache = other.GetFcnCache();
		fFCNMaxValue = other.GetFcnMaxValue();
		LoadFCNParameters();

		return this;
	}

	virtual ~FCN()=default;

	// from Minuit2
	double ErrorDef() const{
		return fErrorDef;
	}

	void   SetErrorDef(double error){
		fErrorDef=error;
	}

	double Up() const{
		return fErrorDef;
	}

	virtual GReal_t operator()(const std::vector<double>& parameters) const {

		GReal_t fcn_value = GetFCNValue(parameters) ;


		if(!std::isnormal(fcn_value)){
			if (INFO >= Print::Level()  )
			{
				std::ostringstream stringStream;
				stringStream << "NaN found. Returning fFCNMaxValue=" << fFCNMaxValue << std::endl;
				HYDRA_LOG(INFO, stringStream.str().c_str() )
			}
			return fFCNMaxValue;
		}
		if(fcn_value > fFCNMaxValue) fFCNMaxValue=fcn_value;

		return fcn_value  ;

	}

	//this class
	GReal_t GetErrorDef() const {
		return fErrorDef;
	}

	Iterator  begin() const  {
		return fBegin;
	}

	Iterator  end()  const {
		return fEnd;
	}

	witerator  wbegin() const  {
		return fWBegin;
	}

	witerator  wend()  const {
		return fWEnd;
	}

	void SetBegin(Iterator begin) {
		fBegin = begin;
	}

	void SetEnd(Iterator end) {
		fEnd = end;
	}

	PDF& GetPDF() {
		return fPDF;
	}

	const PDF& GetPDF() const {
		return fPDF;
	}

	void SetPDFParameters(const std::vector<double>& parameters ) const {
		fPDF.SetParameters( parameters);
	}

	hydra::UserParameters& GetParameters() {
		return fUserParameters;
	}

	const hydra::UserParameters& GetParameters() const {
		return fUserParameters;
	}

	void SetParameters(const hydra::UserParameters& userParameters) {
		fUserParameters = userParameters;
	}

	size_t GetDataSize() const
	{
		return fDataSize;
	}

	Iterator GetBegin() const
	{
		return fBegin;
	}

	Iterator GetEnd() const
	{
		return fEnd;
	}

	witerator GetWBegin() const
	{
		return fWBegin;
	}

	witerator GetWEnd() const
	{
		return fWEnd;
	}

	GReal_t GetFcnMaxValue() const
	{
		return fFCNMaxValue;
	}

	void SetFcnMaxValue(GReal_t fcnMaxValue)
	{
		fFCNMaxValue = fcnMaxValue;
	}

private:

	std::unordered_map<size_t, GReal_t>& GetFcnCache() const {
		return fFCNCache;
	}

	void SetFcnCache(std::unordered_map<size_t, GReal_t> fcnCache) {
		fFCNCache = fcnCache;
	}

	GReal_t GetFCNValue(const std::vector<double>& parameters) const {

		size_t key = hydra::detail::hash_range(parameters.begin(),parameters.end());

		auto search = fFCNCache.find(key);

		GReal_t value = 0.0;

		if (search != fFCNCache.end() && fFCNCache.size()>0) {

			if (INFO >= Print::Level()  )
			{
				std::ostringstream stringStream;
				stringStream <<" Found in cache: key "
						<<  search->first
						<< " value "
						<< search->second << std::endl;
				HYDRA_LOG(INFO, stringStream.str().c_str() )
			}

			value = search->second;
		}
		else {
			value = EvalFCN(parameters);

			fFCNCache[key] = value;

			if (INFO >= Print::Level()  )
			{
				std::ostringstream stringStream;
				stringStream <<" Not found in cache. Calculated and cached: key "
						<<  key
						<< " value "
						<< value << std::endl;
				HYDRA_LOG(INFO, stringStream.str().c_str() )
			}
		}

		return value;
	}

	GReal_t EvalFCN(const std::vector<double>& parameters) const {
		return static_cast<const estimator_type*>(this)->Eval(parameters);
	}

	void LoadFCNParameters(){
		std::vector<hydra::Parameter*> temp;
		fPDF.AddUserParameters(temp );
		fUserParameters.SetVariables( temp);
	}


	mutable PDF fPDF;
	witerator fWBegin;
	witerator fWEnd;
	Iterator fBegin;
	Iterator fEnd;
	GReal_t  fErrorDef;
	GReal_t  fDataSize;
	mutable GReal_t  fFCNMaxValue;
	hydra::UserParameters fUserParameters ;
	mutable std::unordered_map<size_t, GReal_t> fFCNCache;


};

}  // namespace hydra

#endif /* FCN1_INL_ */
