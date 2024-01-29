#pragma once

#include "ComponentArray.h"

namespace Night
{
	class ComponentManager
	{
	public:
		/*!*************************************************************************
		Register Component for use in ECS
		****************************************************************************/
		template <typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

			auto CompArray = std::make_shared<ComponentArray<T>>();

			// Add this component type to the component type map
			mComponentTypes.insert({ typeName, mNextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.insert({ typeName, CompArray });

			ComponentType cType = mComponentTypes[typeName];

			mComponentArraysFromType.try_emplace(cType, CompArray);

			++mNextComponentType;

			++ComponentsRegistered;
		}

		/*!*************************************************************************
		Get unique ID of component type
		****************************************************************************/
		template <typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return mComponentTypes[typeName];
		}

		/*!*************************************************************************
		Gets the component name in type string
		****************************************************************************/
		std::string GetComponentTypeName(ComponentType Type)
		{
			std::string TypeName;
			for (std::unordered_map<const char*, ComponentType>::const_iterator it = mComponentTypes.begin(); it != mComponentTypes.end(); ++it)
			{
				if (it->second == Type)
				{
					TypeName = it->first;
				}
			}
			TypeName = TypeName.substr(TypeName.find_first_of("::") + 2, TypeName.length());
			return TypeName;
		}

		/*!*************************************************************************
		Add Component to Entity via the ECS
		****************************************************************************/
		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			// Add a component to the array for an entity
			component.SetComponentEntityID(entity);
			GetComponentArray<T>()->InsertData(entity, component);
		}

		/*!*************************************************************************
		Remove Component from entity
		****************************************************************************/
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		/*!*************************************************************************
		Retrieve Component from entity based on T type
		****************************************************************************/
		template<typename T>
		T& GetComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		/*!*************************************************************************
		Checks if Entity has Component
		****************************************************************************/
		template<typename T>
		bool HaveComponent(Entity entity)
		{
			return GetComponentArray<T>()->HaveComponent(entity);
		}

		/*!*************************************************************************
		If Entity is destroyed, Remove the data associated with it
		****************************************************************************/
		void EntityDestroyed(Entity entity)
		{
			for (auto const& pair : mComponentArrays)
			{
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}


		/*!*************************************************************************
		Returns the EntityToIndex Mapping
		****************************************************************************/
		std::array<size_t, MAX_ENTITIES>& GetEntityToIndexMap(ComponentType Type)
		{
			return GetComponentArrayFromType(Type)->GetEntityToIndexMap();
		}

		/*!*************************************************************************
		Returns the IndextoEntity Mapping
		****************************************************************************/
		std::array<Entity, MAX_ENTITIES>& GetIndexToEntityMap(ComponentType Type)
		{
			return GetComponentArrayFromType(Type)->GetIndexToEntityMap();
		}

		/*!*************************************************************************
		Get total number of components registered
		****************************************************************************/
		const ComponentType GetTotalRegisteredComponents()
		{
			return ComponentsRegistered;
		}

		/*!*************************************************************************
		Gets the size of entities inside the Component Array
		****************************************************************************/
		const size_t GetEntitySize(ComponentType Type)
		{
			return GetComponentArrayFromType(Type)->GetEntitySize();
		}

		/*!*************************************************************************
		Clear the Component mappings for world build
		****************************************************************************/
		void ClearArrayForWorldBuild(ComponentType Type)
		{
			GetComponentArrayFromType(Type)->ClearForWorldBuild();
		}

		/*!*************************************************************************
		Returns the Component Mapping from Component Type to Shared ptr of IComponent
		****************************************************************************/
		inline std::shared_ptr<IComponentArray> GetComponentArrayFromType(ComponentType Type)
		{
			return mComponentArraysFromType[Type];
		}

	private:

		// Static Counter for number of components registered
		inline static ComponentType ComponentsRegistered{};

		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> mComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

		// Map from type ComponentType to a component array
		std::unordered_map<ComponentType, std::shared_ptr<IComponentArray>> mComponentArraysFromType{};

		// Initial component type id
		ComponentType mNextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
		}

	};
}