/*!***********************************************************************
 * @file CameraController.h
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Implementation file for the Camera class.
 *
 * This file contains the implementation for the Camera class, which
 * represents a camera used for rendering. The Camera class provides
 * functionality for setting projections, position, rotation, and generating
 * view and projection matrices for rendering scenes.
*************************************************************************/

#pragma once

#include "Camera.h"
#include "../Core/Core.h"
#include "../Events/Event.h"
#include "../Events/MouseEvent.h"
#include "../Events/ApplicationEvent.h"

namespace Night {
	class CameraController
	{
	public:
		//Constructor
		CameraController(float aspectRatio, bool rotation = false);

		void OnUpdate();
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() { return m_ZoomLevel; }

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 3.f;
		Camera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f, m_CameraTranslationSpeed = 0.2f;

		bool m_Rotation;
	};
}

