#include "Application.h"
#include <Graphics/Rendering/RenderingFramework.h>
#include <pix3.h>

// ------------------------------------------------------------

Application::Application(const AppRuntimeContext& context, int winW, int winH, const wchar_t* winT)
	: _window{ static_cast<HINSTANCE>(context.GetInstanceHandle()), context.GetCmdLineOpt(), {winW, winH} , winT }
{
}

void Application::Run()
{
	IRenderFramework* renderFramework = Rendering::CreateFramework();
	if (!renderFramework->Initialize(_window))
	{
		return;
	}

	IRenderer& renderer = renderFramework->GetRenderer();

	IShaderCompiler* shaderCompiler = Rendering::CreateShaderCompiler();
	auto vertexShader = shaderCompiler->CompileShader("Shaders/Test.hlsl", EShaderType::Vertex);
	auto pixelShader = shaderCompiler->CompileShader("Shaders/Test.hlsl", EShaderType::Pixel);

	if (vertexShader.Status == false || pixelShader.Status == false)
	{
		return;
	}

	RenderStateDesc stateDesc{};
	stateDesc.VertexShader = vertexShader.Shader;
	stateDesc.PixelShader = pixelShader.Shader;
	stateDesc.VertexFormatInfo = Vertex_Pos4_fCol4::StaticVertexFormatInfo();

	RenderStateHandle state = renderer.CreateRenderState(stateDesc);

	Vertex_Pos4_fCol4 triAVerts[] = {
		{ {-1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
		{ {-1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
		{ {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} }
	};

	Vertex_Pos4_fCol4 triBVerts[] = {
		{{1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
		{ { -1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
		{ {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.5f, 0.6f, 1.0f} }
	};

	Drawable triADrawable{};
	triADrawable.VertexFormatID = Vertex_Pos4_fCol4::StaticVertexFormatID();
	triADrawable.VertexSize = static_cast<unsigned>(sizeof(Vertex_Pos4_fCol4));
	triADrawable.VertexCount = 3;
	triADrawable.VertexDataBuffer = triAVerts;

	Drawable triBDrawable{};
	triBDrawable.VertexFormatID = Vertex_Pos4_fCol4::StaticVertexFormatID();
	triBDrawable.VertexSize = static_cast<unsigned>(sizeof(Vertex_Pos4_fCol4));
	triBDrawable.VertexCount = 3;
	triBDrawable.VertexDataBuffer = triBVerts;

	state.AddDrawable(triADrawable);
	state.AddDrawable(triBDrawable);

	while (_window.GetIsOpen())
	{
		_window.ProcessEvents();
		renderer.BeginRenderFrame(RGBAColor_u8(50, 180, 255, 255));
		renderer.ExecuteRenderState(state);
		renderer.EndRenderFrame();
		renderer.PresentFrame();
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------