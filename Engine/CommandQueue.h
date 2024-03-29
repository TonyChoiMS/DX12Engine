#pragma once

class SwapChain;
class DescriptorHeap;

// *********************
// GraphicsCommandQueue
// *********************

class GraphicsCommandQueue
{
public:
	~GraphicsCommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin();
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCmdList() { return	_cmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return	_resCmdList; }

private:
	// 이 3개가 핵심.
	// CommandQueue : DX12에 등장
	// GPU에 일을 요청할 때 하나씩 요청하면 비효율적
	// [Command 목록]에 일감을 차곡차곡 기록했다가 한번에 요청하는 것.
	ComPtr<ID3D12CommandQueue>			_cmdQueue;		// 일감 목록	
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;		// 일감 자체를 넣어주기 위한 메모리 공간을 할당
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;		// 일감 리스트

	ComPtr<ID3D12CommandAllocator>		_resCmdAlloc;		// 리소스를 로드하는 용도의 메모리 공간을 할당
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;		// 리소스를 로드하는 용도의 리스트

	// Fence : 울타리(?)
	// CPU / GPU 동기화를 위한 간단한 도구
	// 주어진 일이 있을 때 그 일이 다 끝날 때까지 기다리는 행동을 할 수 있게 해주는 인터페이스
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
};

// *********************
// ComputeCommandQueue
// *********************

class ComputeCommandQueue
{
public:
	~ComputeCommandQueue();

	void Init(ComPtr<ID3D12Device> device);
	void WaitSync();
	void FlushComputeCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetComputeCmdList() { return _cmdList; }

private:
	ComPtr<ID3D12CommandQueue> _cmdQueue;
	ComPtr<ID3D12CommandAllocator> _cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> _cmdList;

	ComPtr<ID3D12Fence> _fence;
	uint32 _fenceValue = 0;
	HANDLE _fenceEvent = INVALID_HANDLE_VALUE;
};