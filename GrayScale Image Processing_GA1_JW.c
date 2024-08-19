#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h> // GUI 지원 기능
#include <math.h>

/////////////////////
// 전역 변수부
/////////////////////
HWND hwnd; // 윈도 화면 (외부, 물리적)
HDC hdc; // 윈도 화면 (내부,논리적)

FILE* rfp, * wfp;
char fileName[200];
int inH, inW, outH, outW;
//double PI = 3.141592;
#define PI 3.141594

// 메모리 확보
unsigned char** inImage, ** outImage, **accImage;
double** tempImage;

/////////////////////
// 함수 선언부
/////////////////////
void loadImage(); int getIntValue(); void printImage(); void printMenu(); double getDoubleValue();
void saveImage(); void freeInputMemory(); void freeOutputMemory(); void freeTempMemory();
void mallocInputMemory(); void mallocOutputMemory(); void mallocTempMemory(); 
double** mallocDoubleMemory(int, int); void freeDoubleMemory(double**, int);

void equalImage(); void addImage(); void subImage(); void reverseImage(); void bwImage();
void bwAvgImage(); void bwMedImage(); int compare(const unsigned char*, const unsigned char*);
void gammaCorrectImage(); void paraImage();
void mulImage(); void divImage();
void andImage(); void orImage(); void xorImage(); void stressImage(); void postImage();
void zoomOut(); void zoomIn(); void zoomIn2();
void rotate();  void rotate2(); void move();
void mirrorHor(); void mirrorVer(); void zoomOutAvg(); void zoomOutMed();
void rotate3(); void bilinear(); void histoStretch(); void endIn(); 
void histoEqual(); void emboss(); void blurr(); void sharpen(); void hpfSharp(); void blurr2();
void morphing(); void rotateBilinear();
void edge1(); void sobel();  void laplacian(); void homogen(); void subEdge(); void gauss();
void accum();

void main() {
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL); // Windows 10 : hwnd
	char inKey = 0, inKey2 = 0;

	while (inKey != '9') {
		printMenu();
		inKey = _getch(); 
		printf("%c", inKey);
		inKey2 = _getch();
		printf("%c", inKey2);
//		system("cls");

		switch (inKey)

		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '2': accum(); break;
		case '9': break;
		case 'a': case 'A': if(inKey2 == '0') equalImage(); break;
		case 'b': case 'B': if (inKey2 == '0') addImage();
							else if(inKey2 == '1')  subImage(); break;
		case 'c': case 'C': if(inKey2 == '0') reverseImage(); break;
		case 'd': case 'D': if(inKey2 == '0') bwImage(); 
							else if (inKey2 == '1') bwAvgImage(); 
							else if (inKey2 == '2') bwMedImage(); break;
		case 'e': case 'E': if (inKey2 == '0') gammaCorrectImage();
							else if (inKey2 == '1') paraImage(); 
							break;
		case 'f': case 'F': if (inKey2 == '0') mulImage(); 
							else if (inKey2 == '1') divImage(); 
							else if (inKey2 == '2') andImage(); // 128
							else if (inKey2 == '3') orImage();
							else if (inKey2 == '4') xorImage();
							break;
		case 'g': case 'G': if (inKey2 == '0') stressImage();
							else if (inKey2 == '1') postImage(); break;
		case 'h': case 'H': if (inKey2 == '0') zoomOut(); 
							else if (inKey2 == '1') zoomOutAvg(); 
							else if (inKey2 == '2') zoomOutMed(); 
							break;
		case 'i': case 'I': if (inKey2 == '0') zoomIn(); 
							else if (inKey2 == '1') zoomIn2();
							else if (inKey2 == '2') bilinear();
							break;
		case 'j': case 'J': if (inKey2 == '0') rotate(); 
							else if (inKey2 == '1') rotate2();
							else if (inKey2 == '2') rotate3();
							else if (inKey2 == '3') rotateBilinear();
							break;
		case 'k': case 'K': if (inKey2 == '0') move();
							else if (inKey2 == '1') mirrorHor(); 
							else if (inKey2 == '2') mirrorVer(); 
							else if (inKey2 == '3') morphing();
							break;
		case 'l': case 'L': if (inKey2 == '0') histoStretch();
							else if (inKey2 == '1') endIn();
							else if (inKey2 == '2') histoEqual();
							break;
		case 'm': case 'M': if (inKey2 == '0') emboss();
							else if (inKey2 == '1') blurr(); 
							else if (inKey2 == '2' ) blurr2();
							else if (inKey2 == '3') sharpen();
							else if (inKey2 == '4') hpfSharp();
							break;
		case 'n': case 'N': if (inKey2 == '0') edge1();
							else if (inKey2 == '1') homogen();
							else if (inKey2 == '2') subEdge();
							else if (inKey2 == '3') sobel();
							else if (inKey2 == '4') laplacian();
							else if (inKey2 == '5') gauss();
							break;
		case 'o': case 'O': break;
		case 'p': case 'P': break;
		case 'q': case 'Q': break;
		case 'r': case 'R': break;
		case 's': case 'S': break;
		case 't': case 'T': break;
		case 'u': case 'U': break;
		case 'v': case 'V': break;
		case 'w': case 'W': break;
		case 'x': case 'X': break;
		case 'y': case 'Y': break;
		case 'z': case 'Z': break;
		default: break;

		}
	}
	freeInputMemory();
	freeOutputMemory();
	freeTempMemory();

}
/////////////////////
// 함수 정의부
/////////////////////
/// 공통 함수
//////////////

