#include "hzpch.h"

#include "Camera2D.h"

#include "glm/gtc/matrix_transform.hpp"

#include "NightEngine/Timer/Timer.h"
#include "Platform/LevelEditor/LevelEditor.h"

namespace Night
{
	/*!*************************************************************************
	Constructor for Camera2D
	****************************************************************************/
	Camera2D::Camera2D(float left, float right, float bottom, float top)//left, right, bottom and top
		: mProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), mViewMatrix(1.0f),
		mPosition{ 0.0f, 0.0f, 0.0f }
	{
		mViewportWidth = mWindow.Getter().m_Width;
		mViewportHeight = mWindow.Getter().m_Height;
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	/*!*************************************************************************
	Set projection matrix
	****************************************************************************/
	void Camera2D::SetProjection(float left, float right, float bottom, float top)
	{
		mProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	/*!*************************************************************************
	Use input mouse scroll wheel to adjust camera
	****************************************************************************/
	bool Camera2D::MouseScrolling()
	{
		p_Editor->mSceneMouse = ImGui::GetMousePos();
		if (p_Editor->mSceneMouse.x >= p_Editor->mViewportBounds[0].x &&
			p_Editor->mSceneMouse.x <= p_Editor->mViewportBounds[1].x &&
			p_Editor->mSceneMouse.y >= p_Editor->mViewportBounds[0].y &&
			p_Editor->mSceneMouse.y <= p_Editor->mViewportBounds[1].y &&
			p_Editor->is_ShowWindow && p_Editor->p_Editor->mViewportFocused)
		{
			if (p_Input->mMouseScrollStatus < 0)
			{
				mZoomLevel -= 10.0f * Timer::GetInstance().GetGlobalDT();
			}
			else if (p_Input->mMouseScrollStatus > 0)
			{
				mZoomLevel += 10.0f * Timer::GetInstance().GetGlobalDT();
			}
		}
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mZoomLevel = std::min(mZoomLevel, 5.00f);
		p_Input->mMouseScrollStatus = 0.0f;

		SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
		RecalculateMatrix();
		return false;
	}

	/*!*************************************************************************
	Resize camera
	****************************************************************************/
	void Camera2D::Resize(float width, float height)
	{
		mAspectRatio = width / height;
		SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
	}

	/*!*************************************************************************
	Reset zoom level
	****************************************************************************/
	void Camera2D::resetZoomLevel()
	{
		mZoomLevel = 1.0f;
	}

	/*!*************************************************************************
	Recalculate Camera2D matrix
	****************************************************************************/
	void Camera2D::RecalculateMatrix()
	{
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::normalize(glm::cross(front, worldup));
		glm::vec3 up = glm::normalize(glm::cross(right, front));
		mViewMatrix = glm::lookAt(mPosition, mPosition + front, up);
		mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	}

	void Camera2D::SetPositionSmooth(const float x, const float y, const float speed)
	{
		glm::vec2 target = glm::vec2(mPosition.x + x, mPosition.y + y); //how much we want to see infront/ behind

		if (abs(mPosition.x) != abs(target.x))
		{
			mPosition.x += x * Timer::GetInstance().GetGlobalDT() * speed;//offset position
		}
		RecalculateMatrix();
	}
	void Camera2D::Follow(glm::vec2 pos)
	{
		if (mPosition.x != pos.x)
		{
			mPosition.x += pos.x * Timer::GetInstance().GetGlobalDT() * 1.0f;
		}
		if (mPosition.y != pos.y)
			mPosition.y += pos.y * Timer::GetInstance().GetGlobalDT() * 1.0f;
		RecalculateMatrix();
	}
}