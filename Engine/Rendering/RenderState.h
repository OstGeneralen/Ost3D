#pragma once
#include <string>
#include <Engine/Container/DynamicList.h>
#include <Engine/OstEngineTypedefs.h>
#include <Engine/Rendering/Shader/Shader.h>
#include <Engine/Rendering/Shader/IShaderCompiler.h>

namespace ost
{
	namespace dx
	{
		class RenderingBackend;
	}

	enum class ERenderInputAlignment
	{
		f32_RGBA,
		f32_RGB,
		f32_RG,
		f32_R,
	};

	extern uint32 RenderInputAlignmentToBytes(ERenderInputAlignment alignment);

	struct DrawableInputElement
	{
		std::string Name;
		uint32 Idx{ 0 };
		ERenderInputAlignment Alignment;
		uint32 Slot{ 0 };
		uint32 Offset{ 0 };
		bool PerVertex{ true };
		uint32 Step{ 0 };
	};

	struct RenderStateDesc
	{
		friend dx::RenderingBackend;

	public:
		bool DepthEnabled = false;
		bool StencilEnabled = false;
		void AddInputElement(const char* name, ERenderInputAlignment alignment, bool perVertex = true, uint32 idx = 0, uint32 slot = 0, uint32 step = 0);
		void SetShaderPath(const char* path, EShaderType shaderType);
	private:
		ShaderCompileInfo CompileVertexShader(IShaderCompiler& compiler) const;
		ShaderCompileInfo CompilePixelShader(IShaderCompiler& compiler) const;
		void PopulateInputElementsDesc(void** buffer) const;
		uint32 GetInputElementNum() const;

		TDynamicList <DrawableInputElement> _inputElements;
		uint32 _currentNextInputElementOffset{0};
		std::string _vertexShaderPath;
		std::string _pixelShaderPath;
	};

	struct RenderState
	{
		friend dx::RenderingBackend;

		bool Valid() const { return _psoPtr != nullptr && _rootSigPtr != nullptr; }

	private:
		void* _psoPtr{nullptr};
		void* _rootSigPtr{nullptr};
	};
}