void printMenu() {
	puts("\n ## GrayScale Image Processing (GA1) ##");
	puts(" =======  파일 ========================================================================");
	puts(" 00. 열기 11. 저장 22. 효과누적 99. 종료");
	puts("\n ======= 영상처리 < 화소점 + 기하학 처리 > ==========================================");
	puts(" A0.동일  B0.밝게   B1.어둡게  C0.반전   D0.흑백  D1.흑백(평균값) D2.흑백(중앙값)");
	puts(" E0.감마  E1.파라볼라 F0.곱셈  F1.나눗셈 F2.AND   F3.OR    F4.XOR  G0.강조 ");
	puts(" G1.포스터라이징      H0.축소  H1.축소(평균값)    H2.축소(중앙값)");
	puts(" I0.확대(포워딩)      I1.확대(백워딩)             I2.확대(양선형 보간)");
	puts(" J0.회전              J1.회전(중앙,백워딩)        J2.회전(확대)    J3.회전(확대,양선형)");
	puts(" K0.이동              K1.좌우대칭                 K2.상하대칭      K3.모핑");
	puts("\n ======= 영상처리 < 히스토그램 + 영역 처리 + 경계선 검출 > ==========================");
	puts(" L0.히스토그램 스트레칭    L1.앤드-인       L2.평활화   ");
	puts(" M0.엠보싱   M1.블러링     M2.블러링(9x9)   M3.샤프닝   M4.고주파샤프닝   ");   
	puts(" N0.경계선1  N1.에지검출(유사연산)  N2.에지검출(차연산)  ");
	puts(" N3.미분회선(소벨)  N4.라플라시안   N5.DoG(가우시안)");
	puts(" ======================================================================================");
	printf(" 메뉴를 입력하세요 : ");
	//	puts(" ");
}

void printImage() {
	system("cls");
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 50, i + 250, RGB(px, px, px));
		}
	}
}

void loadImage() {
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf(", 파일명 --> "); // flower512, LENA256 
	scanf("%s", &tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (중요!) 이미지의 폭과 높이를 결정
	rfp = fopen(fileName, "rb");
	if (rfp == NULL) {
		MessageBox(hwnd, L"존재하지 않는 파일입니다.", L"정보 창", NULL);
		printf(" 존재하지 않는 파일입니다 \n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(rfp);

	// 메모리 해제
	freeInputMemory();

	// 높이, 폭 구하기
	inH = inW = (int)sqrt(fsize);

	// 메모리 할당
	mallocInputMemory();

	// 파일 --> 메모리
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	// printf("%d ", inImage[100][100]);
	equalImage();
}

void saveImage() {
	if (outImage == NULL) // 저장할 내용 없음을 noti
		return;
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf(" 파일명-->"); // out01, out02......
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	wfp = fopen(fileName, "wb");
	// 메모리 --> 파일 (한행씩)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);
	MessageBox(hwnd, L"저장 완료", L"저장 창", NULL);
	printf("저장 완료");
}


int getIntValue() {
	int retValue;
	printf(" 정수값--> "); scanf("%d", &retValue);
	return retValue;
}

double getDoubleValue() {
	double retValue;
	printf(" 실수값--> "); scanf("%lf", &retValue);
	return retValue;
}

void freeInputMemory() {
	if (inImage == NULL)
		return;
	for (int i = 0; i < inH; i++)
		free(inImage[i]);
	free(inImage);
	inImage = NULL;
}

void mallocInputMemory() {
	inImage = (unsigned char**)malloc(sizeof(unsigned char*) * inH);
	for (int i = 0; i < inH; i++)
		inImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * inW);
}

void freeOutputMemory() {
	if (outImage == NULL)
		return;
	for (int i = 0; i < outH; i++)
		free(outImage[i]);
	free(outImage);
	outImage = NULL;
}

void mallocOutputMemory() {
	outImage = (unsigned char**)malloc(sizeof(unsigned char*) * outH);

	for (int i = 0; i < outH; i++)
		outImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * outW);

}

void freeAccMemory() {
	if (accImage == NULL)
		return;
	for (int i = 0; i < inH; i++)
		free(accImage[i]);
	free(accImage);
	accImage = NULL;
}

void mallocAccMemory() {
	accImage = (unsigned char**)malloc(sizeof(unsigned char*) * outH);

	for (int i = 0; i < outH; i++)
		accImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * outW);

}

void freeTempMemory() {
	if (tempImage == NULL)
		return;
	for (int i = 0; i < inH; i++)
		free(tempImage[i]);
	free(tempImage);
	tempImage = NULL;
}

void mallocTempMemory() {
	tempImage = (double**)malloc(sizeof(double*) * inH);
	for (int i = 0; i < inH; i++)
		tempImage[i] = (double*)malloc(sizeof(double) * inW);
}

double** mallocDoubleMemory(int h, int w) {
	double** retMemory;

	retMemory = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++)
		retMemory[i] = (double*)malloc(sizeof(double) * w);
	return retMemory;
}

void freeDoubleMemory(double** memory, int h) {
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
	memory = NULL;
}

//////////////////
//  영상처리 함수
////////////////////
void equalImage() { // 동일영상 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}
void addImage() { // 더하기 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	int val = getIntValue();
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (inImage[i][k] + val < 255)
				outImage[i][k] = inImage[i][k] + val;
			else
				outImage[i][k] = 255;
		}
	}
	printImage();
}

