#pragma once
/*
 * CompiledShader 클래스는 컴파일된 셰이더 코드가 담겨있는 클래스임.
 * hlsl중복 컴파일을 방지하기 위해 만들어져 있음.
 */


#include "Source/D3DUtil.h"
#include <map>

class CompiledShader
{
public:
	CompiledShader();
	~CompiledShader();

	static CompiledShader* Instance();

public:
	D3D12_SHADER_BYTECODE	ShaderByteCode(ComPtr<ID3DBlob> shaderCode);
	D3D12_SHADER_BYTECODE	GetShaderByteCode(const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);

private:
	std::unordered_map<std::string, ComPtr<ID3DBlob>> compiledShaders;
};

#define COMPILEDSHADER CompiledShader::Instance()