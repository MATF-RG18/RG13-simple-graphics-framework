#ifndef SENSEFX_ANIMATED_OBJECT_H
#define SENSEFX_ANIMATED_OBJECT_H

class AnimatedObject {

	protected:
		bool animationOngoing = false;
		float step = 0.01;

	public:

		float animationParameter=0.0;
		virtual void animate()=0;
		void startAnimation();
		void endAnimation();
		bool isAnimationOngoing();
		void setStep(float);
		void inctementForStep();

};

#endif //ANIMATED_OBJECT
