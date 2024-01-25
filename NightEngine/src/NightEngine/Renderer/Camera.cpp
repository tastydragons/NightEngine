/*!***********************************************************************
 * @file Camera.cpp
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

#include "hzpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Night {
	/*!***********************************************************************
	\brief Constructs a 2D orthographic camera.

	This constructor creates a 2D orthographic camera with the specified
	left, right, bottom, and top clipping planes. The camera's projection
	matrix is set to orthographic projection, and the view and view-
	projection matrices are initialized accordingly.
	
	\param left   
	The coordinate of the left clipping plane.

	\param right  
	The coordinate of the right clipping plane.

	\param bottom 
	The coordinate of the bottom clipping plane.

	\param top    
	The coordinate of the top clipping plane.
	*************************************************************************/
	Camera::Camera(float left, float right, float bottom, float top) 
		: m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f) 
	{
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	/*!***********************************************************************
	\brief 
	Set the camera's projection matrix.

	This function sets the camera's projection matrix based on the specified
	left, right, bottom, and top values, which define the viewing volume in
	orthographic projection. The resulting projection matrix is used to
	transform 3D world coordinates to 2D screen coordinates.

	\param[in] left   
	The left coordinate of the viewing volume.

	\param[in] right  
	The right coordinate of the viewing volume.

	\param[in] bottom 
	The bottom coordinate of the viewing volume.

	\param[in] top    
	The top coordinate of the viewing volume.
	*************************************************************************/
	void Camera::SetProjection(float left, float right, float bottom, float top) {
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	/*!***********************************************************************
	\brief 
	Recalculate the camera's view and view-projection matrices.

	This function recalculates the camera's view matrix based on its current
	position and rotation. It then updates the view-projection matrix by
	multiplying it with the existing projection matrix.
	*************************************************************************/
	void Camera::RecalculateMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}