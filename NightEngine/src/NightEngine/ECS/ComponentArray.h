#pragma once

#include "Types.h"

#include "Components/Components.h"

namespace Night
{
	/*!*************************************************************************
	Interface Class for Component Array
	****************************************************************************/
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;

		virtual void EntityDestroyed(Entity entity) = 0;

		virtual void CopyComponent(Entity entity, Entity entityToCopy) = 0;

		virtual std::array<size_t, MAX_ENTITIES>& GetEntityToIndexMap() = 0;

		virtual std::array<Entity, MAX_ENTITIES>& GetIndexToEntityMap() = 0;

		virtual const size_t GetEntitySize() = 0;

		virtual void ClearForWorldBuild() = 0;

	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		/*!*************************************************************************
		inline Function for filling Entity and Index Arrays with empty entities,
		MAX_ENTITIES is being used to denote invalid entity
		****************************************************************************/
		inline ComponentArray()
		{
			std::fill(mEntityToIndexMap.begin(), mEntityToIndexMap.end(), MAX_ENTITIES);
			std::fill(mIndexToEntityMap.begin(), mIndexToEntityMap.end(), MAX_ENTITIES);
		}

		/*!*************************************************************************
		Inserts Component Data into mComponentArray and Update mappings
		****************************************************************************/
		void InsertData(Entity entity, T component)
		{
			assert(mEntityToIndexMap[entity] == MAX_ENTITIES && "Component added to same entity more than once.");

			size_t newIndex = mSize;
			mEntityToIndexMap[entity] = newIndex;
			mIndexToEntityMap[newIndex] = entity;
			mComponentArray[newIndex] = component;
			component.SetComponentEntityID(entity);
			++mSize;
		}

		/*!*************************************************************************
		Remove the data from the mComponentArray by shifting the data from the last
		element to the removed element to keep density, mappings for the position in
		the 2 arrays are then updated accordingly
		****************************************************************************/
		void RemoveData(Entity entity)
		{
			assert(mEntityToIndexMap[entity] != MAX_ENTITIES && "Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
			size_t indexOfLastElement = mSize - 1;
			mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
			mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;
			mEntityToIndexMap[entity] = MAX_ENTITIES;
			mIndexToEntityMap[indexOfLastElement] = MAX_ENTITIES;

			--mSize;
		}

		/*!*************************************************************************
		Retrieves the Data from the mComponentArray
		****************************************************************************/
		T& GetData(Entity entity)
		{
			assert(mEntityToIndexMap[entity] != MAX_ENTITIES && "Retrieving non-existent component.");
			// Return a reference to the entity's component
			return mComponentArray[mEntityToIndexMap[entity]];
		}

		/*!*************************************************************************
		Checks if Component exist by checking if there is an entity inside the
		mEntityToIndexMap array
		****************************************************************************/
		bool HaveComponent(Entity entity)
		{
			if (mEntityToIndexMap[entity] != MAX_ENTITIES)
			{
				return true;
			}
			return false;
		}

		/*!*************************************************************************
		If Entity is destroyed, Remove the data associated with it
		****************************************************************************/
		void EntityDestroyed(Entity entity) override
		{
			if (mEntityToIndexMap[entity] != MAX_ENTITIES)
			{
				// Remove the entity's component if it existed
				RemoveData(entity);
			}
		}

		/*!*************************************************************************
		Copy Component from another entity
		****************************************************************************/
		void CopyComponent(Entity entity, Entity entityToCopy) override
		{
			T* CopyComponent = &mComponentArray[mEntityToIndexMap[entity]];
			T* CloneComponent = new T(*CopyComponent);
			CloneComponent->SetComponentEntityID(entityToCopy);
			InsertData(entityToCopy, *CloneComponent);
			delete CloneComponent;
		}

		/*!*************************************************************************
		If Entity is destroyed, Remove the data associated with it
		****************************************************************************/
		std::array<size_t, MAX_ENTITIES>& GetEntityToIndexMap()
		{
			return mEntityToIndexMap;
		}

		/*!*************************************************************************
		If Entity is destroyed, Remove the data associated with it
		****************************************************************************/
		std::array<Entity, MAX_ENTITIES>& GetIndexToEntityMap()
		{
			return mIndexToEntityMap;
		}

		/*!*************************************************************************
		Return size of array
		****************************************************************************/
		const size_t GetEntitySize()
		{
			return mSize;
		}

		/*!*************************************************************************
		Clears Mapping for Scene Manager File Loading
		****************************************************************************/
		void ClearForWorldBuild()
		{
			std::fill(mEntityToIndexMap.begin(), mEntityToIndexMap.end(), MAX_ENTITIES);
			std::fill(mIndexToEntityMap.begin(), mIndexToEntityMap.end(), MAX_ENTITIES);
		}

	private:

		// Array containing Component's Data
		std::array<T, MAX_ENTITIES> mComponentArray{};

		// Array from an entity ID to an array index.
		std::array<size_t, MAX_ENTITIES> mEntityToIndexMap;

		// Array from an array index to an entity ID.
		std::array<Entity, MAX_ENTITIES> mIndexToEntityMap;

		// Total size of valid entries in the array.
		size_t mSize = 0;
	};
}