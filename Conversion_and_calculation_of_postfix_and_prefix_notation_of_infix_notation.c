#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_STACK_SIZE 100 //배열의 최대크기
typedef double Element; //배열의 타입

Element data[MAX_STACK_SIZE];//배열 선언
int top;//top변수 선언

void error(char str[])
{
	printf("%s\n", str);
	exit(1);
} //에러메세지 출력함수

void init_stack() { top = -1; } //배열 초기화
int is_empty() { return top == -1; } //스택 공백상태 확인
int is_full() { return top == MAX_STACK_SIZE - 1; } //스택 포화상태 확인
int size() { return top + 1; }//스택의 현재 크기 확인

void push(Element e)
{
	if (is_full())
		error("스택 포화 에러");//포화상태라면 에러
	data[++top] = e;//top을 1증가시킨 후 값 저장
}

Element pop()
{
	if (is_empty())
		error("스택 공백에러 ");//공백상태라면 에러
	return data[top--];//top의 값 반환 후 top 1감소
}

Element peek()
{
	if (is_empty())
		error("스택 공백에러");//공백상태라면 에러
	return data[top];//top에 있는 값 반환
}

void Stringadd(char temp[], char c)
{
	int i = 0;
	while (temp[i] != NULL)temp[i++];
	temp[i] = c;
	temp[i + 1] = NULL;
}//널문자인 지점까지 이동한후 뒤에 문자열을 추가하고 끝에 널문자 추가

double calc_postfix(char temp[])//후위 연산자 계산 함수
{
	char c;
	int i = 0;
	double val, val1, val2;

	init_stack();
	while (temp[i] != '\0') {//문장의 끝까지 반복
		c = temp[i++];//한 문자를 받아옴
		if (c >= '0' && c <= '9') {//숫자인 경우
			val = c - '0';
			while (temp[i] >= '0' && temp[i] <= '9')//다음문자도 숫자인 경우 자릿수를 높여 계산
			{
				c = temp[i++];
				val = val * 10 + c - '0';
			}
			push(val);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//연산자의 경우 앞의 두 수를 pop한 뒤 연산하여 push
			val2 = pop();
			val1 = pop();
			switch (c) {
			case '+':push(val1 + val2); break;
			case '-':push(val1 - val2); break;
			case '*':push(val1 * val2); break;
			case '/':push(val1 / val2); break;
			case '^':push((Element)pow(val1, val2)); break;
			}
		}
	}
	return pop();
}

int precedence(char op)//연산자 우선순위
{
	switch (op) {
	case'(':case')':return 0;
	case'+':return 1;
	case'-':return 2;
	case'*':case'/':return 3;
	case'^':return 4;
	}
	return -1;
}