void subImage() { // 어둡게
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	int val = getIntValue();
	//	printf("sub val = %d", val);
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] - val > 0)
				outImage[i][k] = inImage[i][k] - val;
			else
				outImage[i][k] = 0;
		}
	}
	printImage();
}

void reverseImage() { // 반전 알고리즘 0->255, 1->254 ..... 255->0
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}

void bwImage() { // 흑백 알고리즘 (128기준)
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (inImage[i][k] < 128) {
				outImage[i][k] = 0;
			}
			else {
				outImage[i][k] = 255;
			}
		}
	}
	printImage();
}

void bwAvgImage() { // 흑백(평균값)
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int sum = 0, avg = 0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			sum += inImage[i][k];
		}
	}
	avg = sum / (outW * outH);

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (inImage[i][k] < avg) {
				outImage[i][k] = 0;
			}
			else {
				outImage[i][k] = 255;
			}
		}
	}
	printImage();
}


void bwMedImage() { // 흑백(중앙값)

	int median = 0, number = 0;
	//	unsigned char array[height * width];
	unsigned char* array = NULL;
	array = (unsigned char*)malloc(sizeof(unsigned char) * inH * inW);
	if (array == NULL)
		return;

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			array[number] = inImage[i][k];
			number++;
		}
	}
	qsort(array, number, sizeof(unsigned char), compare);
	median = array[inH * inW / 2];
	//	printf("===  흑백(중앙값) %d =================\n", median);
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (inImage[i][k] < median) {
				outImage[i][k] = 0;
			}
			else {
				outImage[i][k] = 255;
			}
		}
	}
	printImage();
	free(array);
}

int compare(const unsigned char* a, const unsigned char* b) {
	if (*a > *b)
		return 1;
	else
		return -1;
}

void gammaCorrectImage() {

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	double gamma_val;
	printf("감마값(0.2~1.8) --> "); scanf("%lf", &gamma_val);
	double temp;
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			temp = inImage[i][k];
			outImage[i][k] = (unsigned char)255.0 * pow(temp / 255.0, gamma_val);
			//temp = pow((double)inImage[i][k], 1 / gamma_val);
			//if (temp < 0)
			//	outImage[i][k] = 0;
			//else if (temp > 0)
			//	outImage[i][k] = 255;
			//else
			//	outImage[i][k] = (unsigned char)temp;
		}
	}
	printImage();
}

void paraImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	//	printf("===  파라볼라 =================\n");
	//	printf("     밝은 곳 입체형 (CAP)\n");
	double temp;
	//	for (int i = 0; i < height; i++) {
	//		for (int k = 0; k < width; k++) {
	//			temp = inImage[i][k];;
	////		double val = 255.0 - 255.0 * pow((inImage[i][k] / 128.0 - 1.0), 2);
	//			if (val > 255.0) val = 255.0;
	//			else if (val < 0.0) val = 0.0;
	//			outImage[i][k] = (unsigned char)val;
	//		}
	//	}
	//	printImage();

	//	printf("     어두운 곳 입체형 (CUP)\n");
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			temp = inImage[i][k];
			double val = 255.0 * pow((temp / 128.0 - 1.0), 2);
			if (val > 255.0) val = 255.0;
			else if (val < 0.0) val = 0.0;
			outImage[i][k] = (unsigned char)val;
		}
	}
	printImage();
}
/*
void paraCapImage() { // 파라볼라 - 밝은 곳 입체형 (CAP)
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	double temp;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			temp = inImage[i][k];;
			double val = 255.0 - 255.0 * pow((inImage[i][k] / 128.0 - 1.0), 2);
			if (val > 255.0) val = 255.0;
			else if (val < 0.0) val = 0.0;
			outImage[i][k] = (unsigned char)val;
		}
	}
	printImage();
}

void paraCupImage() { // 파라볼라 - 밝은 곳 입체형 (CAP)
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	double temp;
	//	printf("     어두운 곳 입체형 (CUP)\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			temp = inImage[i][k];
			double val = 255.0 * pow((temp / 128.0 - 1.0), 2);
			if (val > 255.0) val = 255.0;
			else if (val < 0.0) val = 0.0;
			outImage[i][k] = (unsigned char)val;
		}
	}
	printImage();
}*/
void mulImage() { // 화소 값 상수 곱셈
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	double val;
	while(1) {
		printf(" 0.1에서 3사이의 실수값--> "); scanf("%lf", &val);
		if (val >= 0.1 && val <= 3.0) break;
	}

	// 입력 배열 --> 출력배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < outW; k++) {
			if (inImage[i][k] * val > 255)
				outImage[i][k] = 255;
			else if (inImage[i][k] * val < 0)
				outImage[i][k] = 0;
			else
				outImage[i][k] = (unsigned char)(inImage[i][k] * val);
		}
	}
	printImage();
}

void divImage() { // 화소 값 상수 나눗셈
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	double val;
	while (1) {
		printf(" 0.2에서 10사이의 실수값--> "); scanf("%lf", &val);
		if (val >= 0.1 && val <= 3.0) break;
	}

	// 입력 배열 --> 출력배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] / val > 255)
				outImage[i][k] = 255;
			else if (inImage[i][k] / val < 0)
				outImage[i][k] = 0;
			else
				outImage[i][k] = (unsigned char)(inImage[i][k] / val);
		}
	}
	printImage();
}

