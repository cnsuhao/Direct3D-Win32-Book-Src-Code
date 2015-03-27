//-----------------------------------������˵����----------------------------------------------
//  ��������:��D3Ddemo16
//	 2013��4�� Create by ǳī
//  ��������������ά�ռ䣺��һ�˳��������ʵ�� ʾ������
//  ͼ���زĳ����� ħ������3�������� Dota
//  ���������زĳ���������-����
//  ����ģ���زĳ�������������˫6 ��Ԫ��  ���ư�
//------------------------------------------------------------------------------------------------


//-----------------------------------���궨�岿�֡�--------------------------------------------
// ����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	932						//Ϊ���ڿ��ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ���
#define WINDOW_HEIGHT	700							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	_T("������������Ϩ�����Ϸ�������롿 ��������ά�ռ䣺��һ�˳��������ʵ��") //Ϊ���ڱ��ⶨ��ĺ�


//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------                                                                                     
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include "DirectInputClass.h"
#include "CameraClass.h"


//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------  
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // ʹ��DirectInput��������Ŀ��ļ���ע��������8
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 


//���嶥��ṹ��
struct CUSTOMVERTEX
{
	FLOAT _x,  _y,  _z;
	FLOAT _nx, _ny, _nz;
	FLOAT _u,  _v;
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, 
		FLOAT nx, FLOAT ny, FLOAT nz, FLOAT u, FLOAT v)
	{
		_x  = x,  _y  = y,  _z  = z;
		_nx = nx, _ny = ny, _nz = nz;
		_u  = u,  _v  = v;
	}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)


//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9					g_pd3dDevice = NULL; //Direct3D�豸����
LPD3DXFONT								g_pTextFPS				=NULL;    //����COM�ӿ�
LPD3DXFONT								g_pTextAdaperName           = NULL;  // �Կ���Ϣ��2D�ı�
LPD3DXFONT								g_pTextHelper          = NULL;  // ������Ϣ��2D�ı�
LPD3DXFONT								g_pTextInfor           = NULL;  // ������Ϣ��2D�ı�
float											g_FPS								= 0.0f;       //һ�������͵ı���������֡����
wchar_t										g_strFPS[50]={0};    //����֡���ʵ��ַ�����
wchar_t										g_strAdapterName[60]={0};    //�����Կ����Ƶ��ַ�����
D3DXMATRIX							g_matWorld;   //�������
DInputClass*								g_pDInput = NULL;         //һ��DInputClass���ָ��
CameraClass*							g_pCamera  = NULL;
LPD3DXMESH							g_pMesh     = NULL; // �������
D3DMATERIAL9*						g_pMaterials    = NULL; // ����Ĳ�����Ϣ
LPDIRECT3DTEXTURE9*				g_pTextures     = NULL; // �����������Ϣ
DWORD									g_dwNumMtrls    = 0;    // ���ʵ���Ŀ
LPD3DXMESH							g_cylinder = NULL;     //�����������
D3DMATERIAL9							g_MaterialCylinder; //����
LPDIRECT3DVERTEXBUFFER9		g_pVertexBuffer           = NULL;  //���ƲݵصĶ��㻺�����
LPDIRECT3DTEXTURE9				g_pTexture             = NULL;  //���Ʋݵص���������


//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK		WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
HRESULT						Direct3D_Init(HWND hwnd,HINSTANCE hInstance);
HRESULT						Objects_Init();
void								Direct3D_Render( HWND hwnd);
void								Direct3D_Update( HWND hwnd);
void								Direct3D_CleanUp( );
float								Get_FPS();
void								HelpText_Render(HWND hwnd);

