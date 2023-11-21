#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"
#include "GUI.h"
#include "Text.h"
#include "GamePadRR.h"
#include <cstdlib>


class DXRR {

private:
	int ancho;
	int alto;
public:
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState* alphaBlendState, * commonBlendState;

	int frameBillboard;
	bool isActive;
	bool isKeyboardActive;
	TerrenoRR* terreno;
	SkyDome* skydome;

	BillboardRR* Billboard;
	BillboardRR* arboles[50];
	BillboardRR* arbusto1[50];
	BillboardRR* arbusto2[50];
	BillboardRR* arbusto3[50];

	Camara* camara;

	//Modelos
	ModeloRR* gato_lampara;
	ModeloRR* hielera;
	ModeloRR* iphone;
	ModeloRR* libro_1;
	ModeloRR* libro_2;
	ModeloRR* silla_1;
	ModeloRR* silla_2;
	ModeloRR* nintendo_switch;
	ModeloRR* bote;
	ModeloRR* arma;
	ModeloRR* lancha;
	ModeloRR* building_1;
	ModeloRR* building_3;
	ModeloRR* building_5;
	ModeloRR* dirt_pile[80];

	//Gui
	GUI* vida[3];
	GUI* pickedItems[4];
	GUI* textGUI;
	Text* texto;
	Text* textoBlaco;


	bool phonePicked = false;
	bool catPicked = false;
	bool bookPicked = false;
	bool nwPicked = false;
	bool boatPicked = false;
	float boatCollision = 5.0f;

	int placeX[50];
	int placeZ[50];
	int placeXG1[50];
	int placeZG1[50];
	int placeXG2[50];
	int placeZG2[50];
	int placeXG3[50];
	int placeZG3[50];

	float randomizer[80];
	string mensaje;
	float izqder;
	float arriaba;
	float vel;
	float velIzqDer;
	bool breakpoint;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	int plusX = 490;
	int minusX = -490;
	int plusZ = 490;
	int minusZ = -490;

	XACTINDEX cueIndex;
	XACTINDEX collisionSound;
	XACTINDEX footstepsSound;
	CXACT3Util m_XACT3;

	float tiempo;
	bool tipoVista;
	bool audioTerminado;
	
    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		isActive = false;
		isKeyboardActive = false;
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		mensaje = "";
		velIzqDer = 0.0;
		//billCargaFuego();
		camara = new Camara(D3DXVECTOR3(0,80,6), D3DXVECTOR3(0,80,0), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(1000, 1000, d3dDevice, d3dContext);
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"SkyDome.jpg");
		tipoVista = false;

		for (int i = 0; i < 50; i++) {
			placeX[i] = (rand() % 450) - 450;
			placeZ[i] = (rand() % 450) - 450;
			placeXG1[i] = (rand() % 450) - 450;
			placeZG1[i] = (rand() % 450) - 450;
			placeXG2[i] = (rand() % 450) - 450;
			placeZG2[i] = (rand() % 450) - 450;
			placeXG3[i] = (rand() % 450) - 450;
			placeZG2[i] = (rand() % 450) - 450;
			arboles[i] = new BillboardRR(L"Assets/Arbol/tree_texture.png", L"Assets/Arbol/NormalMap.png", 
				d3dDevice, d3dContext, 20);
			arbusto1[i] = new BillboardRR(L"Assets/Cesped/grass.png", L"Assets/Cesped/GrassNormal.png",
				d3dDevice, d3dContext, 20);
			arbusto2[i] = new BillboardRR(L"Assets/Cesped/grass2.png", L"Assets/Cesped/Grass2Normal.png",
				d3dDevice, d3dContext, 20);
			arbusto3[i] = new BillboardRR(L"Assets/Cesped/grass3.png", L"Assets/Cesped/Grass3Normal.png",
				d3dDevice, d3dContext, 20);
		}
	
