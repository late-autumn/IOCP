#pragma once
#include "Define.h"

class CMapObject
{
public:
	int type;         // 영역 설정 0 , 오브젝트(건물 등) 1~
	int currentDepth; // 현재 깊이
	int maxDepth;     // 최대 깊이
	CPoint3D pos; // 기준
	CPoint3D min; // 최소
	CPoint3D max; // 최대
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

	// 최대 깊이까지 4등분하며 생성한다.
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
		// 시작
		// 현재 깊이가 최대 깊이보다 적을 경우 공간을 4등분 한다.
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
				case 1: // 왼쪽 앞
					newPos.SetXYZ(pos.x - newSize.x, pos.y, pos.z + newSize.z);
					//	cout << i << " 왼앞 번 ";
					break;
				case 2: // 오른쪽 앞
					newPos.SetXYZ(pos.x + newSize.x, pos.y, pos.z + newSize.z);
					//	cout << i << " 오앞 번 ";
					break;
				case 3: // 왼쪽 뒤 
					newPos.SetXYZ(pos.x - newSize.x, pos.y, pos.z - newSize.z);
					//	cout << i << " 왼뒤번 ";
					break;
				case 4: // 오른쪽 뒤
					newPos.SetXYZ(pos.x + newSize.x, pos.y, pos.z - newSize.z);
					//	cout << i << " 오뒤번 ";
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
			//cout << " 구역생성 안함 ";
			//cout << "vector.size " << v_obj.size() << endl;
		}
	};

	// 오브젝트 추가하기
	void AddObject(CMapObject &obj){
		if (currentDepth >= maxDepth){ // 최대깊이에 도달했을 경우 오브젝트를 삽입한다.
			if (pos.x < obj.pos.x){// 대상의 위치가 오른쪽
				if (pos.z < obj.pos.z){ // 오른쪽이면서 앞
					if (max.x > obj.min.x && max.z > obj.min.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 추가됨" << endl;
					}
				}
				if (pos.z >= obj.pos.z){ // 오른쪽이면서 뒤
					if (max.x > obj.min.x && min.z < obj.max.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 추가됨" << endl;
					}
				}
			}
			if (pos.x >= obj.pos.x){ // 대상의 위치가 왼쪽
				if (pos.z < obj.pos.z){ // 왼쪽이면서 앞
					if (min.x < obj.max.x && max.z > obj.min.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 추가됨" << endl;
					}
				}
				if (pos.z >= obj.pos.z){ // 왼쪽이면서 뒤
					if (min.x < obj.max.x && min.z < obj.max.z){
						v_obj.push_back(&obj);
						//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 추가됨" << endl;
					}
				}
			}
		}
		else{ // 최대깊이에 도달하지 못햇을 경우 다음 깊이에 전달한다.
			for (auto temp : v_obj){
				if (pos.x < obj.pos.x){// 대상의 위치가 오른쪽
					if (pos.z < obj.pos.z){ // 오른쪽이면서 앞
						if (max.x > obj.min.x && max.z > obj.min.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 들어감" << endl;
							temp->AddObject(obj);
						}
					}
					if (pos.z >= obj.pos.z){ // 오른쪽이면서 뒤
						if (max.x > obj.min.x && min.z < obj.max.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 들어감" << endl;
							temp->AddObject(obj);
						}
					}
				}
				if (pos.x >= obj.pos.x){ // 대상의 위치가 왼쪽
					if (pos.z < obj.pos.z){ // 왼쪽이면서 앞
						if (min.x < obj.max.x && max.z > obj.min.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 들어감" << endl;
							temp->AddObject(obj);
						}
					}
					if (pos.z >= obj.pos.z){ // 왼쪽이면서 뒤
						if (min.x < obj.max.x && min.z < obj.max.z){
							//cout << currentDepth << " Depth " << pos.x << ", " << pos.y << ", " << pos.z << " 에 들어감" << endl;
							temp->AddObject(obj);
						}
					}
				}
			}
		}
	}


	bool CollisionCheck(CMapObject &obj){
		// 지역 안에 있는지 체크
		if (min.x <= obj.pos.x && max.x >= obj.pos.x &&
			//min.y <= obj.pos.y && max.y >= obj.pos.y &&
			min.z <= obj.pos.z && max.z >= obj.pos.z){

			if (currentDepth >= maxDepth){ // 최대깊이에 도달했을 경우 가지고있는 오브젝트들과 충돌체크한다.
				for (auto temp : v_obj){
					if (temp->pos.x < obj.pos.x){// 대상의 위치가 오른쪽
						if (temp->pos.z < obj.pos.z){ // 오른쪽이면서 앞
							if (temp->max.x > obj.min.x && temp->max.z > obj.min.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " 에 있는 오브젝트와 충돌됨 " << endl;
								return true;
							}
						}
						else{ // 오른쪽이면서 뒤
							if (temp->max.x > obj.min.x && temp->min.z < obj.max.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " 에 있는 오브젝트와 충돌됨 " << endl;
								return true;
							}
						}
					}
					else{ // 대상의 위치가 왼쪽
						if (temp->pos.z < obj.pos.z){ // 왼쪽이면서 앞
							if (temp->min.x < obj.max.x && temp->max.z > obj.min.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " 에 있는 오브젝트와 충돌됨 " << endl;
								return true;
							}
						}
						else{ // 왼쪽이면서 뒤
							if (temp->min.x < obj.max.x && temp->min.z < obj.max.z){
								//cout << currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " 에 있는 오브젝트와 충돌됨 " << endl;
								return true;
							}
						}
					}
				}
			}
			else{ // 최대깊이에 도달하지 못햇을 경우 다음 깊이에 전달한다.
				for (auto temp : v_obj){
					if (temp->CollisionCheck(obj)){
						//cout << temp->currentDepth << " Depth " << temp->pos.x << ", " << temp->pos.y << ", " << temp->pos.z << " 구역에 들어감" << endl;
						return true;
					}
				}
			}
		}
		else if (currentDepth == 0){
			cout << "맵 밖으로 나감" << endl;
			return true;
		}
		// 충돌이 안됬을 경우
		return false;
	}
};