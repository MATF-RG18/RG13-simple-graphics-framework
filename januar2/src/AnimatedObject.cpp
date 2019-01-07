#include "AnimatedObject.h"

void AnimatedObject::startAnimation() {
	this->animationOngoing = true;
}

void AnimatedObject::endAnimation() {
	this->animationOngoing = false;
}

bool AnimatedObject::isAnimationOngoing() {
	return this->animationOngoing;
}

void AnimatedObject::setStep(float step) {
	this->step = step;
}

void AnimatedObject::inctementForStep() {

	if (animationParameter + step > 1)
		animationParameter = 0;

	this->animationParameter += this->step;
}
