#include "hzpch.h"
#include "Picker.h"
#include "LevelEditor.h"

namespace Night
{
	/*!*************************************************************************
	Set the AABB box for the entity in the scene viewer
	****************************************************************************/
	int Picker::Pick(Camera2D* camera, std::multimap<float, Transform*> scene)
	{
		const float x_ndc = p_Editor->mGameMousePosition.x;
		const float y_ndc = p_Editor->mGameMousePosition.y;

		if (abs(x_ndc) > 1.0f || abs(y_ndc) > 1.0f)
			return p_Editor->selectedEntity;

		glm::vec4 clip{ x_ndc, y_ndc, 0.0f, 1.0f };

		glm::vec4 Eye = glm::inverse(camera->GetProjectionMatrix()) * clip;
		glm::vec4 World = glm::inverse(camera->GetViewMatrix()) * Eye;

		std::map<Transform*, Entity> EntityTransform;

		for (Entity entity = 0; entity < p_ecs.GetTotalEntities(); entity++)
		{
			if (p_ecs.HaveComponent<Transform>(entity))
				EntityTransform.insert({ &p_ecs.GetComponent<Transform>(entity), entity });
		}

		int index = -1;

		for (std::multimap<float, Transform*>::const_reverse_iterator it = scene.rbegin();
			it != scene.rend(); ++it)
		{
			AABB bounding;


			bounding.min.x = -0.5f * std::abs(it->second->GetScale().x) + it->second->GetPos().x;
			bounding.min.y = -0.5f * std::abs(it->second->GetScale().y) + it->second->GetPos().y;


			bounding.max.x = 0.5f * std::abs(it->second->GetScale().x) + it->second->GetPos().x;
			bounding.max.y = 0.5f * std::abs(it->second->GetScale().y) + it->second->GetPos().y;

			if (World.x < bounding.min.x || World.x > bounding.max.x || World.y < bounding.min.y || World.y > bounding.max.y)
				continue;

			index = EntityTransform.at(it->second);


			return index;
		}

		return -1;
	}
}