void andImage() { // AND Operation
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int val = getIntValue();

	// 입력 배열 --> 출력배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if ((inImage[i][k] & (unsigned char)val) >= 255)
				outImage[i][k] = 255;
			else if ((inImage[i][k] & (unsigned char)val) < 0)
				outImage[i][k] = 0;
			else
				outImage[i][k] = (unsigned char)(inImage[i][k] & (unsigned char)val);
		}
	}
	printImage();
}

void orImage() {  // OR Operation
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int val = getIntValue();
	// 입력 배열 --> 출력배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if ((inImage[i][k] | (unsigned char)val) >= 255)
				outImage[i][k] = 255;
			else if ((inImage[i][k] | (unsigned char)val) < 0)
				outImage[i][k] = 0;
			else
				outImage[i][k] = (unsigned char)(inImage[i][k] | (unsigned char)val);
		}
	}
	printImage();
}

void xorImage() {  // XOR Operation
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int val = getIntValue();
	// 입력 배열 --> 출력배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if ((inImage[i][k] ^ (unsigned char)val) >= 255)
				outImage[i][k] = 255;
			else if ((inImage[i][k] ^ (unsigned char)val) < 0)
				outImage[i][k] = 0;
			else
				outImage[i][k] = (unsigned char)(inImage[i][k] ^ (unsigned char)val);
		}
	}
	printImage();
}

void stressImage() { // Stress Operation
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int start, end;
	while (1) {
		printf(", 0부터 255까지 두 정수값--> ");
		scanf("%d%d", &start, &end);
		printf("===  Stress %d, %d =================\n", start, end);

		if (start > end) {
			int temp = start;
			start = end;
			end = temp;
		}

		if (start != end && start >= 0 && end <= 255)
			break;
	}
	// 입력 배열 --> 출력배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] >= start && inImage[i][k] <= end)
				outImage[i][k] = 255;
			else
				outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

void postImage() { // 포스터라이징
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int val = getIntValue();
	int interval = 255 / val; // 값이 제일 낮은 영역 0으로 할당
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < interval)
				outImage[i][k] = 0;
		}
	}
	for (int i = 0; i < inH; i++) { // 값이 제일 큰 영역 255로 할당
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] > interval * (val - 1))
				outImage[i][k] = 255;
		}
	}
	for (int i = 1; i < val - 1; i++) { // 나머지 영역 입력값에 비례하여 할당
		for (int j = 0; j < inH; j++) {
			for (int k = 0; k < inW; k++) {
				if (inImage[j][k] > interval * i && inImage[j][k] < interval * (i + 1))
					outImage[j][k] = interval * (i + 1);
			}
		}
	}
	printImage();
}

void zoomOut() { // 축소 알고리즘
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열, 3축소 오류, 1.5배 축소 고민해서 해보자.
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}

void zoomIn() { // 확대 알고리즘
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i * scale)][(int)(k * scale)] = inImage[i][k];
			outImage[(int)(i * scale + 1)][(int)(k * scale + 1)] = inImage[i][k];
		}
	}
	printImage();
}

void zoomIn2() { // 확대 알고리즘
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = inImage[(int)(i / scale)][(int)k / scale];
		}
	}
	printImage();
}

void rotate() { // 회전 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int degree;
	while (1) {
		printf(" 각도 입력 --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	double radian = degree * 3.141592 / 180.0;

	// xd = cos*xs - sin*ys;
	// yd = sin*xs + cos*ys;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			int xs = i;
			int ys = k;
			int xd = (int)(cos(radian) * xs - sin(radian) * ys);
			int yd = (int)(sin(radian) * xs + cos(radian) * ys);

			if ((0 <= xd && xd < outH) && (0 <= yd && yd < outW))
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}

void rotate2() { // 회전 알고리즘 + 중앙/백워딩
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int degree;
	while (1) {
		printf(" 각도 입력 --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	//	double radian = -degree * 3.141592 / 180.0;
	double radian = degree * 3.141592 / 180.0;
	// xd = cos*xs - sin*ys;
	// yd = sin*xs + cos*ys;


	int cx = inH / 2;
	int cy = inW / 2;
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cy));
			int ys = (int)(-sin(radian) * (xd - cx) + cos(radian) * (yd - cy));
			xs += cx;
			ys += cy;

			if ((0 <= xs && xs < outH) && (0 <= ys && ys < outW))
				outImage[xd][yd] = inImage[xs][ys];
			else
				outImage[xd][yd] = 0;
		}
	}
	printImage();
}

void move() { // 동일영상 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	int moveX = 0, moveY = 0;

	printf(" 움직일 X좌료의 정수값 --> "); scanf("%d", &moveX);
	printf(" 움직일 Y좌표의 정수값 --> "); scanf("%d", &moveY);

	// 입력 배열 --> 출력 배열
	int x = 0, y = 0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			x = k - moveX;
			y = i - moveY;
			if (x >= 0 && x < inW && y >= 0 && y < inH)
				outImage[i][k] = inImage[y][x];
			else
				outImage[i][k] = 0; // 배경색 검정
		}
	}

	printImage();
}

void mirrorHor() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][outW - 1 - k];
		}
	}
	printImage();
}

void mirrorVer() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[inH - 1 - i][k];
		}
	}
	printImage();
}

