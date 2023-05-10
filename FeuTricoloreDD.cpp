// modifié par Patrice:)

/*
Voici une version en mode Direct Draw 
Code remanié pour etre compatible avec les nouvelles normes C11 et C17
*/

#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <Winuser.h>
#include <stdio.h>
#include <conio.h>
#include "ddraw.h"

#define IDM_TIMER1 0x01000
#define IDM_TIMER_R 0x02000
#define IDM_TIMER_V 0x02001
#define IDM_TIMER_J 0x02002

#define Screen_Height	600
#define Screen_Width	800
#define Color_Depth		16
#define KEY_DOWN(key) (GetAsyncKeyState(key) &0x8000)
#define LOWORD(l)           ((WORD)(l))

//#define DDSCL_ALLOWREBOOT      =     2

#define IDC_STATUT                      2000
#define IDC_CYC_R                       2003
#define IDC_CYC_V                       2004
#define IDC_CYC_J                       2005

__declspec(dllexport)struct RGB16{
USHORT S_Rouge;
USHORT S_Bleu;
USHORT S_Vert;
}rgb16;

HWND		__declspec(dllexport)g_hWnd,hMain, hWnd,imghWnd,imgV,imgJ,imgR,imgR2,imgL,imgRg;
HINSTANCE	__declspec(dllexport)g_hInstance;
WNDCLASS	__declspec(dllexport)wc;
SYSTEMTIME	__declspec(dllexport)st;
CHAR		buffer[260];
HINSTANCE __declspec(dllexport)hInst;
NOTIFYICONDATAA __declspec(dllexport)nf;
RECT __declspec(dllexport)rc,rect;
LPDRAWITEMSTRUCT __declspec(dllexport)lpdis;

//direct X
LPDIRECTDRAW7			__declspec(dllexport)g_pDirectDraw;
LPDIRECTDRAWSURFACE7	__declspec(dllexport)g_pPrimarySurface;
LPDIRECTDRAWSURFACE7	__declspec(dllexport)g_pSecondarySurface;
LPDIRECTDRAWSURFACE7	__declspec(dllexport)g_pOffsreenSurface;
LPDIRECTDRAWSURFACE7	__declspec(dllexport)g_pOffsreenSurface2;
DDSURFACEDESC2			__declspec(dllexport)g_ddsd;
DDSCAPS2				__declspec(dllexport)g_ddscaps;

extern "C" const GUID IID_IDirectDrawSurface7 ={0x06675a80,0x3b9b,0x11d2,0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b };
extern "C" const GUID CLSID_DirectDraw={0xD7B70EE0,0x4340,0x11CF,0xB0,0x63,0x00,0x20,0xAF,0xC2,0xCD,0x35 } ;
extern "C" const GUID CLSID_DirectDraw7={0x3c305196,0x50db,0x11d3,0x9c,0xfe,0x00,0xc0,0x4f,0xd9,0x30,0xc5 } ;
extern "C" const GUID CLSID_DirectDrawClipper={0x593817A0,0x7DB3,0x11CF,0xA2,0xDE,0x00,0xAA,0x00,0xb9,0x33,0x56 } ;
extern "C" const GUID IID_IDirectDraw={0x6C14DB80,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60 } ;
extern "C" const GUID IID_IDirectDraw2={0xB3A6F3E0,0x2B43,0x11CF,0xA2,0xDE,0x00,0xAA,0x00,0xB9,0x33,0x56 } ;
extern "C" const GUID IID_IDirectDraw4={0x9c59509a,0x39bd,0x11d1,0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5 } ;
extern "C" const GUID IID_IDirectDraw7={0x15e65ec0,0x3b9c,0x11d2,0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b } ;
extern "C" const GUID IID_IDirectDrawSurface={0x6C14DB81,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60 } ;
extern "C" const GUID IID_IDirectDrawSurface2={0x57805885,0x6eec,0x11cf,0x94,0x41,0xa8,0x23,0x03,0xc1,0x0e,0x27 } ;
extern "C" const GUID IID_IDirectDrawSurface3={0xDA044E00,0x69B2,0x11D0,0xA1,0xD5,0x00,0xAA,0x00,0xB8,0xDF,0xBB } ;
extern "C" const GUID IID_IDirectDrawSurface4={0x0B2B8630,0xAD35,0x11D0,0x8E,0xA6,0x00,0x60,0x97,0x97,0xEA,0x5B } ;
extern "C" const GUID IID_IDirectDrawPalette={0x6C14DB84,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60 } ;
extern "C" const GUID IID_IDirectDrawClipper={0x6C14DB85,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60 } ;
extern "C" const GUID IID_IDirectDrawColorControl={0x4B9F0EE0,0x0D7E,0x11D0,0x9B,0x06,0x00,0xA0,0xC9,0x03,0xA3,0xB8 } ;
extern "C" const GUID IID_IDirectDrawGammaControl={0x69C11C3E,0xB46B,0x11D1,0xAD,0x7A,0x00,0xC0,0x4F,0xC2,0x9B,0x4E } ;
// fin DirectX

