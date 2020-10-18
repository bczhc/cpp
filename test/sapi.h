#ifndef SAPI_SIMPLE_H
#define SAPI_SIMPLE_H

// COM objects
#include <Objbase.h>

// GUIDs
const GUID CLSID_SpVoice = {0x96749377, 0x3391, 0x11d2, 0x9e,0xe3, 0x00,0xc0,0x4f,0x79,0x73,0x96};
const GUID IID_ISpVoice = {0x6c44df74, 0x72b9, 0x4992, 0xa1,0xec, 0xef,0x99,0x6e,0x04,0x22,0xd4};

// Enums
typedef /* [hidden] */
enum SPEAKFLAGS
{	SPF_DEFAULT	= 0,
    SPF_ASYNC	= ( 1L << 0 ) ,
    SPF_PURGEBEFORESPEAK	= ( 1L << 1 ) ,
    SPF_IS_FILENAME	= ( 1L << 2 ) ,
    SPF_IS_XML	= ( 1L << 3 ) ,
    SPF_IS_NOT_XML	= ( 1L << 4 ) ,
    SPF_PERSIST_XML	= ( 1L << 5 ) ,
    SPF_NLP_SPEAK_PUNC	= ( 1L << 6 ) ,
    SPF_PARSE_SAPI	= ( 1L << 7 ) ,
    SPF_PARSE_SSML	= ( 1L << 8 ) ,
    SPF_PARSE_AUTODETECT	= 0,
    SPF_NLP_MASK	= SPF_NLP_SPEAK_PUNC,
    SPF_PARSE_MASK	= ( SPF_PARSE_SAPI | SPF_PARSE_SSML ) ,
    SPF_VOICE_MASK	= ( ( ( ( ( ( ( SPF_ASYNC | SPF_PURGEBEFORESPEAK )  | SPF_IS_FILENAME )  | SPF_IS_XML )  | SPF_IS_NOT_XML )  | SPF_NLP_MASK )  | SPF_PERSIST_XML )  | SPF_PARSE_MASK ) ,
    SPF_UNUSED_FLAGS	= ~SPF_VOICE_MASK
} 	SPEAKFLAGS;

// Enum dummies
typedef enum{SPVPRIORITY_X} SPVPRIORITY;
typedef enum{SPEVENTENUM_X} SPEVENTENUM;

// Interface dummies
typedef void ISpNotifySink;
typedef void SPNOTIFYCALLBACK;
typedef void ISpNotifyCallback;
typedef void SPEVENT;
typedef void SPEVENTSOURCEINFO;
typedef void ISpObjectToken;
typedef void ISpStreamFormat;
typedef void SPVOICESTATUS;

// Voice class (C-style)
typedef interface ISpVoice ISpVoice;

