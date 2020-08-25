#pragma once
#include "Define.h"

class CMapObject
{
public:
	int type;         // ���� ���� 0 , ������Ʈ(�ǹ� ��) 1~
	int currentDepth; // ���� ����
	int maxDepth;     // �ִ� ����
	CPoint3D pos; // ����
	CPoint3D min; // �ּ�
	CPoint3D max; // �ִ�
	vector<CMapObject*> v_obj;
	vector<CMapObject*> v_AddObj;

public:
	CMapObject()
	{
		type = 0;
		pos.SetXYZ(0, 0, 0);
		min.SetXYZ(0, 0, 0);
		max.SetXYZ(0, 0, 0);
		v_obj.clear();
		v_AddObj.clear();
	};
	CMapObject(int startDepth, int Depth, int type1, CPoint3D pos1, CPoint3D size1)
	{
		type = type1;
		currentDepth = startDepth;
		maxDepth = Depth;
		pos.SetXYZ(pos1.x, pos1.y, pos1.z);
		min.x = pos.x - size1.x; min.y = pos.y - size1.y; min.z = pos.z - size1.z;
		max.x = pos.x + size1.x; max.y = pos.y + size1.y; max.z = pos.z + size1.z;
		v_obj.clear();
	};
	~CMapObject(){ v_obj.clear(); }

	// �ִ� ���̱��� 4����ϸ� �����Ѵ�.
	void SetBox(int startDepth, int Depth, int type1, CPoint3D pos1, CPoint3D size1)
	{
		if (Depth > 9) return;
		type = type1;
		currentDepth = startDepth;
		maxDepth = Depth;
		pos.SetXYZ(pos1.x, pos1.y, pos1.z);
		min.x = pos.x - size1.x; min.y = pos.y - size1.y; min.z = pos.z - size1.z;
		max.x = pos.x + size1.x; max.y = pos.y + size1.y; max.z = pos.z + size1.z;
		v_obj.clear();


		//cout << currentDepth << " Depth  ";
		//cout << "pos " << pos.x << ", " << pos.y << ", " << pos.z;
		//if (currentDepth != maxDepth) cout << endl;
		// ����
		// ���� ���̰� �ִ� ���̺��� ���� ��� ������ 4��� �Ѵ�.
		if (startDepth < maxDepth)
		{
			CPoint3D newSize(size1.x / 2, size1.y, size1.z / 2);

			for (int i = 1; i <= 4; ++i)
			{
				CMapObject *box = new CMapObject();
				CPoint3D newPos;
				/*for (int j = 0; j <= currentDepth; ++j){
				cout << "  ";
				}*/
				switch (i){
				case 1: // ���� ��
					newPos.SetXYZ(pos.x - newSize.x, pos.y, pos.z + newSize.z);
					//	cout << i << " �޾� �� ";
					break;
				case 2: // ������ ��
					newPos.SetXYZ(pos.x + newSize.x, pos.y, pos.z + newSize.z);
					//	cout << i << " ���� �� ";
					break;
				case 3: // ���� �� 
					newPos.SetXYZ(pos.x - newSize.x, pos.y, pos.z - newSize.z);
					//	cout << i << " �޵ڹ� ";
					break;
				case 4: // ������ ��
					newPos.SetXYZ(pos.x + newSize.x, pos.y, pos.z - newSize.z);
					//	cout << i << " ���ڹ� ";
					break;
				}

				box->SetBox(currentDepth + 1, maxDepth, type1, newPos, newSize);
				v_obj.push_back(box);
			}
			/*for (int j = 0; j <= currentDepth; ++j){
			cout << "  ";
			}*/
			//cout << currentDepth << " Depth " << " vector.size " << v_obj.size() << endl;
		}
		else{
			//cout << " �������� ���� ";
			//cout << "vector.size " << v_obj.size() << endl;
		}
	};

