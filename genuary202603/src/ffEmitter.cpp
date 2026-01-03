#include "ffEmitter.h"

ffEmitter::ffEmitter() : mCount(0), mColor(ColorA(1,1,1,1)), mOrigin(vec3(0.0f))
{
}

ffEmitter::ffEmitter(int ffCount, vec4 ffColor, vec3 ffOrigin)
{
	mCount = ffCount;
	mColor = ffColor;
	mOrigin = ffOrigin;

	float thStep = glm::two_pi<float>() / static_cast<float>(mCount);
	for (int i = 0; i < mCount; i++)
	{
		ffParticle particle;
		float th = thStep * static_cast<float>(i);

		particle.mPos = vec3(0.01f * cos(th), 0.01f*sin(th), 0.0f);
		particle.mVel = normalize(particle.mPos);

		mParticles.push_back(particle);
	}
}

void ffEmitter::step()
{
	mAge -= 1;
	if (mAge > 0)
	{
		for (int i = 0; i < mCount; i++)
		{
			mParticles[i].mPos += 0.025f * mParticles[i].mVel;
		}
		mColor.a = glm::clamp<float>(static_cast<float>(mAge) / 15.0f, 0.0f, 1.0f);
	}
	else
	{
		mAlive = false;
	}
}

void ffEmitter::show()
{
}
