using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
public class ObjectManager
{
	//Dictionary<int, GameObject> _objects = new Dictionary<int, GameObject>();
	List<GameObject> _objects = new List<GameObject>();

	public void Add(GameObject go)
	{
		_objects.Add(go);
	}

	public void Remove(GameObject go)
	{
		_objects.Remove(go);
	}

	public GameObject Find(Vector3Int cellPos)
	{
		foreach (GameObject obj in _objects)
		{
			CreatureController cc = obj.GetComponent<CreatureController>();
			if (cc == null)
				continue;

			if (cc.CellPos == cellPos)
				return obj;
		}

		return null;
	}
	public GameObject Find(Func<GameObject, bool>condition)//func이 Gameobject를 받고 bool로 뱉어주는 함수를 받아줌
    {
		foreach (GameObject obj in _objects)
		{
			
			if (condition.Invoke(obj))//컨디션을 받아서 invoke해서 경과가 true면 외부에서 입력해준 조건에 부합하는 객체
				return obj;
		}

		return null;
    }
	public void Clear()
	{
		_objects.Clear();
	}
}
