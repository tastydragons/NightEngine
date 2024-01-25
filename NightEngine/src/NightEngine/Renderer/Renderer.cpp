/*!***********************************************************************
 * @file Renderer.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 2
 * @date 2-11-2023
 * @brief
 * This file contains the implementation of the Renderer class, which
 * provides functions for initializing, managing, and rendering graphics
 * in the Engine. It includes functions for drawing basic shapes,
 * rendering textured quads, and handling the rendering pipeline.
*************************************************************************/

#include "Renderer.h"
//#include "../Composition/ComponentCreator.h"
//#include "../Factory/GameObjFactory.h"
//#include "../Physics/Body.h"
#include "../Renderer/Sprite.h"
#include "hzpch.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Night {

	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct CircleVertex
	{
		glm::vec3 w_Position; //world position
		glm::vec3 l_Position; //local position
		glm::vec4 Color;
		float Thickness;
		float Fade;
	};

	struct RendererData {
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Texture> WhiteTexture;
		Ref<Shader> TextureShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotsIndex = 1; // 0 = Blank/White texture

		glm::vec4 QuadVertexPositions[4];

		Renderer::Statistics Stats;
	};

	static RendererData storage_Data;

	/*!***********************************************************************
	\brief
	Initialize the Renderer.

	This function initializes the Renderer, setting up the necessary OpenGL settings and buffers to enable rendering.
	The initialization process involves:
	- Defining the vertices for a unit quad (four vertices) and creating a VertexArray for quad rendering.
	- Defining the indices for a quad (two triangles) and creating an IndexBuffer for quad rendering.
	- Creating a VertexArray and VertexBuffer for rendering circles.
	- Creating a Circle shader for rendering circles.
	- Creating a VertexArray and VertexBuffer for rendering lines.
	- Creating a Line shader for rendering lines.
	*************************************************************************/
	void Renderer::Init() {
		glewInit();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		storage_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		storage_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		storage_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		storage_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		storage_Data.QuadVertexArray = std::make_unique<VertexArray>();
		Ref<VertexBuffer> QuadVB;
		storage_Data.QuadVertexBuffer.reset(VertexBuffer::Create(storage_Data.MaxVertices * sizeof(QuadVertex)));
		storage_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"}
			});
		storage_Data.QuadVertexArray->AddVertexBuffer(storage_Data.QuadVertexBuffer);
		storage_Data.QuadVertexBufferBase = new QuadVertex[storage_Data.MaxVertices];

		uint32_t* QuadIndices = new uint32_t[storage_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < storage_Data.MaxIndices; i += 6) {
			QuadIndices[i + 0] = offset + 0;
			QuadIndices[i + 1] = offset + 1;
			QuadIndices[i + 2] = offset + 2;

			QuadIndices[i + 3] = offset + 2;
			QuadIndices[i + 4] = offset + 3;
			QuadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> QuadIB;
		QuadIB.reset(IndexBuffer::Create(QuadIndices, storage_Data.MaxIndices));
		storage_Data.QuadVertexArray->SetIndexBuffer(QuadIB);
		delete[] QuadIndices;

		storage_Data.WhiteTexture = CreateRef<Texture>(1, 1);
		uint32_t whiteTexture = 0xffffffff;
		storage_Data.WhiteTexture->TextureData(&whiteTexture);

		int32_t samplers[storage_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < storage_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		storage_Data.TextureShader = std::make_shared<Shader>("Assets/shaders/Texture.glsl");
		storage_Data.TextureShader->Bind();
		storage_Data.TextureShader->SetUniform("u_Textures", samplers, storage_Data.MaxTextureSlots);

		//Set texture slot 0 to white texture
		storage_Data.TextureSlots[0] = storage_Data.WhiteTexture;

		//CIRCLE
		storage_Data.CircleVertexArray = std::make_unique<VertexArray>();

		storage_Data.CircleVertexBuffer.reset(VertexBuffer::Create(storage_Data.MaxVertices * sizeof(CircleVertex)));
		storage_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"  }
			});
		storage_Data.CircleVertexArray->AddVertexBuffer(storage_Data.CircleVertexBuffer);
		storage_Data.CircleVertexArray->SetIndexBuffer(QuadIB);
		storage_Data.CircleVertexBufferBase = new CircleVertex[storage_Data.MaxVertices];
		storage_Data.CircleShader = std::make_shared<Shader>("Assets/shaders/Circle.glsl");

		//LINE
		storage_Data.LineVertexArray = std::make_unique<VertexArray>();
		storage_Data.LineVertexBuffer.reset(VertexBuffer::Create(storage_Data.MaxVertices * sizeof(LineVertex)));
		storage_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"}
			});

		storage_Data.LineVertexArray->AddVertexBuffer(storage_Data.LineVertexBuffer);
		storage_Data.LineVertexBufferBase = new LineVertex[storage_Data.MaxVertices];

		storage_Data.LineShader = std::make_shared<Shader>("Assets/shaders/Line.glsl");

		RegisterComponent(Sprite);
		RegisterComponent(Body);
	}

	/*!***********************************************************************
	\brief
	Handle window resize events.

	This function sets the viewport to match the new dimensions of the
	window when it is resized.

	\param width
	The new width of the window.

	\param height
	The new height of the window.
	*************************************************************************/
	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		SetViewport(0, 0, width, height);
	}

	/*!***********************************************************************
	\brief
	Shutdown the renderer and release resources.

	This function is responsible for cleaning up and releasing any
	resources used by the renderer before shutting down the application.
	*************************************************************************/
	void Renderer::Shutdown() {
		delete[] storage_Data.QuadVertexBufferBase;
	}

	/*!***********************************************************************
	\brief
	Set the viewport dimensions for rendering.

	This function sets the dimensions and position of the viewport in the
	rendering context. It determines the portion of the framebuffer where
	rendering will occur.

	\param x
	The x-coordinate of the viewport's lower-left corner.

	\param y
	The y-coordinate of the viewport's lower-left corner.

	\param width
	The width of the viewport.

	\param height
	The height of the viewport.
	*************************************************************************/
	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	/*!***********************************************************************
	\brief
	Render geometry from the provided VertexArray using indexed rendering.

	\param vertexArray
	A reference to the VertexArray containing the geometry to be rendered.

	\param indexCount
	The number of indices used for rendering. If set to 0, it uses the count from the VertexArray's index buffer.
	*************************************************************************/
	void Renderer::DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	/*!***********************************************************************
	\brief Render lines from the provided VertexArray without using indices.

	\param vertexArray A reference to the VertexArray containing the lines to be rendered.

	\param vertexCount The number of vertices used for rendering.
	*************************************************************************/
	void Renderer::DrawLineIndexed(const Ref<VertexArray>& vertexArray, unsigned int vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	/*!***********************************************************************
	\brief
	Set the clear color for the OpenGL context.

	This function sets the color that the OpenGL context will use when
	clearing the color buffer. It takes a glm::vec4 color value with RGBA
	components in the range [0.0, 1.0].

	\param color
	The clear color to set.
	*************************************************************************/
	void Renderer::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	/*!***********************************************************************
	\brief
	Clear the OpenGL color and depth buffers.

	This function clears the OpenGL color and depth buffers using the
	previously set clear color. It should be called before rendering a new
	frame to prepare the rendering context.
	*************************************************************************/
	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/*!***********************************************************************
	\brief
	Begin rendering a new scene with the specified camera.

	This function prepares the renderer to start rendering a new scene with
	the given camera. It sets up the shaders and uniforms for rendering with
	both flat colors and textures based on the provided camera's view-projection
	matrix.

	\param camera
	The camera to use for rendering the scene.
	*************************************************************************/
	void Renderer::BeginScene(Camera& camera) {
		m_sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
		StartBatch();
	}

	/*!***********************************************************************
	\brief
	End the current scene rendering.

	This function finalizes the rendering of the current scene. It can be
	used to perform any necessary cleanup or post-processing after rendering
	the scene.
	*************************************************************************/
	void Renderer::EndScene() {
		Flush();
	}

	void Renderer::StartBatch()
	{
		//Texutre/Colored quad
		storage_Data.QuadIndexCount = 0;
		storage_Data.QuadVertexBufferPtr = storage_Data.QuadVertexBufferBase; //keep track of the buffer 

		//Line
		storage_Data.LineVertexCount = 0;
		storage_Data.LineVertexBufferPtr = storage_Data.LineVertexBufferBase;

		//Circle
		storage_Data.CircleIndexCount = 0;
		storage_Data.CircleVertexBufferPtr = storage_Data.CircleVertexBufferBase;

		storage_Data.TextureSlotsIndex = 1;
	}

	void Renderer::RefreshBatch()
	{
		Flush();
		StartBatch();
	}

	/*!***********************************************************************
	\brief Flushes the renderer's internal buffers and performs rendering.

	This function is responsible for flushing the renderer's internal buffers and performing rendering. It involves several steps, including:
	- Preparing quad data (if available) and sending it to the GPU.
	- Binding texture slots and setting view-projection matrices.
	- Drawing indexed quads.
	- Preparing line data (if available) and sending it to the GPU.
	- Drawing indexed lines.
	- Preparing circle data (if available) and sending it to the GPU.
	- Drawing indexed circles.
	*************************************************************************/
	void Renderer::Flush()
	{
		if (storage_Data.QuadIndexCount) {
			uint32_t QuadDataSize = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(storage_Data.QuadVertexBufferPtr) - reinterpret_cast<uintptr_t>(storage_Data.QuadVertexBufferBase));
			storage_Data.QuadVertexBuffer->SetData(storage_Data.QuadVertexBufferBase, QuadDataSize);

			for (uint32_t i = 0; i < storage_Data.TextureSlotsIndex; i++) {
				storage_Data.TextureSlots[i]->Bind(i);
			}

			storage_Data.TextureShader->Bind();
			storage_Data.TextureShader->SetUniform("u_ViewProjection", m_sceneData->viewProjectionMatrix);

			DrawIndexed(storage_Data.QuadVertexArray, storage_Data.QuadIndexCount);
			storage_Data.Stats.DrawCalls++;
		}

		if (storage_Data.LineVertexCount) {
			uint32_t LineDataSize = (uint32_t)((uint8_t*)storage_Data.LineVertexBufferPtr - (uint8_t*)storage_Data.LineVertexBufferBase);
			storage_Data.LineVertexBuffer->SetData(storage_Data.LineVertexBufferBase, LineDataSize);

			storage_Data.LineShader->Bind();
			storage_Data.LineShader->SetUniform("u_ViewProjection", m_sceneData->viewProjectionMatrix);
			glLineWidth(1.5f);
			DrawLineIndexed(storage_Data.LineVertexArray, storage_Data.LineVertexCount);
			storage_Data.Stats.DrawCalls++;
		}

		if (storage_Data.CircleIndexCount) {
			uint32_t CircleDataSize = (uint32_t)((uint8_t*)storage_Data.CircleVertexBufferPtr - (uint8_t*)storage_Data.CircleVertexBufferBase);
			storage_Data.CircleVertexBuffer->SetData(storage_Data.CircleVertexBufferBase, CircleDataSize);

			storage_Data.CircleShader->Bind();
			storage_Data.CircleShader->SetUniform("u_ViewProjection", m_sceneData->viewProjectionMatrix);
			Renderer::DrawIndexed(storage_Data.CircleVertexArray, storage_Data.CircleIndexCount);
			storage_Data.Stats.DrawCalls++;
		}
	}

	/*!***********************************************************************
	\brief
	Draw a colored quad at the specified position and size.

	This function draws a colored quad at the given position and size with
	the provided color.

	\param position
	The position of the quad.

	\param size
	The size of the quad.

	\param color
	The color of the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		const float texIndex = 0.0f;
		constexpr size_t quadCount = 4;
		constexpr glm::vec2 texCoord[] = { { 0.0f, 0.0f },  { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadCount; i++) {
			storage_Data.QuadVertexBufferPtr->Position = transform * storage_Data.QuadVertexPositions[i];
			storage_Data.QuadVertexBufferPtr->Color = color;
			storage_Data.QuadVertexBufferPtr->TexCoord = texCoord[i];
			storage_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			storage_Data.QuadVertexBufferPtr++;
		}

		storage_Data.QuadIndexCount += 6;

		storage_Data.Stats.QuadCount++;
	}

	/*!***********************************************************************
	\brief
	Draw a rotated colored quad at the specified position and size.

	This function draws a colored quad at the given position and size with
	the provided rotation and color. The quad is rotated in 2D space.

	\param position
	The position of the quad.

	\param size
	The size of the quad.

	\param color
	The color of the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 1.0f }, size, color);
	}


	/*!***********************************************************************
	\brief
	Draw a rotated colored quad at the specified position and size.

	This function draws a colored quad at the given position and size with
	the provided rotation and color. The quad is rotated in 2D space.

	\param position
	The position of the quad.

	\param size
	The size of the quad.

	\param rotation
	The rotation angle in radians.

	\param color
	The color of the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	/*!***********************************************************************
	\brief
	Draw a rotated colored quad at the specified position, size, and rotation.

	This function draws a colored quad at the given 3D position with the
	specified size and rotation. The quad is colored using the provided color
	and can be oriented in 2D space.

	\param position
	The 3D position of the quad.

	\param size
	The size of the quad.

	\param rotation
	The rotation angle in radians.

	\param color
	The color of the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		const float texIndex = 0.0f;
		constexpr size_t quadCount = 4;
		constexpr glm::vec2 texCoord[] = { { 0.0f, 0.0f },  { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadCount; i++) {
			storage_Data.QuadVertexBufferPtr->Position = transform * storage_Data.QuadVertexPositions[i];
			storage_Data.QuadVertexBufferPtr->Color = color;
			storage_Data.QuadVertexBufferPtr->TexCoord = texCoord[i];
			storage_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			storage_Data.QuadVertexBufferPtr++;
		}

		storage_Data.QuadIndexCount += 6;

		storage_Data.Stats.QuadCount++;
	}

	/*!***********************************************************************
	\brief
	Draw a colored quad at the specified position, size, and texture.

	This function draws a colored quad at the given 2D position with the
	specified size and texture. The quad is colored using the provided texture.

	\param position
	The 2D position of the quad.

	\param size
	The size of the quad.

	\param texture
	The texture to be applied to the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	/*!***********************************************************************
	\brief
	Draw a colored quad at the specified position, size, and texture.

	This function draws a colored quad at the given 2D position with the
	specified size and texture. The quad is colored using the provided texture.

	\param position
	The 3D position of the quad.

	\param size
	The size of the quad.

	\param rotation
	The rotation angle in radians.

	\param texture
	The texture to be applied to the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture);
	}

	/*!***********************************************************************
	\brief
	Draw a rotated textured quad at the specified position, size, and rotation.

	This function draws a textured quad at the given 3D position with the
	specified size and rotation. The quad is textured using the provided texture
	and can be oriented in 2D space.

	\param position
	The 3D position of the quad.

	\param size
	The size of the quad.

	\param rotation
	The rotation angle in radians.

	\param texture
	The texture to be applied to the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture)
	{
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr size_t quadCount = 4;
		constexpr glm::vec2 texCoord[] = { { 0.0f, 0.0f },  { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < storage_Data.TextureSlotsIndex; i++) {
			if (*storage_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			RefreshBatch();
			textureIndex = (float)storage_Data.TextureSlotsIndex;
			storage_Data.TextureSlots[storage_Data.TextureSlotsIndex] = texture;
			storage_Data.TextureSlotsIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadCount; i++) {
			storage_Data.QuadVertexBufferPtr->Position = transform * storage_Data.QuadVertexPositions[i];
			storage_Data.QuadVertexBufferPtr->Color = color;
			storage_Data.QuadVertexBufferPtr->TexCoord = texCoord[i];
			storage_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			storage_Data.QuadVertexBufferPtr++;
		}

		storage_Data.QuadIndexCount += 6;

		storage_Data.Stats.QuadCount++;
	}

	void Renderer::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<SpriteRenderer>& sprite)
	{
		DrawSprite({ position.x, position.y, 0.0f }, size, sprite);
	}

	void Renderer::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<SpriteRenderer>& sprite)
	{
		constexpr size_t quadCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* texCoords = sprite->GetTexCoords();
		const Ref<Texture> texure = sprite->GetTexture();

		float textureIndex = 0.0f;
		for (unsigned int i = 1; i < storage_Data.TextureSlotsIndex; i++)
		{
			if (*storage_Data.TextureSlots[i] == *sprite->GetTexture())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			RefreshBatch();

			textureIndex = (float)storage_Data.TextureSlotsIndex;
			storage_Data.TextureSlots[storage_Data.TextureSlotsIndex] = sprite->GetTexture();
			storage_Data.TextureSlotsIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadCount; i++)
		{
			storage_Data.QuadVertexBufferPtr->Position = transform * storage_Data.QuadVertexPositions[i];
			storage_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			storage_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			storage_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			storage_Data.QuadVertexBufferPtr++;
		}

		storage_Data.QuadIndexCount += 6;

	}

	/*!***********************************************************************
	\brief
	Draw a textured quad at the specified position and size.

	This function draws a textured quad at the given 3D position with the
	specified size. The quad is textured using the provided texture.

	\param position
	The 3D position of the quad.

	\param size
	The size of the quad.

	\param texture
	The texture to be applied to the quad.
	*************************************************************************/
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture) {
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr size_t quadCount = 4;
		constexpr glm::vec2 texCoord[] = { { 0.0f, 0.0f },  { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < storage_Data.TextureSlotsIndex; i++) {
			if (*storage_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			RefreshBatch();
			textureIndex = (float)storage_Data.TextureSlotsIndex;
			storage_Data.TextureSlots[storage_Data.TextureSlotsIndex] = texture;
			storage_Data.TextureSlotsIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadCount; i++) {
			storage_Data.QuadVertexBufferPtr->Position = transform * storage_Data.QuadVertexPositions[i];
			storage_Data.QuadVertexBufferPtr->Color = color;
			storage_Data.QuadVertexBufferPtr->TexCoord = texCoord[i];
			storage_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			storage_Data.QuadVertexBufferPtr++;
		}

		storage_Data.QuadIndexCount += 6;

		storage_Data.Stats.QuadCount++;
	}

	void Renderer::DrawString(std::string text, glm::vec2 pos, float scale, Camera& cam, glm::vec3 color)
	{
		Font::RenderText(text, pos, scale, cam, color);
	}

	/*!***********************************************************************
	\brief
	Draw an Axis-Aligned Bounding Box (AABB) with a specified color.

	This function draws an Axis-Aligned Bounding Box (AABB) with the specified
	color. The AABB is defined by its position (`position`) and size (`size`)
	in 2D space. It uses the provided color to draw the lines of the AABB.

	\param position
	The position of the center of the AABB in 2D space.

	\param size
	The size of the AABB in 2D space, specified as a glm::vec2 representing the
	width and height.

	\param color
	The color of the AABB specified as a glm::vec4, where each component
	(r, g, b, a) represents the red, green, blue, and alpha values, respectively.
	The alpha value controls transparency.
	*************************************************************************/
	void Renderer::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	/*!***********************************************************************
	\brief
	Draw a line segment between two 3D points with a specified color.

	This function draws a line segment between two 3D points, `start` and `end`,
	with the specified color. It uses OpenGL to draw the line with the provided
	color.

	\param start
	The starting point of the line segment.

	\param end
	The ending point of the line segment.

	\param color
	The color of the line specified as a glm::vec4, where each component
	(r, g, b, a) represents the red, green, blue, and alpha values, respectively.
	The alpha value controls transparency.
	*************************************************************************/
	void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color) {
		storage_Data.LineVertexBufferPtr->Position = start;
		storage_Data.LineVertexBufferPtr->Color = color;
		storage_Data.LineVertexBufferPtr++;

		storage_Data.LineVertexBufferPtr->Position = end;
		storage_Data.LineVertexBufferPtr->Color = color;
		storage_Data.LineVertexBufferPtr++;

		storage_Data.LineVertexCount += 2;
	}

	/*!***********************************************************************
	\brief Renders a circle.

	This function is responsible for rendering a circle on the screen. It takes care of transforming the circle's vertices, setting its color and appearance parameters, and adding the necessary data to the rendering buffer.

	\param transform The transformation matrix to position and orient the circle.

	\param color The color of the circle.

	\param thickness The thickness of the circle's outline.

	\param fade The fading effect of the circle (opacity).
	*************************************************************************/
	void Renderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
	{
		for (size_t i = 0; i < 4; i++)
		{
			storage_Data.CircleVertexBufferPtr->w_Position = transform * storage_Data.QuadVertexPositions[i];
			storage_Data.CircleVertexBufferPtr->l_Position = storage_Data.QuadVertexPositions[i] * 2.0f;
			storage_Data.CircleVertexBufferPtr->Color = color;
			storage_Data.CircleVertexBufferPtr->Thickness = thickness;
			storage_Data.CircleVertexBufferPtr->Fade = fade;
			storage_Data.CircleVertexBufferPtr++;
		}

		storage_Data.CircleIndexCount += 6;

		storage_Data.Stats.QuadCount++;
	}

	/*!***********************************************************************
	\brief Resets the rendering statistics.

	This function is responsible for resetting the rendering statistics, effectively setting all statistical counters to zero.
	*************************************************************************/
	void Renderer::ResetStats()
	{
		memset(&storage_Data.Stats, 0, sizeof(Statistics));
	}

	/*!***********************************************************************
	\brief Retrieves the rendering statistics.

	This function returns the current rendering statistics, providing information on the number of rendering operations and other related metrics.

	\return A structure containing the rendering statistics, such as the number of rendered quads and vertices.
	*************************************************************************/
	Renderer::Statistics Renderer::GetStats()
	{
		return storage_Data.Stats;
	}
}