//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{

	//��ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };				//��WINDCLASSEX������һ�������࣬����wndClassʵ������WINDCLASSEX������֮�󴰿ڵĸ����ʼ��    
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;	//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;				//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance = hInstance;				//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon=(HICON)::LoadImage(NULL,_T("icon.ico"),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); //��ȫ�ֵ�::LoadImage�����ӱ��ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = _T("ForTheDreamOfGameDevelop");		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	HWND hwnd = CreateWindow( _T("ForTheDreamOfGameDevelop"),WINDOW_TITLE,			//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );


	//Direct3D��Դ�ĳ�ʼ��������ʧ����messagebox������ʾ
	if (!(S_OK==Direct3D_Init (hwnd,hInstance)))
	{
		MessageBox(hwnd, _T("Direct3D��ʼ��ʧ��~��"), _T("ǳī����Ϣ����"), 0); //ʹ��MessageBox����������һ����Ϣ���� 
	}
	PlaySound(L"���� - ����.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);   //ѭ�����ű�������

	

	MoveWindow(hwnd,200,10,WINDOW_WIDTH,WINDOW_HEIGHT,true);   //����������ʾʱ��λ�ã��������Ͻ�λ����Ļ���꣨200��10����
	ShowWindow( hwnd, nShowCmd );    //����Win32����ShowWindow����ʾ����
	UpdateWindow(hwnd);  //�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//����DirectInput��ĳ�ʼ��
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd,hInstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//��Ϣѭ������
	MSG msg = { 0 };  //��ʼ��msg
	while( msg.message != WM_QUIT )			//ʹ��whileѭ��
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );		//�ú����ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			Direct3D_Update(hwnd);         //���ø��º��������л���ĸ���
			Direct3D_Render(hwnd);			//������Ⱦ���������л������Ⱦ			
		}
	}

	UnregisterClass(_T("ForTheDreamOfGameDevelop"), wndClass.hInstance);
	return 0;  
}



//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )   //���ڹ��̺���WndProc
{
	switch( message )				//switch��俪ʼ
	{
	case WM_PAINT:					 // �ͻ����ػ���Ϣ
		Direct3D_Render(hwnd);          //����Direct3D_Render���������л���Ļ���
		ValidateRect(hwnd, NULL);   // ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:                // ���̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ESC��
			DestroyWindow(hwnd);    // ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;
	case WM_DESTROY:				//����������Ϣ
		Direct3D_CleanUp();     //����Direct3D_CleanUp����������COM�ӿڶ���
		PostQuitMessage( 0 );		//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;						//������switch���

	default:						//������case�����������ϣ���ִ�и�default���
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ�����ΪӦ�ó���û�д����Ĵ�����Ϣ�ṩȱʡ�Ĵ�����
	}

	return 0;					//�����˳�
}


//-----------------------------------��Direct3D_Init( )������----------------------------------
//	������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd,HINSTANCE hInstance)
{

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
 			return E_FAIL;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
		{
			return E_FAIL;
		}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò���������������

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = WINDOW_WIDTH;
	d3dpp.BackBufferHeight           = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 2;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	//��ȡ�Կ���Ϣ��g_strAdapterName�У������Կ�����֮ǰ���ϡ���ǰ�Կ��ͺţ����ַ���
	 wchar_t TempName[60]=L"��ǰ�Կ��ͺţ�";   //����һ����ʱ�ַ������ҷ����˰�"��ǰ�Կ��ͺţ�"�ַ����������ǵ�Ŀ���ַ�����
	 D3DADAPTER_IDENTIFIER9 Adapter;  //����һ��D3DADAPTER_IDENTIFIER9�ṹ�壬���ڴ洢�Կ���Ϣ
	 pD3D->GetAdapterIdentifier(0,0,&Adapter);//����GetAdapterIdentifier����ȡ�Կ���Ϣ
	 int len = MultiByteToWideChar(CP_ACP,0, Adapter.Description, -1, NULL, 0);//�Կ����������Ѿ���Adapter.Description���ˣ�������Ϊchar���ͣ�����Ҫ����תΪwchar_t����
	 MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);//�ⲽ������ɺ�g_strAdapterName�о�Ϊ��ǰ���ǵ��Կ���������wchar_t���ַ�����
	 wcscat_s(TempName,g_strAdapterName);//�ѵ�ǰ���ǵ��Կ����ӵ�����ǰ�Կ��ͺţ����ַ������棬�������TempName��
	 wcscpy_s(g_strAdapterName,TempName);//��TempName�еĽ��������ȫ�ֱ���g_strAdapterName�У��󹦸��~

	if(!(S_OK==Objects_Init())) return E_FAIL;

	SAFE_RELEASE(pD3D) //LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

	return S_OK;
}

