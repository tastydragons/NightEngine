/*!***********************************************************************
 * @file CameraController.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Implementation file for the CameraController class.
 *
 * This file contains the implementation for the CameraController class,
 * which is responsible for controlling and manipulating a camera in a
 * graphical application or game. It handles user input, camera position
 * updates, zooming, and window resizing.
*************************************************************************/

#include "CameraController.h"
#include "../Input/Input.h"
#include "../Input/KeyCodes.h"
#include "hzpch.h"

#include "../Engine/NightImgui.h"

namespace Night {
	/*!***********************************************************************
	\brief
	Constructor for the CameraController class.

	This constructor initializes the CameraController with the specified
	aspect ratio and rotation mode. It creates a camera with an initial
	projection matrix and sets up the controller to handle user input
	for camera movement and manipulation.

	\param aspectRatio 
	The aspect ratio of the camera.

	\param rotation 
	A flag indicating whether rotation is enabled.
	*************************************************************************/
	CameraController::CameraController(float aspectRatio, bool rotation) 
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	/*!***********************************************************************
	\brief
	Update camera position based on user input.

	This function checks for specific keyboard input (eg. right, left, up, or down arrow keys)
	and adjusts the camera's position accordingly. It is typically called in an
	update loop to continuously respond to user input and move the camera in a
	graphical application or game.
	*************************************************************************/
	void CameraController::OnUpdate() {
		if (Input::IsKeyPressed(KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraTranslationSpeed;
		}

		else if (Input::IsKeyPressed(KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed;
		}

		if (Input::IsKeyPressed(KEY_UP)) {
			m_CameraPosition.y += m_CameraTranslationSpeed;
		}

		else if (Input::IsKeyPressed(KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed;
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	/*!***********************************************************************
	\brief
	Handle events relevant to camera control.

	This function is responsible for handling various events, such as mouse scrolling
	and window resizing, that are relevant to camera control. It is typically called
	in response to incoming events in a graphical application or game.

	\param[in] e
	The event to be handled.
	*************************************************************************/
	void CameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		if (NightImgui::getInstance().GetMouseFocus() || !NightImgui::getInstance().GetImguiSwitch()) {
			dispatcher.Dispatch<MouseScrolledEvent>(NIGHT_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		}
		dispatcher.Dispatch<WindowResizeEvent>(NIGHT_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	void CameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	/*!***********************************************************************
	\brief 
	Handle mouse scroll event for zooming the camera.

	This function is responsible for adjusting the camera's zoom level in response
	to a mouse scroll event. It scales the camera's projection matrix based on the
	mouse scroll input.

	\param[in] e 
	The `MouseScrolledEvent` containing scroll information.

	\return 
	`false` to indicate that the event has been fully handled.
	*************************************************************************/
	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	/*!***********************************************************************
	\brief 
	Handle window resize event for adjusting camera aspect ratio.

	This function is responsible for updating the camera's aspect ratio in response
	to a window resize event. It recalculates the camera's projection matrix based
	on the new aspect ratio.

	\param[in] e 
	The `WindowResizeEvent` containing the new window dimensions.

	\return 
	`false` to indicate that the event has been fully handled.
	*************************************************************************/
	bool CameraController::OnWindowResized(WindowResizeEvent& e) {
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}