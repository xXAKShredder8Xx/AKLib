#pragma once

#include "AKLib/common.h"
#include "AKLib/ECS/Component.h"
#include "AKLib/ECS/System.h"

NSB(AKLib)

class ECS
{
public:
	inline ECS();
	inline ~ECS();

	inline ECSEntityHandle CreateEntity(ECSBaseComponent* components, const uint32_t* componentIDs, size_t componentCount);
	inline void RemoveEntity(ECSEntityHandle handle);

	template <typename Component>
	inline void AddComponent(ECSEntityHandle entity, Component* component);
	inline void RemoveComponent(ECSEntityHandle entity);

	template <typename Component>
	inline void GetComponent(ECSEntityHandle entity);

	inline void AddSystem(ECSBaseSystem& system);
	inline void RemoveSystem(ECSBaseSystem& system);
	inline void UpdateSystems(float delta);

private:
	Vector<ECSBaseSystem*> systems;
	HashMap<uint32_t, Vector<uint8_t>> components;
	Vector<std::pair<uint32_t, Vector<std::pair<uint32_t, uint32_t>>>* > entities;

	inline std::pair<uint32_t, Vector<std::pair<uint32_t, uint32_t>>>* HandleToRawType(ECSEntityHandle handle);
	inline uint32_t HandleToEntityIndex(ECSEntityHandle handle);
	inline Vector<std::pair<uint32_t, uint32_t>>& HandleToEntity(ECSEntityHandle handle);
};

NSE()

#define AKL_INLINE inline
#include "AKLib/ECS/Ecs.inl"
