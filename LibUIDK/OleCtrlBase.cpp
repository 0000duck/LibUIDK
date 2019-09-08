#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////

CRichMsgItem::CRichMsgItem()
{
	m_uOleWinID = -1;
	m_lParam = 0;
}

CRichMsgItem::~CRichMsgItem()
{

}

// IOleCtrlBase��ΪOLE�ӿؼ���������
// IOleCtrlBase���Լ��ؽ���༭���еĴ��ڣ����ز�ͬ�Ĵ��ڣ�ʵ�ֲ�ͬ��OLE
// ���磬�ڽ���༭���д���һ�����ڣ������ﴴ��һ��ͼƬ�ؼ��� IOleCtrlBase����������ں󣬾ͱ��һ��Image Ole
// �ڽ���༭���д���һ�����ڣ����洴��һ����Ƶ�ؼ���IOleCtrlBase���غ󣬾ͱ��һ����ƵOLE
// IOleCtrlBaseֻ��Ҫ�ṩ���������๦�ܼ��ɣ�
// 1. IOleCtrlBase�����ӿؼ��Ľ���
// 2. �����ӿؼ���RichEdit����
// 3. �����ӿؼ�
// LibUIDK����������IM����������ͼƬOLE�� ���������ļ�����ƬOLE�����û���IOleCtrlBase�����Լ�ʵ��
IOleCtrlBase::IOleCtrlBase(HWND hRichEdit)
{
	IDD = -1;
	m_pOleClientSite = NULL;
	m_lParam = 0;

	// ����OLE�ؼ���һ������Create�������������������ﱣ�����ĸ�����
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;
	pMember->m_pParent = CWnd::FromHandle(hRichEdit);
}

IOleCtrlBase::~IOleCtrlBase()
{

}

BOOL IOleCtrlBase::Create(LPCTSTR lpszClassName, LPCTSTR lpszControlName, DWORD dwStyle,
	CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	return CWLWnd::Create(lpszClassName, lpszControlName, dwStyle, CRect(0, 0, 0, 0),
			pParentWnd, nID, pVirtualParent);
}

UINT IOleCtrlBase::GetIDD() const
{
	return IDD;
}

int IOleCtrlBase::SetIDD(UINT nID)
{
	IDD = nID;

	return 0;
}

// IUnknown������ӿ�
HRESULT IOleCtrlBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if (this == NULL)
	{
		return S_FALSE;
	}

	// �����thisָ������ת��Ϊ��Ӧ��ָ�롣 ����Ole�޷����뵽RichEdit�С�
	if (riid == IID_IViewObject)
	{
		*ppvObject = (IViewObject *)this;
	}
	else if (riid == IID_IViewObject2)
	{
		*ppvObject = (IViewObject2 *)this;
	}
	else if (riid == IID_IUnknown || riid == IID_IOleObject)
	{
		*ppvObject = (IOleObject *)this;
	}
	else
	{
		return E_NOINTERFACE;
	}

	// �Ӳ��Ӷ��У�����һ��Ole��Ӧһ��C++����
	AddRef();

	return S_OK;
}

ULONG IOleCtrlBase::AddRef(void)
{
	return S_OK;
}

ULONG IOleCtrlBase::Release(void)
{
	return S_OK;
}

// ����CRichEditCtrlBase::InsertOle��InsertObject֮�󣬻����SetClientSite
// ���Կ�����Ϊ����m_pOleClientSite��ֵ��Чʱ���ͱ�ʾOLE�ѱ����뵽��RichEdit�С�
HRESULT IOleCtrlBase::SetClientSite(/* [unique][in] */ IOleClientSite *pClientSite)
{
	m_pOleClientSite = pClientSite;

	if (m_pOleClientSite == NULL)
	{
		return S_FALSE;
	}

	return S_OK;
}

HRESULT IOleCtrlBase::GetClientSite(/* [out] */IOleClientSite **ppClientSite)
{
	if (ppClientSite == NULL)
	{
		return E_INVALIDARG;
	}

	*ppClientSite = m_pOleClientSite;

	return S_OK;
}

// {92E05F99-41F4-4914-A50E-96D14D175760}
const GUID LibUIDK::g_guidImageOle =
{ 0x92e05f99, 0x41f4, 0x4914, { 0xa5, 0xe, 0x96, 0xd1, 0x4d, 0x17, 0x57, 0x60 } };

HRESULT IOleCtrlBase::GetUserClassID(/* [out] */ CLSID *pClsid)
{
	*pClsid = g_guidImageOle;

	return S_OK;
}

HRESULT IOleCtrlBase::SetExtent(/* [in] */ DWORD dwDrawAspect, /* [in] */ SIZEL *psizel)
{
	m_sizeExtent = *psizel;
	return S_OK;
}

HRESULT IOleCtrlBase::GetExtent(/* [in] */ DWORD dwDrawAspect, /* [out] */ SIZEL *psizel)
{
	if (psizel == NULL)
	{
		return E_INVALIDARG;
	}

	*psizel = m_sizeExtent;

	return S_OK;
}

