// Project|Przyklad 01 Properties|Linker|Input|Additional Dependencies|Opengl32.lib Glu32.lib GlAux.lib

#define WIN32_LEAN_AND_MEAN				// "odchudza" aplikacj� Windows

// Pliki nag��wkowe
#include <windows.h>					// standardowy plik nag��wkowy Windows
#include <gl/gl.h>						// standardowy plik nag��wkowy OpenGL
#include <gl/glu.h>						// plik nag��wkowy biblioteki GLU
//#include <gl/glaux.h>					// funkcje pomocnicze OpenGL 

#pragma comment(lib, "legacy_stdio_definitions.lib")

// Zmienne globalne
HDC g_HDC;								// globalny kontekst urz�dzenia
bool fullScreen = false;				// true = tryb pe�noekranowy; false = tryb okienkowy

// Parametry o�wietlenia
float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };	    // �wiat�o otoczenia
float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };	    // �wiat�o rozproszone
float lightPosition[] = { 20.0f, 5.0f, 5.0f, 0.0f };	    // po�o�enie �r�d�a �wiat�a

void Initialize()       // Inicjacja OpenGL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// czarny kolor t�a

	glShadeModel(GL_SMOOTH);					// cieniowanie g�adkie
	glEnable(GL_DEPTH_TEST);					// w��czenie bufora g��bi
	glEnable(GL_CULL_FACE);						// ukrywanie tylnych stron wielok�t�w
	glFrontFace(GL_CCW);						// porz�dek wierzcho�k�w przeciwny do kierunku ruchu wskaz�wek zegara
	glEnable(GL_LIGHTING);						// w��czenie o�wietlenia

	// Ustawienie �r�d�a �wiat�a GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		// sk�adowa otoczenia
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		// sk�adowa rozproszona
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a

	// W��czenie �r�d�a �wiat�a
	glEnable(GL_LIGHT0);
}


void DrawCube(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot, float color[]) //Narysowanie sze�cianu w xPos,yPos,zPos obr�conego o k�ty xRot,yRot,zRot
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);	// g�rna �ciana (w p�aszczy�nie XZ)
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);	// przednia �ciana (w p�aszczy�nie XY)
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0.0f, 0.0f);	// prawa �ciana (w p�aszczy�nie YZ)
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);	// lewa �ciana (w p�aszczy�nie YZ)
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);	// dolna �ciana (w p�aszczy�nie XZ)
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);	// tylna �ciana (w p�aszczy�nie XY)
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	glPopMatrix();
}

float r = 1.0f;
float g = 1.0f;
float b = 1.0f;
float y = 1.0f;
bool isRed = true;
bool isGreen = true;
bool isYellow = true;
float colorChangeSpeed = 0.007;
int m = 1;
float X = 0, Y = 0, Z = 0, RX = 0, RY = 0, RZ = 0, rotation = 0;

