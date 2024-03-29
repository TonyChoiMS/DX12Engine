#pragma once
#include "Component.h"

enum class LIGHT_TYPE : uint8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightColor
{
	Vec4 diffuse;
	Vec4 ambient;
	Vec4 specular;
};

struct LightInfo
{
	LightColor color;
	Vec4 position;			// Directional Light는 position이 의미 없습니다.
	Vec4 direction;			// Point LIght는 큰 의미 없습니다.
	int32 lightType;		// shader와 padding을 맞추기 위해 enum이 int8이어도 여기는 int32로 정의.
	float range;			// 빛의 최대 범위
	float angle;			// 빛이 비추는 각도
	int32 padding;			// PackingRule!!! 어떤 구조체를 만들때 구조체가 실질적으로 메모리에 올라가는 메모리 정렬 순서. LightInfo 데이터의 사이즈를 16배수로 맞추기 위해서 사용.
};

// 쉐이더에 넘겨줄 빛 데이터
// 라이트는 물체마다 매번 교체해서 작업하는게 아니라, 프레임당 딱 한번만 모든 빛에 대한 정보를 
// 세팅해서 사용합니다.
struct LightParams
{
	uint32 lightCount;
	Vec3 padding;
	LightInfo lights[50];			// Material은 각각 하나씩 쉐이더에 넘겼지만, 라이트는 한번에 모두 넘깁니다.
};
// 8:00
class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;
	void Render();
	void RenderShadow();

public:
	LIGHT_TYPE GetLightType() { return static_cast<LIGHT_TYPE>(_lightInfo.lightType); }
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(Vec3 direction);
	
	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { _lightInfo.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type);
	void SetLightRange(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

	void SetLightIndex(int8 index) { _lightIndex = index; }

private:
	LightInfo _lightInfo = {};

	int8 _lightIndex = -1;
	shared_ptr<class Mesh> _volumeMesh;
	shared_ptr<class Material> _lightMaterial;

	shared_ptr<GameObject> _shadowCamera;
};

