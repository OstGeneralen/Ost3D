#include "DXShaderInfo.h"
#include <exception>

std::string DXShaderInfo::ShaderModelStr(EType type, EVersion model)
{
    if (type == DXShaderInfo::EType::Vertex)
    {
        switch (model)
        {
        case DXShaderInfo::EVersion::General:   return "vs_5_1";
        case DXShaderInfo::EVersion::DX12:      return "vs_6_0";
        case DXShaderInfo::EVersion::RayTrace:  return "vs_6_1";
        }
    }
    else if (type == DXShaderInfo::EType::Pixel)
    {
        switch (model)
        {
        case DXShaderInfo::EVersion::General:   return "ps_5_1";
        case DXShaderInfo::EVersion::DX12:      return "ps_6_0";
        case DXShaderInfo::EVersion::RayTrace:  return "ps_6_1";
        }
    }

    throw std::exception("Shader type invalid");
    return "INVALID";
}


