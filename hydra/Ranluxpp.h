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
 * Ranluxpp.h
 *
 *  Created on: 28/06/2020
 *      Author: Antonio Augusto Alves Junior
 */

#ifndef RANLUXPP_H_
#define RANLUXPP_H_


#include<hydra/detail/Config.h>
#include <stdint.h>

namespace hydra {


#define   likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

class ranluxpp {

protected:

  uint64_t _x[9]; // state vector
  uint64_t _A[9]; // multiplier
  uint64_t _doubles[11]; // cache for double precision numbers
  uint32_t _floats[24];  // cache for single precision numbers
  uint32_t _dpos; // position in cache for doubles
  uint32_t _fpos; // position in cache for floats

  // get a = m - (m-1)/b = 2^576 - 2^552 - 2^240 + 2^216 + 1
  static const uint64_t *geta();

  // fill the cache with float type numbers
  void nextfloats();

  // fill the cache with double type numbers
  void nextdoubles();

  // transfrom the binary state vector of LCG to 24 floats
  void unpackfloats(float *a);

  // transfrom the binary state vector of LCG to 11 doubles
  void unpackdoubles(double *d);
public:
  // The LCG constructor:
  // seed -- jump to the state x_seed = x_0 * A^(2^96 * seed) mod m
  // p    -- the exponent of to get the multiplier A = a^p mod m
  ranluxpp(uint64_t seed, uint64_t p);
  ranluxpp(uint64_t seed) : ranluxpp(seed, 2048){}

  // get access to the state vector
  uint64_t *getstate() { return _x;}

  // get access to the multiplier
  uint64_t *getmultiplier() { return _A;}

  // seed the generator by
  // jumping to the state x_seed = x_0 * A^(2^96 * seed) mod m
  // the scheme guarantees non-colliding sequences
  void init(unsigned long int seed);

  // set the multiplier A to A = a^2048 + 13, a primitive element modulo
  // m = 2^576 - 2^240 + 1 to provide the full period (m-1) of the sequence.
  void primitive();

  // produce next state by the modular mulitplication
  void nextstate();

  // return single precision random numbers uniformly distributed in [0,1).
  float operator()(float __attribute__((unused))) __attribute__((noinline)){
    if(unlikely(_fpos >= 24)) nextfloats();
    return *(float*)(_floats + _fpos++);
  }

  // return double precision random numbers uniformly distributed in [0,1).
  double operator()(double __attribute__((unused))) __attribute__((noinline)){
    if(unlikely(_dpos >= 11)) nextdoubles();
    return *(double*)(_doubles + _dpos++);
  }

  // Fill array size of n by single precision random numbers uniformly
  // distributed in [0,1).
  void getarray(int n, float *a);

  // Fill array size of n by double precision random numbers uniformly
  // distributed in [0,1).
  void getarray(int n, double *a);

  // jump ahead by n 24-bit RANLUX numbers
  void jump(uint64_t n);

  // set skip factor to emulate RANLUX behaviour
  void setskip(uint64_t n);
};

}  // namespace hydra



#endif /* RANLUXPP_H_ */
