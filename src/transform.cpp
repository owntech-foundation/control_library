#include "transform.h"

clarke_t Transform::clarke(three_phase_t Xabc)
{
	clarke_t Xab;
	Xab.alpha = 2.0 / 3.0 * (Xabc.a - 0.5 * (Xabc.b + Xabc.c)); 
	Xab.beta = SQRT3_INVERSE * (Xabc.b - Xabc.c);
	Xab.o = 2.0 / 3.0 * 0.5 * (Xabc.a + Xabc.b + Xabc.c);
	return Xab;	
}

three_phase_t Transform::clarke_inverse(clarke_t Xab)
{
	three_phase_t Xabc;

	Xabc.a = Xab.alpha + Xab.o;
	Xabc.b = -0.5  * Xab.alpha  + SQRT3_DIV_2 * Xab.beta + Xab.o; 
	Xabc.c = -0.5  * Xab.alpha  - SQRT3_DIV_2 * Xab.beta + Xab.o; 

	return Xabc;
}

dqo_t Transform::rotation_to_dqo(clarke_t Xab, float32_t theta)
{
	dqo_t Xdq;
	float32_t cos_theta = ot_cos(theta);
	float32_t sin_theta = ot_sin(theta);
	Xdq.d = Xab.alpha * cos_theta + Xab.beta * sin_theta;
	Xdq.q = - Xab.alpha * sin_theta + Xab.beta * cos_theta;
	Xdq.o = Xab.o;

	return Xdq;
}

clarke_t Transform::rotation_to_clarke(dqo_t Xdq, float32_t theta)
{
	// FIXME: change the way to have rotation_to_clarke and rotation_to_clarke equals
	clarke_t Xab;
	float32_t cos_theta = ot_cos(theta);
	float32_t sin_theta = ot_sin(theta);
	Xab.alpha = Xdq.d * cos_theta - Xdq.q * sin_theta;
	Xab.beta = + Xdq.d * sin_theta + Xdq.q * cos_theta;
	Xab.o = Xdq.o;

	return Xab;

}

dqo_t Transform::to_dqo(three_phase_t Xabc, float32_t theta) 
{
	return Transform::rotation_to_dqo(Transform::clarke(Xabc), theta);	
};

three_phase_t Transform::to_threephase(dqo_t Xdq, float32_t theta) 
{
	return Transform::clarke_inverse(Transform::rotation_to_clarke(Xdq, theta));	
};


