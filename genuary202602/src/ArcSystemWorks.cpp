#include "ArcSystemWorks.h"
#include "cinder/Rand.h"

ArcLite::ArcLite()
{
	this->reset();
}

void ArcLite::step(vec2 _gravity)
{
	this->mAge -= 1.0f;
	if (mAge <= 0.0f) {
		this->reset();
		return;
	}

	float t_Life = this->mAge / this->mLife;
	vec2 t_grav = _gravity * (1.0f - t_Life);
	this->mSpd += t_grav * this->mMass;
	this->mPos += (this->mSpd + _gravity);
}

void ArcLite::show()
{
	float t_Life = (this->mAge / this->mLife);
	gl::drawSolidCircle(this->mPos, this->mRadius * t_Life);
}

void ArcLite::reset()
{
	this->mPos = vec2(Rand::randFloat(60.0f, 480.0f), Rand::randFloat(900.0f, 960.0f));
	this->mSpd = vec2(Rand::randFloat(-3.0f, 3.0f), Rand::randFloat(-8.1f, -2.2f));
	this->mAge = Rand::randFloat(30.0f, 240.0f);
	this->mLife = mAge;
	this->mMass = Rand::randFloat(0.1f, 0.6f);
	this->mRadius = Rand::randFloat(4.0f, 10.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void ArcSystem::step()
{
	if (mArcList.size() < mCount)
	{
		mArcList.push_back(ArcLite());
	}
	if (mArcList.size() > 0)
	{
		auto arcs = mArcList.size();
		for (int i = 0; i < arcs; i++)
		{
			mArcList[i].step(this->mGravity);
		}
	}
}

void ArcSystem::show()
{
	if (mArcList.size() > 0)
	{
		auto arcs = mArcList.size();
		for (int i = 0; i < arcs; i++)
		{
			mArcList[i].show();
		}
	}
}

ArcSystem::ArcSystem()
{
}

ArcSystem::ArcSystem(int _count, vec2 _gravity)
{
	this->mCount = _count;
	this->mGravity = _gravity;
	this->mArcList.clear();
}
