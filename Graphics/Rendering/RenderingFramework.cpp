#include "RenderingFramework.h"
#include "../DX/Framework.h"
#include "../DX/Renderer.h"
#include "../DX/ShaderCompiler.h"

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "Common_Debug")

IRenderFramework* Rendering::CreateFramework()
{
    return new DX::Framework();
}

IShaderCompiler* Rendering::CreateShaderCompiler()
{
    return new DX::ShaderCompiler();
}
