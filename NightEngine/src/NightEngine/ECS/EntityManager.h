#pragma once

#include "Types.h"
#include "NightEngine/Core/Log.h"

namespace Night
{
	class EntityManager
	{
	public:

		/*!*************************************************************************
		Initializes the queue for for all available entity IDs
		****************************************************************************/
		EntityManager()
		{
			// Initialize the queue with all possible entity IDs up to the max number of entities
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				mAvailableEntities.push(entity);
			}
		}

		/*!*************************************************************************
		Grabs a new id from the queue and return it
		****************************************************************************/
		Entity CreateEntity()
		{
			assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
			// Take an ID from the front of the queue
			Entity id = mAvailableEntities.top();
			mAvailableEntities.pop();	//Removes elements from the front of the queue
			mAliveEntities.insert(id);
			++mLivingEntityCount;	//Dictates how many entities are alive

			return id;
		}

		/*!*************************************************************************
		Destroy the entities id and push it to end of queue
		****************************************************************************/
		void DestroyEntity(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Put the destroyed ID at the back of the queue
			mSignatures[entity].reset();
			mAvailableEntities.push(entity);
			mAliveEntities.erase(entity);
			--mLivingEntityCount;
		}

		/*!*************************************************************************
		Parse the entity's signature into an array of Signatures
		****************************************************************************/
		void SetSignature(Entity entity, Signature signature)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Put this entity's signature into the array
			mSignatures[entity] = signature;
		}

		/*!*************************************************************************
		Get the Signature of the entity
		****************************************************************************/
		Signature GetSignature(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Get this entity's signature from the array
			return mSignatures[entity];
		}


		/*!*************************************************************************
		Returns the Total Entities in existence (both destroyed + alive)
		****************************************************************************/
		Entity GetTotalEntities()
		{
			return mLivingEntityCount;
		}

		/*!*************************************************************************
		Builds the Total entities for Scene Build
		****************************************************************************/
		void SetTotalEntitiesForWorld(Entity entity)
		{
			while (mAliveEntities.size() < entity)
			{
				CreateEntity();
			}
		}

		/*!*************************************************************************
		Returns the live entities
		****************************************************************************/
		const std::set<Entity> GetAliveEntities()
		{
			return mAliveEntities;
		}

		std::array<Signature, MAX_ENTITIES>& GetEntitySignatureArray()
		{
			return mSignatures;
		}

		void ResetAllEntitySignatures()
		{
			mSignatures.fill(0);
		}


	private:

		// Queue of unused entity IDs
		std::priority_queue<Entity, std::vector<Entity>, std::greater<Entity>> mAvailableEntities{};

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> mSignatures{};

		// Total living entities - used to keep limits on how many exist
		Entity mLivingEntityCount{};

		// Alive entities that are still in use
		std::set<Entity> mAliveEntities;
	};
}