#ifndef _H_PWM_
#define _H_PWM_

void PWM_startup();
void PWM_configure();

void PWM_set_DC(int A, int B, int C);
void PWM_set_Z(int Z);
void PWM_halt_Z();

#endif /* _H_PWM_ */

