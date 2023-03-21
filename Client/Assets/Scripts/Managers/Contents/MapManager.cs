using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
public class MapManager
{
    public Grid CurrentGrid { get; private set; }//그리드를 물고있게

    public int MinX { get; set; }
    public int MaxX { get; set; }
    public int MinY { get; set; }
    public int MaxY { get; set; }

    bool[,] _collision;
    //GameObject[,] _obj;
    //게임오브젝트의 좌표가 변할때마다 매니저에 그것을 통보
    public bool CanGo(Vector3Int cellPos)//셀포지션을 가지고 이 영역을 갈 수있나 물어봄 
    {
        if (cellPos.x < MinX || cellPos.x > MaxX)
            return false;
        if (cellPos.y < MinY || cellPos.y > MaxY)
            return false;

        int x = cellPos.x - MinX;
        int y = MaxY - cellPos.y;

        return !_collision[y, x];
    }
    public void LoadMap(int mapId)
    {
        DestroyMap();
        //맵 프리팹 로드
        string mapName = "Map_" + mapId.ToString("000");
        GameObject go = Managers.Resource.Instantiate($"Map/{mapName}");//게임객체 새로 생성 맵산하에 mapname을 불러와서 로드
        go.name = "Map";

        //찾은 맵을 찾아가서 Tilemap_Collision을 꺼주는 기능
        GameObject collision = Util.FindChild(go, "Tilemap_Collision", true);
        if (collision != null)
            collision.SetActive(false);
        CurrentGrid = go.GetComponent<Grid>();//그리드를 찾아서 물고있음


        //collision 관련 파일
        TextAsset txt = Managers.Resource.Load<TextAsset>($"Map/{mapName}");
        StringReader reader = new StringReader(txt.text);//txt의 text를 꺼내서 저장한내용을 string으로 반환
        MinX = int.Parse(reader.ReadLine());//방금 추출한 첫번째 줄을 int로 파싱해서 넣어줌
        MaxX = int.Parse(reader.ReadLine());
        MinY = int.Parse(reader.ReadLine());
        MaxY = int.Parse(reader.ReadLine());

        int xCount = MaxX - MinX+1;
        int yCount = MaxY - MinY + 1;
        _collision = new bool[yCount, xCount];

        for(int y=0; y<yCount; y++)
        {
            string line = reader.ReadLine();
            for(int x =0;x<xCount;x++)
            {
                _collision[y, x] = (line[x] == '1' ? true : false);
            }
        }
    }
    public void DestroyMap()
    {
        GameObject map = GameObject.Find("Map");
        if(map !=null)
        {
            GameObject.Destroy(map);//맵 삭제
            CurrentGrid = null;
        }
    }


}
