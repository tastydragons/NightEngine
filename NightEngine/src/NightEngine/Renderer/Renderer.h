/*!***********************************************************************
 * @file Renderer.h
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 2
 * @date 2-11-2023
 * @brief Provides rendering functionality for 2D graphics.
 *
 * The Renderer class encapsulates rendering functionality for 2D graphics
 * in the Night Engine. It allows for drawing various primitives, such as
 * quads, lines, and Axis-Aligned Bounding Boxes (AABBs), with the ability
 * to specify colors and textures. It also handles camera transformations
 * and scene management.
*************************************************************************/

#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Font.h"
#include "SpriteRenderer.h"
#include <glm/glm.hpp>

namespace Night {
	class Renderer {
	public:
		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Shutdown();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int indexCount = 0);
		static void DrawLineIndexed(const Ref<VertexArray>& vertexArray, unsigned int vertexCount = 0);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture);

		//Quads with rotation
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotaton, const Ref<Texture>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotaton, const Ref<Texture>& texture);

		//Sprite sheet rendereing
		static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<SpriteRenderer>& sprite);
		static void DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SpriteRenderer>& sprite);

		//Render text
		static void DrawString(std::string text, glm::vec2 pos, float scale, Camera& cam, glm::vec3 color);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawLine(const glm::vec3& startPoint, const glm::vec3& endPoint, const glm::vec4& color);
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f);

		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Flush();
		static void StartBatch();
		static void RefreshBatch();

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_sceneData;
	};

}