		gato_lampara = new ModeloRR(d3dDevice, d3dContext, "Assets/Gato_lampara/CatLamp.obj", L"Assets/Gato_lampara/Textures/CatBody_Emissive.png", L"Assets/Gato_lampara/Textures/CatBody_Diffuse.png", -125, -15, false);
		hielera = new ModeloRR(d3dDevice, d3dContext, "Assets/Hielera/Hielera.obj", L"Assets/Hielera/Hielera_Base.jpg", L"Assets/Hielera/Hielera_Spec.jpg", -400, 210, false);
		iphone = new ModeloRR(d3dDevice, d3dContext, "Assets/Iphone/iPhone_X.obj", L"Assets/Iphone/textures/iPhone_X_D.png", L"Assets/Iphone/textures/iPhone_X_S.png", 315, 345, false);
		libro_1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Libro_1/OldBook001.obj", L"Assets/Libro_1/OldBook001_tex.png", L"Assets/Libro_1/SpecularMap.png", 90, -400, false);
		libro_2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Libro_2/OldBook002.obj", L"Assets/Libro_2/Textures/book_Old_book_BaseColor.png", L"Assets/Libro_2/Textures/book_Old book_Roughness.png", 50, 50, false);
		silla_1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Silla/SillaMadera.obj", L"Assets/Silla/SillaMadera_Base.jpg", L"Assets/Silla/SillaMadera_Spec.png", 150, -200, false);
		silla_2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Silla_Salvavidas/SillaSalvavidas.obj", L"Assets/Silla_Salvavidas/SillaSalv_Base.jpg", L"Assets/Silla_Salvavidas/SillaSalv_Spec.png", 90, 415, false);
		nintendo_switch = new ModeloRR(d3dDevice, d3dContext, "Assets/Switch/nintendo_switch.obj", L"Assets/Switch/Textures/Nintendo_Switch_lambert1_BaseColor.png", L"Assets/Switch/Textures/Nintendo_Switch_lambert1_Roughness.png", -230, -230, false);
		bote = new ModeloRR(d3dDevice, d3dContext, "Assets/Bote/bote.obj", L"Assets/Bote/texture/bench_1_Base_color.png", L"Assets/Bote/texture/bench_1_Height.png", 80, 145, false);
		arma = new ModeloRR(d3dDevice, d3dContext, "Assets/Arma/ak47.obj", L"Assets/Arma/textures/AK_Base_color.png", L"Assets/Arma/textures/AK_Roughness.png", -350, -410, false);
		building_1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Casa_ruinas_1/Casa_ruinas_1.obj", L"Assets/Casa_ruinas_1/AO.png", L"Assets/Casa_ruinas_1/roughnesshouse.png", 90, 254, false);
		building_3 = new ModeloRR(d3dDevice, d3dContext, "Assets/Wood_house/Woodhouse.obj", L"Assets/Wood_house/WoodHouseDiffuse.png", L"Assets/Wood_house/WoodHouse_Specular.png", 448, 290, false);
		building_5 = new ModeloRR(d3dDevice, d3dContext, "Assets/wood_tower/torreMadera.obj", L"Assets/wood_tower/SillaSalv_Base.jpg", L"Assets/wood_tower/SillaSalv_Spec.png", -240, 412, false);
		for (int i = 0; i < 20; i++){
			randomizer[i] = rand() % 359;
			dirt_pile[i] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/JunkPile01_Obj.obj",
				L"Assets/Tierra/JunkPile01_Base-Diffuse1.png", L"Assets/Tierra/JunkPile01_Base-Specular.png", minusX, plusZ, false);
			minusX += 50;
		}
		minusX = -490;
		for (int i = 20; i < 40; i++){
			randomizer[i] = rand() % 359;
			dirt_pile[i] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/JunkPile01_Obj.obj",
				L"Assets/Tierra/JunkPile01_Base-Diffuse1.png", L"Assets/Tierra/JunkPile01_Base-Specular.png", plusX, plusZ, false);
			plusZ -= 50;
		}
		plusZ = 490;
		for (int i = 40; i < 60; i++) {
			randomizer[i] = rand() % 359;
			dirt_pile[i] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/JunkPile01_Obj.obj",
				L"Assets/Tierra/JunkPile01_Base-Diffuse1.png", L"Assets/Tierra/JunkPile01_Base-Specular.png", plusX, minusZ, false);
			plusX -= 50;
		}
		plusX = 490;
		for (int i = 60; i < 80; i++) {
			randomizer[i] = rand() % 359;
			dirt_pile[i] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/JunkPile01_Obj.obj",
				L"Assets/Tierra/JunkPile01_Base-Diffuse1.png", L"Assets/Tierra/JunkPile01_Base-Specular.png", minusX, minusZ, false);
			minusZ += 50;
		}

		vida[0] = new GUI(d3dDevice, d3dContext, 0.15, 0.25, L"Assets/GUI/health_full.png");
		vida[1] = new GUI(d3dDevice, d3dContext, 0.15, 0.25, L"Assets/GUI/health_2.png");
		vida[2] = new GUI(d3dDevice, d3dContext, 0.15, 0.25, L"Assets/GUI/health_1.png");
		pickedItems[0] = new GUI(d3dDevice, d3dContext, 0.11, 0.06, L"Assets/Items/cat_icon.png");
		pickedItems[1] = new GUI(d3dDevice, d3dContext, 0.11, 0.06, L"Assets/Items/book_icon.png");
		pickedItems[2] = new GUI(d3dDevice, d3dContext, 0.11, 0.06, L"Assets/Items/nw_icon.png");
		pickedItems[3] = new GUI(d3dDevice, d3dContext, 0.11, 0.06, L"Assets/Items/phone_icon.png");
		textGUI = new GUI(d3dDevice, d3dContext, 0.5, 1.5, L"Assets/GUI/DSI-Text_GUI.png");
		textoBlaco = new Text(d3dDevice, d3dContext, 2.0, 1.0, L"Assets/GUI/font_v2.png", XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
		texto = new Text(d3dDevice, d3dContext, 2.0, 1.0, L"Assets/GUI/font.png", XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
		tiempo = 300;
		audioTerminado = false;
	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{

		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{
			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		//Audio
		bool res = m_XACT3.Initialize();
		if (!res)
			return res;

		res = m_XACT3.LoadWaveBank(L"Assets\\Sonido\\Win\\Wave_Bank.xwb");
		if (!res)
			return res;

		res = m_XACT3.LoadSoundBank(L"Assets\\Sonido\\Win\\Sound_Bank.xsb");
		if (!res)
			return res;

		//Reproducir audio
		cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("background01");
		collisionSound = m_XACT3.m_pSoundBank->GetCueIndex("colission02");;
		footstepsSound = m_XACT3.m_pSoundBank->GetCueIndex("footsteps01");;
		m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);

		return true;				
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		float sphere[3] = { 0,0,0 };
		boatCollision = 3.0f;
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if( d3dContext == 0 )
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 5 ;
		camara->posCam3p.y = terreno->Superficie(camara->posCam3p.x, camara->posCam3p.z) + 10;

#pragma region Collisions
		//Gato lampara
		//bool collisionGL = isPointInsideSphere(camara->getPoint(), gato_lampara->getSphere(3));
		if (!isPointInsideSphere(camara->getPoint(), gato_lampara->getSphere(7)) ) {
			gato_lampara->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado){
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}			
			gato_lampara->isActive = true;
			mensaje = "Es una lampara gato. Presiona [E] para recoger el objeto.";
			if (gato_lampara->isActive && isKeyboardActive) {
				mensaje = "Es un gato. Si hubiera baterias, podria encenderlo.";			
				catPicked = true;
			}
		}

		//Iphone
		//bool collisionP = isPointInsideSphere(camara->getPoint(), iphone->getSphere(3));
		if (!isPointInsideSphere(camara->getPoint(), iphone->getSphere(7))) {
			iphone->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			iphone->isActive = true;
			mensaje = "Es un celular. Presiona [E] para recoger el objeto.";
			if (iphone->isActive && isKeyboardActive) {
				mensaje = "Es un celular.";
				phonePicked = true;
			}	
		}

		////Libro
		//bool collisionL = isPointInsideSphere(camara->getPoint(), libro_1->getSphere(3));
		if (!isPointInsideSphere(camara->getPoint(), libro_1->getSphere(5))){
			libro_1->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			libro_1->isActive = true;
			mensaje = "Es un libro. Presiona [E] para recoger el objeto.";
			if (libro_1->isActive && isKeyboardActive){
				mensaje = "Es un libro viejo. Tenía una nota para una persona...";
				bookPicked = true;
			}
		}

		//Nintendo Switch
		if (!isPointInsideSphere(camara->getPoint(), nintendo_switch->getSphere(7))) {
			nintendo_switch->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			nintendo_switch->isActive = true;
			mensaje = "Es una consola de videojuegos. Presiona [E] para recoger el objeto.";
			if (nintendo_switch->isActive && isKeyboardActive) {
				mensaje = "Parece tener aun poca bateria. Al revisar ";
				nwPicked = true;
			}
		}

		//Hielera
		if (!isPointInsideSphere(camara->getPoint(), hielera->getSphere(8))) {
			hielera->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			hielera->isActive = true;
			mensaje = "Es una hielera. Presiona [E] para abrirla el objeto.";
			if (hielera->isActive && isKeyboardActive) {
				mensaje = "Contiene restos de comida. Huele bastante mal... ";
			}
		}

		//Silla_1
		silla_1;
		if (!isPointInsideSphere(camara->getPoint(), silla_1->getSphere(8))) {
			silla_1->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			silla_1->isActive = true;
			mensaje = "Es una sila de madera. Presiona [E] para investigarla el objeto.";
			if (silla_1->isActive && isKeyboardActive) {
				mensaje = "No parece contener nada especial... ";
			}
		}

		//Bote
		if (boatCollision != 0.0f)		{
			if (!isPointInsideSphere(camara->getPoint(), bote->getSphere(5))) {
				bote->isActive = false;
			}
			else {
				//camara->posCam = camara->posCamPast;
				if (!audioTerminado) {
					m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
					audioTerminado = true;
				}
				bote->isActive = true;
				mensaje = "Es un bote de madera. Presiona [E] para subirte en el.";
				if (bote->isActive && isKeyboardActive) {
					boatCollision = 0.0f;
					boatPicked = true;
				}
			}
		}		

		//Arma 
		if (!isPointInsideSphere(camara->getPoint(), arma->getSphere(8))) {
			arma->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			arma->isActive = true;
			mensaje = "Es un arma vieja. Presiona [E] para investigarla.";
			if (arma->isActive && isKeyboardActive) {
				mensaje = "No parece contener nada especial... ";
			}
		}

		//Libro 2 
		if (!isPointInsideSphere(camara->getPoint(), libro_2->getSphere(5))) {
			libro_2->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			libro_2->isActive = true;
			mensaje = "Parece un libro viejo. Presiona [E] para investigarla.";
			if (libro_2->isActive && isKeyboardActive) {
				mensaje = "Sus paginas estan arrancadas. Parece ser un libro de fisica.";
			}
		}

		//Silla_2
		if (!isPointInsideSphere(camara->getPoint(), silla_2->getSphere(8))) {
			silla_2->isActive = false;
		}
		else {
			camara->posCam = camara->posCamPast;
			if (!audioTerminado) {
				m_XACT3.m_pSoundBank->Play(collisionSound, 0, 0, 0);
				audioTerminado = true;
			}
			silla_2->isActive = true;
			mensaje = "Parece una silla. Presiona [E] para investigarla.";
			if (silla_2->isActive && isKeyboardActive) {
				mensaje = "Parece rota y muy vieja. No parace contener nada.";
			}
		}

		//Montañas
		for (int i = 0; i < 80; i++){
			if (!isPointInsideSphere(camara->getPoint(), dirt_pile[i]->getSphere(40))) {
				dirt_pile[i]->isActive = false;
			}
			else {
				camara->posCam = camara->posCamPast;
				dirt_pile[i]->isActive = true;
				mensaje = "No puedes escalar la montania...";
			}
		}

		if (!libro_1->isActive || !iphone->isActive || !gato_lampara->isActive || !nintendo_switch->isActive || !silla_2->isActive
			|| !silla_1->isActive || !hielera->isActive || !bote->isActive || !arma->isActive || !libro_2->isActive)
				camara->UpdateCam(vel, velIzqDer, arriaba, izqder);
		

#pragma endregion

		//camara->UpdateCam(vel, arriaba, izqder);
		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion);
		//TurnOnAlphaBlending();
		//billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			//-11, -78, 4, 5, uv1, uv2, uv3, uv4, frameBillboard);
		/*arbol->Draw(camara->vista, camara->proyeccion, camara->posCam, 0, 0, 
			terreno->Superficie(0, 0), 5, false);*/
		for (int i = 0; i < 50; i++) {
			//D3DXMatrixRotationX();
			arboles[i]->Draw(camara->vista, camara->proyeccion, camara->posCam, this->placeX[i], this->placeZ[i],
				terreno->Superficie(0, 0) - 3, 15, tipoVista);
			arbusto1[i]->Draw(camara->vista, camara->proyeccion, camara->posCam, this->placeXG1[i], this->placeZG1[i],
				terreno->Superficie(0, 0) - 3, 5, tipoVista);
			arbusto2[i]->Draw(camara->vista, camara->proyeccion, camara->posCam, this->placeXG2[i], this->placeZG2[i],
				terreno->Superficie(0, 0) - 3, 5, tipoVista);
			arbusto3[i]->Draw(camara->vista, camara->proyeccion, camara->posCam, this->placeXG3[i], this->placeZG2[i],
				terreno->Superficie(0, 0) - 3, 5, tipoVista);
		}

		gato_lampara->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) + 1, camara->posCam, 10.0f, 0, 'A', 0.25);
		hielera->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 1, camara->posCam, 10.0f, 0, 'A', 0.25);
		iphone->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) + 2, camara->posCam, 10.0f, 120, 'X', 0.25);
		libro_1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 5, camara->posCam, 10.0f, 0, 'A', 0.75);
		libro_2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 1.2, camara->posCam, 10.0f, 0, 'A', 3);
		silla_1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) + 2, camara->posCam, 10.0f, 0, 'A', 0.25);
		silla_2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 10, camara->posCam, 10.0f, 0, 'A', 0.25);
		nintendo_switch->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 1, camara->posCam, 10.0f, 90, 'Z', 1);
		bote->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 1, camara->posCam, 10.0f, 0, 'A', 2);
		arma->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 120, 'X', 1);
		for (int j = 0; j < 80; j++){
			dirt_pile[j]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 11, camara->posCam, 10.0f, randomizer[j], 'Y', 1);
		}

		building_1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 20, camara->posCam, 10.0f, 0, 'A', 5);
		building_3->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 8, camara->posCam, 10.0f, 0, 'A', 5);
		building_5->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 5, camara->posCam, 10.0f, 0, 'A', 5);

		tiempo -= 0.01;
		vida[0]->Draw(-0.8, 0.8);

		//Objetivos logrados
		if (catPicked) {
			pickedItems[0]->Draw(-0.8, 0.6);
		}
		if (bookPicked) {
			pickedItems[1]->Draw(-0.7, 0.6);
		}
		if (nwPicked) {
			pickedItems[2]->Draw(-0.6, 0.6);
		}
		if (phonePicked) {
			pickedItems[3]->Draw(-0.9, 0.6);
		}

		/*if (catPicked && bookPicked && nwPicked && phonePicked) {

		}*/

		//Dibujo el vehículo
		if (boatPicked) {
			 lancha->setPosX(camara->hdveo.x);
			 lancha->setPosZ(camara->hdveo.z);
			 lancha->Draw(camara->vista, camara->proyeccion, 
				 terreno->Superficie(bote->getPosX(), bote->getPosZ()), camara->posCam, 30.0f, XM_PI, 'Y', 5, true, tipoVista);
		}

		//Dibujo de GUI y mensajes
		TurnOnAlphaBlending();
		texto->DrawText(-0.9f, 0.9f, "Vida: ", 0.009f);
		textoBlaco->DrawText(-0.8f, -0.3f, mensaje, 0.009f);
		if (gato_lampara->isActive || iphone->isActive || libro_1->isActive || nintendo_switch->isActive || silla_2->isActive
			 || silla_1->isActive || hielera->isActive || bote->isActive || arma->isActive || libro_2->isActive)
				textGUI->Draw(-0.2f, -0.3f);
		texto->DrawText(0.2f, 0.8f, "Tiempo: " + texto->Time(tiempo), 0.009f);
		TurnOffAlphaBlending();

		swapChain->Present( 1, 0 );
		mensaje = "";
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1])) /* +
			(point[1] - sphere[1]) * (point[1] - sphere[1]))*/;

		if (distance <= sphere[2])
			collition = true;

		if (distance > 25)
			audioTerminado = false;

		return collition;
	}
	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}
	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

};
#endif