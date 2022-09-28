#pragma once

#include "AKLib/common.h"
#include "AKLib/Vector.h"

NSB(AKLib)

typedef void* ECSEntityHandle;

struct ECSBaseComponent;

typedef uint32_t (*ECSCreateComponentFn)(Vector<uint8_t>& memory, ECSEntityHandle entity, ECSBaseComponent* comp);
typedef void (*ECSDestroyComponentFn)(ECSBaseComponent* comp);

#define NULL_ENTITY_HANDLE ((ECSEntityHandle)0)

struct ECSBaseComponent
{
	inline static uint32_t RegisterComponentType(ECSCreateComponentFn createFn, ECSDestroyComponentFn destroyFn, size_t size);
	inline static ECSCreateComponentFn GetTypeCreateFn(uint32_t id);
	inline static ECSDestroyComponentFn GetTypeDestroyFn(uint32_t id);
	inline static size_t GetTypeSize(uint32_t id);

	ECSEntityHandle entity = NULL_ENTITY_HANDLE;
private:
	static Vector<std::tuple<ECSCreateComponentFn, ECSDestroyComponentFn, size_t>> s_ComponentTypes;
};

template <typename T>
struct ECSComponent : public ECSBaseComponent
{
	inline static uint32_t CreateComponent(Vector<uint8_t>& memory, ECSEntityHandle entity, ECSBaseComponent* comp);
	inline static void DestroyComponent(ECSBaseComponent* comp);
	static const uint32_t ID;
	static const uint32_t SIZE;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/ECS/Component.inl"
