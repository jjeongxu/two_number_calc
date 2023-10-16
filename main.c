// 3팀 | 팀장: 김정수, 팀원: 김주현, 김현성, 정창윤

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ANSI_COLOR_RESET   "\x1b[0m" // 기본 출력 색깔
#define ANSI_COLOR_GREEN "\x1b[32m" // 초록색 텍스트로 출력


char* delete_space(char *str) {  // 2. 입력받은 문자열에서 공백을 지워주는 함수
	char* ret = (char *)calloc(sizeof(char), 0x200); // 입력받은 문자열(init_str)에서 spacebar를 제거한 문자열을 저장해둘 공간을 할당받은 후, 그 주소를 ret에 저장함
	int k = 0;

	for (int i = 0; i < strlen(str); i++) { // init_str 문자열의 길이만큼 반복
		if (str[i] != ' ') { // ' '가 아니면 ret에 추가
			ret[k++] = str[i];
		}
	} // for문 끝

	return ret; // ret가 가리키고 있는 공간의 주소를 반환해주면서 함수 종료
}

char is_valid(char *str) { // formula(init_str에서 spacebar를 제거한 문자열)에 숫자와 연산자 외에 다른 문자가 있는지 검사 
	char flag = 0;
	char valid[] = "0123456789+-*/"; // 우리가 받을 수 있는 문자들의 집합
	int formula_char; // formula에서 문자를 하나씩 떼와서 strchr를 통해 검사한 결괏값을 저장해놓을 변수

	for (int i = 0; i < strlen(str); i++) {
		formula_char = strchr(valid, str[i]);

		if (formula_char == NULL) { // 우리가 받아야 하는 문자 외에 엉뚱한 문자가 있을 경우
			flag = 1; // flag를 1로 설정하고
			break; // for문을 종료시킴
		}
	}

	return flag; // flag값을 반환하고 함수 종료
}

char parser(char *formula, char *num1_str, char *num2_str, char *op) { // 3. formula를 받아서 num1, num2, op로 쪼개주는 함수 && 올바른 입력인지 검사
																											// ex. 입력이 "523"인 경우 or 입력이 "4+"인 경우 오류값을 처리하기 위해 1을 반환해줌
																											// 리턴값으로 '비정상적일 경우 1을 <-> 정상적일 경우 0'을 반환해줌

	char op_list[] = "+-*/"; // 입력 가능한 연산자 문자의 집합
	char num_list[] = "0123456789"; // 입력 가능한 숫자들의 집합

	if (strpbrk(formula, op_list) == 0) { // ex. 입력이 "524"일 경우
		return 1;
	}

	if (formula[0] == '-' && formula[1] == '-') { // num1의 다중 음수 예외 처리
		return 2;
	}

	if (formula[0] == '*' || formula[0] == '/') { // ex. *1025 - 100 일 경우 예외 처리
		return 1;
	}

	if ((strchr("+-*/", formula[0]) != NULL) && (formula[1] == '+' || formula[1] == '-' || formula[1] == '*' || formula[1] == '/')) { // ex. -+1025 - 100 일 경우 예외 처리
		return 1;
	}

	int operatorIndex = -1; // 연산자의 인덱스를 저장할 변수, 인덱스의 첫 주소가 0이므로 -1로 초기화
	int length = strlen(formula); // 입력 문자열의 길이를 구함
	// 연산자를 찾아서 해당 인덱스를 operatorIndex에 저장
	for (int i = 1; i < length; i++) { // 0->1 로 바꾸면서 첫번째 음수는 해결 

		if ((formula[i] == '+') || (formula[i] == '-') || (formula[i] == '*') || (formula[i] == '/')) {
			operatorIndex = i;// 위 예시면 i에 1 저장
			break; // 연산자를 찾으면 루프 종료
		}
	}

	if (operatorIndex != -1) {
		// 연산자를 기준으로 문자열 분리
		strncpy_s(num1_str, _msize(num1_str), formula, operatorIndex);
		//num1[operatorIndex] = '\0'; / strncpy 에서 strncpy_s 로 바뀜에 따라 null 수동저장을 안해도 됨 
		*op = formula[operatorIndex];
		//operatorStr[1] = '\0';
		strcpy_s(num2_str, _msize(num2_str), formula + operatorIndex + 1);
		// 분리된 문자열을 숫자 또는 연산자로 저장
	}

	if (num2_str[0] == '\0') {  // ex. 입력이 524*일 경우
		return 1;
	}

	if ((num2_str[0] != '-' && strchr(num_list, num2_str[0]) == NULL)) { // ex1. 입력이 104+*582일 경우 or ex2. 입력이 12 - --1824일 경우
		return 1;
	}

	if (num2_str[1] == '-') { // ex. 계산식이 "102 - --12"일 경우 다중 음수 오류 처리
		return 2;
	}

	if (strpbrk(num2_str, "+*/") != NULL) { // ex. 계산식이 "32 - -*46"일 경우 오류 처리하게 함
		return 1;
	}

	return 0; // 위의 모든 오류처리 검사를 통과했다면 num1_str, op, num2_str을 구분해낸 상태로, 0을 return 해주며 함수 종료
}

