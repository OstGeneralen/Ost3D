#include "Application.h"
#include <Graphics/Rendering/RenderingFramework.h>
#include <Graphics/Rendering/Camera.h>
#include <Graphics/Rendering/ConstantBufferData.h>
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

	ConstantBuffer cameraConstantBuffer;
	bool result = renderFramework->MakeConstantBuffer(cameraConstantBuffer, 0);

	float windowAspect = static_cast<float>(_window.GetDimensions().X) / static_cast<float>(_window.GetDimensions().Y);
	Camera camera{ windowAspect, Degrees(90.0f) };
	camera.SetPosition({ 0.0f, 0.0f, -1.f });
	camera.SetForward({ 0,0,1 });

	TConstantBufferData<Mat4x4> cameraCBufData;
	cameraCBufData.Content = camera.GetViewProjection().GetTransposed();
	cameraConstantBuffer.CopyAcross(cameraCBufData);

	IShaderCompiler* shaderCompiler = Rendering::CreateShaderCompiler();
	auto vertexShader = shaderCompiler->CompileShader("Shaders/TestWithCamera.hlsl", EShaderType::Vertex);
	auto pixelShader = shaderCompiler->CompileShader("Shaders/TestWithCamera.hlsl", EShaderType::Pixel);

	if (vertexShader.Status == false || pixelShader.Status == false)
	{
		return;
	}

	RenderStateDesc stateDesc{};
	stateDesc.VertexShader = vertexShader.Shader;
	stateDesc.PixelShader = pixelShader.Shader;
	stateDesc.VertexFormatInfo = Vertex_Pos4_fCol4::StaticVertexFormatInfo();
	stateDesc.HasConstantBuffer = true;

	RenderStateHandle state = renderer.CreateRenderState(stateDesc);
	state.SetConstBuffer(&cameraConstantBuffer);

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

	Vertex_Pos4_fCol4 triCVerts[] = {
	{{1.0f, -1.0f, 2.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
	{ { -1.0f, 1.0f, 2.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }},
	{ {1.0f, 1.0f, 2.0f, 1.0f}, {1.0f, 0.5f, 0.0f, 1.0f} }
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

	Drawable triCDrawable{};
	triCDrawable.VertexFormatID = Vertex_Pos4_fCol4::StaticVertexFormatID();
	triCDrawable.VertexSize = static_cast<unsigned>(sizeof(Vertex_Pos4_fCol4));
	triCDrawable.VertexCount = 3;
	triCDrawable.VertexDataBuffer = triCVerts;

	state.AddDrawable(triADrawable);
	state.AddDrawable(triBDrawable);
	state.AddDrawable(triCDrawable);

	Vector3f cameraForward = { 0,0,1 };
	float v = 0;

	while (_window.GetIsOpen())
	{

		_window.ProcessEvents();
		renderer.BeginRenderFrame(RGBAColor_u8(50, 180, 255, 255));

		// TMP Camera forward update
		v += 0.01f;
		cameraForward.X = Math::Sin(v);
		cameraForward.Z = 1;
		cameraForward.Normalize();
		//camera.SetForward(cameraForward);
		Vector3f pos = { Math::Cos(v) * 4.0f, 0.0f, Math::Sin(v) * 4.0f };
		camera.SetPosition(pos);
		Vector3f fwd = Vector3f(0, 0, 0) - pos;
		camera.SetForward(fwd.Normalize());

		

		cameraCBufData.Content = camera.GetViewProjection().GetTransposed();

		cameraConstantBuffer.CopyAcross(cameraCBufData);

		renderer.ExecuteRenderState(state);

		renderer.EndRenderFrame();
		renderer.PresentFrame();

	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------