void zoomOutAvg() {
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열, 3축소 오류, 1.5배 축소 고민해서 해보자.
	int sum, avg;
	for (int i = 0; i < inH; i += scale) {
		for (int k = 0; k < inW; k += scale) {
			sum = 0, avg = 0;

			for (int x = 0; x < scale; x++) {
				for (int y = 0; y < scale; y++) {
					sum += inImage[i + x][k + y];
				}
			}
			avg = (int)(sum / (scale * scale));
			outImage[(int)(i / scale)][(int)(k / scale)] = avg;
		}
	}
	printImage();
}

void zoomOutMed() {
	int scale = getIntValue();
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당
	mallocOutputMemory();

	unsigned char* array = NULL;
	array = (unsigned char*)malloc(sizeof(unsigned char) * scale * scale);
	if (array == NULL)
		return;

	// 입력 배열 --> 출력 배열, 3축소 오류, 1.5배 축소 고민해서 해보자.

	int med = 0, number = 0;
	for (int i = 0; i < inH; i += scale) {
		for (int k = 0; k < inW; k += scale) {

			number = 0;
			for (int x = 0; x < scale; x++) {
				for (int y = 0; y < scale; y++) {
					array[number] = inImage[i + x][k + y];
					number++;
				}
			}
			qsort(array, number, sizeof(unsigned char), compare);
			med = array[scale * scale / 2];

			outImage[(int)(i / scale)][(int)(k / scale)] = med;
		}
	}
	printImage();
	free(array);
}
void rotate3() { // 회전 알고리즘 + 중앙/백워딩 + 출력크기 재조정
	// 메모리 해제
	freeOutputMemory();

	int degree;
	while (1) {
		printf(" 각도 입력 --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// 메모리 할당
	mallocOutputMemory();

	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;


	int cx = inH / 2;
	int cy = inW / 2;
	int centerX = outH / 2;
	int centerY = outW / 2;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int xs = (int)(cos(radian) * (xd - centerX) + sin(radian) * (yd - centerY));
			int ys = (int)(-sin(radian) * (xd - centerX) + cos(radian) * (yd - centerY));
			xs += cx;
			ys += cy;

			if ((0 <= xs && xs < inH) && (0 <= ys && ys < inW))
				outImage[xd][yd] = inImage[xs][ys];
			else
				outImage[xd][yd] = 0;
		}
	}
	printImage();
}

void bilinear() { // 확대(양선형 보간)

	int point, i_H, i_W;
	unsigned char newValue;
	double ZoomRate = 2.0, r_H, r_W, s_H, s_W; // TODO 수정 필요
	double C1, C2, C3, C4;

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	ZoomRate = getDoubleValue();

	outH = (int)(inH * ZoomRate);
	outW = (int)(inW * ZoomRate);
	// 메모리 할당
	mallocOutputMemory();

	freeTempMemory();
	mallocTempMemory();
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tempImage[i][k] = (double)inImage[i][k];
		}
	}

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			r_H = i / ZoomRate;
			r_W = k / ZoomRate;

			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);

			s_H = r_H - i_H;
			s_W = r_W - i_W;

			if (i_H < 0 || i_H >= (inH - 1) || i_W < 0 || i_W >= (inW - 1)) {
				outImage[i][k] = 255;
			}
			else { // 소수점 값 보간하기
				C1 = (double)tempImage[i_H][i_W];
				C2 = (double)tempImage[i_H][i_W + 1];
				C3 = (double)tempImage[i_H + 1][i_W + 1];
				C4 = (double)tempImage[i_H + 1][i_W];

				newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				outImage[i][k] = newValue;
			}

		}
	}
	printImage();

}

void rotateBilinear() { // 회전 (확대+양선형)

	// 메모리 해제
	freeOutputMemory();

	int degree;
	while (1) {
		printf(" 각도 입력 --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// 메모리 할당
	mallocOutputMemory();

	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;

	int point, i_H, i_W;
	unsigned char newValue;
	double ZoomRate, r_H, r_W, s_H, s_W; // TODO 수정 필요
	double C1, C2, C3, C4;
	ZoomRate = (double)outH / (double)inH;

	freeTempMemory();
	mallocTempMemory();
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tempImage[i][k] = (double)inImage[i][k];
		}
	}

	int cx = inH / 2;
	int cy = inW / 2;
	int centerX = outH / 2;
	int centerY = outW / 2;


	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int xs = (int)(cos(radian) * (xd - centerX) + sin(radian) * (yd - centerY));
			int ys = (int)(-sin(radian) * (xd - centerX) + cos(radian) * (yd - centerY));
			xs += cx;
			ys += cy;

			r_H = i / ZoomRate;
			r_W = k / ZoomRate;

			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);

			s_H = r_H - i_H;
			s_W = r_W - i_W;

			if ((0 <= xs && xs < inH) && (0 <= ys && ys < inW)) {
				C1 = (double)tempImage[xs][ys];
				C2 = (double)tempImage[xs][ys + 1];
				C3 = (double)tempImage[xs + 1][ys + 1];
				C4 = (double)tempImage[xs + 1][ys];
				newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				outImage[xd][yd] = newValue;
//				outImage[xd][yd] = inImage[xs][ys];
			}
			else
				outImage[xd][yd] = 0;
		}
	}
