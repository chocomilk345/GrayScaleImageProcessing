#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h> // GUI ���� ���
#include <math.h>

/////////////////////
// ���� ������
/////////////////////
HWND hwnd; // ���� ȭ�� (�ܺ�, ������)
HDC hdc; // ���� ȭ�� (����,����)

FILE* rfp, * wfp;
char fileName[200];
int inH, inW, outH, outW;
//double PI = 3.141592;
#define PI 3.141594

// �޸� Ȯ��
unsigned char** inImage, ** outImage, **accImage;
double** tempImage;

/////////////////////
// �Լ� �����
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
// �Լ� ���Ǻ�
/////////////////////
/// ���� �Լ�
//////////////

void printMenu() {
	puts("\n ## GrayScale Image Processing (GA1) ##");
	puts(" =======  ���� ========================================================================");
	puts(" 00. ���� 11. ���� 22. ȿ������ 99. ����");
	puts("\n ======= ����ó�� < ȭ���� + ������ ó�� > ==========================================");
	puts(" A0.����  B0.���   B1.��Ӱ�  C0.����   D0.���  D1.���(��հ�) D2.���(�߾Ӱ�)");
	puts(" E0.����  E1.�Ķ󺼶� F0.����  F1.������ F2.AND   F3.OR    F4.XOR  G0.���� ");
	puts(" G1.�����Ͷ���¡      H0.���  H1.���(��հ�)    H2.���(�߾Ӱ�)");
	puts(" I0.Ȯ��(������)      I1.Ȯ��(�����)             I2.Ȯ��(�缱�� ����)");
	puts(" J0.ȸ��              J1.ȸ��(�߾�,�����)        J2.ȸ��(Ȯ��)    J3.ȸ��(Ȯ��,�缱��)");
	puts(" K0.�̵�              K1.�¿��Ī                 K2.���ϴ�Ī      K3.����");
	puts("\n ======= ����ó�� < ������׷� + ���� ó�� + ��輱 ���� > ==========================");
	puts(" L0.������׷� ��Ʈ��Ī    L1.�ص�-��       L2.��Ȱȭ   ");
	puts(" M0.������   M1.����     M2.����(9x9)   M3.������   M4.�����Ļ�����   ");   
	puts(" N0.��輱1  N1.��������(���翬��)  N2.��������(������)  ");
	puts(" N3.�̺�ȸ��(�Һ�)  N4.���ö�þ�   N5.DoG(����þ�)");
	puts(" ======================================================================================");
	printf(" �޴��� �Է��ϼ��� : ");
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
	printf(", ���ϸ� --> "); // flower512, LENA256 
	scanf("%s", &tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (�߿�!) �̹����� ���� ���̸� ����
	rfp = fopen(fileName, "rb");
	if (rfp == NULL) {
		MessageBox(hwnd, L"�������� �ʴ� �����Դϴ�.", L"���� â", NULL);
		printf(" �������� �ʴ� �����Դϴ� \n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); // ������ ������ �̵�
	long long fsize = ftell(rfp); // �� ���������?  262,144  --> 512
	fclose(rfp);

	// �޸� ����
	freeInputMemory();

	// ����, �� ���ϱ�
	inH = inW = (int)sqrt(fsize);

	// �޸� �Ҵ�
	mallocInputMemory();

	// ���� --> �޸�
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	// printf("%d ", inImage[100][100]);
	equalImage();
}

void saveImage() {
	if (outImage == NULL) // ������ ���� ������ noti
		return;
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf(" ���ϸ�-->"); // out01, out02......
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	wfp = fopen(fileName, "wb");
	// �޸� --> ���� (���྿)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);
	MessageBox(hwnd, L"���� �Ϸ�", L"���� â", NULL);
	printf("���� �Ϸ�");
}


int getIntValue() {
	int retValue;
	printf(" ������--> "); scanf("%d", &retValue);
	return retValue;
}

double getDoubleValue() {
	double retValue;
	printf(" �Ǽ���--> "); scanf("%lf", &retValue);
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
//  ����ó�� �Լ�
////////////////////
void equalImage() { // ���Ͽ��� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}
void addImage() { // ���ϱ� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
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

void subImage() { // ��Ӱ�
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
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

void reverseImage() { // ���� �˰��� 0->255, 1->254 ..... 255->0
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}

void bwImage() { // ��� �˰��� (128����)
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
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

void bwAvgImage() { // ���(��հ�)
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
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


void bwMedImage() { // ���(�߾Ӱ�)

	int median = 0, number = 0;
	//	unsigned char array[height * width];
	unsigned char* array = NULL;
	array = (unsigned char*)malloc(sizeof(unsigned char) * inH * inW);
	if (array == NULL)
		return;

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			array[number] = inImage[i][k];
			number++;
		}
	}
	qsort(array, number, sizeof(unsigned char), compare);
	median = array[inH * inW / 2];
	//	printf("===  ���(�߾Ӱ�) %d =================\n", median);
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

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	double gamma_val;
	printf("������(0.2~1.8) --> "); scanf("%lf", &gamma_val);
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
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	//	printf("===  �Ķ󺼶� =================\n");
	//	printf("     ���� �� ��ü�� (CAP)\n");
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

	//	printf("     ��ο� �� ��ü�� (CUP)\n");
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
void paraCapImage() { // �Ķ󺼶� - ���� �� ��ü�� (CAP)
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
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

void paraCupImage() { // �Ķ󺼶� - ���� �� ��ü�� (CAP)
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	double temp;
	//	printf("     ��ο� �� ��ü�� (CUP)\n");
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
void mulImage() { // ȭ�� �� ��� ����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	double val;
	while(1) {
		printf(" 0.1���� 3������ �Ǽ���--> "); scanf("%lf", &val);
		if (val >= 0.1 && val <= 3.0) break;
	}

	// �Է� �迭 --> ��¹迭
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

void divImage() { // ȭ�� �� ��� ������
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	double val;
	while (1) {
		printf(" 0.2���� 10������ �Ǽ���--> "); scanf("%lf", &val);
		if (val >= 0.1 && val <= 3.0) break;
	}

	// �Է� �迭 --> ��¹迭
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
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int val = getIntValue();

	// �Է� �迭 --> ��¹迭
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
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int val = getIntValue();
	// �Է� �迭 --> ��¹迭
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
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int val = getIntValue();
	// �Է� �迭 --> ��¹迭
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
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int start, end;
	while (1) {
		printf(", 0���� 255���� �� ������--> ");
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
	// �Է� �迭 --> ��¹迭
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

void postImage() { // �����Ͷ���¡
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int val = getIntValue();
	int interval = 255 / val; // ���� ���� ���� ���� 0���� �Ҵ�
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < interval)
				outImage[i][k] = 0;
		}
	}
	for (int i = 0; i < inH; i++) { // ���� ���� ū ���� 255�� �Ҵ�
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] > interval * (val - 1))
				outImage[i][k] = 255;
		}
	}
	for (int i = 1; i < val - 1; i++) { // ������ ���� �Է°��� ����Ͽ� �Ҵ�
		for (int j = 0; j < inH; j++) {
			for (int k = 0; k < inW; k++) {
				if (inImage[j][k] > interval * i && inImage[j][k] < interval * (i + 1))
					outImage[j][k] = interval * (i + 1);
			}
		}
	}
	printImage();
}

void zoomOut() { // ��� �˰���
	int scale = getIntValue();
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭, 3��� ����, 1.5�� ��� ����ؼ� �غ���.
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}

void zoomIn() { // Ȯ�� �˰���
	int scale = getIntValue();
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i * scale)][(int)(k * scale)] = inImage[i][k];
			outImage[(int)(i * scale + 1)][(int)(k * scale + 1)] = inImage[i][k];
		}
	}
	printImage();
}

void zoomIn2() { // Ȯ�� �˰���
	int scale = getIntValue();
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = inImage[(int)(i / scale)][(int)k / scale];
		}
	}
	printImage();
}

void rotate() { // ȸ�� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int degree;
	while (1) {
		printf(" ���� �Է� --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	double radian = degree * 3.141592 / 180.0;

	// xd = cos*xs - sin*ys;
	// yd = sin*xs + cos*ys;

	// �Է� �迭 --> ��� �迭
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

void rotate2() { // ȸ�� �˰��� + �߾�/�����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int degree;
	while (1) {
		printf(" ���� �Է� --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	//	double radian = -degree * 3.141592 / 180.0;
	double radian = degree * 3.141592 / 180.0;
	// xd = cos*xs - sin*ys;
	// yd = sin*xs + cos*ys;


	int cx = inH / 2;
	int cy = inW / 2;
	// �Է� �迭 --> ��� �迭
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

void move() { // ���Ͽ��� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	int moveX = 0, moveY = 0;

	printf(" ������ X�·��� ������ --> "); scanf("%d", &moveX);
	printf(" ������ Y��ǥ�� ������ --> "); scanf("%d", &moveY);

	// �Է� �迭 --> ��� �迭
	int x = 0, y = 0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			x = k - moveX;
			y = i - moveY;
			if (x >= 0 && x < inW && y >= 0 && y < inH)
				outImage[i][k] = inImage[y][x];
			else
				outImage[i][k] = 0; // ���� ����
		}
	}

	printImage();
}

void mirrorHor() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][outW - 1 - k];
		}
	}
	printImage();
}

void mirrorVer() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[inH - 1 - i][k];
		}
	}
	printImage();
}