void Render()   // Renderowanie sceny
{
	float green[] = { 0,g,1 - g,0 };
	float red[] = { r,0,1 - r,0 };
	float yellow[] = { y,y,1 - y,0 };
	float blue[] = { 0,0,b,0 };
	static int a = GetTickCount64();
	static bool isExpanding = true;
	static float posMultiplier = 1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Opr�nienie bufora ekranu i bufora g��bi
	glLoadIdentity();										// Zresetowanie macierzy modelowania

	glTranslatef(0, 0.0f, -9.0f);
	glRotatef(30, 1.0f, -1.0f, 0.0f);
	glRotatef(-30, 0.0f, 1.0f, 0.0f);
	glRotatef(rotation, 0.0f, 1.0f, 0.0f);

	switch (m)
	{
	case 1:
		if (GetTickCount64() - a > 20)
		{
			if (rotation >= 360.0f)
			{
				rotation = 0.0f;
			}
			rotation -= 0.6f;
			a = GetTickCount64();
		}
		break;
	case 2:
		if (GetTickCount64() - a > 20)
		{
			if (RY >= 360.0f)
			{
				RY = 0.0f;
			}
			RY = RY - 0.5f;
			a = GetTickCount64();
		}
		break;
	case 3:
		if (GetTickCount64() - a > 20)
		{
			if (isExpanding && posMultiplier < 1.7)
			{
				posMultiplier = posMultiplier + 0.01;
			}
			else if (isExpanding && posMultiplier >= 1.7)
			{
				posMultiplier = posMultiplier - 0.01;
				isExpanding = false;
			}
			else if (!isExpanding && posMultiplier > 1)
			{
				posMultiplier = posMultiplier - 0.01;
			}
			else if (!isExpanding && posMultiplier <= 1)
			{
				posMultiplier = posMultiplier + 0.01;
				isExpanding = true;
			}
			a = GetTickCount64();
		}
		break;
	case 4:
		if (GetTickCount64() - a > 1)
		{
			if (g <= 0)
				isGreen = false;
			if (g >= 1)
				isGreen = true;

			if (isGreen)
				g -= colorChangeSpeed;
			else
				g += colorChangeSpeed;

			if (g <= 0)
				isRed = false;
			if (g >= 1)
				isRed = true;

			if (isRed)
				r -= colorChangeSpeed;
			else
				r += colorChangeSpeed;

			if (g <= 0)
				isYellow = false;
			if (g >= 1)
				isYellow = true;

			if (isYellow)
				y -= colorChangeSpeed;
			else
				y += colorChangeSpeed;
		}
		a = GetTickCount64();
		break;
	}

	for (int i = -1.5; i < 1.5; i += 1.5)
	{
		for (int j = -1.5; j < 1.5; j += 1.5)
		{
			for (int k = -1.5; k < 1.5; k += 1.5)
			{
				if (i == 0 && j == 1 && k == 0)
					DrawCube(i * 1.5 * posMultiplier, j * 1.5 * posMultiplier, k * 1.5 * posMultiplier, RX, RY, RZ, red);
				else if (i == 1 && j == 1 && k == 1)
					DrawCube(i * 1.5 * posMultiplier, j * 1.5 * posMultiplier, k * 1.5 * posMultiplier, RX, RY, RZ, yellow);
				else if (i == -1 && j == 1 && k == -1)
					DrawCube(i * 1.5 * posMultiplier, j * 1.5 * posMultiplier, k * 1.5 * posMultiplier, RX, RY, RZ, green);
				else {
					DrawCube(i * 1.5 * posMultiplier, j * 1.5 * posMultiplier, k * 1.5 * posMultiplier, RX, RY, RZ, green);
				}
			}

		}

	}

	glFlush();								// Zrzucenie bufora graficznego na ekran

	SwapBuffers(g_HDC);						// Prze��czenie bufor�w
}


