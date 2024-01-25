/*!***********************************************************************
 * @file Camera.h
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Header file for the Camera class.
 *
 * This file defines the Camera class, which represents a camera used for
 * rendering. The Camera class provides functionality for setting
 * projections, position, rotation, and generating view and projection
 * matrices for rendering scenes.
*************************************************************************/

#pragma once

#include <glm/glm.hpp>

namespace Night {
	class Camera {
	public:
		Camera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec3& position) { m_position = position; RecalculateMatrix();  }

		float GetRotation() const { return m_rotation; }
		void SetRotation(float rotation) { m_rotation = rotation; RecalculateMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
		
	private:
		void RecalculateMatrix();

	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;
	};
}