//-----------------------------------��Object_Init( )������--------------------------------------
//	��������Ⱦ��Դ��ʼ���������ڴ˺����н���Ҫ����Ⱦ���������Դ�ĳ�ʼ��
//--------------------------------------------------------------------------------------------------
HRESULT Objects_Init()
{
	//��������
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"��������", &g_pTextAdaperName); 
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"΢���ź�", &g_pTextHelper); 
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"����", &g_pTextInfor); 

	// ��X�ļ��м�����������
	LPD3DXBUFFER pAdjBuffer  = NULL;
	LPD3DXBUFFER pMtrlBuffer = NULL;

	D3DXLoadMeshFromX(L"WYJ.X", D3DXMESH_MANAGED, g_pd3dDevice, 
		&pAdjBuffer, &pMtrlBuffer, NULL, &g_dwNumMtrls, &g_pMesh);

	// ��ȡ���ʺ���������
	D3DXMATERIAL *pMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer(); //����һ��D3DXMATERIAL�ṹ�����ڶ�ȡ���ʺ�������Ϣ
	g_pMaterials = new D3DMATERIAL9[g_dwNumMtrls];
	g_pTextures  = new LPDIRECT3DTEXTURE9[g_dwNumMtrls];

	for (DWORD i=0; i<g_dwNumMtrls; i++) 
	{
		//��ȡ���ʣ�������һ�»��������ɫֵ
		g_pMaterials[i] = pMtrls[i].MatD3D;
		g_pMaterials[i].Ambient = g_pMaterials[i].Diffuse;

		//����һ����������
		g_pTextures[i]  = NULL;
		D3DXCreateTextureFromFileA(g_pd3dDevice, pMtrls[i].pTextureFilename, &g_pTextures[i]);
	}
	SAFE_RELEASE(pAdjBuffer)
	SAFE_RELEASE(pMtrlBuffer)


	// ����һƬ��ƺ,50X50=250������
	g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, 
	D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, 0);

	CUSTOMVERTEX *pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-500.0f, 0.0f, -500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 50.0f);
	pVertices[1] = CUSTOMVERTEX(-500.0f, 0.0f,  500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	pVertices[2] = CUSTOMVERTEX( 500.0f, 0.0f, -500.0f, 0.0f, 1.0f, 0.0f, 50.0f, 50.0f);
	pVertices[3] = CUSTOMVERTEX( 500.0f, 0.0f,  500.0f, 0.0f, 1.0f, 0.0f, 50.0f, 0.0f);
	g_pVertexBuffer->Unlock();

	// �����ذ�����
	D3DXCreateTextureFromFile(g_pd3dDevice, L"grass.jpg", &g_pTexture);

	//��������
	D3DXCreateCylinder(g_pd3dDevice, 10.0f, 10.0f, 500.0f, 60, 60,  &g_cylinder, 0);
	g_MaterialCylinder.Ambient  = D3DXCOLOR(0.9f, 0.0f, 0.8f, 1.0f);  
	g_MaterialCylinder.Diffuse  = D3DXCOLOR(0.9f, 0.0f, 0.8f, 1.0f);  
	g_MaterialCylinder.Specular = D3DXCOLOR(0.9f, 0.2f, 0.9f, 0.9f);  
	g_MaterialCylinder.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.9f, 1.0f);

	// ���ù���  
	D3DLIGHT9 light;  
	::ZeroMemory(&light, sizeof(light));  
	light.Type          = D3DLIGHT_DIRECTIONAL;  
	light.Ambient       = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);  
	light.Diffuse       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  
	light.Specular      = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);  
	light.Direction     = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  
	g_pd3dDevice->SetLight(0, &light);  
	g_pd3dDevice->LightEnable(0, true);  
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);  
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	// ��������ʼ�����������
	g_pCamera = new CameraClass(g_pd3dDevice);
	g_pCamera->SetCameraPosition(&D3DXVECTOR3(0.0f, 200.0f, -300.0f));  //������������ڵ�λ��
	g_pCamera->SetTargetPosition(&D3DXVECTOR3(0.0f, 300.0f, 0.0f));  //����Ŀ��۲�����ڵ�λ��
	g_pCamera->SetViewMatrix();  //����ȡ���任����
	g_pCamera->SetProjMatrix();  //����ͶӰ�任����

	// �����������˺�����Ѱַ��ʽ
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}


