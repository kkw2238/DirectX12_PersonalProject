/*
 *	D3DUtil 클래스에는 DirectX관련 유틸리티,
 *  ExeptionUtility에는 예외에 대한 메시지 처리   
 */

#pragma once
#include "../stdafx.h"

using namespace Microsoft::WRL;

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

	static bool IsKeyDown(char key);

	static std::wstring StringToWString(std::string& str);
	static std::wstring GetExtension(const std::wstring& path);
	static std::wstring GetFileName(const std::wstring& path);
	static std::wstring GetPathExceptFileName(const std::wstring& path);
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

template<typename T>
class ObjectResourceBuffer {
public:
	ObjectResourceBuffer() {};
	~ObjectResourceBuffer() {};

public:
	ID3D12Resource* ResourceBuffer() { return m_ID3DUploadResourceBuffer.Get(); };
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() { return m_ID3DUploadResourceBuffer.Get()->GetGPUVirtualAddress(); }

	void CreateResourceBuffer(ID3D12Device* id3dDevice, UINT objectCount, bool isConstant = true) {

		if (isConstant)
			m_DataSize = D3DUtil::CalcConstantBufferByteSize(sizeof(T));
		else
			m_DataSize = sizeof(T);

		ThrowIfFail(id3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(m_DataSize * objectCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_ID3DUploadResourceBuffer.GetAddressOf())
		));

		ThrowIfFail(m_ID3DUploadResourceBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_Data)));
	}

	void CopyData(UINT index, const T & data) {
		memcpy(&m_Data[m_DataSize * index], &data, sizeof(T));
	}

	UINT DataSize() const { return m_DataSize; }

protected:
	ComPtr<ID3D12Resource> m_ID3DUploadResourceBuffer;
	UINT m_DataSize;
	BYTE* m_Data;
};