#pragma comment (lib,"comctl32.lib")
#ifdef _WIN64
#define titre "Feux Tricolores en DirectDraw Remaniement pour VS2022 64bits"
#pragma comment (lib,"ddraw64.lib")
#else
#define titre "Feux Tricolores en DirectDraw Remaniement pour VS2022 32bits"
#pragma comment (lib,"ddraw.lib")
#endif

int x,y,z;
static char jours[7][10] = {"dimanche", "lundi","mardi","mercredi","jeudi","vendredi","samedi"};
static char mois[12][10] = {"janvier", "février","mars", "avril", "mai", "juin","juillet","aout","septembre", "octobre", "novembre", "décembre"};

LRESULT CALLBACK WindowProc(HWND hDlg,UINT mssg,WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hinst,HINSTANCE hPrev,LPSTR lpcmd,int cmd);
__declspec(dllexport)int Initialize();
__declspec(dllexport)int Loop();
__declspec(dllexport)int Fermeture();
__declspec(dllexport)int getRGB16(DWORD r,DWORD g,DWORD b);
__declspec(dllexport)void implentationDlg(HWND hDlg);
__declspec(dllexport)void Demarre(HWND hDlg);
__declspec(dllexport)void Arreter();
__declspec(dllexport)void afficherMessage(HWND hDlg,LPCTSTR Message);

