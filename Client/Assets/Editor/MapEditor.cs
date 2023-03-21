using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;
using System.IO;

#if UNITY_EDITOR
using UnityEditor;
#endif

public class MapEditor
{
#if UNITY_EDITOR

    //%(Ctrl), #(Shift), &(alt)
    [MenuItem("Tools/GenerateMap %#g")]
    private static void GeneratorMap()
    {
        //특정 폴드 안에 있는 모든 파일을 긁어오는 기능
        GameObject[] gameObjects = Resources.LoadAll<GameObject>("Prefabs/Map");
        foreach (GameObject go in gameObjects)
        {
            //타일맵을 찾아주는데 게임오브젝트에서 Tilemap_Cloision이라는 이름
            Tilemap tmBase = Util.FindChild<Tilemap>(go, "Tilemap_Base", true);
            Tilemap tm = Util.FindChild<Tilemap>(go, "Tilemap_Collision", true);
           
            //블록킹된 목록을 하나씩 추출
            //별도의 파일로 관리해서 서버쪽으로 전해줌

            //받아야하는것은 맵의 크기 min max 크기
            //바이너리 or text
            //갈 수 있는 영역은 0 막힌곳 1
            using (var writer = File.CreateText($"Assets/Resources/Map/{go.name}.txt"))
            {
                writer.WriteLine(tmBase.cellBounds.xMin);
                writer.WriteLine(tmBase.cellBounds.xMax);
                writer.WriteLine(tmBase.cellBounds.yMin);
                writer.WriteLine(tmBase.cellBounds.yMax);

                for (int y = tmBase.cellBounds.yMax; y >= tmBase.cellBounds.yMin; y--)
                {//맨 윗줄부터
                    for (int x = tmBase.cellBounds.xMin; x <= tmBase.cellBounds.xMax; x++)
                    {//왼쪽에서 오른쪽
                        TileBase tile = tm.GetTile(new Vector3Int(x, y, 0));
                        if (tile != null)
                            writer.Write("1");
                        else
                            writer.Write("0");
                    }
                    writer.WriteLine();
                }
            }

        }


    }


#endif
}
