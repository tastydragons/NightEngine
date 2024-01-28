#pragma once

#include <GL/glew.h>

namespace Night
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 8;
		case ShaderDataType::Float3:	return 12;
		case ShaderDataType::Float4:	return 16;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 8;
		case ShaderDataType::Int3:		return 12;
		case ShaderDataType::Int4:		return 16;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		NIGHT_CORE_ASSERT(false, "Unknow Data type!");
		return 0;
	}



	//buffer elements
	struct BufferElements
	{
		ShaderDataType Type;
		std::string Name;
		size_t Size;
		__int64 Offset;
		bool Normalized;


		BufferElements() = default;
		BufferElements(ShaderDataType type, const std::string& name, bool normalized = GL_FALSE)
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		unsigned int GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			NIGHT_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	};

	//bufferlayout
	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElements>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		//getter
		inline unsigned int GetStride() const { return m_stride; }
		inline const std::vector<BufferElements>& GetElements() const { return m_Elements; }

		std::vector<BufferElements>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElements>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElements>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElements>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride()
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = static_cast<unsigned int> (offset);
				offset += element.Size;
				m_stride += static_cast<unsigned int>(element.Size);
			}
		}
	private:
		std::vector<BufferElements> m_Elements;
		unsigned int m_stride = 0;

	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual void SetBufferData(const void* data, unsigned int size) = 0;

		static Ref<VertexBuffer> Create(float* data, unsigned int size);
		static Ref<VertexBuffer> Create(uint32_t size);
	};

	class IndexBuffer
	{
	public:

		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static Ref<IndexBuffer> Create(unsigned int* indices, unsigned int count);
	};
}