/*
	int point, i_H, i_W;
	unsigned char newValue;
	double ZoomRate = 2.0, r_H, r_W, s_H, s_W; // TODO 수정 필요
	double C1, C2, C3, C4;

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	ZoomRate = getDoubleValue();

	outH = (int)(inH * ZoomRate);
	outW = (int)(inW * ZoomRate);

	int degree;
	while (1) {
		printf(" 각도 입력 --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	// 메모리 할당
	mallocOutputMemory();

	freeTempMemory();
	mallocTempMemory();
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tempImage[i][k] = (double)inImage[i][k];
		}
	}

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			r_H = i / ZoomRate;
			r_W = k / ZoomRate;

			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);

			s_H = r_H - i_H;
			s_W = r_W - i_W;

			if (i_H < 0 || i_H >= (inH - 1) || i_W < 0 || i_W >= (inW - 1)) {
				outImage[i][k] = 255;
			}
			else { // 소수점 값 보간하기
				C1 = (double)tempImage[i_H][i_W];
				C2 = (double)tempImage[i_H][i_W + 1];
				C3 = (double)tempImage[i_H + 1][i_W + 1];
				C4 = (double)tempImage[i_H + 1][i_W];

				newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				outImage[i][k] = newValue;
			}

		}
	}*/
/*	// 메모리 해제
	freeOutputMemory();

	int degree = getIntValue();
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// 메모리 할당
	mallocOutputMemory();


	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;

	int cx = inH / 2;
	int cy = inW / 2;
	int centerX = outH / 2;
	int centerY = outW / 2;

	double** tmpInImage = mallocDoubleMemory(outH, outW);

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int xs = (int)(cos(radian) * (xd - centerX) + sin(radian) * (yd - centerY));
			int ys = (int)(-sin(radian) * (xd - centerX) + cos(radian) * (yd - centerY));
			xs += cx;
			ys += cy;

			if ((0 <= xs && xs < inH) && (0 <= ys && ys < inW))
				tmpInImage[xd][yd] = inImage[xs][ys];
			else
				tmpInImage[xd][yd] = 0;
		}
	}

	// 보간법
	unsigned char newValue;
	double ZoomRate = 1.0, r_H, r_W, s_H, s_W;
	int point, i_H, i_W;
	double C1, C2, C3, C4;
	
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			r_H = i / ZoomRate;
			r_W = k / ZoomRate;

			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);

			s_H = r_H - i_H;
			s_W = r_W - i_W;

			if (i_H < 0 || i_H >= (outH - 1) || i_W < 0 || i_W >= (outW - 1)) {
				outImage[i][k] = 255;
			}
			else { // 소수점 값 보간하기
				C1 = (double)tmpInImage[i_H][i_W];
				C2 = (double)tmpInImage[i_H][i_W + 1];
				C3 = (double)tmpInImage[i_H + 1][i_W + 1];
				C4 = (double)tmpInImage[i_H + 1][i_W];

				newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				outImage[i][k] = newValue;
			}

		}
	}
	freeDoubleMemory(tmpInImage, outH);

	printImage();
*/
}
/*
void rotateBilinear() { // 회전 (확대+양선형) 
	// 메모리 해제
	freeOutputMemory();

	int degree = getIntValue();
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// 메모리 할당
	mallocOutputMemory();


	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;

	int cx = inH / 2;
	int cy = inW / 2;
	int centerX = outH / 2;
	int centerY = outW / 2;

	double** tmpInImage = mallocDoubleMemory(outH, outW);

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int xs = (int)(cos(radian) * (xd - centerX) + sin(radian) * (yd - centerY));
			int ys = (int)(-sin(radian) * (xd - centerX) + cos(radian) * (yd - centerY));
			xs += cx;
			ys += cy;

			if ((0 <= xs && xs < inH) && (0 <= ys && ys < inW))
				tmpInImage[xd][yd] = inImage[xs][ys];
			else
				tmpInImage[xd][yd] = 0;
		}
	}

	// 보간법
	unsigned char newValue;
	double ZoomRate = 1.0, r_H, r_W, s_H, s_W;
	int point, i_H, i_W;
	double C1, C2, C3, C4;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			r_H = i / ZoomRate;
			r_W = k / ZoomRate;

			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);

			s_H = r_H - i_H;
			s_W = r_W - i_W;

			if (i_H < 0 || i_H >= (outH - 1) || i_W < 0 || i_W >= (outW - 1)) {
				outImage[i][k] = 255;
			}
			else { // 소수점 값 보간하기
				C1 = (double)tmpInImage[i_H][i_W];
				C2 = (double)tmpInImage[i_H][i_W + 1];
				C3 = (double)tmpInImage[i_H + 1][i_W + 1];
				C4 = (double)tmpInImage[i_H + 1][i_W];

				newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				outImage[i][k] = newValue;
			}

		}
	}
	freeDoubleMemory(tmpInImage, outH);

	printImage();
}*/

void histoStretch() { // 히스토그램 스트레칭 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 진짜 영상처리 알고리즘
	// new = (old - low) / (high - low) * 255.0
	int high=inImage[0][0], low = inImage[0][0];

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] > high)
				high = inImage[i][k];
			if (inImage[i][k] < low)
				low = inImage[i][k];
		}
	}
	int old, new;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)((double)(old - low) / (double)(high - low) * 255.0);
			if (new > 255)
				new = 255;
			else if (new < 0)
				new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

