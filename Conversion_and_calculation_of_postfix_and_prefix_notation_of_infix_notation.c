#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_STACK_SIZE 100 //�迭�� �ִ�ũ��
typedef double Element; //�迭�� Ÿ��

Element data[MAX_STACK_SIZE];//�迭 ����
int top;//top���� ����

void error(char str[])
{
	printf("%s\n", str);
	exit(1);
} //�����޼��� ����Լ�

void init_stack() { top = -1; } //�迭 �ʱ�ȭ
int is_empty() { return top == -1; } //���� ������� Ȯ��
int is_full() { return top == MAX_STACK_SIZE - 1; } //���� ��ȭ���� Ȯ��
int size() { return top + 1; }//������ ���� ũ�� Ȯ��

void push(Element e)
{
	if (is_full())
		error("���� ��ȭ ����");//��ȭ���¶�� ����
	data[++top] = e;//top�� 1������Ų �� �� ����
}

Element pop()
{
	if (is_empty())
		error("���� ���鿡�� ");//������¶�� ����
	return data[top--];//top�� �� ��ȯ �� top 1����
}

Element peek()
{
	if (is_empty())
		error("���� ���鿡��");//������¶�� ����
	return data[top];//top�� �ִ� �� ��ȯ
}

void Stringadd(char temp[], char c)
{
	int i = 0;
	while (temp[i] != NULL)temp[i++];
	temp[i] = c;
	temp[i + 1] = NULL;
}//�ι����� �������� �̵����� �ڿ� ���ڿ��� �߰��ϰ� ���� �ι��� �߰�

double calc_postfix(char temp[])//���� ������ ��� �Լ�
{
	char c;
	int i = 0;
	double val, val1, val2;

	init_stack();
	while (temp[i] != '\0') {//������ ������ �ݺ�
		c = temp[i++];//�� ���ڸ� �޾ƿ�
		if (c >= '0' && c <= '9') {//������ ���
			val = c - '0';
			while (temp[i] >= '0' && temp[i] <= '9')//�������ڵ� ������ ��� �ڸ����� ���� ���
			{
				c = temp[i++];
				val = val * 10 + c - '0';
			}
			push(val);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//�������� ��� ���� �� ���� pop�� �� �����Ͽ� push
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

int precedence(char op)//������ �켱����
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

void infix_to_postfix(char expr[], char temp[])//������������� ��ȯ
{
	int i = 0;
	char c, op;

	init_stack();
	while (expr[i] != '\0') {//������ ������
		c = expr[i++];
		if ((c >= '0' && c <= '9')) {//������ ��� �Ŀ� �߰�
			Stringadd(temp, c);
			if (!(expr[i] >= '0' && expr[i] <= '9')) {//������ ���ڰ� �ƴѰ�� ���� �߰�
				Stringadd(temp, ' ');
			}
		}
		else if (c == '(')//���°�ȣ�ΰ�� ��ȣ Ǫ��
			push(c);
		else if (c == ')') {//�ݴ� ��ȣ�� ��� ���°�ȣ������ ������ ���Ͽ� Ǫ�����ְ� �����߰�
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
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//�������� ��� ���� �켱������ ���� �˽��Ѽ� �߰� ���� �����߰�
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
	while (is_empty() == 0)//���� ������ ���� pop�Ͽ� �߰� ���� �����߰�
	{
		Stringadd(temp, pop());
		Stringadd(temp, ' ');
	}
}

void reverseString(char* s) {//������ �Ųٷ� ������ �ִ� �Լ�
	size_t size = strlen(s);
	char temp;

	for (size_t i = 0; i < size / 2; i++) {
		temp = s[i];
		s[i] = s[(size - 1) - i];
		s[(size - 1) - i] = temp;
	}
}

void infix_to_prefix(char expr[], char temp[])//������������� ��ȯ
{
	reverseString(expr);//������ ������
	int i = 0;
	char c, op;

	init_stack();
	while (expr[i] != '\0') {//������ ó������
		c = expr[i++];
		if ((c >= '0' && c <= '9')) {//������ ��� �Ŀ� �߰�
			Stringadd(temp, c);
			if (!(expr[i] >= '0' && expr[i] <= '9')) {//������ ���ڰ� �ƴѰ�� ���� �߰�
				Stringadd(temp, ' ');
			}
		}
		else if (c == ')')//�ݴ°�ȣ�ΰ�� ��ȣ Ǫ��
			push(c);
		else if (c == '(') {//���� ��ȣ�� ��� ���°�ȣ������ ������ ���Ͽ� Ǫ�����ְ� �����߰�
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
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//�������� ��� ���� �켱������ ���� �˽��Ѽ� �߰� ���� �����߰�
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
	while (is_empty() == 0)//���� ������ ���� pop�Ͽ� �߰� ���� �����߰�
	{
		Stringadd(temp, pop());
		Stringadd(temp, ' ');
	}
	reverseString(temp);//���� ������
}

double calc_prefix(char temp[])//���� ������ ��� �Լ�
{
	reverseString(temp);
	char c;
	int i = 0;
	double val, val1, val2;

	init_stack();
	while (temp[i] != '\0') {//������ ������ �ݺ�
		c = temp[i++];//�� ���ڸ� �޾ƿ�
		if (c >= '0' && c <= '9') {//������ ���
			val = c - '0';
			while (temp[i] >= '0' && temp[i] <= '9')//�������ڵ� ������ ��� �ڸ����� ���� ���
			{
				c = temp[i++] - '0';
				val = c * 10 + val;
			}
			push(val);
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {//�������� ��� ���� �� ���� pop�� �� �����Ͽ� push
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

	printf("\n\n���� 1�� ����\n\n");
	printf("��������: %s\n", expr[0]);
	infix_to_postfix(expr[0], temp[0]);
	printf("��������: %s\n", temp[0]);
	printf("�������� ��� : %f\n", calc_postfix(temp[0]));
	infix_to_prefix(expr[0], temp1[0]);
	printf("��������: %s\n", temp1[0]);
	printf("�������� ��� : %f", calc_prefix(temp1[0]));

	printf("\n\n���� 2�� ����\n\n");
	printf("��������: %s\n", expr[1]);
	infix_to_postfix(expr[1], temp[1]);
	printf("��������: %s\n", temp[1]);
	printf("�������� ��� : %f\n", calc_postfix(temp[1]));
	infix_to_prefix(expr[1], temp1[1]);
	printf("��������: %s\n", temp1[1]);
	printf("�������� ��� : %f", calc_prefix(temp1[1]));
}