#Asynchronous Socket
###비동기 소켓이란?
지금까지쓰레드, 프로세스, 에코서버 등 올린 것들은 동기 서버입니다.

동기가 뭐냐 하면, 예를 들어 socket에서 Read/Recv를 하면 Send가 올때까지 기다리는 것을 동기라고 합니다.

비동기는 Read/Recv에서 send가 오지 않으면 그냥 다른 명령어를 실행하다 Send신호가 오면 그 때 Read/Recv를 합니다.

예는 아래 그림 처럼 동기 방법일때는 Recv에서 Send가 올때까지 기다렸는데, 비동기는 그렇지 않고

반복문을 계속 반복하다 도중에 Client가 Send를 보내면 데이터를 받습니다.

그림[1]


사용할 함수는 #include <fcnt.h>에있는 fcntl()함수를 사용하여 non-blocking 소켓 프로그래밍을 할 것입니다.

###소스코드

####Server
```
```
####Client
```
```
