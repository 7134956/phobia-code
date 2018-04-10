/*
   Phobia Motor Controller for RC and robotics.
   Copyright (C) 2017 Roman Belov <romblv@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_PM_MATH_
#define _H_PM_MATH_

#define M_PI_F			3.14159265f
#define M_EPS_F			1.2E-7f

#ifndef _MATH_H

inline float
fabsf(float x) { return __builtin_fabsf(x); }

inline float
sqrtf(float x) { return __builtin_sqrtf(x); }

#endif /* _MATH_H */

void pm_rotf(float y[2], float r, const float x[2]);
float pm_atan2f(float y, float x);
float pm_sinf(float x);
float pm_cosf(float x);

float pm_DFT_const_R(const float DFT[8]);
void pm_DFT_const_L(const float DFT[8], float freq, float LDQ[3]);

#endif /* _H_PM_MATH_ */

