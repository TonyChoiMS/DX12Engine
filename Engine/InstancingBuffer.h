#pragma once

// 물체 정보
// 월드, 월드뷰, 월드 뷰 프로젝션
struct InstancingParams
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
};

class InstancingBuffer
{
public:
	InstancingBuffer();
	~InstancingBuffer();

	void Init(uint32 maxCount = 10);

	void Clear();
	void AddData(InstancingParams& params);
	void PushData();

public:
	uint32 GetCount() { return static_cast<uint32>(_data.size()); }
	ComPtr<ID3D12Resource> GetBuffer() { return _buffer; }
	D3D12_VERTEX_BUFFER_VIEW GetBufferView() { return _bufferView; }

	void SetID(uint64 instanceId) { _instanceId = instanceId; }
	uint64 GetId() { return _instanceId; }

private:
	uint64 _instanceId = 0;
	ComPtr<ID3D12Resource> _buffer;
	D3D12_VERTEX_BUFFER_VIEW _bufferView;

	uint32 _maxCount = 0;
	vector<InstancingParams> _data;
};