void SetupPixelFormat(HDC hDC)  // Funkcja okre�laj�ca format pikseli
{
	int nPixelFormat;					// indeks formatu pikseli

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
		1,								// domy�lna wersja
		PFD_DRAW_TO_WINDOW |			// grafika w oknie
		PFD_SUPPORT_OPENGL |			// grafika OpenGL 
		PFD_DOUBLEBUFFER,				// podw�jne buforowanie
		PFD_TYPE_RGBA,					// tryb kolor�w RGBA 
		32,								// 32-bitowy opis kolor�w
		0, 0, 0, 0, 0, 0,				// nie specyfikuje bit�w kolor�w
		0,								// bez bufora alfa
		0,								// nie specyfikuje bitu przesuni�cia
		0,								// bez bufora akumulacji
		0, 0, 0, 0,						// ignoruje bity akumulacji
		16,								// 16-bitowy bufor Z
		0,								// bez bufora powielania
		0,								// bez bufor�w pomocniczych
		PFD_MAIN_PLANE,					// g��wna p�aszczyzna rysowania
		0,								// zarezerwowane
		0, 0, 0 };						// ignoruje maski warstw

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// wybiera najbardziej zgodny format pikseli 
	SetPixelFormat(hDC, nPixelFormat, &pfd);		// okre�la format pikseli dla danego kontekstu urz�dzenia
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  // Procedura okienkowa
{
	static HGLRC hRC;					// kontekst tworzenia grafiki
	static HDC hDC;						// kontekst urz�dzenia
	int width, height;					// szeroko�� i wysoko�� okna

	switch (message)						// obs�uga komunikat�w
	{
	case WM_CREATE:					// Utworzenie okna
		hDC = GetDC(hwnd);			// pobiera kontekst urz�dzenia dla okna
		g_HDC = hDC;
		SetupPixelFormat(hDC);		// wywo�uje funkcj� okre�laj�c� format pikseli
		// tworzy kontekst grafiki i czyni go bie��cym
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		return 0;
		break;

	case WM_CLOSE:					// Zamkni�cie okna
		wglMakeCurrent(hDC, NULL);  // usuwa kontekst renderowania okna
		wglDeleteContext(hRC);
		PostQuitMessage(0);			// wysy�a WM_QUIT do kolejki komunikat�w
		return 0;
		break;

	case WM_SIZE:					// Zmiana wymiar�w okna
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		if (height == 0)				// zabezpieczenie przed dzieleniem przez 0
			height = 1;
		glViewport(0, 0, width, height);		// nadanie nowych wymiar�w okna OpenGL
		glMatrixMode(GL_PROJECTION);			// prze��czenie macierzy rzutowania
		glLoadIdentity();						// zresetowanie macierzy rzutowania
		gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);  // wyznaczenie proporcji obrazu i ustawienie rzutowania perspektywicznego
		glMatrixMode(GL_MODELVIEW);				// prze��czenie macierzy modelowania
		glLoadIdentity();						// zresetowanie macierzy modelowania
		return 0;
		break;
	case WM_CHAR:					// rozpoznanie przyci�ni�tego klawisza
		m = wParam - 48;
		if (m < 0 || m>9) m = 0;
		break;
	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

// Funkcja g��wna, od kt�rej rozpoczyna si� wykonywanie aplikacji
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX windowClass;		// klasa okna
	HWND	   hwnd;			// uchwyt okna
	MSG		   msg;				// komunikat
	bool	   done;			// znacznik zako�czenia aplikacji
	DWORD	   dwExStyle;		// rozszerzony styl okna
	DWORD	   dwStyle;			// styl okna
	RECT	   windowRect;		// rozmiar okna

	// parametry okna
	int width = 800;
	int height = 600;
	int bits = 32;

	// fullScreen = TRUE/FALSE;		// Prze��cznik aplikacja okienkowa/aplikacja pe�noekranowa

	windowRect.left = (long)0;						// struktura okre�laj�ca rozmiary okna
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	// definicja klasy okna
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// domy�lna ikona
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// domy�lny kursor
	windowClass.hbrBackground = NULL;								// bez t�a
	windowClass.lpszMenuName = NULL;								// bez menu
	windowClass.lpszClassName = "MojaKlasa";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		// logo Windows

	// zarejestrowanie klasy okna
	if (!RegisterClassEx(&windowClass))
		return 0;

	MessageBox(NULL, "Prze��czanie tryb�w - klawisze: 1,2,3,4", "Info - Keyboard", //wiadomo�� przed programem
		MB_OK | MB_ICONEXCLAMATION);

	if (fullScreen)								// W��czenie trybu pe�noekranowego
	{
		DEVMODE dmScreenSettings;						// tryb urz�dzenia
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;			// szeroko�� ekranu
		dmScreenSettings.dmPelsHeight = height;			// wysoko�� ekranu
		dmScreenSettings.dmBitsPerPel = bits;			// ilo�� bit�w na piksel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// je�eli prze��czenie trybu na pe�noekranowy nie powiod�o si�, prze��czenie z powrotem na tryb okienkowy
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "Prze��czenie trybu wyswietlania nie powiod�o si�", NULL, MB_OK);
			fullScreen = FALSE;
		}
	}

	if (fullScreen)								// Tryb pe�noekranowy?
	{
		dwExStyle = WS_EX_APPWINDOW;				// rozszerzony styl okna
		dwStyle = WS_POPUP;						// styl okna
		ShowCursor(FALSE);						// ukrycie kursora myszy
	}
	else										// Tryb okienkowy?
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// definicja klasy okna
		dwStyle = WS_OVERLAPPEDWINDOW;					// styl okna
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// skorygowanie rozmiaru okna
	// utworzenie okna
	hwnd = CreateWindowEx(NULL,									// styl rozszerzony
		"MojaKlasa",							// nazwa klasy
		"Sze�ciany",		// nazwa aplikacji
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,								// wsp�rz�dne x,y
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,	// szeroko��, wysoko��
		NULL,									// uchwyt okna nadrz�dnego
		NULL,									// uchwyt menu
		hInstance,							// instancja aplikacji
		NULL);

	if (!hwnd)	// sprawdzenie, czy utworzenie okna nie powiod�o si� (wtedy warto�� hwnd r�wna NULL)
		return 0;

	ShowWindow(hwnd, SW_SHOW);			// wy�wietlenie okna
	UpdateWindow(hwnd);					// aktualizacja okna

	done = false;						// inicjacja zmiennej warunku p�tli przetwarzania komunikat�w
	Initialize();						// inicjacja OpenGL

	while (!done)	// p�tla przetwarzania komunikat�w
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)		// aplikacja otrzyma�a komunikat WM_QUIT?
			done = true;				// je�li tak, to ko�czy dzia�anie
		else
		{
			Render();					// je�li nie to renderuje scen�,
			TranslateMessage(&msg);		// t�umaczy komunikat i wysy�a do systemu
			DispatchMessage(&msg);
		}
	}

	if (fullScreen)						// je�eli by� tryb pe�noekranowy
	{
		ChangeDisplaySettings(NULL, 0);	// to przywr�cenie pulpitu
		ShowCursor(TRUE);				// i wska�nika myszy
	}

	return msg.wParam;
}
