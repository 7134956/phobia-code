#ifndef _H_LIB_M_
#define _H_LIB_M_

#define M_EPS_F			1.2E-7f
#define M_PI_F			3.14159265f
#define M_LOG2_F		0.69314718f

inline float m_fabsf(float x) { return __builtin_fabsf(x); }
inline float m_sqrtf(float x) { return __builtin_sqrtf(x); }

int m_isfinitef(float x);
void m_rotf(float y[2], float r, const float x[2]);
float m_atan2f(float y, float x);
float m_sinf(float x);
float m_cosf(float x);
float m_log2f(float x);
float m_logf(float x);
float m_exp2f(float x);
float m_expf(float x);

#endif /* _H_LIB_M_ */

