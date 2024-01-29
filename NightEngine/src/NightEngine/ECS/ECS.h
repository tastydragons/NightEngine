#pragma once

#include "hzpch.h"

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Platform/System/SystemManager.h"
#include "Types.h"

#define p_ecs Night::ECS::GetInstance()

namespace Night
{
	class ECS
	{
	public:

		/*!*************************************************************************
		Returns an instance of the ECS
		****************************************************************************/
		static ECS& GetInstance()
		{
			static ECS instance;
			return instance;
		}

		/*!*************************************************************************
		Initializes Unique pointers to the different systems for communications
		****************************************************************************/
		void Init()
		{
			mComponentManager = std::make_unique<ComponentManager>();
			mEntityManager = std::make_unique<EntityManager>();
			mSystemManager = std::make_unique<SystemManager>();
		}

		// Entity methods

		/*!*************************************************************************
		Pulls a new entity ID from queue and return it
		****************************************************************************/
		Entity CreateEntity()
		{
			return mEntityManager->CreateEntity();
		}

		/*!*************************************************************************
		Destroys the Entity ID and push it back into the back of the queue
		****************************************************************************/
		void DestroyEntity(Entity entity)
		{
			mEntityManager->DestroyEntity(entity);
			mComponentManager->EntityDestroyed(entity);
			mSystemManager->EntityDestroyed(entity);
		}

		/*!*************************************************************************
		Clones the Entity and all its available components
		****************************************************************************/
		Entity CloneEntity(Entity entity)
		{
			Entity newEntity = mEntityManager->CreateEntity();
			mEntityManager->SetSignature(newEntity, GetEntitySignature(entity));
			for (ComponentType i = 0; i < GetTotalRegisteredComponents(); ++i)
			{
				if (GetEntitySignature(newEntity).test(i))
				{
					mComponentManager->GetComponentArrayFromType(i)->CopyComponent(entity, newEntity);
				}
			}
			auto signature = mEntityManager->GetSignature(newEntity);
			mSystemManager->EntitySignatureChanged(newEntity, signature);

			return newEntity;
		}

		/*!*************************************************************************
		Retrieves the entity's signature
		****************************************************************************/
		Signature GetEntitySignature(Entity entity)
		{
			return mEntityManager->GetSignature(entity);
		}

		/*!*************************************************************************
		Sets the Entity's signature for cloning
		****************************************************************************/
		void SetEntitySignature(Entity entity, Signature signature)
		{
			mEntityManager->SetSignature(entity, signature);
		}

		/*!*************************************************************************
		Returns the number of live entities
		****************************************************************************/
		Entity GetTotalEntities()
		{
			return mEntityManager->GetTotalEntities();
		}

		/*!*************************************************************************
		Returns the number of live entities
		****************************************************************************/
		Entity GetAliveEntities()
		{
			return static_cast<Entity>(mEntityManager->GetAliveEntities().size());
		}

		std::set<Entity> GetAliveEntitiesSet()
		{
			return mEntityManager->GetAliveEntities();
		}

		/*!*************************************************************************
		Sets the total entities for Scene build
		****************************************************************************/
		void SetTotalEntitiesForWorldBuild(Entity entity)
		{
			mEntityManager->SetTotalEntitiesForWorld(entity);
		}

		/*!*************************************************************************
		Resets all the entities for Scene build
		****************************************************************************/
		void ResetEntities()
		{
			auto aliveset = mEntityManager->GetAliveEntities();

			for (const auto& iter : aliveset)
			{
				DestroyEntity(iter);
			}

		}

		void ResetAllEntitySignatures()
		{
			mEntityManager->ResetAllEntitySignatures();
		}

		// Component methods
		/*!*************************************************************************
		Registers Component for use in ECS
		****************************************************************************/
		template<typename T>
		void RegisterComponent()
		{
			mComponentManager->RegisterComponent<T>();
		}

		/*!*************************************************************************
		Adds Component to Component Array with the assigned Entity
		****************************************************************************/
		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			component.SetComponentEntityID(entity);
			mComponentManager->AddComponent<T>(entity, component);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), true);
			mEntityManager->SetSignature(entity, signature);
			mSystemManager->EntitySignatureChanged(entity, signature);
		}

		/*!*************************************************************************
		Remove Component and data from selected entity
		****************************************************************************/
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			mComponentManager->RemoveComponent<T>(entity);

			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), false);
			mEntityManager->SetSignature(entity, signature);

			mSystemManager->EntitySignatureChanged(entity, signature);
		}

		/*!*************************************************************************
		Retrieve Component Data from the following component and selected entity
		****************************************************************************/
		template<typename T>
		T& GetComponent(Entity entity)
		{
			return mComponentManager->GetComponent<T>(entity);
		}

		/*!*************************************************************************
		Retrieves Component signature type
		****************************************************************************/
		template<typename T>
		ComponentType GetComponentType()
		{
			return mComponentManager->GetComponentType<T>();
		}

		/*!*************************************************************************
		Get name of component based on component type
		****************************************************************************/
		std::string GetComponentTypeName(ComponentType Type)
		{
			return mComponentManager->GetComponentTypeName(Type);
		}

		/*!*************************************************************************
		Checks if component exist
		****************************************************************************/
		template<typename T>
		bool HaveComponent(Entity entity)
		{
			return mComponentManager->HaveComponent<T>(entity);
		}

		/*!*************************************************************************
		Returns array containing the EntityToIndexMap
		****************************************************************************/
		std::array<size_t, MAX_ENTITIES>& GetEntityToIndexMapECS(ComponentType Type)
		{
			return mComponentManager->GetEntityToIndexMap(Type);
		}

		/*!*************************************************************************
		Returns array containing the IndexToEntityMap
		****************************************************************************/
		std::array<Entity, MAX_ENTITIES>& GetIndexToEntityMapECS(ComponentType Type)
		{
			return mComponentManager->GetIndexToEntityMap(Type);
		}

		/*!*************************************************************************
		Returns total number of Components registered in the ECS
		****************************************************************************/
		const ComponentType GetTotalRegisteredComponents()
		{
			return mComponentManager->GetTotalRegisteredComponents();
		}

		/*!*************************************************************************
		Returns the size of Array based on ComponentType
		****************************************************************************/
		const size_t GetEntitySize(ComponentType Type)
		{
			return mComponentManager->GetEntitySize(Type);
		}

		/*!*************************************************************************
		Clears the Mappings for Scene Build
		****************************************************************************/
		void ClearArrayForWorldBuild(ComponentType Type)
		{
			mComponentManager->ClearArrayForWorldBuild(Type);
		}

		/*!*************************************************************************
		Returns ComponentArray Based on ComponentType
		****************************************************************************/
		std::shared_ptr<IComponentArray> GetComponentArrayFromType(ComponentType Type)
		{
			return mComponentManager->GetComponentArrayFromType(Type);
		}


		// System methods
		/*!*************************************************************************
		Registers the System for use in the ECS
		****************************************************************************/
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return mSystemManager->RegisterSystem<T>();
		}

		/*!*************************************************************************
		Sets the System signature for which component the system shall look out for
		****************************************************************************/
		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			mSystemManager->SetSignature<T>(signature);
		}

	private:
		// Unique Pointer to ComponentManager
		std::unique_ptr<ComponentManager> mComponentManager;
		// Unique Pointer to EntityManager
		std::unique_ptr<EntityManager> mEntityManager;
		// Unique Pointer to SystemManager
		std::unique_ptr<SystemManager> mSystemManager;
		// Unique Pointer to ECS instance
		inline static std::unique_ptr<ECS> m_instance;
	};
}