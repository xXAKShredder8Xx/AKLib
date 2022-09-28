
NSB(AKLib)

AKL_INLINE ECS::ECS()
{}

AKL_INLINE ECS::~ECS()
{}

AKL_INLINE ECSEntityHandle ECS::CreateEntity(ECSBaseComponent* components, const uint32_t* componentIDs, size_t componentCount)
{
}

AKL_INLINE void ECS::RemoveEntity(ECSEntityHandle handle)
{
}

template <typename Component>
AKL_INLINE void ECS::AddComponent(ECSEntityHandle entity, Component* component) 
{
}

AKL_INLINE void ECS::RemoveComponent(ECSEntityHandle entity)
{
}

template <typename Component>
AKL_INLINE void ECS::GetComponent(ECSEntityHandle entity)
{
}

AKL_INLINE void ECS::AddSystem(ECSBaseSystem& system)
{
}

AKL_INLINE void ECS::RemoveSystem(ECSBaseSystem& system)
{
}

AKL_INLINE void ECS::UpdateSystems(float delta)
{
}

AKL_INLINE std::pair<uint32_t, Vector<std::pair<uint32_t, uint32_t>>>* ECS::HandleToRawType(ECSEntityHandle handle)
{
	return (std::pair<uint32_t, Vector<std::pair<uint32_t, uint32_t>>>*)handle;
}

AKL_INLINE uint32_t ECS::HandleToEntityIndex(ECSEntityHandle handle)
{
	return HandleToRawType(handle)->first;
}

AKL_INLINE Vector<std::pair<uint32_t, uint32_t>>& ECS::HandleToEntity(ECSEntityHandle handle)
{
	return HandleToRawType(handle)->second;
}

NSE()