	// ������Ʈ �߰��ϱ�
	void AddObject(CMapObject &obj){
		if (currentDepth >= maxDepth){ // �ִ���̿� �������� ��� ������Ʈ�� �����Ѵ�.
			if (pos.x < obj.pos.x){// ����� ��ġ�� ������
				if (pos.z < obj.pos.z){ // �������̸鼭 ��
					if (max.x > obj.min.x && max.z > obj.min.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� �߰���" << endl;
					}
				}
				if (pos.z >= obj.pos.z){ // �������̸鼭 ��
					if (max.x > obj.min.x && min.z < obj.max.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� �߰���" << endl;
					}
				}
			}
			if (pos.x >= obj.pos.x){ // ����� ��ġ�� ����
				if (pos.z < obj.pos.z){ // �����̸鼭 ��
					if (min.x < obj.max.x && max.z > obj.min.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� �߰���" << endl;
					}
				}
				if (pos.z >= obj.pos.z){ // �����̸鼭 ��
					if (min.x < obj.max.x && min.z < obj.max.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� �߰���" << endl;
					}
				}
			}
		}
		else{ // �ִ���̿� �������� ������ ��� ���� ���̿� �����Ѵ�.
			for (auto temp : v_obj){
				if (pos.x < obj.pos.x){// ����� ��ġ�� ������
					if (pos.z < obj.pos.z){ // �������̸鼭 ��
						if (max.x > obj.min.x && max.z > obj.min.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� ��" << endl;
							temp->AddObject(obj);
						}
					}
					if (pos.z >= obj.pos.z){ // �������̸鼭 ��
						if (max.x > obj.min.x && min.z < obj.max.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� ��" << endl;
							temp->AddObject(obj);
						}
					}
				}
				if (pos.x >= obj.pos.x){ // ����� ��ġ�� ����
					if (pos.z < obj.pos.z){ // �����̸鼭 ��
						if (min.x < obj.max.x && max.z > obj.min.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� ��" << endl;
							temp->AddObject(obj);
						}
					}
					if (pos.z >= obj.pos.z){ // �����̸鼭 ��
						if (min.x < obj.max.x && min.z < obj.max.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " �� ��" << endl;
							temp->AddObject(obj);
						}
					}
				}
			}
		}
	}


	bool CollisionCheck(CMapObject &obj){
		// ���� �ȿ� �ִ��� üũ
		if (min.x <= obj.pos.x && max.x >= obj.pos.x &&
			//min.y <= obj.pos.y && max.y >= obj.pos.y &&
			min.z <= obj.pos.z && max.z >= obj.pos.z){

			if (currentDepth >= maxDepth){ // �ִ���̿� �������� ��� �������ִ� ������Ʈ��� �浹üũ�Ѵ�.
				for (auto temp : v_obj){
					if (temp->pos.x < obj.pos.x){// ����� ��ġ�� ������
						if (temp->pos.z < obj.pos.z){ // �������̸鼭 ��
							if (temp->max.x > obj.min.x && temp->max.z > obj.min.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " �� �ִ� ������Ʈ�� �浹�� " << endl;
								return true;
							}
						}
						else{ // �������̸鼭 ��
							if (temp->max.x > obj.min.x && temp->min.z < obj.max.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " �� �ִ� ������Ʈ�� �浹�� " << endl;
								return true;
							}
						}
					}
					else{ // ����� ��ġ�� ����
						if (temp->pos.z < obj.pos.z){ // �����̸鼭 ��
							if (temp->min.x < obj.max.x && temp->max.z > obj.min.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " �� �ִ� ������Ʈ�� �浹�� " << endl;
								return true;
							}
						}
						else{ // �����̸鼭 ��
							if (temp->min.x < obj.max.x && temp->min.z < obj.max.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " �� �ִ� ������Ʈ�� �浹�� " << endl;
								return true;
							}
						}
					}
				}
			}
			else{ // �ִ���̿� �������� ������ ��� ���� ���̿� �����Ѵ�.
				for (auto temp : v_obj){
					if (temp->CollisionCheck(obj)){
						//cout << temp->currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " ������ ��" << endl;
						return true;
					}
				}
			}
		}
		else if (currentDepth == 0){
			cout << "�� ������ ����" << endl;
			return true;
		}
		// �浹�� �ȉ��� ���
		return false;
	}
};