//-----------------------------------��Direct3D_Update( )������--------------------------------
//	���������Ǽ�ʱ��Ⱦ���뵫����Ҫ��ʱ���õģ��簴���������ĸ��ģ�����������
//--------------------------------------------------------------------------------------------------
void	Direct3D_Update( HWND hwnd)
{
	//ʹ��DirectInput���ȡ����
	g_pDInput->GetInput();

	// ��������������ƶ��ӽ�
	if (g_pDInput->IsKeyDown(DIK_A))  g_pCamera->MoveAlongRightVec(-0.3f);
	if (g_pDInput->IsKeyDown(DIK_D))  g_pCamera->MoveAlongRightVec( 0.3f);
	if (g_pDInput->IsKeyDown(DIK_W)) g_pCamera->MoveAlongLookVec( 0.3f);
	if (g_pDInput->IsKeyDown(DIK_S))  g_pCamera->MoveAlongLookVec(-0.3f);
	if (g_pDInput->IsKeyDown(DIK_R))  g_pCamera->MoveAlongUpVec( 0.3f);
	if (g_pDInput->IsKeyDown(DIK_F))  g_pCamera->MoveAlongUpVec(-0.3f);

	//���������������ת�ӽ�
	if (g_pDInput->IsKeyDown(DIK_LEFT))  g_pCamera->RotationUpVec(-0.003f);
	if (g_pDInput->IsKeyDown(DIK_RIGHT))  g_pCamera->RotationUpVec( 0.003f);
	if (g_pDInput->IsKeyDown(DIK_UP))  g_pCamera->RotationRightVec(-0.003f);
	if (g_pDInput->IsKeyDown(DIK_DOWN))  g_pCamera->RotationRightVec( 0.003f);
	if (g_pDInput->IsKeyDown(DIK_Q)) g_pCamera->RotationLookVec( 0.001f);
	if (g_pDInput->IsKeyDown(DIK_E)) g_pCamera->RotationLookVec(-0.001f);


	//������������������������ת
	g_pCamera->RotationUpVec(g_pDInput->MouseDX()* 0.001f);
	g_pCamera->RotationRightVec(g_pDInput->MouseDY() * 0.001f);

	//�����ֿ��ƹ۲����������
	static FLOAT fPosZ=0.0f;
	fPosZ += g_pDInput->MouseDZ()*0.03f;

	//���㲢����ȡ���任����
	D3DXMATRIX matView;
	g_pCamera->CalculateViewMatrix(&matView);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	//����ȷ������任����浽g_matWorld��
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);

	//������δ�����������������ƶ�����
	POINT lt,rb;
	RECT rect;
	GetClientRect(hwnd,&rect);  //ȡ�ô����ڲ�����
	//���������ϵ��������lt��
	lt.x = rect.left;
	lt.y = rect.top;
	//�����������������rb��
	rb.x = rect.right;
	rb.y = rect.bottom;
	//��lt��rb�Ĵ�������ת��Ϊ��Ļ����
	ClientToScreen(hwnd,&lt);
	ClientToScreen(hwnd,&rb);
	//����Ļ���������趨��������
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//����������ƶ�����
	ClipCursor(&rect);

	ShowCursor(false);		//���������
}


