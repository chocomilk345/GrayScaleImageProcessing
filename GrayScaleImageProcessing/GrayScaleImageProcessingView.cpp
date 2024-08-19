
// GrayScaleImageProcessingView.cpp: CGrayScaleImageProcessingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GrayScaleImageProcessing.h"
#endif

#include "GrayScaleImageProcessingDoc.h"
#include "GrayScaleImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrayScaleImageProcessingView

IMPLEMENT_DYNCREATE(CGrayScaleImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CGrayScaleImageProcessingView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CGrayScaleImageProcessingView::OnEqualImage)
END_MESSAGE_MAP()

// CGrayScaleImageProcessingView 생성/소멸

CGrayScaleImageProcessingView::CGrayScaleImageProcessingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CGrayScaleImageProcessingView::~CGrayScaleImageProcessingView()
{
}

BOOL CGrayScaleImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGrayScaleImageProcessingView 그리기

void CGrayScaleImageProcessingView::OnDraw(CDC* pDC)
{
	CGrayScaleImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	unsigned char R, G, B;
	for (int i = 0; i < pDoc->m_inH; i++) {
		for (int k = 0; k < pDoc->m_inW; k++) {
			R = G = B = pDoc->m_inImage[i][k];
			pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
		}
	}
	for (int i = 0; i < pDoc->m_outH; i++) {
		for (int k = 0; k < pDoc->m_outW; k++) {
			R = G = B = pDoc->m_outImage[i][k];
			pDC->SetPixel(pDoc->m_inW + k + 5 + 5, i + 5, RGB(R, G, B));
		}
	}


}


// CGrayScaleImageProcessingView 인쇄

BOOL CGrayScaleImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGrayScaleImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGrayScaleImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CGrayScaleImageProcessingView 진단

#ifdef _DEBUG
void CGrayScaleImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CGrayScaleImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGrayScaleImageProcessingDoc* CGrayScaleImageProcessingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrayScaleImageProcessingDoc)));
	return (CGrayScaleImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CGrayScaleImageProcessingView 메시지 처리기


void CGrayScaleImageProcessingView::OnEqualImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGrayScaleImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEqualImage();
	Invalidate(TRUE); // OnDraw() 호출 효과
}
