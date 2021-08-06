#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	shared_ptr<Transform> GetTransform();

	void AddComponent(shared_ptr<Component> component);


private:
	// ������Ʈ�� �ϳ����� ���� �� �ֱ� ������ �ִ� ������ ������ �Ǳ⿡ array �ڷᱸ�� ����մϴ�.
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// MonoBehaviour�� ����ڰ� �ִ� ����� ������ �� �� ���� ������ vector�� ����մϴ�.
	vector<shared_ptr<MonoBehaviour>> _scripts;

};
