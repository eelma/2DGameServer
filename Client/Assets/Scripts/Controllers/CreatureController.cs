using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using static Define;

public class CreatureController : MonoBehaviour
{
    public float _speed = 5.0f;


    //protected Vector3Int _cellPos = Vector3Int.zero;
    public Vector3Int CellPos { get; set; } = Vector3Int.zero;
    protected Animator _animator;
    protected SpriteRenderer _sprite;

    CreatureState _state = CreatureState.Idle;

    public CreatureState State
    {
        get { return _state; }
        set
        {
            if (_state == value)
                return;

            _state = value;
            UpdateAnimation();
        }
    }
    MoveDir _lastDir = MoveDir.Down;
    MoveDir _dir = MoveDir.Down;
    public MoveDir Dir
    {
        get { return _dir; }
        set
        {
            if (_dir == value)
            {

                return;
            }
            
            _dir = value;
            if (value != MoveDir.None)
                _lastDir = value;
            UpdateAnimation();
        }
    }

    protected virtual void UpdateAnimation()
    {
        if(_state == CreatureState.Idle)
        {
            switch(_lastDir)
            {
            case MoveDir.Up:
                _animator.Play("IDLE_BACK");
                _sprite.flipX = false;
            break;

            case MoveDir.Down:
                _animator.Play("IDLE_FRONT");
                _sprite.flipX = false;
            break;
            case MoveDir.Left:
                _animator.Play("IDLE_RIGHT");
                _sprite.flipX = true;
            break;

            default:
                _animator.Play("IDLE_RIGHT");
                _sprite.flipX = false;
            break;
        }
        }
        else if(_state == CreatureState.Moving)
        {
            switch (_dir)//어느쪽 방향을 보고있나
            {
                case MoveDir.Up:
                    _animator.Play("WALK_BACK");
                    _sprite.flipX = false;
                    break;

                case MoveDir.Down:
                    _animator.Play("WALK_FRONT");
                    _sprite.flipX = false;
                    break;

                case MoveDir.Left:
                    _animator.Play("WALK_RIGHT");
                    _sprite.flipX = true;
                    break;

                case MoveDir.Right:
                    _animator.Play("WALK_RIGHT");
                    _sprite.flipX = false;
                    break;
            }
        }
        else if(_state == CreatureState.Skill)
        {

        }
        else
        {

        }
    }

    void Start()
    {
        Init();
        
    }

    void Update()
    {
        UpdateController();
    }

    protected virtual void Init()
    {
        _animator = GetComponent<Animator>();//컴포넌트를 찾아서 넣어주세요
        _sprite = GetComponent<SpriteRenderer>();
        Vector3 Pos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.5f, 0.5f);//월드좌표로 반환
        transform.position = Pos;
    }
    protected virtual void UpdateController()
    {
        UpdatePosition();
        UpdateIsMoving();
    }
  
   
    //스르륵 이동하는것을 처리
    void UpdatePosition()//서버에선 이미 이동해있음
    {
        if (State !=CreatureState.Moving)
            return;
        Vector3 destPos = Managers.Map.CurrentGrid.CellToWorld(CellPos) + new Vector3(0.5f, 0.5f);//월드좌표로 반환
        Vector3 moveDir = destPos - transform.position;//목적지에서 현재위치를 뺌

        //도착 여부 체크
        float dist = moveDir.magnitude;//방향벡터의 크기를 긁어옴
        if (dist < _speed * Time.deltaTime)
        {
            transform.position = destPos;
            //예외적으로 애니메이션을 직접 컨트롤
            _state = CreatureState.Idle;//움직임이 끝났다
            if (_dir == MoveDir.None)
                UpdateAnimation();
        }
        else//아직 도착 안함
        {
            transform.position += moveDir.normalized * _speed * Time.deltaTime;//아직 움직이고있다
            State = CreatureState.Moving;
        }
    }
    //이동 가능 상태일 때 실제 좌표를 이동한다
    void UpdateIsMoving()
    {
        if (State == CreatureState.Idle && _dir != MoveDir.None)//멈춰있을때 움직일 수 있다
        {
            Vector3Int destPos = CellPos;
            switch (_dir)
            {
                case MoveDir.Up:
                    destPos += Vector3Int.up;
                    break;
                case MoveDir.Down:
                    destPos += Vector3Int.down;
                    break;
                case MoveDir.Left:
                    destPos += Vector3Int.left;
                    break;
                case MoveDir.Right:
                    destPos += Vector3Int.right;
                    break;
            }
            if (Managers.Map.CanGo(destPos))
            {
                if(Managers.Object.Find(destPos))
                {
                    CellPos = destPos;
                    State = CreatureState.Moving;
                }
                
            }
        }
    }
}
