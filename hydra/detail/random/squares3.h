/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 - 2022 Antonio Augusto Alves Junior
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
 * squares3.h
 *
 *  Created on: 11/07/2020
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef SQUARES3_H_
#define SQUARES3_H_



#include <hydra/detail/Config.h>
#include <stdint.h>
#include <hydra/detail/random/detail/squares_key.h>

namespace hydra {

namespace random {

/*
*  Three round counter-based middle square
*
*  squares3 - returns a 32-bit unsigned int [0,0xffffffff]
*
*
*  Three rounds of squaring are performed and the result is returned.
*  For the first two rounds, the result is rotated right 32 bits.
*  This places the random data in the best position for the next round.
*  y = ctr*key or z = (ctr+1)*key is added on each round.  For keys
*  generated by the key utility, either ctr*key or (ctr+1)*key will
*  have non-zero digits.  This improves randomization and also provides
*  for a uniform output.
*
*  Note:  The squares RNG was based on ideas derived from Middle Square
*  Weyl Sequence RNG.  One of the ideas was to obtain uniformity by adding
*  the Weyl sequence after squaring.  Richard P. Brent (creator of the
*  xorgens RNG) suggested this method.  It turns out that adding ctr*key
*  is equivalent.  Brent's idea provides the basis for uniformity in this
*  generator.
*
*  Implementation of the algorithm authored by Bernard Widynski and
*  described in https://arxiv.org/pdf/2004.06278v2.pdf
*/

class squares3
{

public:

	typedef uint64_t   state_type;
	typedef uint64_t    seed_type;
	typedef uint64_t advance_type;
	typedef uint32_t  result_type;

	__hydra_host__ __hydra_device__
	squares3()=delete;

	__hydra_host__ __hydra_device__
	squares3(seed_type  s):
	  fState(0),
      fSeed(seed_type{ splitmix<seed_type>(s) })
    {}

	__hydra_host__ __hydra_device__
	squares3( squares3 const& other):
	  fState(other.GetState() ),
      fSeed(other.GetSeed() )
     {}

	__hydra_host__ __hydra_device__
	inline squares3& operator=( squares3 const& other)
	{
	  if(this==&other) return *this;

	  fState  = other.GetState();
      fSeed   = other.GetSeed();

      return *this;
    }


	__hydra_host__ __hydra_device__
	inline result_type operator()(void)
	{
		uint64_t x, y, z;

		y = x = fSeed*fState ; z = y + fSeed;

		x = x*x + y; x = (x>>32) | (x<<32);       /* round 1 */

		x = x*x + z; x = (x>>32) | (x<<32);       /* round 2 */

		++fState;                                 /* advance state */

		return (x*x + y) >> 32;                   /* round 3 */

	}

	__hydra_host__ __hydra_device__
	inline void discard(advance_type n){

		fState += n;
	}

	__hydra_host__ __hydra_device__
	inline seed_type GetSeed() const {
		return fSeed;
	}

	__hydra_host__ __hydra_device__
	inline void SetSeed(seed_type seed) {
		fSeed = seed;
	}

	__hydra_host__ __hydra_device__
	inline state_type GetState() const {
		return fState;
	}

	__hydra_host__ __hydra_device__
	inline void SetState( state_type state) {
		fState = state;
	}

	inline static uint64_t GenerateSeed( size_t i ){
			return random::detail::keys[i];
		}

	static const  uint32_t HYDRA_PREVENT_MACRO_SUBSTITUTION min  = 0;

	static const  uint32_t HYDRA_PREVENT_MACRO_SUBSTITUTION max = std::numeric_limits<result_type>::max();

private:

	state_type fState;
	seed_type   fSeed;
};

}  // namespace random

}  // namespace hydra


#endif /* SQUARES3_H_ */
