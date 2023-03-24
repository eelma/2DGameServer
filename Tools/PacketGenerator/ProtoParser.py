

class ProtoParser():
    def __init__(self, start_id, recv_prefix, send_prefix):
        self.recv_pkt = [] # 수신 패킷 목록
        self.send_pkt = [] # 송신 패킷 목록
        self.total_pkt = [] # 모든 패킷 목록
        self.start_id = start_id
        self.id=start_id
        self.recv_prefix=recv_prefix
        self.send_prefix=send_prefix

    def parse_proto(self, path): #self는 디스포인터라서 인자가아니고 path를 인자로 받아줄것이다
        f = open(path, 'r') #경로는 path 타입은 read
        lines = f.readlines() #모든 줄을 한줄한줄 읽어서 lines에 저장
        
        for line in lines:
            if line.startswith('message')==False:# 메세지로 시작하는지 체크
                continue

            pkt_name = line.split()[1].upper()
            #스플릿을하면 모든 단어들을 추출해서 배열로 만듬 0번째 단어는 메세지이고
            #1번 인덱스에 있는 단어가 실제 패킷 이름
            if pkt_name.startswith(self.recv_prefix):
                self.recv_pkt.append(Packet(pkt_name, self.id))
            #만약에 패킷 name이 C_나 S_로 시작한다면 recv_pkt에 append해줘라(벡터의 push_back과 비슷함)
            elif pkt_name.startswith(self.send_prefix):
                self.send_pkt.append(Packet(pkt_name, self.id))
            else:
                continue

            self.total_pkt.append(Packet(pkt_name, self.id))#total_pkt안에 append해서 패킷등록 나중에 전체 코드를 만들때 c_건 s_건 상관없이 enum값을 넣어줘야하기 때문에
            self.id+=1
            
        f.close()



class Packet:
    def __init__(self,name,id):
        self.name = name
        self.id = id