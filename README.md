# OSS_JSON_project - json parser와 application만들기  
  Start Date : 2019.5.16   
  Due Date : 2019.6.3  
  
### Group6
  21400240 김형준  
  21600301 박지현  
  21800060 김대영   
  
## What does this project do?  
  이 프로젝트는 .json 형식의 파일로부터 데이터를 읽어와 파싱하고 토큰으로 나누는 것에서 시작한다. .json형식으로 불러올 파일은 application에 쓰일 데이터들을 담고 있다.첫번째 데이터를 토큰으로 나누는 단계를 거치면, 우리가 만든 application에 의미있는 데이터로 쓸 수 있다.  
  우리가 만들고자 하는 application은 개인의 맘스키친 이용과 관리에 대한 것이다. 그래서 맘스 메뉴와 사용자,즉 학생들에 대한 이용에 대한 정보를 mamsmenu.json파일에 저장하고 파서에 전달해 가공할 것이다. application을 구현하기 위한 함수들 역시 json_parser.c파일에 다 넣어져 있고, 처음 json_parser.c를 실행시키고나면 바로 우리가 만든 맘스키친 어플리케이션에 대한 실행기능을 선택할 수 있다.   
  application의 기능은 다음과 같다
  1. 특정 요일에 대한 메뉴보기      
  2. 한 user가 한주동안 먹은 특정 메뉴 조회    
  3. 한 user가 한주동안 맘스키친에서 소비한 금액    
  4. parsing에 성공한 결과로 만들어진 모든 토큰 출력      
  
## Why is this project useful?    
 한동대학교 학생이라면 맘스키친을 사용하기 마련이다. 하지만 맘스키친을 사용하는데 학생들 개개인에 대한 정보를 저장한 데이터베이스나 어플이 없기 때문에, 자신들이 얼마나 맘스키친을 이용하고 무엇을 얼마나 먹었는지 잘 모른다. 우리는 맘스키친을 잘 이용하는 학생들이 자신들이 시단기록과 소비를 확인할 수 있게끔, 편리하게 확인하게 하고자 이러한 어플리케이션을 만들었다. 아직은 cli수준에 간단한 기능들 밖에 없지만, 맘스키친 이용에 대한 프로젝트 주제가 다른 사람들에게 영향을 주어, 더 좋은, 발전된 어플리케이션이 나오기를 기대한다.

## How do I get started?
  1. gcc 컴파일한다 : gcc -o json_parser.exe json_parser.c   
  2. 인자에 .json파일을 같이 넣어 실행한다 : ./jsonparser mamsmenu.git.
  3. 1~4번 기능 중에 실행하고 싶은 기능의 숫자를 입력한다 : (ex)1    
     3-1. 1을 입력한 후 요일 입력 : (ex)monday    
     3-2. 2를 입력한 후 user이름과 메뉴 입력 : (ex)Kimhyeongjune 냉면    
     3-3. 3을 입력한 후 user이름 입력 : Kimdaeyoung    
     3-4. 4을 입력한 후 parsing 후 token에 대한 모든 내용 출력     
          
## Where you need more help, please contact here   
  21400240@handong.edu (Hyeongjun,Kim)      
  21600301@handong.edu (Jihyun,Park)    
  21800060@handong.edu (Daeyoung,Kim)   
 