long long int calculator(long long int num1, char op, long long int num2) { // 4. 사칙연산을 수행하는 함수
	long long int res = 0; // res == 곱셈의 결과 or 나눗셈일 때는 덧셈의 결과를 임시적으로 저장하는 데 쓰임
	long long int count = 0; // 나눗셈의 몫으로 쓰임

	switch (op) { // switch-case문 시작
	case '+':
		return num1 + num2;

	case '-':
		num2 = ~num2;
		num2 += 1; // 2의 보수를 이용하여 양수인 num2의 부호를 음수로 변환

		return num1 + num2;

	case '*':
		if (num2 >= 0) { // num2가 양수일 경우
			if (num2 >= 1000000000) { // num2가 1,000,000,000 이상일 경우 루프문을 너무 많이 돌아서 프로그램이 멈추는 문제가 생김
				return num1 * num2;
			}
			for (int i = 0; i < num2; i++) {
				res = res + num1;
			}
		}
		else { // num2가 음수일 경우
			num2 = -num2;
			if (num2 >= 1000000000) { // num2가 1,000,000,000 이상일 경우 루프문을 너무 많이 돌아서 프로그램이 멈추는 문제가 생김
				return -(num1 * num2);
			}
			for (int i = 0; i < num2; i++) {
				res = res + num1;
			}
			res = -res;
		}

		return res;

	case '/':
		if (num1 > 0 && num2 > 0) { // (1) num1과 num2가 모두 양수일 경우
			if (num1 / num2 >= 1000000000) { // 루프 횟수가 10억을 넘어가면 루프를 너무 오래 도는 문제가 생겨서 10억을 넘게 돌아야할 경우 예외처리 해줌
				return num1 / num2;
			}
			res = num2;
			while (num1 >= res) {
				res = res + num2;
				count++;
			}
		} // (1) if문 끝
		else if (num1 < 0 && num2>0) { // (2) num1은 음수, num2는 양수일 경우
			if (num1 / num2 >= 1000000000) { // 루프 횟수가 10억을 넘어가면 루프를 너무 오래 도는 문제가 생겨서 10억을 넘게 돌아야할 경우 예외처리 해줌
				return num1 / num2;
			}
			res = num2;
			num1 = -num1;
			while (num1 >= res) {
				res = res + num2;
				count++;
			}
			count = -count;
		} // (2) else-if문 끝
		else if (num1 > 0 && num2 < 0) { // (3) num1은 양수, num2는 음수일 경우
			if (num1 / num2 >= 1000000000) { // 루프 횟수가 10억을 넘어가면 루프를 너무 오래 도는 문제가 생겨서 10억을 넘게 돌아야할 경우 예외처리 해줌
				return num1 / num2;
			}
			num2 = -num2;
			res = num2;
			while (num1 >= res) {
				res = res + num2;
				count++;
			}
			count = -count;
		} // (3) else-if문 끝
		else { // (4) num1, num2 모두 음수일 경우
			if (num1 / num2 >= 1000000000) { // 루프 횟수가 10억을 넘어가면 루프를 너무 오래 도는 문제가 생겨서 10억을 넘게 돌아야할 경우 예외처리 해줌
				return num1 / num2;
			}
			num1 = -num1;
			num2 = -num2;
			res = num2;
			while (num1 >= res) {
				res = res + num2;
				count++;
			}
		} // (4) else문 끝
		return count;

	} // switch-case문 끝
}

char* res_comma(long long int target) { // 숫자에 콤마(,)를 찍어줌
	char *temp = (char *)calloc(sizeof(char), 0x200); // 결과값을 문자열로 저장해둘 임시 저장소 ( ex. 1000000(백만) -> "1000000" )
	char *res = (char *)calloc(sizeof(char), 0x200); // 콤마를 붙인 최종 문자열을 담아둘 공간

	if (target >= 0) { // (1) target(==res 결괏값)가 양수거나 0인 경우
		int idx = 0;

		sprintf(temp, "%lld", target); // target == 1000000 -> temp == "1000000"
		char stdrd = strlen(temp) % 3; // ,를 붙일 위치의 기준값

		for (int i = 0; i < strlen(temp); i++) { // res에 콤마 붙이기 시작 // 자세한 매커니즘은 위의 stdrd가 어떻게 쓰이는지에 집중해서 for문 따라가보면 이해 될듯
			if (i != 0 && stdrd == i % 3) {
				res[idx++] = ',';
			}
			res[idx++] = temp[i];
		} // res에 콤마 붙이기 끝
	} // (1) target(== res 결괏값)가 양수거나 0인 경우 끝

	else { // (2) target(==res)가 음수일 경우
		int idx = 1;
		target = -target;

		sprintf(temp, "%lld", target);
		char stdrd = strlen(temp) % 3;

		for (int i = 0; i < strlen(temp); i++) {
			if (i != 0 && stdrd == i % 3) {
				res[idx++] = ',';
			}
			res[idx++] = temp[i];
		}

		res[0] = '-';
	} // (2) target(==res)가 음수일 경우 끝

	free(temp); // 임시로 쓰였던 문자열인 temp의 공간을 운영체제에게 돌려줌
	temp = NULL;
	return res; // 결괏값에 콤마를 붙인 문자열인 res를 리턴해주며 함수 종료
}