int WINAPI WinMain(HINSTANCE hinst,HINSTANCE hPrev,LPSTR lpcmd,int cmd){
	MSG			msg;
	int			nErreur;
	g_hInstance=hinst;
	wc.style=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_OWNDC;
	wc.lpfnWndProc=WindowProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hinst;
	wc.hIcon=LoadIcon(hinst,(LPCTSTR)114);
	wc.hCursor=LoadCursor(hinst,(LPCTSTR)112);
	wc.hbrBackground=(HBRUSH)CreateSolidBrush(RGB(202,202,202));
	wc.lpszMenuName=NULL;
	wc.lpszClassName="FTDD32";
	if(!(RegisterClass(&wc))){ return 1;}
	if(!(hMain=CreateWindow(wc.lpszClassName,titre,WS_POPUP|WS_VISIBLE,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hinst,NULL))){ return 2;}
	implentationDlg(hMain);
	if(nErreur=Initialize()){return nErreur;}
	while(!KEY_DOWN(VK_ESCAPE)){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if(msg.message==WM_QUIT){
				break;
			}
			DispatchMessage(&msg);
		}else{
			if(nErreur=Loop()){
				break;
			}
		}
	}
	Fermeture();
	ShowCursor(TRUE);
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hDlg,UINT mssg,WPARAM wParam,LPARAM lParam){
	PAINTSTRUCT ps;
	HDC			hdc;
	g_hWnd=hDlg;
	SetTimer(hDlg,IDM_TIMER1,1000,(TIMERPROC) NULL);
	switch(mssg){
		case WM_CREATE:
			SetTimer(hDlg,IDM_TIMER1,1000,(TIMERPROC) NULL); 
			return 0;
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case 1052:
					Demarre(hDlg);
					break;
				case 1054:
					Arreter();
					break;
				case IDCANCEL:
					KillTimer(hDlg,IDM_TIMER1);
					Arreter();
					PostQuitMessage(0);
					return 1;
			}
			break;
		case WM_PAINT:
			hdc=BeginPaint(g_hWnd,&ps);
			DrawIcon(hdc,0,32,wc.hIcon);
			RECT rc;
			GetClientRect(hDlg,&rc);
			GetLocalTime(&st);
			wsprintf(buffer,"Nous sommes %s, le %2d %s %4d",jours[st.wDayOfWeek],st.wDay,mois[st.wMonth-1],st.wYear); //creation du string de date
			SetDlgItemText(hDlg,1001,buffer);
			EndPaint(g_hWnd,&ps);
			return 0;
		case WM_TIMER:
			GetLocalTime(&st);
			wsprintf(buffer,"Nous sommes %s, le %2d %s %4d",jours[st.wDayOfWeek],st.wDay,mois[st.wMonth-1],st.wYear);
			SetDlgItemText(hDlg,1001,buffer);
			wsprintf(buffer,"%.2d:%.2d:%.2d",st.wHour,st.wMinute,st.wSecond);
			SetDlgItemText(hDlg,1002,buffer);
			switch (wParam)  { 
				case IDM_TIMER_R: 
					x++;
					SetDlgItemInt(hDlg,IDC_CYC_R,x,FALSE);
					afficherMessage(hDlg,NULL);
					SendMessage(imgV, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)134));	
					SendMessage(imgJ, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));	
					SendMessage(imgR, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgR2, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgL, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)118));
					afficherMessage(hDlg," Roulez");
					 return 0;
				case IDM_TIMER_V:
					y++;
					SetDlgItemInt(hDlg,IDC_CYC_V,y,FALSE);
					afficherMessage(hDlg,"");
					SendMessage(imgJ, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)133));
					SendMessage(imgV, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgR, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgRg, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)117));
					SendMessage(imgR2, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgL, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					afficherMessage(hDlg," Freinez");
					return 0;
				case IDM_TIMER_J:
					z++;
					SetDlgItemInt(hDlg,IDC_CYC_J,z,FALSE);
					afficherMessage(hDlg,"");
					SendMessage(imgR, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)132));
					SendMessage(imgR2, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)132));
					SendMessage(imgJ, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgV, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgRg, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					SendMessage(imgL, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
					afficherMessage(hDlg," Arrêtez-vous");
					return 0;
			}
			break;
		case WM_CTLCOLORDLG:
			return (long)wc.hbrBackground;
			break;
		case WM_CTLCOLORSTATIC:
			SetTextColor( (HDC)wParam, RGB(255,0, 255) );
			SetBkMode( (HDC)wParam, TRANSPARENT ); 
			return (LONG)wc.hbrBackground;
		case WM_CTLCOLOREDIT:
			SetTextColor( (HDC)wParam, RGB(0xA0,0xFF,0xAA ) );
			SetBkMode( (HDC)wParam, TRANSPARENT ); 
			return (LONG)(HBRUSH)CreateSolidBrush(RGB(0,0,128));
		case WM_CTLCOLORLISTBOX:
			SetTextColor( (HDC)wParam, RGB(0,255, 255) );
			SetBkMode( (HDC)wParam, TRANSPARENT ); 
			return (LONG)(HBRUSH)CreateSolidBrush(RGB(0,128,0));
		case WM_CTLCOLORBTN: 
			SetTextColor( (HDC)wParam, RGB(0,255, 255) );
			SetBkMode( (HDC)wParam, TRANSPARENT ); 
			return (LONG)(HBRUSH)CreateSolidBrush(RGB(128,0,0));
		case WM_CTLCOLORMSGBOX:
			return (long)(HBRUSH)CreateSolidBrush(RGB(255,0,128));
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return (DefWindowProc(hDlg,mssg,wParam,lParam));
}