void endIn() { // 엔드인 탐색 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 진짜 영상처리 알고리즘
	// new = (old - low) / (high - low) * 255.0
	int high = inImage[0][0], low = inImage[0][0];

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] > high)
				high = inImage[i][k];
			if (inImage[i][k] < low)
				low = inImage[i][k];
		}
	}
	high -= 50;
	low += 50;

	int old, new;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)((double)(old - low) / (double)(high - low) * 255.0);
			if (new > 255)
				new = 255;
			else if (new < 0)
				new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

void histoEqual() { // 히스토그램 평활화 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 1단계 : 빈도수 세기 (=히스토그램
	int histo[256] = { 0, };
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			histo[inImage[i][k]]++;

	// 2단계 : 누적히스토그램 생성
	int sumHisto[256] = { 0, };
	sumHisto[0] = histo[0];
	for (int i = 0; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];
	
	// 3단계 : 정규화된 히스토그램 생성 normalHisto = sumHisto * (1.0/(inH*inW)) * 255.0
	double normalHisto[256] = { 1.0, };
	for (int i = 0; i < 256; i++) {
		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0;
	}

	// 4단계 : inImage를 정규화된 값으로 치환
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char) normalHisto[inImage[i][k]];
		}
	}

	printImage();
}

void emboss() { // 화소영역 처리: 엠보싱 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////
	double mask[3][3] = { {-1.0, 0.0, 0.0}, // 엠보싱 마스크
						  {0.0,  0.0, 0.0},
						  {0.0,  0.0, 1.0} };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH+2, inW+2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH; i++) 
		for (int k = 0; k < inW; k++) 
			tmpInImage[i][k] = 127;
		
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++) 
		for (int k = 0; k < inW; k++) 
			tmpInImage[i+1][k+1] = inImage[i][k];
		
	// ** 회선 연산 ** 
	double S; 
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++) 
		for (int k = 0; k < outW; k++) 
			tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);
	printImage();
}

void blurr() { // 화소영역 처리: 블러링 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////
	double mask[3][3] = { {1./9, 1./9, 1./9}, // 블러링 마스크
						  {1. / 9, 1. / 9, 1. / 9},
						  {1. / 9, 1. / 9, 1. / 9} };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void blurr2() { // 화소영역 처리: 블러링 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////
	double mask[9][9] = {{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 }, // 블러링 마스크
					    {1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 }};

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 8, inW + 8);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH+8; i++)
		for (int k = 0; k < inW+8; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 4][k + 4] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 9; m++)
				for (int n = 0; n < 9; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 8);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void sharpen() { // 화소영역 처리: 샤프닝 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////
	double mask[3][3] = { {-1., -1., -1.}, // 샤프닝 마스크
						  {-1., 9., -1.},
						  {-1., -1., -1.} };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void hpfSharp() { // 화소영역 처리: 고주파 샤프닝 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////
	double mask[3][3] = { {-1./9., -1./9., -1./9.}, // 샤프닝 마스크
						  {-1./9., 8./9., -1./9.},
						  {-1./9., -1./9., -1./9.} };
//	double mask[3][3] = { {1. / 9., 1. / 9., 1. / 9.}, // 샤프닝 마스크
////						  {-1./9., 1.-(-1/9.)*8.0, -1./9.},
//						{1. / 9., 1. / 9., -1. / 9.},
//						{-1. / 9., -1. / 9., -1. / 9.} };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] += 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			tmpOutImage[i][k] += tmpInImage[i][k];
//			tmpOutImage[i][k] += 127.0;
		}
	}

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void morphing() {
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("모핑할 파일명 --> ");  
	scanf("%s", &tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (중요!) 이미지의 폭과 높이를 결정
	rfp = fopen(fileName, "rb");
	if (rfp == NULL) {
		MessageBox(hwnd, L"존재하지 않는 파일입니다.", L"정보 창", NULL);
		printf(" 존재하지 않는 파일입니다 \n");
		return;
	}

	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(rfp);

	int morRate = 0;
	while(1) {
		printf(" 1부터 9사이의 정수값--> "); scanf("%d", &morRate);
		if (morRate >= 1 && morRate <= 9)
			break;
	}
	unsigned char** morImage = NULL;
	int morphH, morphW, inX=0, inY = 0, morX = 0, morY = 0;

	// 높이, 폭 구하기
	morphH = morphW = (int)sqrt(fsize);
	printf(" morphH = %d \n", morphH);
	// 메모리 할당
	morImage = (unsigned char**)malloc(sizeof(unsigned char*) * morphH);
	if (morImage == NULL) return;
	for (int i = 0; i < morphH; i++)
		morImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * morphW);

	freeOutputMemory();
	if (morphH <= inH && morphW <= inW) {
		outH = inH;
		outW = inW;
		morX = (int)((inH - morphH) / 2.);
		morY = (int)((inW - morphW) / 2.);
	}
	else { // inImage가 작으면 
		outH = morphH;
		outW = morphW;
		inX = (int)((morphH - inH) / 2.);
		inY = (int)((morphW - inW) / 2.);
	}
//	printf(" OutH = %d, inX = %d, morX = %d\n", outH,inX,morX );
	// 메모리 할당
	mallocOutputMemory();

	// 파일 --> 메모리
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < morphH; i++)
		fread(morImage[i], sizeof(unsigned char), morphW, rfp);
	fclose(rfp);

	// 영상처리 -> 어두워지는 문제를 해결할것 
	if (morphH <= inH) { // inImage가 더 크거나 같음
		for (int i = 0; i < outH; i++) {
			for (int k = 0; k < outW; k++) {
				if (i >= morX && k >= morY && i < morX + morphH && k < morY + morphW)
					outImage[i][k] = (unsigned char)((inImage[i][k] * (10. - morRate) / 10.));
				else
					outImage[i][k] = inImage[i][k];
			}
		}
		for (int i = 0; i < morphH; i++) {
			for (int k = 0; k < morphW; k++) {
				outImage[i + morX][k + morY] += (unsigned char)((morImage[i][k] * (morRate) / 10.));
			}
		}
	}
	else { // inImage가 작음
		for (int i = 0; i < outH; i++) {
			for (int k = 0; k < outW; k++) {
				if (i >= inX && k >= inY && i < inX + inH && k < inY + inW)
					outImage[i][k] = (unsigned char)((morImage[i][k] * (morRate) / 10.));
				else
					outImage[i][k] = morImage[i][k];
			}
		}
		for (int i = 0; i < inH; i++) {
			for (int k = 0; k < inW; k++) {
				outImage[i + inX][k + inY] += (unsigned char)((inImage[i][k] * (10-morRate) / 10.));
			}
		}
	}

	// 메모리 해제 
	for (int i = 0; i < morphH; i++)
		free(morImage[i]);
	free(morImage);

	printImage();
}

