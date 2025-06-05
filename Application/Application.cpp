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

	IRenderState* state = renderer.CreateRenderState(stateDesc);
	state->Initialize(stateDesc);

	VertexDrawable vertTri;
	vertTri
		.SetVertexCount(3)
		.AddVertex(Vertex{ {-1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} })
		.AddVertex(Vertex{ {-1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} })
		.AddVertex(Vertex{ {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} });

	VertexDrawable vertTriTwo;
	vertTriTwo
		.SetVertexCount(3)
		.AddVertex(Vertex{ {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} })
		.AddVertex(Vertex{ {-1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} })
		.AddVertex(Vertex{ {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.5f, 0.6f, 1.0f} });

	state->AddDrawable(&vertTri);
	state->AddDrawable(&vertTriTwo);

	while (_window.GetIsOpen())
	{
		_window.ProcessEvents();
		renderer.BeginRenderFrame(RGBAColor_u8(50, 180, 255, 255));
		state->Execute();
		renderer.EndRenderFrame();
		renderer.PresentFrame();
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------