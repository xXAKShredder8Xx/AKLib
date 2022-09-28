
NSB(AKLib)

AKL_INLINE Vector<std::tuple<ECSCreateComponentFn, ECSDestroyComponentFn, size_t>> ECSBaseComponent::s_ComponentTypes;

AKL_INLINE uint32_t ECSBaseComponent::RegisterComponentType(ECSCreateComponentFn createFn, ECSDestroyComponentFn destroyFn, size_t size)
{
	uint32_t id = s_ComponentTypes.size();
	s_ComponentTypes.push_back(std::tuple<ECSCreateComponentFn, ECSDestroyComponentFn, size_t>(createFn, destroyFn, size));
	return id;
}

AKL_INLINE ECSCreateComponentFn ECSBaseComponent::GetTypeCreateFn(uint32_t id)
{
	return std::get<0>(s_ComponentTypes[id]);
}

AKL_INLINE ECSDestroyComponentFn ECSBaseComponent::GetTypeDestroyFn(uint32_t id)
{
	return std::get<1>(s_ComponentTypes[id]);
}

AKL_INLINE size_t ECSBaseComponent::GetTypeSize(uint32_t id)
{
	return std::get<2>(s_ComponentTypes[id]);
}

template <typename T>
AKL_INLINE const uint32_t ECSComponent<T>::ID(ECSBaseComponent::RegisterComponentType(ECSComponent<T>::CreateComponent, ECSComponent<T>::DestroyComponent, sizeof(T)));

template <typename T>
AKL_INLINE const uint32_t ECSComponent<T>::SIZE(sizeof(T));

template <typename T>
AKL_INLINE uint32_t ECSComponent<T>::CreateComponent(Vector<uint8_t>& memory, ECSEntityHandle entity, ECSBaseComponent* comp)
{
	static_assert(std::is_base_of<ECSComponent<T>, T>::value, "Component type must be a subclass of ECSComponent!");
	uint32_t index = memory.size();
	memory.resize(index + T::SIZE);
	T* component = new(&memory[index]) T(*(T*)comp);
	component->entity = entity;
	return index;
}

template <typename T>
AKL_INLINE void ECSComponent<T>::DestroyComponent(ECSBaseComponent* comp)
{
	static_assert(std::is_base_of<ECSComponent<T>, T>::value, "Component type must be a subclass of ECSComponent!");
	T* component = (T*)comp;
	component->~T();
}

NSE()
