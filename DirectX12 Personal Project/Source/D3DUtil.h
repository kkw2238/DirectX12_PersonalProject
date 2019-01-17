#pragma once
#include "../stdafx.h"

using namespace Microsoft::WRL;


struct DefaultOption { };

#define DEFAULTOPT DefaultOption()

class D3DUtil
{
public:
	static unsigned int CalcConstantBufferByteSize(UINT byteSize)
	{
		// Constant버퍼는 256 배수의 크기의 바이트로 정해져야함. 
		return (byteSize + 255) & ~255;
	}
	static ComPtr<ID3DBlob> LoadFileBinary(const std::wstring& fileName);

	static ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		const void* defaultData,
		UINT64 byteSize,
		ComPtr<ID3D12Resource>& uploadBuffer);

	static ComPtr<ID3DBlob> CompileShader(
		const std::wstring& fileName,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);

	static void ChangeResourceState(
		ID3D12Resource* resource,
		ID3D12GraphicsCommandList* commandList,
		D3D12_RESOURCE_STATES beforeState,
		D3D12_RESOURCE_STATES afterState);

	static std::wstring StringToWString(std::string& str);
};

class D3DDescriptorOption
{
public:
	static D3D12_RESOURCE_DESC DepthStencilDesc(DefaultOption);
	static D3D12_RESOURCE_DESC DepthStencilDesc() { return D3D12_RESOURCE_DESC(); }

	static D3D12_RESOURCE_DESC RenderTargetDesc(DefaultOption);
	static D3D12_RESOURCE_DESC RenderTargetDesc() { return D3D12_RESOURCE_DESC(); }
};

class ExeptionUtility
{
public:
	ExeptionUtility(const HRESULT hr, const int line, const std::string& fileName, const std::string& functionName);

	~ExeptionUtility() = default;

	std::wstring D3DErrorToString();
	std::wstring WinErrorToString();

protected:
	HRESULT errorCode;
	int errorLine;
	std::string errorFileName;
	std::string errorFunctionName;
};

#define ThrowIfFail(hr)  { \
	if(FAILED(hr)) throw ExeptionUtility(hr, __LINE__, __FILE__, __FUNCTION__); \
}