void infix_to_postfix(char expr[], char temp[])//후위연산식으로 변환
{
	int i = 0;
	char c, op;

	init_stack();
	while (expr[i] != '\0') {//문장의 끝까지
		c = expr[i++];
		if ((c >= '0' && c <= '9')) {//숫자인 경우 식에 추가
			Stringadd(temp, c);
			if (!(expr[i] >= '0' && expr[i] <= '9')) {//다음이 숫자가 아닌경우 공백 추가
				Stringadd(temp, ' ');
			}
		}
		else if (c == '(')//여는괄호인경우 괄호 푸시
			push(c);
		else if (c == ')') {//닫는 괄호인 경우 여는괄호까지의 내용을 팝하여 푸시해주고 공백추가
			while (is_empty() == 0) {
				op = pop();
				if (op == '(') break;
				else
				{
					Stringadd(temp, op);
					Stringadd(temp, ' ');
				}
			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//연산자의 경우 연산 우선순위에 따라 팝시켜서 추가 이후 공백추가
			while (is_empty() == 0) {
				op = peek();
				if (precedence(c) <= precedence(op)) {

					Stringadd(temp, op);
					Stringadd(temp, ' ');
					pop();
				}
				else break;
			}
			push(c);
		}
	}
	while (is_empty() == 0)//이후 나머지 전부 pop하여 추가 이후 공백추가
	{
		Stringadd(temp, pop());
		Stringadd(temp, ' ');
	}
}

void reverseString(char* s) {//문장을 거꾸로 뒤집어 주는 함수
	size_t size = strlen(s);
	char temp;

	for (size_t i = 0; i < size / 2; i++) {
		temp = s[i];
		s[i] = s[(size - 1) - i];
		s[(size - 1) - i] = temp;
	}
}

void infix_to_prefix(char expr[], char temp[])//전위연산식으로 변환
{
	reverseString(expr);//문장을 뒤집음
	int i = 0;
	char c, op;

	init_stack();
	while (expr[i] != '\0') {//문장의 처음까지
		c = expr[i++];
		if ((c >= '0' && c <= '9')) {//숫자인 경우 식에 추가
			Stringadd(temp, c);
			if (!(expr[i] >= '0' && expr[i] <= '9')) {//다음이 숫자가 아닌경우 공백 추가
				Stringadd(temp, ' ');
			}
		}
		else if (c == ')')//닫는괄호인경우 괄호 푸시
			push(c);
		else if (c == '(') {//여는 괄호인 경우 여는괄호까지의 내용을 팝하여 푸시해주고 공백추가
			while (is_empty() == 0) {
				op = pop();
				if (op == ')') break;
				else
				{
					Stringadd(temp, op);
					Stringadd(temp, ' ');
				}
			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//연산자의 경우 연산 우선순위에 따라 팝시켜서 추가 이후 공백추가
			while (is_empty() == 0) {
				op = peek();
				if (precedence(c) <= precedence(op)) {
					Stringadd(temp, op);
					Stringadd(temp, ' ');
					pop();
				}
				else break;
			}
			push(c);
		}
	}
	while (is_empty() == 0)//이후 나머지 전부 pop하여 추가 이후 공백추가
	{
		Stringadd(temp, pop());
		Stringadd(temp, ' ');
	}
	reverseString(temp);//문장 뒤집기
}

double calc_prefix(char temp[])//전위 연산자 계산 함수
{
	reverseString(temp);
	char c;
	int i = 0;
	double val, val1, val2;

	init_stack();
	while (temp[i] != '\0') {//문장의 끝까지 반복
		c = temp[i++];//한 문자를 받아옴
		if (c >= '0' && c <= '9') {//숫자인 경우
			val = c - '0';
			while (temp[i] >= '0' && temp[i] <= '9')//다음문자도 숫자인 경우 자릿수를 높여 계산
			{
				c = temp[i++] - '0';
				val = c * 10 + val;
			}
			push(val);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//연산자의 경우 앞의 두 수를 pop한 뒤 연산하여 push
			val1 = pop();
			val2 = pop();
			switch (c) {
			case '+':push(val1 + val2); break;
			case '-':push(val1 - val2); break;
			case '*':push(val1 * val2); break;
			case '/':push(val1 / val2); break;
			case '^':push((Element)pow(val1, val2)); break;
			}
		}
	}
	return pop();
}

void main()
{
	char expr[2][80] = { "5-8*20+4^2/3","(15*10+3^(5-2))/((20+12)*3-25/5)" };
	char temp[2][80] = { NULL,NULL };
	char temp1[2][80] = { NULL,NULL };

	printf("\n\n예시 1번 문제\n\n");
	printf("중위수식: %s\n", expr[0]);
	infix_to_postfix(expr[0], temp[0]);
	printf("후위수식: %s\n", temp[0]);
	printf("후위수식 계산 : %f\n", calc_postfix(temp[0]));
	infix_to_prefix(expr[0], temp1[0]);
	printf("전위수식: %s\n", temp1[0]);
	printf("전위수식 계산 : %f", calc_prefix(temp1[0]));

	printf("\n\n예시 2번 문제\n\n");
	printf("중위수식: %s\n", expr[1]);
	infix_to_postfix(expr[1], temp[1]);
	printf("후위수식: %s\n", temp[1]);
	printf("후위수식 계산 : %f\n", calc_postfix(temp[1]));
	infix_to_prefix(expr[1], temp1[1]);
	printf("전위수식: %s\n", temp1[1]);
	printf("전위수식 계산 : %f", calc_prefix(temp1[1]));
}