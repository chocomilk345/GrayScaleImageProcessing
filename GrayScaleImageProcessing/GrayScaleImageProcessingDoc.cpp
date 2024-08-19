
// GrayScaleImageProcessingDoc.cpp: CGrayScaleImageProcessingDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GrayScaleImageProcessing.h"
#endif

#include "GrayScaleImageProcessingDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGrayScaleImageProcessingDoc

IMPLEMENT_DYNCREATE(CGrayScaleImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CGrayScaleImageProcessingDoc, CDocument)
END_MESSAGE_MAP()


// CGrayScaleImageProcessingDoc 생성/소멸

CGrayScaleImageProcessingDoc::CGrayScaleImageProcessingDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CGrayScaleImageProcessingDoc::~CGrayScaleImageProcessingDoc()
{
}

BOOL CGrayScaleImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGrayScaleImageProcessingDoc serialization

void CGrayScaleImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGrayScaleImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CGrayScaleImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGrayScaleImageProcessingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGrayScaleImageProcessingDoc 진단

#ifdef _DEBUG
void CGrayScaleImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGrayScaleImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGrayScaleImageProcessingDoc 명령


unsigned char** CGrayScaleImageProcessingDoc::OnMalloc2D(int h, int w, int initVal)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** retMemory;

	retMemory = new unsigned char* [h];
	for (int i = 0; i < h; i++)
		retMemory[i] = new unsigned char[w];

	return retMemory;
}


void CGrayScaleImageProcessingDoc::OnFree2D(unsigned char** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		delete memory[i];
	delete[] memory;
}


BOOL CGrayScaleImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	// 선택된 파일 열기
	CFile  File;
	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	// 파일의 크기를 파악 --> 가로세로 크기 결정
	ULONGLONG fLength = File.GetLength();
	// 기존 메모리 모두 해제하고, 관련값 초기화
	OnFree2D(m_inImage, m_inH);
	m_inImage = NULL;
	m_inH = m_inW = 0;
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;
	// (중요!) 입력 이미지의 높이와 폭을 결정 + 메모리 할당
	m_inH = m_inW = (int) sqrt((double)fLength);
	m_inImage = OnMalloc2D(m_inH, m_inW, 0);

	// 파일 --> 메모리
	for (int i = 0; i < m_inH; i++)
		File.Read(m_inImage[i], m_inW);

	File.Close();

	return TRUE;
}


void CGrayScaleImageProcessingDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 기존 메모리 모두 해제하고, 관련값 초기화
	OnFree2D(m_inImage, m_inH);
	m_inImage = NULL;
	m_inH = m_inW = 0;
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	CDocument::OnCloseDocument();
}


void CGrayScaleImageProcessingDoc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 출력 메모리 해제
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;
	// (중요!) 출력 이미지의 크기를 결정 --> 알고리즘에 의존
	m_outH = m_inH;
	m_outW = m_inW;
	// 출력 이미지 메모리 할당
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);
	// *** 진짜 영상처리 알고리즘 ***
	for (int i=0; i<m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			m_outImage[i][k] = m_inImage[i][k];
		}
}