void edge1() { // 경계선검출: 수직 에지 검출 마스크
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////

	//double mask[3][3] = { {0.0, 0.0, 0.0}, // 수직 에지 검출 마스크
	//					  {-1.0, 1.0, 0.0},
	//					  {0.0,  0.0, 0.0} };
	double mask[3][3] = { {0.0, -1.0, 0.0}, // 수평 에지 검출 마스크
					  {0.0, 1.0, 0.0},
					  {0.0,  0.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void sobel() { // 경계선검출: 1차 미분 회선 
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////

	double mask[3][3] = { {-1.0, -2.0, -1.0}, // 소벨 마스크
						  {0.0,  0.0, 0.0},
						  {1.0,  2.0, 1.0} };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void laplacian() { // 경계선검출: 라플라시안
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////

	double mask[3][3] = { {-1.0, -1.0, -1.0}, // 라플라시안 마스크
						  {-1.0,  8.0, -1.0},
						  {-1.0, -1.0, -1.0} };

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void homogen() { // 유사 연산자 에지 검출
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 임시 메모리 할당(실수형)
	double max, **tmpInImage, **tmpOutImage;
	tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	tmpOutImage = mallocDoubleMemory(inH, inW);

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = (double)inImage[i][k];
		}
	}

	int n, m;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			max = 0.0; // 블록이 이동할 때마다 최대값 초기화
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					if (fabs(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]) >= max)
						max = fabs(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]);
				}
			}
			tmpOutImage[i][k] = max;
		}
	}
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (tmpOutImage[i][k] > 255.)
				tmpOutImage[i][k] = 255.;
			if (tmpOutImage[i][k] < 0.)
				tmpOutImage[i][k] = 0.;
		}
	}
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}

	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);
	printImage();
}

void subEdge() { // 유사 연산자 에지 검출
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	// 임시 메모리 할당(실수형)
	double max, temp, ** tmpInImage, ** tmpOutImage;
	tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	tmpOutImage = mallocDoubleMemory(inH, inW);

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = (double)inImage[i][k];
		}
	}

	int n, m;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			max = 0.0; // 블록이 이동할 때마다 최대값 초기화
			temp = fabs(tmpInImage[i][k] - tmpInImage[i + 2][k + 2]);
			if (temp >= max) max = temp;
			temp = fabs(tmpInImage[i][k+1] - tmpInImage[i + 2][k + 1]);
			if (temp >= max) max = temp;
			temp = fabs(tmpInImage[i][k + 2] - tmpInImage[i + 2][k]);
			if (temp >= max) max = temp;
			temp = fabs(tmpInImage[i+1][k] - tmpInImage[i + 1][k+1]);
			if (temp >= max) max = temp;

			tmpOutImage[i][k] = max;
		}
	}
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (tmpOutImage[i][k] > 255.)
				tmpOutImage[i][k] = 255.;
			if (tmpOutImage[i][k] < 0.)
				tmpOutImage[i][k] = 0.;
		}
	}
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}

	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);
	printImage();
}

void gauss() { // 경계선검출: 가우시안
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당
	mallocOutputMemory();

	////////////////////
	// 화소 영역 처리
	////////////////////

	double mask[7][7] = { {0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0}, // 가우시안 마스크
						{0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0},
						{-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0},
						{-1.0, -3.0, 5.0, 16.0, 5.0, -3.0, -1.0},
						{-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0},
						{0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0},
						{0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0} };


	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 6, inW + 6);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값 
	for (int i = 0; i < inH+6; i++)
		for (int k = 0; k < inW+6; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 3][k + 3] = inImage[i][k];

	// ** 회선 연산 ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3)와 한점을 중심으로 한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 7; m++)
				for (int n = 0; n < 7; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상 --> 출력 영상. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 6);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}

void accum() { // 효과 누적하기: 현재의 outImage를 inImage로..
	// inW와 outW가 다르면 in을 다시 잡음.
	if (inW != outW) {
		freeInputMemory();
		inH = outH;
		inW = outW;
		mallocInputMemory();
	}
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			inImage[i][k] = outImage[i][k];
		}
	}
}
