#include <iostream>
#include <stdexcept>
class Test {

public:
	Test(char id) : id_(id) {}

	~Test() {
		std::cout << "Destructor execution: "
			<< id_ << std::endl;
	}

private:
	char id_;
};
int funcB() {
	Test r('B');
	throw std::runtime_error("Exception from funcB!\n");
	std::cout << "Executed in B" << std::endl;
	return 0;
}
int funcA() {
	Test r('A');
	funcB();
	std::cout << "Executed in A" << std::endl;
	return 0;
}
int main() {
	try {
		funcA();
	}
	catch (std::exception& e) {
		std::cout << "Exception : " << e.what();
	}
}
/*

(실습 1-1) 분석결과 :

함수 호출 순서는 다음과 같습니다.

main -> funcA() -> funcB()

실행 결과는 아래와 같습니다.

Executed in B
Destructor execution: B
Executed in A
Destructor execution: A

분석은 아래와 같습니다.

funcA()에서 id A를 가지는 Test 객체를 생성한 직후 funcB()로 진입, funcB()에서 id B를 가지는 Test 객체를 생성합니다.

Executed in B 문구를 포함한 라인이 실행되고, 리턴문이 실행되며 funcB()는 종료됨과 동시에 소멸자(파괴자)가 실행됩니다.

이후 funcA()에서 funcB() 호출 위치의 다음 라인인 Executed in A 문구를 출력하고 funcA() 리턴문이 실행되며 소멸자(파괴자)가 실행됩니다.

*/


/*

(실습 1-2) 분석결과 :

실행 결과는 아래와 같습니다.

Destructor execution: B
Destructor execution: A
Exception : Exception from funcB!

분석은 아래와 같습니다.

funcB()에서 객체를 생성한 직후 exception을 throw하는데,

예외가 발생하게 되면 예외를 전파시키게 됩니다.

catch 부분으로 점프 하면서, 함수들에 있던 객체들을 해제시키고,

원래 예외를 던지는 부분 이후에 실행되어야 할 코드들은 실행되지 않습니다.

이와 같이 catch 로 점프 하면서 스택 상에서 정의된 객체들을 소멸시키는 과정을 스택 풀기(stack unwinding)라고 합니다.

*/