typedef struct ISpVoiceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
            ISpVoice * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */
            __RPC__deref_out  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
            ISpVoice * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
            ISpVoice * This);

    HRESULT ( STDMETHODCALLTYPE *SetNotifySink )(
            ISpVoice * This,
            /* [in] */ ISpNotifySink *pNotifySink);

    /* [local] */ HRESULT ( STDMETHODCALLTYPE *SetNotifyWindowMessage )(
            ISpVoice * This,
            /* [in] */ HWND hWnd,
            /* [in] */ UINT Msg,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam);

    /* [local] */ HRESULT ( STDMETHODCALLTYPE *SetNotifyCallbackFunction )(
            ISpVoice * This,
            /* [in] */ SPNOTIFYCALLBACK *pfnCallback,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam);

    /* [local] */ HRESULT ( STDMETHODCALLTYPE *SetNotifyCallbackInterface )(
            ISpVoice * This,
            /* [in] */ ISpNotifyCallback *pSpCallback,
            /* [in] */ WPARAM wParam,
            /* [in] */ LPARAM lParam);

    /* [local] */ HRESULT ( STDMETHODCALLTYPE *SetNotifyWin32Event )(
            ISpVoice * This);

    /* [local] */ HRESULT ( STDMETHODCALLTYPE *WaitForNotifyEvent )(
            ISpVoice * This,
            /* [in] */ DWORD dwMilliseconds);

    /* [local] */ HANDLE ( STDMETHODCALLTYPE *GetNotifyEventHandle )(
            ISpVoice * This);

    HRESULT ( STDMETHODCALLTYPE *SetInterest )(
            ISpVoice * This,
            /* [in] */ ULONGLONG ullEventInterest,
            /* [in] */ ULONGLONG ullQueuedInterest);

    HRESULT ( STDMETHODCALLTYPE *GetEvents )(
            ISpVoice * This,
            /* [in] */ ULONG ulCount,
            /* [size_is][out] */ SPEVENT *pEventArray,
            /* [out] */ ULONG *pulFetched);

    HRESULT ( STDMETHODCALLTYPE *GetInfo )(
            ISpVoice * This,
            /* [out] */ SPEVENTSOURCEINFO *pInfo);

    HRESULT ( STDMETHODCALLTYPE *SetOutput )(
            ISpVoice * This,
            /* [in] */ IUnknown *pUnkOutput,
            /* [in] */ BOOL fAllowFormatChanges);

    HRESULT ( STDMETHODCALLTYPE *GetOutputObjectToken )(
            ISpVoice * This,
            /* [out] */ ISpObjectToken **ppObjectToken);

    HRESULT ( STDMETHODCALLTYPE *GetOutputStream )(
            ISpVoice * This,
            /* [out] */ ISpStreamFormat **ppStream);

    HRESULT ( STDMETHODCALLTYPE *Pause )(
            ISpVoice * This);

    HRESULT ( STDMETHODCALLTYPE *Resume )(
            ISpVoice * This);

    HRESULT ( STDMETHODCALLTYPE *SetVoice )(
            ISpVoice * This,
            /* [in] */ ISpObjectToken *pToken);

    HRESULT ( STDMETHODCALLTYPE *GetVoice )(
            ISpVoice * This,
            /* [out] */ ISpObjectToken **ppToken);

    HRESULT ( STDMETHODCALLTYPE *Speak )(
            ISpVoice * This,
            /* [string][in][annotation] */
            LPCWSTR pwcs,
            /* [in] */ DWORD dwFlags,
            /* [out][annotation] */
            ULONG *pulStreamNumber);

    HRESULT ( STDMETHODCALLTYPE *SpeakStream )(
            ISpVoice * This,
            /* [in] */ IStream *pStream,
            /* [in] */ DWORD dwFlags,
            /* [out][annotation] */
            ULONG *pulStreamNumber);

    HRESULT ( STDMETHODCALLTYPE *GetStatus )(
            ISpVoice * This,
            /* [out] */ SPVOICESTATUS *pStatus,
            /* [out] */ LPWSTR *ppszLastBookmark);

    HRESULT ( STDMETHODCALLTYPE *Skip )(
            ISpVoice * This,
            /* [string][in] */ LPCWSTR pItemType,
            /* [in] */ long lNumItems,
            /* [out] */ ULONG *pulNumSkipped);

    HRESULT ( STDMETHODCALLTYPE *SetPriority )(
            ISpVoice * This,
            /* [in] */ SPVPRIORITY ePriority);

    HRESULT ( STDMETHODCALLTYPE *GetPriority )(
            ISpVoice * This,
            /* [out] */ SPVPRIORITY *pePriority);

    HRESULT ( STDMETHODCALLTYPE *SetAlertBoundary )(
            ISpVoice * This,
            /* [in] */ SPEVENTENUM eBoundary);

    HRESULT ( STDMETHODCALLTYPE *GetAlertBoundary )(
            ISpVoice * This,
            /* [out] */ SPEVENTENUM *peBoundary);

    HRESULT ( STDMETHODCALLTYPE *SetRate )(
            ISpVoice * This,
            /* [in] */ long RateAdjust);

    HRESULT ( STDMETHODCALLTYPE *GetRate )(
            ISpVoice * This,
            /* [out] */ long *pRateAdjust);

    HRESULT ( STDMETHODCALLTYPE *SetVolume )(
            ISpVoice * This,
            /* [in] */ USHORT usVolume);

    HRESULT ( STDMETHODCALLTYPE *GetVolume )(
            ISpVoice * This,
            /* [out] */ USHORT *pusVolume);

    HRESULT ( STDMETHODCALLTYPE *WaitUntilDone )(
            ISpVoice * This,
            /* [in] */ ULONG msTimeout);

    HRESULT ( STDMETHODCALLTYPE *SetSyncSpeakTimeout )(
            ISpVoice * This,
            /* [in] */ ULONG msTimeout);

    HRESULT ( STDMETHODCALLTYPE *GetSyncSpeakTimeout )(
            ISpVoice * This,
            /* [out] */ ULONG *pmsTimeout);

    /* [local] */ HANDLE ( STDMETHODCALLTYPE *SpeakCompleteEvent )(
            ISpVoice * This);

    /* [local] */ HRESULT ( STDMETHODCALLTYPE *IsUISupported )(
            ISpVoice * This,
            /* [in] */ LPCWSTR pszTypeOfUI,
            /* [in] */ void *pvExtraData,
            /* [in] */ ULONG cbExtraData,
            /* [out] */ BOOL *pfSupported);

    /* [local] */ HRESULT ( STDMETHODCALLTYPE *DisplayUI )(
            ISpVoice * This,
            /* [in] */ HWND hwndParent,
            /* [in] */ LPCWSTR pszTitle,
            /* [in] */ LPCWSTR pszTypeOfUI,
            /* [in] */ void *pvExtraData,
            /* [in] */ ULONG cbExtraData);

    END_INTERFACE
} ISpVoiceVtbl;

interface ISpVoice
{
    CONST_VTBL struct ISpVoiceVtbl *lpVtbl;
};

#endif	// SAPI_SIMPLE_H