__declspec(dllexport)void Demarre(HWND hDlg){
	SetDlgItemInt(hDlg,IDC_CYC_R,0,FALSE);
	SetDlgItemInt(hDlg,IDC_CYC_V,0,FALSE);
	SetDlgItemInt(hDlg,IDC_CYC_J,0,FALSE);
	SetTimer(hDlg,IDM_TIMER_R,2500,(TIMERPROC) NULL);
	SetTimer(hDlg,IDM_TIMER_V,4500,(TIMERPROC) NULL);
	SetTimer(hDlg,IDM_TIMER_J,6500,(TIMERPROC) NULL);
	imgR =CreateWindow( "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON|WS_BORDER,410, 320, 10, 10,hDlg , (HMENU)45000, wc.hInstance, NULL);
	imgR2 =CreateWindow( "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON|WS_BORDER,290,320, 10, 10,hDlg , (HMENU)45000, wc.hInstance, NULL);
	imgJ =CreateWindow( "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON|WS_BORDER,370, 320, 10, 10,hDlg, (HMENU)45000, wc.hInstance, NULL);
	imgV =CreateWindow( "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON|WS_BORDER,330, 320, 10, 10,hDlg , (HMENU)45000, wc.hInstance, NULL);
	imgL =CreateWindow( "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON|WS_BORDER,370, 360, 10, 10,hDlg , (HMENU)45000, wc.hInstance, NULL);
	imgRg =CreateWindow( "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON|WS_BORDER,330, 360, 10, 10,hDlg , (HMENU)45000, wc.hInstance, NULL);
	SendMessage(imgV, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));	
	SendMessage(imgJ, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));	
	SendMessage(imgR, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
	SendMessage(imgR2, STM_SETIMAGE, IMAGE_ICON,(LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
	SendMessage(imgL, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
	SendMessage(imgRg, STM_SETIMAGE, IMAGE_ICON,(LPARAM)LoadIcon(wc.hInstance,(LPCTSTR)116));
}
__declspec(dllexport)void Arreter(){
	KillTimer(hWnd,IDM_TIMER_R);
	KillTimer(hWnd,IDM_TIMER_V);
	KillTimer(hWnd,IDM_TIMER_J);
	DestroyWindow(imgV);
	DestroyWindow(imgR);
	DestroyWindow(imgR2);
	DestroyWindow(imgJ);
	DestroyWindow(imgRg);
	DestroyWindow(imgL);
}

__declspec(dllexport)int Initialize(){
//	ShowCursor(FALSE);
	if(FAILED(DirectDrawCreateEx(NULL,(void **)&g_pDirectDraw,IID_IDirectDraw7,NULL))){	return 1;}
	if(FAILED(g_pDirectDraw->SetCooperativeLevel(g_hWnd,DDSCL_FULLSCREEN|DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE))){return 2;}
	if(FAILED(g_pDirectDraw->SetDisplayMode(Screen_Width,Screen_Height,Color_Depth,0,0))){return 3;}
	g_ddsd.dwSize=sizeof(DDSURFACEDESC2);
	g_ddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
	g_ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
	g_ddsd.dwBackBufferCount=1;
	if(g_pDirectDraw->CreateSurface(&g_ddsd,&g_pPrimarySurface,NULL)!=DD_OK){return 4;}
	g_ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	if(g_pPrimarySurface->GetAttachedSurface(&g_ddscaps,&g_pSecondarySurface)!=DD_OK){return 5;}
	DDCOLORKEY colkey;
	colkey.dwColorSpaceHighValue=0;
	colkey.dwColorSpaceLowValue=0;
	if(FAILED(g_pSecondarySurface->SetColorKey(DDCKEY_SRCBLT,&colkey)!=DD_OK)){return 6;}
	DDPIXELFORMAT pxFmt;
	g_pPrimarySurface->GetPixelFormat(&pxFmt);
	getRGB16(pxFmt.dwRBitMask,pxFmt.dwGBitMask,pxFmt.dwBBitMask);
}
__declspec(dllexport)int Loop(){return 0;}

__declspec(dllexport)int Fermeture(){
	if(g_pPrimarySurface){
		if(FAILED(g_pPrimarySurface->Release())){return 1;}
	}
	if(g_pDirectDraw){
		if(FAILED(g_pDirectDraw->Release())){return 2;}
	}
	return 0;
}

__declspec(dllexport)int getRGB16(DWORD r,DWORD g,DWORD b){
	int numpx = 0;
	while(r>0){
		numpx++;
		r=(div(r,2).quot);
	}
	rgb16.S_Rouge=numpx-5;
	numpx=0;
	while(g>0){
		numpx++;
		g=(div(g,2).quot);
	}
	rgb16.S_Bleu=numpx-5;
	numpx=0;
	while(b>0){
		numpx++;
		b=(div(b,2).quot);
	}
	rgb16.S_Vert=numpx-5;
	numpx=0;
return 0;
}
			
__declspec(dllexport)void implentationDlg(HWND hDlg){
	CreateWindow("BUTTON","Quitter Feux Tricolores",WS_VISIBLE|WS_CHILD,630,550,160,21,hDlg , (HMENU)0x02,wc.hInstance,NULL);
	CreateWindow("STATIC",__argv[0],WS_VISIBLE|WS_CHILD|WS_BORDER,0,578,798,21,hDlg , (HMENU)1000,wc.hInstance,NULL);
	CreateWindow("STATIC","Démonstration pour l'emploi de DirectDraw",WS_VISIBLE|WS_CHILD,0,0,250,21,hDlg , (HMENU)1001,wc.hInstance,NULL);
	CreateWindow("STATIC","00:00:00",WS_VISIBLE|WS_CHILD,738,0,60,21,hDlg , (HMENU)1002,wc.hInstance,NULL);
	CreateWindow("STATIC","Prototype de Feux Tricolores en DirectDraw",WS_VISIBLE|WS_CHILD|WS_DISABLED,303,0,360,18,hDlg , (HMENU)-1,wc.hInstance,NULL);
	CreateWindow("STATIC","Compte cycles:",WS_VISIBLE|WS_CHILD,100,80,47,18,hDlg , (HMENU)-1,wc.hInstance,NULL);
	CreateWindow("STATIC","Rouge",WS_VISIBLE|WS_CHILD,100,100,60,18,hDlg , (HMENU)-1,wc.hInstance,NULL);
	CreateWindow("STATIC","Vert:",WS_VISIBLE|WS_CHILD,100,120,60,18,hDlg , (HMENU)-1,wc.hInstance,NULL);
	CreateWindow("STATIC","Jaune:",WS_VISIBLE|WS_CHILD,100,140,60,18,hDlg , (HMENU)-1,wc.hInstance,NULL);
	CreateWindow("STATIC","0",SS_CENTERIMAGE | WS_DISABLED|WS_VISIBLE|WS_CHILD,200,100,60,18,hDlg , (HMENU)IDC_CYC_R,wc.hInstance,NULL);
	CreateWindow("STATIC","0",SS_CENTERIMAGE | WS_DISABLED|WS_VISIBLE|WS_CHILD,200,120,60,18,hDlg , (HMENU)IDC_CYC_V,wc.hInstance,NULL);
	CreateWindow("STATIC","0",SS_CENTERIMAGE | WS_DISABLED|WS_VISIBLE|WS_CHILD,200,140,60,18,hDlg , (HMENU)IDC_CYC_J,wc.hInstance,NULL);
	CreateWindowEx( WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME,"STATIC","0",SS_CENTERIMAGE | WS_DISABLED|WS_VISIBLE|WS_CHILD|WS_BORDER,600,140,160,34,hDlg , (HMENU)1053,wc.hInstance,NULL);
	CreateWindow("BUTTON","Ativer",WS_VISIBLE|WS_CHILD,500,550,60,21,hDlg , (HMENU)1052,wc.hInstance,NULL);
	CreateWindow("BUTTON","Arrêter",WS_VISIBLE|WS_CHILD,560,550,60,21,hDlg , (HMENU)1054,wc.hInstance,NULL);
Demarre(hDlg);
}

__declspec(dllexport)void afficherMessage(HWND hDlg,LPCTSTR Message){
	SetWindowText(GetDlgItem(hDlg,1053),Message);
}
