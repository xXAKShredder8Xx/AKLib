#pragma once

#include "AKLib/common.h"
#include "AKLib/ECS/Component.h"

NSB(AKLib)

class ECSBaseSystem
{
public:
	ECSBaseSystem(const Vector<uint32_t>& types);
	virtual void UpdateComponents(float delta, ECSBaseComponent** components) = 0;
private:
	Vector<uint32_t> componentTypes;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/ECS/System.inl"