void zoomOutAvg() {
	int scale = getIntValue();
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭, 3��� ����, 1.5�� ��� ����ؼ� �غ���.
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
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// �޸� �Ҵ�
	mallocOutputMemory();

	unsigned char* array = NULL;
	array = (unsigned char*)malloc(sizeof(unsigned char) * scale * scale);
	if (array == NULL)
		return;

	// �Է� �迭 --> ��� �迭, 3��� ����, 1.5�� ��� ����ؼ� �غ���.

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
void rotate3() { // ȸ�� �˰��� + �߾�/����� + ���ũ�� ������
	// �޸� ����
	freeOutputMemory();

	int degree;
	while (1) {
		printf(" ���� �Է� --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// �޸� �Ҵ�
	mallocOutputMemory();

	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;


	int cx = inH / 2;
	int cy = inW / 2;
	int centerX = outH / 2;
	int centerY = outW / 2;

	// �Է� �迭 --> ��� �迭
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

void bilinear() { // Ȯ��(�缱�� ����)

	int point, i_H, i_W;
	unsigned char newValue;
	double ZoomRate = 2.0, r_H, r_W, s_H, s_W; // TODO ���� �ʿ�
	double C1, C2, C3, C4;

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	ZoomRate = getDoubleValue();

	outH = (int)(inH * ZoomRate);
	outW = (int)(inW * ZoomRate);
	// �޸� �Ҵ�
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
			else { // �Ҽ��� �� �����ϱ�
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

void rotateBilinear() { // ȸ�� (Ȯ��+�缱��)

	// �޸� ����
	freeOutputMemory();

	int degree;
	while (1) {
		printf(" ���� �Է� --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// �޸� �Ҵ�
	mallocOutputMemory();

	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;

	int point, i_H, i_W;
	unsigned char newValue;
	double ZoomRate, r_H, r_W, s_H, s_W; // TODO ���� �ʿ�
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


	// �Է� �迭 --> ��� �迭
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
	double ZoomRate = 2.0, r_H, r_W, s_H, s_W; // TODO ���� �ʿ�
	double C1, C2, C3, C4;

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	ZoomRate = getDoubleValue();

	outH = (int)(inH * ZoomRate);
	outW = (int)(inW * ZoomRate);

	int degree;
	while (1) {
		printf(" ���� �Է� --> "); scanf("%d", &degree);
		if (degree >= 1 && degree <= 180)
			break;
	}
	// �޸� �Ҵ�
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
			else { // �Ҽ��� �� �����ϱ�
				C1 = (double)tempImage[i_H][i_W];
				C2 = (double)tempImage[i_H][i_W + 1];
				C3 = (double)tempImage[i_H + 1][i_W + 1];
				C4 = (double)tempImage[i_H + 1][i_W];

				newValue = (unsigned char)(C1 * (1 - s_H) * (1 - s_W) + C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				outImage[i][k] = newValue;
			}

		}
	}*/
/*	// �޸� ����
	freeOutputMemory();

	int degree = getIntValue();
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// �޸� �Ҵ�
	mallocOutputMemory();


	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;

	int cx = inH / 2;
	int cy = inW / 2;
	int centerX = outH / 2;
	int centerY = outW / 2;

	double** tmpInImage = mallocDoubleMemory(outH, outW);

	// �Է� �迭 --> ��� �迭
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

	// ������
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
			else { // �Ҽ��� �� �����ϱ�
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
void rotateBilinear() { // ȸ�� (Ȯ��+�缱��) 
	// �޸� ����
	freeOutputMemory();

	int degree = getIntValue();
	double radian = (double)(degree * 3.141592 / 180.0);
	double radian2 = (double)((90 - degree) * 3.141592 / 180.0);

	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outW = (int)(inH * cos(radian2) + inW * cos(radian));
	outH = (int)(inH * cos(radian) + inW * cos(radian2));
	// �޸� �Ҵ�
	mallocOutputMemory();


	printf("inH = %d, inW = %d, outH = %d, outW = %d", inH, inW, outH, outW);
	//// xd = cos*xs - sin*ys;
	//// yd = sin*xs + cos*ys;

	int cx = inH / 2;
	int cy = inW / 2;
	int centerX = outH / 2;
	int centerY = outW / 2;

	double** tmpInImage = mallocDoubleMemory(outH, outW);

	// �Է� �迭 --> ��� �迭
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

	// ������
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
			else { // �Ҽ��� �� �����ϱ�
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

void histoStretch() { // ������׷� ��Ʈ��Ī �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// ��¥ ����ó�� �˰���
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

void endIn() { // ������ Ž�� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// ��¥ ����ó�� �˰���
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

void histoEqual() { // ������׷� ��Ȱȭ �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// 1�ܰ� : �󵵼� ���� (=������׷�
	int histo[256] = { 0, };
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			histo[inImage[i][k]]++;

	// 2�ܰ� : ����������׷� ����
	int sumHisto[256] = { 0, };
	sumHisto[0] = histo[0];
	for (int i = 0; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];
	
	// 3�ܰ� : ����ȭ�� ������׷� ���� normalHisto = sumHisto * (1.0/(inH*inW)) * 255.0
	double normalHisto[256] = { 1.0, };
	for (int i = 0; i < 256; i++) {
		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0;
	}

	// 4�ܰ� : inImage�� ����ȭ�� ������ ġȯ
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char) normalHisto[inImage[i][k]];
		}
	}

	printImage();
}

void emboss() { // ȭ�ҿ��� ó��: ������ �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////
	double mask[3][3] = { {-1.0, 0.0, 0.0}, // ������ ����ũ
						  {0.0,  0.0, 0.0},
						  {0.0,  0.0, 1.0} };

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH+2, inW+2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH; i++) 
		for (int k = 0; k < inW; k++) 
			tmpInImage[i][k] = 127;
		
	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++) 
		for (int k = 0; k < inW; k++) 
			tmpInImage[i+1][k+1] = inImage[i][k];
		
	// ** ȸ�� ���� ** 
	double S; 
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	for (int i = 0; i < outH; i++) 
		for (int k = 0; k < outW; k++) 
			tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void blurr() { // ȭ�ҿ��� ó��: ���� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////
	double mask[3][3] = { {1./9, 1./9, 1./9}, // ���� ����ũ
						  {1. / 9, 1. / 9, 1. / 9},
						  {1. / 9, 1. / 9, 1. / 9} };

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void blurr2() { // ȭ�ҿ��� ó��: ���� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////
	double mask[9][9] = {{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 }, // ���� ����ũ
					    {1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 },
						{1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81,1. / 81, 1. / 81, 1. / 81 }};

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 8, inW + 8);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH+8; i++)
		for (int k = 0; k < inW+8; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 4][k + 4] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 9; m++)
				for (int n = 0; n < 9; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void sharpen() { // ȭ�ҿ��� ó��: ������ �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////
	double mask[3][3] = { {-1., -1., -1.}, // ������ ����ũ
						  {-1., 9., -1.},
						  {-1., -1., -1.} };

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	
	// ��ó�� (����ũ ���� �հ迡 ����...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void hpfSharp() { // ȭ�ҿ��� ó��: ������ ������ �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////
	double mask[3][3] = { {-1./9., -1./9., -1./9.}, // ������ ����ũ
						  {-1./9., 8./9., -1./9.},
						  {-1./9., -1./9., -1./9.} };
//	double mask[3][3] = { {1. / 9., 1. / 9., 1. / 9.}, // ������ ����ũ
////						  {-1./9., 1.-(-1/9.)*8.0, -1./9.},
//						{1. / 9., 1. / 9., -1. / 9.},
//						{-1. / 9., -1. / 9., -1. / 9.} };

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] += 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	// ��ó�� (����ũ ���� �հ迡 ����...)
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			tmpOutImage[i][k] += tmpInImage[i][k];
//			tmpOutImage[i][k] += 127.0;
		}
	}

	// �ӽ� ��� ���� --> ��� ����. 
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
	printf("������ ���ϸ� --> ");  
	scanf("%s", &tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (�߿�!) �̹����� ���� ���̸� ����
	rfp = fopen(fileName, "rb");
	if (rfp == NULL) {
		MessageBox(hwnd, L"�������� �ʴ� �����Դϴ�.", L"���� â", NULL);
		printf(" �������� �ʴ� �����Դϴ� \n");
		return;
	}

	fseek(rfp, 0L, SEEK_END); // ������ ������ �̵�
	long long fsize = ftell(rfp); // �� ���������?  262,144  --> 512
	fclose(rfp);

	int morRate = 0;
	while(1) {
		printf(" 1���� 9������ ������--> "); scanf("%d", &morRate);
		if (morRate >= 1 && morRate <= 9)
			break;
	}
	unsigned char** morImage = NULL;
	int morphH, morphW, inX=0, inY = 0, morX = 0, morY = 0;

	// ����, �� ���ϱ�
	morphH = morphW = (int)sqrt(fsize);
	printf(" morphH = %d \n", morphH);
	// �޸� �Ҵ�
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
	else { // inImage�� ������ 
		outH = morphH;
		outW = morphW;
		inX = (int)((morphH - inH) / 2.);
		inY = (int)((morphW - inW) / 2.);
	}
//	printf(" OutH = %d, inX = %d, morX = %d\n", outH,inX,morX );
	// �޸� �Ҵ�
	mallocOutputMemory();

	// ���� --> �޸�
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < morphH; i++)
		fread(morImage[i], sizeof(unsigned char), morphW, rfp);
	fclose(rfp);

	// ����ó�� -> ��ο����� ������ �ذ��Ұ� 
	if (morphH <= inH) { // inImage�� �� ũ�ų� ����
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
	else { // inImage�� ����
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

	// �޸� ���� 
	for (int i = 0; i < morphH; i++)
		free(morImage[i]);
	free(morImage);

	printImage();
}

void edge1() { // ��輱����: ���� ���� ���� ����ũ
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////

	//double mask[3][3] = { {0.0, 0.0, 0.0}, // ���� ���� ���� ����ũ
	//					  {-1.0, 1.0, 0.0},
	//					  {0.0,  0.0, 0.0} };
	double mask[3][3] = { {0.0, -1.0, 0.0}, // ���� ���� ���� ����ũ
					  {0.0, 1.0, 0.0},
					  {0.0,  0.0, 0.0} };
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void sobel() { // ��輱����: 1�� �̺� ȸ�� 
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////

	double mask[3][3] = { {-1.0, -2.0, -1.0}, // �Һ� ����ũ
						  {0.0,  0.0, 0.0},
						  {1.0,  2.0, 1.0} };

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void laplacian() { // ��輱����: ���ö�þ�
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////

	double mask[3][3] = { {-1.0, -1.0, -1.0}, // ���ö�þ� ����ũ
						  {-1.0,  8.0, -1.0},
						  {-1.0, -1.0, -1.0} };

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void homogen() { // ���� ������ ���� ����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
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
			max = 0.0; // ����� �̵��� ������ �ִ밪 �ʱ�ȭ
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					if (fabs(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]) >= max)
						max = fabs(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]);
				}
			}
			tmpOutImage[i][k] = max;
		}
	}
	// �Է� �迭 --> ��� �迭
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

void subEdge() { // ���� ������ ���� ����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
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
			max = 0.0; // ����� �̵��� ������ �ִ밪 �ʱ�ȭ
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
	// �Է� �迭 --> ��� �迭
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

void gauss() { // ��輱����: ����þ�
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�
	mallocOutputMemory();

	////////////////////
	// ȭ�� ���� ó��
	////////////////////

	double mask[7][7] = { {0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0}, // ����þ� ����ũ
						{0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0},
						{-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0},
						{-1.0, -3.0, 5.0, 16.0, 5.0, -3.0, -1.0},
						{-1.0, -3.0, 5.0, 5.0, 5.0, -3.0, -1.0},
						{0.0, -2.0, -3.0, -3.0, -3.0, -2.0, 0.0},
						{0.0, 0.0, -1.0, -1.0, -1.0, 0.0, 0.0} };


	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 6, inW + 6);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ� 
	for (int i = 0; i < inH+6; i++)
		for (int k = 0; k < inW+6; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 3][k + 3] = inImage[i][k];

	// ** ȸ�� ���� ** 
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3)�� ������ �߽����� �� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 7; m++)
				for (int n = 0; n < 7; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ���� --> ��� ����. 
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

void accum() { // ȿ�� �����ϱ�: ������ outImage�� inImage��..
	// inW�� outW�� �ٸ��� in�� �ٽ� ����.
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
