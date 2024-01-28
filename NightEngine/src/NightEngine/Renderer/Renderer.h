#pragma once

#include <glm/glm.hpp>

#include "Camera2D.h"
#include "VertexArray.h"
#include "SpriteRendering.h"

#include "NightEngine/Math/NEMath.h"

#include "NightEngine/ResourceManager/ResourceManager.h"

namespace Night
{
	class Renderer
	{
	public:
		void SetClearColor(const Vec4& color);
		void Clear();

		//render call
		static void Init();
		static void ShutDown();

		static void Begin(Camera2D& camera);
		static void End();
		static void Flush();


		static void DrawIndexed(const Ref<VertexArray>& vertexarray, unsigned int IndexCount = 0);
		static void DrawForLine(const Ref<VertexArray>& vertexarray, unsigned int vertexCount);

		void DrawQuad(const vec2D& position, const vec2D& size, const Vec4& color);
		void DrawQuad(const Vec3& position, const vec2D& size, const Vec4& color);
		void DrawQuad(const Mat4x4& transform, const Vec4& color);
		void DrawQuadImpact(const Vec3& position, const vec2D& size, const Vec4& color);
		void DrawQuad(const vec2D& position, const vec2D& size, const Ref<Texture>& texture);
		void DrawQuad(const Vec3& position, const vec2D& size, const Ref<Texture>& texture);
		void DrawQuad(const Mat4x4& transform, const Ref<Texture>& texture);


		void DrawQuad(const vec2D& position, const vec2D& size, float rotation, const Vec4& color);
		void DrawQuad(const Vec3& position, const vec2D& size, float rotation, const Vec4& color);
		void DrawQuad(const vec2D& position, const vec2D& size, float rotation, const Ref<Texture>& texture);
		void DrawQuad(const Vec3& position, const vec2D& size, float rotation, const Ref<Texture>& texture);

		//handle sprite draws
		void DrawSprite(const vec2D& position, const vec2D& size, const float& rotation, const Ref<SpriteRender>& sprite);
		void DrawSprite(const Vec3& position, const vec2D& size, const float& rotation, const Ref<SpriteRender>& sprite);
		void DrawSprite(const Mat4x4& transform, const Ref<SpriteRender>& sprite);



		void DrawLine(const Vec3& position0, const Vec3& position1, const Vec4& color);
		void DrawRect(const Vec3& position, const vec2D& size, const Vec4& color);
		void DrawCircle(const Mat4x4& transform, const Vec4& color, float depth = 0.01f, float decline = 0.01f);


		//Information for renderer
		struct Information
		{
			unsigned int n_DrawCalls = 0;
			unsigned int n_Quad = 0;

			unsigned int TotalVertexUsed()const { return n_Quad * 4; }
			unsigned int TotalIndexUsed()const { return n_Quad * 6; }
		};
		void ResetInfo();
		static Information GetInfo();
	private:

		struct SceneData
		{
			Mat4x4 ViewProjectionMatrix;
		};
		static Scope<SceneData> s_SceneData;

	private:
		static void StartBatch();
		static void NextBatch();
	};
}