//-----------------------------------��Direct3D_Render( )������-------------------------------
//	������ʹ��Direct3D������Ⱦ
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮һ������������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(50, 100, 250), 1.0f, 0);

	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮��������ʼ����
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();                     // ��ʼ����
	
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮��������ʽ����
	//--------------------------------------------------------------------------------------

	//��������
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &g_matWorld);//����ģ�͵��������Ϊ������׼��
	// ��һ��forѭ��������ģ�͵�����������ֵĻ���
	for (DWORD i = 0; i < g_dwNumMtrls; i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMaterials[i]);  //���ô˲��ֵĲ���
		g_pd3dDevice->SetTexture(0, g_pTextures[i]);//���ô˲��ֵ�����
		g_pMesh->DrawSubset(i);  //���ƴ˲���
	}


	// ���Ʋ�ƺ
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pd3dDevice->SetTexture(0, g_pTexture);
	g_pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	//��������
	D3DXMATRIX TransMatrix, RotMatrix, FinalMatrix;
	D3DXMatrixRotationX(&RotMatrix, -D3DX_PI * 0.5f);
	g_pd3dDevice->SetMaterial(&g_MaterialCylinder);
	for(int i = 0; i < 6; i++)
	{
		D3DXMatrixTranslation(&TransMatrix, -100.0f, 0.0f, -150.0f + (i * 75.0f));
		FinalMatrix = RotMatrix * TransMatrix ;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_cylinder->DrawSubset(0);

		D3DXMatrixTranslation(&TransMatrix, 100.0f, 0.0f, -150.0f + (i * 75.0f));
		FinalMatrix = RotMatrix * TransMatrix ;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_cylinder->DrawSubset(0);
	}

	HelpText_Render(hwnd);


	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�ġ�����������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // ��������
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�塿����ʾ��ת
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // ��ת����ʾ
	 
}

//-----------------------------------��HelpText_Render( )������-------------------------------
//	��������װ�˰�����Ϣ�ĺ���
//--------------------------------------------------------------------------------------------------
void HelpText_Render(HWND hwnd)
{
	//����һ�����Σ����ڻ�ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//�ڴ������ϽǴ�����ʾÿ��֡��
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));

	//��ʾ�Կ�������
	g_pTextAdaperName->DrawText(NULL,g_strAdapterName, -1, &formatRect, 
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	// ���������Ϣ
	formatRect.left = 0,formatRect.top = 380;
	g_pTextInfor->DrawText(NULL, L"����˵��:", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(235,123,230,255));
	formatRect.top += 35;
	g_pTextHelper->DrawText(NULL, L"    W����ǰ����     S�������� ", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    A���������     D�����ҷ���", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    R����ֱ���Ϸ���     F����ֱ���·���", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    Q��������б       E��������б", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    �ϡ��¡����ҷ����������ƶ����ӽǱ仯 ", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"     �����֣�����ģ��Y�᷽���ƶ�", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ESC�� : �˳�����", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
}


//-----------------------------------��Get_FPS( )������------------------------------------------
//	���������ڼ���ÿ��֡���ʵ�һ������
//--------------------------------------------------------------------------------------------------
float Get_FPS()
{

	//�����ĸ���̬����
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int    frameCount = 0;//֡��
	static float  currentTime =0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

	//�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if(currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount /(currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount    = 0;//������֡��frameCountֵ����
	}

	return fps;
}


//-----------------------------------��Direct3D_CleanUp( )������--------------------------------
//	��������Direct3D����Դ�����������ͷ�COM�ӿڶ���
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{

	//�ͷ�COM�ӿڶ���
	for (DWORD i = 0; i<g_dwNumMtrls; i++) 
		SAFE_RELEASE(g_pTextures[i]);
	SAFE_DELETE(g_pTextures); 
	SAFE_DELETE(g_pMaterials); 
	SAFE_DELETE(g_pDInput);
	SAFE_RELEASE(g_cylinder);
	SAFE_RELEASE(g_pMesh);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pTextAdaperName)
	SAFE_RELEASE(g_pTextHelper)
	SAFE_RELEASE(g_pTextInfor)
	SAFE_RELEASE(g_pTextFPS)
	SAFE_RELEASE(g_pd3dDevice)
}