char* to_binary(long long int target) { // 5. 결과값을 2진수로 변환해주는 함수
	char *res = (char *)calloc(sizeof(char), 0x200); // 2진수로 변환한 결과를 담을 문자열
	char *temp = (char *)calloc(sizeof(char), 0x200); // 임시 저장소로 쓰임

	if (target > 0) { // (1) 계산의 결괏값이 양수인 경우
		while (target > 0) { // 2진수 변환 시작 ( sprintf(res, "...", res) 하면 값이 duplicated 되는 문제가 있어 추가로 변수 temp를 도입했음
			sprintf(res, "%lld%s", target % 2, temp);
			target = target / 2;

			strcpy(temp, res);
		} // 2진수 변환 끝 // 이것도 자세한 동작 과정 이해하기 위해선 걍 while문 일일이 따라가보는 수밖에 없음. 주석 한 줄로 표현하기엔 어려움이 있음.

		// 4개 단위로 끊기게 하기 시작
		if (strlen(res) % 4 == 1) {
			sprintf(res, "000%s", temp);
		}
		else if (strlen(res) % 4 == 2) {
			sprintf(res, "00%s", temp);
		}
		else if (strlen(res) % 4 == 3) {
			sprintf(res, "0%s", temp);
		}
		// 4개 단위로 끊기게 하기 끝
	}
	else { // (2) 계산의 결괏값이 음수인 경우
		res[0] = '1';
		for (int i = 1; i < 64; i++) { // res = "1000 0000 0000 .... 0000" ( 8byte )
			res[i] = '0';
		}

		char *binary_x = (char *)calloc(sizeof(char), 0x200);
		long long int x;
		x = (target + LLONG_MAX) + 1; // 결괏값 = LLONG_MIN + x  && LLONG_MIN = -(LLONG_MAX+1)
															// 따라서 x = 결괏값 + LLONG_MAX + 1


		while (x > 0) { // x의 2진수 변환 시작 ( sprintf(binary_x, "...", binary_x) 하면 값이 duplicated 되는 문제가 있어 추가로 변수 temp를 도입했음
			sprintf(binary_x, "%lld%s", x % 2, temp);
			x = x / 2;

			strcpy(temp, binary_x);
		} // 2진수 변환 끝 // 이것도 자세한 동작 과정 이해하기 위해선 걍 while문 일일이 따라가보는 수밖에 없음. 주석 한 줄로 표현하기엔 어려움이 있음.

		for (int i = 0; i < strlen(binary_x); i++) { // x를 2진수로 변환한 결괏값을 res의 뒷 부분에 덮어씌움
			res[63 - i] = binary_x[strlen(binary_x) - 1 - i];
		}

		free(binary_x); // binary_x가 할당 받았던 공간을 운영체제에게 반환
		binary_x = NULL;
	}

	free(temp); // 임시 저장소로 쓰였던 temp의 공간을 운영체제에게 돌려줌
	temp = NULL;
	return res;
}

char* to_hex(long long int target) { // 결괏값을 16진수로 변환해주는 함수
	char *res = (char *)calloc(sizeof(char), 0x200);
	char *temp = (char *)calloc(sizeof(char), 0x200);
	char hex[] = "0123456789abcdef";
	int i = 0;

	while (target > 0) {
		temp[i++] = hex[target % 16]; // temp에 16진수 변환값이 역으로 들어감
		target = target / 16;
	}

	i = 0; // i를 다시 0으로 초기화
	for (int j = strlen(temp) - 1; j >= 0; j--) { // temp의 값을 거꾸로 다시 res에 담아줌 -> res에는 정방향 변환값이 들어가게 됨
		res[i++] = temp[j];
	}

	free(temp);
	temp = NULL;
	return res;
}

char* to_oct(long long int target) { // 결괏값을 8진수로 변환해주는 함수
	char *res = (char *)calloc(sizeof(char), 0x200);
	char *temp = (char *)calloc(sizeof(char), 0x200);
	int i = 0;

	while (target > 0) {
		temp[i++] = '0' + target % 8; // temp에 8진수 변환값이 역으로 들어감
		target = target / 8;
	}

	i = 0; // i를 다시 0으로 초기화
	for (int j = strlen(temp) - 1; j >= 0; j--) { // temp의 값을 거꾸로 다시 res에 담아줌 -> res에는 정방향 변환값이 들어가게 됨
		res[i++] = temp[j];
	}

	free(temp);
	temp = NULL;
	return res;
}

