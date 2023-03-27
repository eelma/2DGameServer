import argparse
import jinja2
import ProtoParser

def main():

    arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
    arg_parser.add_argument('--path', type=str, default='C:/Users/kgca418321/Desktop/MMO_Game/2DGameServer/Server/Common/Protobuf/bin/Protocol.proto', help='proto path')
    arg_parser.add_argument('--output', type=str, default='TestPacketHandler', help='output file')
    arg_parser.add_argument('--recv', type=str, default='C_', help='recv convecntion')
    arg_parser.add_argument('--send', type=str, default='S_', help='send convecntion')
    args = arg_parser.parse_args()

    parser = ProtoParser.ProtoParser(1000, args.recv, args.send)#생성자처럼 객체를 만들어주는것
    #아이디는 1000번대에서 시작할것이고 인자로 받은 arg.recv와 args.send를 넘겨줌 위에서 파싱해준 recv와 send를 넘겨줄 수 있게됨
    #딴 인자를 넘겨주면 그 인자를 추적해서 proto파일을 체크한다
    parser.parse_proto(args.path)
    #프로토 파일에 있는 모든 내용을 긁어서 message들을 싹 긁어옴
    #jinja2 템플릿 엔진
    file_loader = jinja2.FileSystemLoader('Templates')#Templates라는 폴더를 검색
    env=jinja2.Environment(loader=file_loader)#초기화
    
    template = env.get_template('PacketHandler.h')#샘플코드를 넘겨줌
    output = template.render(parser = parser, output=args.output)#parser를 통으로 넘겨준다 넘겨준건 오른쪽 PacketHandler은 왼쪽
    #output과 parser를 건내줘서 packethandler.h를 실행해라
    
    f=open(args.output+'.h', 'w+')#템플릿엔진이 완성한 결과물을 output으로 받아서 파일에 쓰고 파일을 닫아달라
    f.write(output)
    f.close()

    print(output)

    return

if __name__=='__main__':
    main()