HRESULT IOleCtrlBase::Advise(/* [unique][in] */ IAdviseSink *pAdvSink, /* [out] */ DWORD *pdwConnection)
{

	return S_OK;
}

HRESULT IOleCtrlBase::Unadvise(
	/* [in] */ DWORD dwConnection)
{

	return S_OK;
}

// IViewObject2 ��Ҫʵ�ֵķ���
HRESULT IOleCtrlBase::SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink)
{
	return S_OK;
}

HRESULT IOleCtrlBase::GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink)
{
	return S_OK;
}

HRESULT IOleCtrlBase::Draw(
	DWORD dwDrawAspect, LONG lindex,
	void *pvAspect,
	DVTARGETDEVICE *ptd,
	HDC hdcTargetDev,
	HDC hdcDraw,
	LPCRECTL lprcBounds,
	LPCRECTL lprcWBounds,
	BOOL(STDMETHODCALLTYPE *pfnContinue)(ULONG_PTR dwContinue),
	ULONG_PTR dwContinue)
{
	// lprcBounds�����꣬�������hdcDraw�ġ� ���������ǰ�Windowsed RichEdit�����ݣ�������
	// RichEdit�ĸ������ϡ� ���ԣ������lprcBounds�����꣬Ҳ�������Windowsed RichEdit�ĸ�����
	// ���ǰ���ת�������GetParent�����ꡣ
	m_rect = (RECT *)lprcBounds;

	// ����RichEdit��OLE���ǻ�����RichEdit��DC�ϣ����ǻ�����RichEdit�����ڵ�DC�ϣ���
	// OLE�������������RichEdit�ģ�����Ҫ������ͼԭ�㣬������Ʋ�����ȷ��λ����
	CPoint ptOrg;

	if (GetParent() != NULL)
	{
		// �ж�Ole�ĸ������ǲ���Windowsed RichEdit�ؼ�
		CString strClass;
		::GetClassName(GetParent()->GetSafeHwnd(), strClass.GetBufferSetLength(MAX_PATH), MAX_PATH);
		strClass.ReleaseBuffer();

		if (strClass == GetRichEditClassName())
		{
			CWnd *pRichParent = GetParent()->GetParent();
			pRichParent->ClientToScreen(m_rect);
			GetParent()->ScreenToClient(m_rect);

			CRect rcRichEdit;
			GetParent()->GetWindowRect(rcRichEdit);
			pRichParent->ScreenToClient(rcRichEdit);
			ptOrg = rcRichEdit.TopLeft();
		}
	}

	// �����Լ�
	if (IDD != -1)
	{
		CDC *pParentDC = CDC::FromHandle(hdcDraw);

		CWLWnd::OnDraw(pParentDC, &ptOrg);
	}

	return S_OK;
}

HRESULT IOleCtrlBase::GetExtent(
	DWORD dwDrawAspect,
	LONG lindex,
	DVTARGETDEVICE *ptd,
	LPSIZEL lpsizel)
{
	return S_OK;
}

BOOL IOleCtrlBase::IsOleControl() const
{
	return TRUE;
}

int IOleCtrlBase::GetWindowRect(LPRECT lprc) const
{
	if (lprc == NULL)
	{
		return -1;
	}

	// �ж�Ole�ĸ������ǲ���Windowsed RichEdit�ؼ�
	CString strClass;
	::GetClassName(GetParent()->GetSafeHwnd(), strClass.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strClass.ReleaseBuffer();

	*lprc = m_rect;
	if (strClass == GetRichEditClassName())
	{
		GetParent()->ClientToScreen(lprc);
	}

	return 0;
}

int IOleCtrlBase::GetClientRect(LPRECT lpRect) const
{
	if (lpRect == NULL)
	{
		return -1;
	}

	lpRect->left = lpRect->top = 0;
	lpRect->right = m_rect.Width();
	lpRect->bottom = m_rect.Height();

	return 0;
}

LPARAM IOleCtrlBase::SetUserData(LPARAM lParam)
{
	LPARAM lOldParam = m_lParam;
	m_lParam = lParam;
	return lOldParam;
}

LPARAM IOleCtrlBase::GetUserData() const
{
	return m_lParam;
}

//////////////////////////////////////////////////////////////////////////
// protected

int IOleCtrlBase::OnCreate()
{
	CString strID = CUIMgr::StringIDFromInt(IDD);
	UIWNDPROPERTIES *pUIProp = CUIMgr::GetUIWNDPROPERTIES(strID);
	if (pUIProp != NULL)
	{
		// May be delay loaded.
		if (!BOOL(pUIProp->m_bLoaded))
		{
			CUIMgr::DelayLoadWindow(pUIProp);
		}

		m_uiwndProperties = *pUIProp;
	}

	CWLWnd::SetBkImage((LPCTSTR)m_uiwndProperties.m_strBKNo, FALSE);
	SetBkImageResizeMode(m_uiwndProperties.m_eBkImageResizeMode);
	SetBkImageResizePoint(m_uiwndProperties.m_ptImageResize);

	std::vector<CChildInfo> vChildren;
	CreateChildControls(&m_uiwndProperties, GetParent(), NULL, NULL, NULL, NULL, &vChildren);

	return 0;
}