char ouflow_check(long long int num1, char op, long long int num2, long long int res) { // Integer Overflow & Underflow 검사하는 함수
																																	// Overflow일 경우엔 1을, Underflow일 경우엔 2를, 정상적일 경우엔 0을 return

	if (num1 > 0 && num2 > 0 && (res <= 0 || res < num1) && (op == '+' || op == '*')) {
		return 1;
	}
	else if (num1 < 0 && num2 > 0 && (res >= 0 || -res < -num1) && (op == '-' || op == '*')) {
		return 2;
	}
	else if (num1 > 0 && num2 < 0 && (res >= 0 || -res < num1) && op == '*') {
		return 2;
	}
	else if (num1 > 0 && num2 < 0 && (res <= 0 || res < num1) && op == '-') {
		return 1;
	}
	else if (num1 < 0 && num2 < 0 && (res >= 0 || -res < -num1) && op == '+') {
		return 2;
	}
	else if (num1 < 0 && num2 < 0 && (res <= 0 || res < -num1) && op == '*') {
		return 1;
	}

	return 0;
}


int main() {
	char *init_str; // 최초의 계산식 문자열을 입력받을 공간
	char *formula; // init_str에서 spacebar를 제거한 문자열
	char *num1_str, *num2_str; // num1, num2를 문자열로 저장할 공간
	char *temp_num1_str, *temp_num2_str; // num1 or num2가 음수일 경우 '-'를 붙여주기 위한 sprintf를 위해 임시로 쓰일 num1_str, num2_str
	char *binary; // 결괏값의 이진수 변환값을 담을 문자열
	char op; // 연산자
	char parser_flag; // parser의 return 값을 저장해둘 플래그 변수 0 == 정상, 1 == 계산식이 완벽하지 않음, 2 == 다중 음수
	long long int num1, num2, res, remainder; // num1_str, num2_str을 정수형으로 변환 && res는 계산의 결과값 && remainder == 나눗셈 계산시 나머지
	char *res_str; // res값을 10진수로 출력할 때 콤마(,)를 넣어주기 위해, res(결괏값)를 문자열로 저장할 변수
	char *res_hex_str; // res값을 16진수로 변환한 문자열을 저장하는 함수
	char *res_oct_str; // res값을 8진수로 변환한 문자열을 저장하는 함수
	char *dummy_pos = NULL; // strtoll 때 걍 인자 채우기 용으로 쓰임. 아무 의미 없는 변수
	char ouflow_flag; // res의 Overflow and Underflow 검사 결과 값 저장하기 위해 쓰이는 플래그

	printf("ooooooooooooo                                         .oooo.          .oooooo.             oooo            \n");
	printf("8'   888   `8                                       .dP\"\"Y88b        d8P'  `Y8b            `888            \n");
	printf("     888       .ooooo.   .oooo.   ooo. .oo.  .oo.         ]8P'      888           .oooo.    888   .ooooo.  \n");
	printf("     888      d88' `88b `P  )88b  `888P\"Y88bP\"Y88b      <88b.       888          `P  )88b   888  d88' `\"Y8 \n");
	printf("     888      888ooo888  .oP\"888   888   888   888       `88b.      888           .oP\"888   888  888       \n");
	printf("     888      888    .o d8(  888   888   888   888  o.   .88P       `88b    ooo  d8(  888   888  888   .o8 \n");
	printf("    o888o     `Y8bod8P' `Y888\"\"8o o888o o888o o888o `8bd88P'         `Y8bood8P'  `Y888\"\"8o o888o `Y8bod8P' \n\n\n");

	printf("#################################################### [이용 방법] ####################################################\n");
	printf("$$$$$$$$$$$$$$$$$ 이 계산기는 두 개의 피연산자를 계산하는 계산기입니다. 다항식은 지원하지 않습니다. $$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$                          +, -, *, / 연산만이 지원됩니다.                          $$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$ '/'(나눗셈) 연산시 첫 번째 줄에 몫이 출력되고, 두 번째 줄에 나머지가 출력됩니다.  $$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$        입력 예시: ex1) 2867 * -18295 || ex2) 10 - -100 || ex3) 582965/289         $$$$$$$$$$$$$$$$$\n");
	printf("$$$$$$$$$$$$$$$$$        프로그램을 종료하시려면 \"exit\" 또는 \"quit\" 또는 \"q\"을 입력해주세요         $$$$$$$$$$$$$$$$$\n");
	printf("########################## Credit: 김정수(팀장), 김주현(팀원), 김현성(팀원), 정창윤(팀원) ###########################\n\n\n");

	while (1) { // 무한 반복문 시작
		init_str = (char *)calloc(sizeof(char), 0x200); // init_str에 문자열을 저장하기 위해 동적으로 메모리를 할당받음

		printf("계산식을 입력하세요 >> ");
		fgets(init_str, _msize(init_str), stdin); // 계산식을, init_str이 할당받은 공간의 크기만큼 입력받음

		if (init_str[0] == 0) { // 'Ctrl+C'를 입력받았을 경우
			free(init_str);
			init_str = NULL;

			exit(0); // 프로그램 종료
		}

		if (init_str[0] != '\n') { // 정상적인 입력인 경우
			init_str[strlen(init_str) - 1] = '\0';  // fgets로 입력받은 문자열의 마지막 원소는 '\n'이기 때문에, '\n'만 입력받은 게 아닌 경우라면 '\n'을 '\0'으로 바꿔줌
		}
		else { // 아무 것도 입력하지 않고 \n만 입력했을 경우
			printf("입력값이 없습니다.\n계산식을 다시 입력해주세요.\n\n");

			free(init_str); // init_str이 calloc()을 통해 할당받았던 동적 메모리 공간을 운영체제에게 돌려줌
			init_str = NULL;

			continue; // 반복문( while(1) )의 처음으로 복귀
		}

		if ((strcmp(init_str, "exit") == 0) || (strcmp(init_str, "quit") == 0) || (strcmp(init_str, "q") == 0)) { // "exit" 또는 "quit" 또는 'q'을 입력한 경우
			free(init_str); // init_str이 calloc()을 통해 할당받았던 동적 메모리 공간을 운영체제에게 돌려줌
			init_str = NULL;

			break; // 반복문( while(1) ) 밖으로 빠져나감
		}

		formula = delete_space(init_str); // 입력받은 계산식인 init_str에서 delete_space()함수를 통해 spacebar를 제거한 뒤, 이 문자열이 있는 메모리의 주소를 formula에 저장

		if (is_valid(formula)) { // 숫자, 연산자 이외의 문자가 있는지 is_valid()함수를 통해 검사함
			printf("정수형 숫자와 연산자(+, -, *, /)만 입력받을 수 있습니다.\n계산식을 다시 입력해주세요.\n\n");

			free(init_str);  // init_str가 할당받은 공간을 운영체제에게 돌려줌
			init_str = NULL;
			free(formula);  // formula가  할당받은 공간을 운영체제에게 돌려줌
			formula = NULL;

			continue; // 반복문( while(1) )의 시작으로 복귀
		}

		num1_str = (char *)calloc(sizeof(char), 0x100); // num1을 문자열로 담아둘 공간을 동적으로 할당받음
		num2_str = (char *)calloc(sizeof(char), 0x100); // num2를 문자열로 담아둘 공간을 동적으로 할당받음

		parser_flag = parser(formula, num1_str, num2_str, &op); // parser()함수의 역할 (1), 입력받은 계산식에서 spacebar를 제거한 문자열(formula)에서 두 개의 피연산항과 연산자를 구분
																					// parser()함수의 역할 (2), 입력이 올바른지 검사함( ex. "104" or "104/"일 경우 1을 return 하여 if문이 실행됨 )
																							// parser()함수의 역할 (3), 입력이 올바른지 검사함( ex. "--104" or "104- --10"일 경우 2을 return 하여 else-if문이 실행됨 )
		if (parser_flag == 1) {  // 계산식이 완벽하지 않을 경우 or 다항식일 경우
			printf("입력하신 계산식이 완벽하지 않거나, 다항식입니다.\n계산식을 다시 입력해주세요.\n\n");

			free(init_str); // 처음에 입력받은 계산식(init_str)의 공간을 운영체제게에 돌려줌
			init_str = NULL;
			free(formula); // init_str에서 spacebar를 제거한 문자열(formula)의 공간을 운영체제에게 돌려줌
			formula = NULL;
			free(num1_str); // formula에서 구분해낸 num1_str 문자열의 공간을 운영체제에게 돌려줌
			num1_str = NULL;
			free(num2_str); // formula에서 구분해낸 num2_str 문자열의 공간을 운영체제에게 돌려줌
			num2_str = NULL;

			continue; // 반복문( while(1) )의 처음으로 복귀
		}
		else if (parser_flag == 2) { // 다중 음수인 경우
			printf("피연산자의 다중 음수 처리는 지원되지 않습니다.\n계산식을 다시 입력해주세요.\n\n");

			free(init_str); // 처음에 입력받은 계산식(init_str)의 공간을 운영체제게에 돌려줌
			init_str = NULL;
			free(formula); // init_str에서 spacebar를 제거한 문자열(formula)의 공간을 운영체제에게 돌려줌
			formula = NULL;
			free(num1_str); // formula에서 구분해낸 num1_str 문자열의 공간을 운영체제에게 돌려줌
			num1_str = NULL;
			free(num2_str); // formula에서 구분해낸 num2_str 문자열의 공간을 운영체제에게 돌려줌
			num2_str = NULL;

			continue; // 반복문( while(1) )의 처음으로 복귀
		}

		num1 = strtoll(num1_str, &dummy_pos, 10);  // num1_str을 정수형(long long int)로 변환
		num2 = strtoll(num2_str, &dummy_pos, 10);  // num2_str을 정수형(long long int)로 변환

		if (op == '/' && num2 == 0) { // 0으로 나눌 경우 예외 처리 시작
			printf("어떤 수를 0으로 나눌 수 없습니다.\n");
			printf("계산식을 다시 입력해주세요.\n\n\n");

			free(init_str); // 처음에 입력받은 계산식(init_str)의 공간을 운영체제게에 돌려줌
			init_str = NULL;
			free(formula); // init_str에서 spacebar를 제거한 문자열(formula)의 공간을 운영체제에게 돌려줌
			formula = NULL;
			free(num1_str); // formula에서 구분해낸 num1_str 문자열의 공간을 운영체제에게 돌려줌
			num1_str = NULL;
			free(num2_str); // formula에서 구분해낸 num2_str 문자열의 공간을 운영체제에게 돌려줌
			num2_str = NULL;

			continue; // while(1)의 처음으로 복귀
		} // 0으로 나눌 경우 예외 처리 끝

		res = calculator(num1, op, num2); // calculator()함수를 통해 num1 op num2의 연산 결과를 구한 뒤 결괏값을 res에 저장

		ouflow_flag = ouflow_check(num1, op, num2, res); // Overflow일 경우엔 1을, Underflow일 경우엔 2를, 정상일 경우엔 0을 반환
		if (ouflow_flag == 1) { // res Overflow
			printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("!!!!!!!!!!!!!!!!!!!>>>> Integer Overflow <<<<!!!!!!!!!!!!!!!!!!!\n");
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("\n\n################################################################\n");
			printf("################## Restarting the Process... ###################\n");
			printf("################################################################\n\n\n");

			free(init_str); // 처음에 입력받은 계산식(init_str)의 공간을 운영체제게에 돌려줌
			init_str = NULL;
			free(formula); // init_str에서 spacebar를 제거한 문자열(formula)의 공간을 운영체제에게 돌려줌
			formula = NULL;
			free(num1_str); // formula에서 구분해낸 num1_str 문자열의 공간을 운영체제에게 돌려줌
			num1_str = NULL;
			free(num2_str); // formula에서 구분해낸 num2_str 문자열의 공간을 운영체제에게 돌려줌
			num2_str = NULL;

			continue;
		}
		else if (ouflow_flag == 2) { // res Underflow
			printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("!!!!!!!!!!!!!!!!!!!>>>> Integer Underflow <<<<!!!!!!!!!!!!!!!!!!\n");
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("\n\n################################################################\n");
			printf("################## Restarting the Process... ###################\n");
			printf("################################################################\n\n\n");

			free(init_str); // 처음에 입력받은 계산식(init_str)의 공간을 운영체제게에 돌려줌
			init_str = NULL;
			free(formula); // init_str에서 spacebar를 제거한 문자열(formula)의 공간을 운영체제에게 돌려줌
			formula = NULL;
			free(num1_str); // formula에서 구분해낸 num1_str 문자열의 공간을 운영체제에게 돌려줌
			num1_str = NULL;
			free(num2_str); // formula에서 구분해낸 num2_str 문자열의 공간을 운영체제에게 돌려줌
			num2_str = NULL;

			continue;
		}

		res_str = res_comma(res); // res_comma()함수를 통해 결괏값에 보기 예쁘게 ,를 붙인 후 이 문자열이 있는 공간의 주소를 res_str에 저장 ( ex. 1000000 -> 1,000,000 )
		temp_num1_str = res_comma(num1); // num1도 보기 좋게 콤마(,) 찍어줌
		temp_num2_str = res_comma(num2); // num2도 보기 좋게 콤마(,) 찍어줌
		strcpy(num1_str, temp_num1_str);
		strcpy(num2_str, temp_num2_str);

		if (num1 < 0) { // num1이 음수일 경우 보기 좋게 괄호를 씌워줌
			sprintf(num1_str, "(%s)", temp_num1_str);
		}
		if (num2 < 0) { // num2가 음수일 경우 보기 좋게 괄호를 씌워줌
			sprintf(num2_str, "(%s)", temp_num2_str);
		}

		free(temp_num1_str);
		temp_num1_str = NULL;
		free(temp_num2_str);
		temp_num2_str = NULL;

		printf("##### 계산 결과 #####\n"); // 계산 결과 출력 시작

		if (res > 0) { // res가 양수일 경우 16진수, 8진수, 2진수 출력
			binary = to_binary(res); // to_binary() 함수를 통해 결괏값(res)를 2진수로 변환한 뒤, 이 문자열이 있는 공간의 주소를 binary에 저장
			res_hex_str = to_hex(res); // to_hex() 함수를 통해 결괏값(res)를 16진수로 변환한 뒤, 이 문자열이 있는 공간의 주소를 res_hex_str에 저장
			res_oct_str = to_oct(res); // // to_oct() 함수를 통해 결괏값(res)를 8진수로 변환한 뒤, 이 문자열이 있는 공간의 주소를 res_oct_str에 저장

			printf("%s %c %s = %s [ 16진수: 0x%s | ", num1_str, op, num2_str, res_str, res_hex_str);
			printf("8진수: 0%s | 2진수: ", res_oct_str); // printf의 인수로 6개 이상을 주면 값이 씹혀서 8진수는 따로 적었음. 인자로 들어가는 레지스터 문제인듯
			for (int count = 0; count < strlen(binary); count++) { // 2진수 출력 시작
				if ((count % 4 == 0) && (count != 0)) { // 2진수 문자열의 원소를 4개씩 출력할 때마다 ' '를 출력해줌. 다만, 첫 번째 원소인 경우라면 ' '를 출력하지 않음.
					printf(" ");
				} // 2진수 출력 끝
				printf("%c", binary[count]);
			}
			printf(" ]");

			free(binary); // 결괏값을 2진수로 변환한 문자열을 담고 있는 binary의 공간을 운영체제에게 돌려줌
			binary = NULL;
			free(res_hex_str); // 결괏값을 16진수로 변환한 문자열을 담고 있는 res_hex_str의 공간을 운영체제에게 돌려줌
			res_hex_str = NULL;
			free(res_oct_str); // 결괏값을 8진수로 변환한 문자열을 담고 있는 res_oct_str의 공간을 운영체제에게 돌려줌
			res_oct_str = NULL;
		} // res가 양수일 경우 끝

		else if (res == 0) { // 결괏값이 0일 경우 시작
			printf("%s %c %s = 0 [ 16진수: 0 | 8진수: 0 | 2진수: 0 ]", num1_str, op, num2_str);
		} // 결괏값이 0일 경우 끝

		else { // 결괏값(res)이 음수일 경우 시작
			binary = to_binary(res);
			printf("%s %c %s = %s [ 2진수: ", num1_str, op, num2_str, res_str);
			for (int count = 0; count < strlen(binary); count++) { // 2진수 출력 시작
				if ((count % 4 == 0) && (count != 0)) { // 2진수 문자열의 원소를 4개씩 출력할 때마다 ' '를 출력해줌. 다만, 첫 번째 원소인 경우라면 ' '를 출력하지 않음.
					printf(" ");
				}
				printf("%c", binary[count]);
			} // 2진수 출력 끝
			printf(" || 음수 결괏값은 부호비트 때문에 16진수와 8진수는 지원되지 않습니다. ]");

			free(binary);
			binary = NULL;
		} // 결괏값이 음수일 경우 끝

		if (op == '/') { // 나눗셈 연산시 나머지도 계산 시작
			char *remainder_str; // 나눗셈 연산시 나머지를 문자열로 저장하는 함수

			remainder = num1 - (num2*res); // 나머지 값 계산
			remainder_str = res_comma(remainder);

			if (remainder > 0) { // (1) 나머지가 양수일 경우 시작
				char *remainder_hex_str = to_hex(remainder); // 나눗셈 연산시 나머지를 16진수로 변환한 문자열을 저장하는 함수
				char *remainder_oct_str = to_oct(remainder); // 나눗셈 연산시 나머지를 8진수로 변환한 문자열을 저장하는 함수
				char *remainder_binary = to_binary(remainder); // 나눗셈 연산시 나머지를 2진수로 변환한 문자열을 저장하는 함수

				printf("\n" ANSI_COLOR_GREEN "(++)=> " ANSI_COLOR_RESET "나머지 = %s [ 16진수: 0x%s | 8진수: 0%s | 2진수: ", remainder_str, remainder_hex_str, remainder_oct_str);
				for (int count = 0; count < strlen(remainder_binary); count++) { // 2진수 출력 시작
					if ((count % 4 == 0) && (count != 0)) { // 2진수 문자열의 원소를 4개씩 출력할 때마다 ' '를 출력해줌. 다만, 첫 번째 원소인 경우라면 ' '를 출력하지 않음.
						printf(" ");
					}// 2진수 출력 끝
					printf("%c", remainder_binary[count]);
				}
				printf(" ]");

				free(remainder_str);
				remainder_str = NULL;
				free(remainder_hex_str);
				remainder_hex_str = NULL;
				free(remainder_oct_str);
				remainder_oct_str = NULL;
				free(remainder_binary);
				remainder_binary = NULL;
			} // 나머지가 양수일 경우 끝
			else if (remainder == 0) { // (2) 나머지가 0일 경우 출력 시작
				printf("\n" ANSI_COLOR_GREEN "(++)=> " ANSI_COLOR_RESET "나머지 = %s [ 16진수: 0 | 8진수: 0 | 2진수: 0 ]", remainder_str);

				free(remainder_str);
				remainder_str = NULL;
			} // (2) 나머지가 0일 경우 출력 끝
			else { // (3) 나머지가 음수일 경우 출력 시작
				char *remainder_binary = to_binary(remainder); // 나눗셈 연산시 나머지를 2진수로 변환한 문자열을 저장하는 함수
				printf("\n" ANSI_COLOR_GREEN "(++)=> " ANSI_COLOR_RESET "나머지 = %s [ 2진수: ", remainder_str);
				for (int count = 0; count < strlen(remainder_binary); count++) { // 2진수 출력 시작
					if ((count % 4 == 0) && (count != 0)) { // 2진수 문자열의 원소를 4개씩 출력할 때마다 ' '를 출력해줌. 다만, 첫 번째 원소인 경우라면 ' '를 출력하지 않음.
						printf(" ");
					}
					printf("%c", remainder_binary[count]);
				} // 2진수 출력 끝
				printf(" || 음수 결괏값은 부호비트 때문에 16진수와 8진수는 지원되지 않습니다. ]");

				free(remainder_binary);
				remainder_binary = NULL;
				free(remainder_str);
				remainder_str = NULL;
			} // (3) 나머지가 음수일 경우 출력 끝
		} // 나눗셈 연산시 나머지도 계산 끝

		printf("\n\n"); // 계산 결과 출력 끝


		free(init_str);  // init_str 청크 free
		init_str = NULL;
		free(formula);  // formula 청크 free
		formula = NULL;
		free(num1_str); // num1_str 청크 free
		num1_str = NULL;
		free(num2_str); // num2_str 청크 free
		num2_str = NULL;
		free(res_str); // res_str 청크 free
		res_str = NULL;
	} // 무한 반복문 끝


	////////// 종료 문구 출력 시작
	printf("ooooooooo.                                                                                                \n");
	printf("`888   `Y88.                                                                                               \n");
	printf(" 888   .d88' oooo d8b  .ooooo.   .oooooooo oooo d8b  .oooo.   ooo. .oo.  .oo.                               \n");
	printf(" 888ooo88P'  `888\"\"8P d88' `88b 888' `88b  `888\"\"8P `P  )88b  `888P\"Y88bP\"Y88b                             \n");
	printf(" 888          888     888   888 888   888   888      .oP\"888   888   888   888                             \n");
	printf(" 888          888     888   888 `88bod8P'   888     d8(  888   888   888   888                             \n");
	printf("o888o        d888b    `Y8bod8P' `8oooooo.  d888b    `Y888\"\"8o o888o o888o o888o                            \n");
	printf("                                d\"     YD                                                                   \n");
	printf("                                \"Y88888P'                                                                   \n");

	printf("    .                                         o8o                            .                   .o8       \n");
	printf("  .o8                                         `\"'                          .o8                  \"888       \n");
	printf(".o888oo  .ooooo.  oooo d8b ooo. .oo.  .oo.   oooo  ooo. .oo.    .oooo.   .o888oo  .ooooo.   .oooo888       \n");
	printf("  888   d88' `88b `888\"\"8P `888P\"Y88bP\"Y88b  `888  `888P\"Y88b  `P  )88b    888   d88' `88b d88' `888       \n");
	printf("  888   888ooo888  888      888   888   888   888   888   888   .oP\"888    888   888ooo888 888   888       \n");
	printf("  888 . 888    .o  888      888   888   888   888   888   888  d8(  888    888 . 888    .o 888   888       \n");
	printf("  \"888\" `Y8bod8P' d888b    o888o o888o o888o o888o o888o o888o `Y888\"\"8o   \"888\" `Y8bod8P' `Y8bod88P\"      \n");

	printf("                                                           oooo  oooo                                      \n");
	printf("                                                           `888  `888                                      \n");
	printf("ooo. .oo.    .ooooo.  oooo d8b ooo. .oo.  .oo.    .oooo.    888   888  oooo    ooo                         \n");
	printf("`888P\"Y88b  d88' `88b `888\"\"8P `888P\"Y88bP\"Y88b  `P  )88b   888   888   `88.  .8'                          \n");
	printf(" 888   888  888   888  888      888   888   888   .oP\"888   888   888    `88..8'                           \n");
	printf(" 888   888  888   888  888      888   888   888  d8(  888   888   888     `888'                            \n");
	printf("o888o o888o `Y8bod8P' d888b    o888o o888o o888o `Y888\"\"8o o888o o888o     .8'                             \n");
	printf("                                                                       .o..P'                              \n");
	printf("                                                                       `Y8P'                               \n");
	///////////// 종료 문구 출력 끝

	return 0;
}
