#define WIN32_LEAN_AND_MEAN				// "odchudza" aplikacj� Windows

// Pliki nag��wkowe
#include <windows.h>					// standardowy plik nag��wkowy Windows
#include <gl/gl.h>						// standardowy plik nag��wkowy OpenGL
#include <gl/glu.h>						// plik nag��wkowy biblioteki GLU
#include <math.h>
//#include <gl/glaux.h>					// funkcje pomocnicze OpenGL 

#pragma comment(lib, "legacy_stdio_definitions.lib")

// Zmienne globalne
HDC g_HDC;								// globalny kontekst urz�dzenia
bool fullScreen = false;				// true = tryb pe�noekranowy; false = tryb okienkowy

// Parametry o�wietlenia
float ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };	    // �wiat�o otoczenia
float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };	    // �wiat�o rozproszone
float lightPosition[] = { 0.0f, 0.0f, 1.0f, 1.0f };	    // po�o�enie �r�d�a �wiat�a
int m = 1;
// Parametry materia�u


GLenum PolygonMode = GL_FILL;  // wyb�r trybu rysowania wielok�t�w (GL_FILL, GL_LINE, GL_POINT)

void Initialize()       // Inicjacja OpenGL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// czarny kolor t�a

	glShadeModel(GL_SMOOTH);					// cieniowanie g�adkie
	glEnable(GL_DEPTH_TEST);					// w��czenie bufora g��bi
	glEnable(GL_CULL_FACE);						// ukrywanie tylnych stron wielok�t�w
	glFrontFace(GL_CCW);						// porz�dek wierzcho�k�w przeciwny do kierunku ruchu wskaz�wek zegara
	glEnable(GL_LIGHTING);						// w��czenie o�wietlenia
	glPolygonMode(GL_FRONT, PolygonMode);		// tryb rysowania wielok�t�w

	// Ustawienie w�a�ciwo�ci materia�u dla pierwszego �r�d�a �wiat�a LIGHT0


	// Ustawienie �r�d�a �wiat�a GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		// sk�adowa otoczenia
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		// sk�adowa rozproszona
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a

	// W��czenie �r�d�a �wiat�a
	glEnable(GL_LIGHT0);

	glPointSize(8.0f);					// rozmiar punkt�w (dla GL_POINTS)
}

void DrawVertexes(int m)
{

	glBegin(GL_POINTS);

	for (float x = -1; x < 1; x += 0.01) {
		for (float y = -1; y < 1; y += 0.01) {
			float z = pow(x, 2) - pow(y, 2);
			float r = (z > 0) ? z*2 : 0, g = (z > 0) ? (1 - z)*2 : (z + 1)*2, b = (z < 0) ? -z*2 : 0; //r=z, g= (z > 0) ? 1-z : z+1, b=-z
			//glColor3f(r, g, b);
			float ambientMaterial[] = { r, g, b, 1.0f };
			float diffuseMaterial[] = { r, g, b, 1.0f };
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

			glVertex3f(x, z, y);
		}
	}


	glEnd();

}

void Render()   // Renderowanie sceny
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Opr�nienie bufora ekranu i bufora g��bi
	glLoadIdentity();										// Zresetowanie macierzy modelowania

	static float rotationAngle = 0;
	static float cameraDistance = -5;
	glTranslatef(0.0f, 0.0f, cameraDistance);		// Wykonanie przekszta�ce� geometrycznych
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);

	DrawVertexes(m);						// Rozmieszczenie wierzcho�k�w

	if (GetKeyState('A') & 0x8000)
	{
		rotationAngle += 1;
	}

	if (GetKeyState('D') & 0x8000)
	{
		rotationAngle -= 1;
	}

	if (GetKeyState('W') & 0x8000 && cameraDistance < -3)
	{
		cameraDistance += 0.1;
	}

	if (GetKeyState('S') & 0x8000 && cameraDistance > -6)
	{
		cameraDistance -= 0.1;
	}



	////light test

	//if (GetKeyState('X') & 0x8000 )
	//{
	//	lightPosition[0] = lightPosition[0] + 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a

	//}
	//if (GetKeyState('Y') & 0x8000 )
	//{
	//	lightPosition[1] = lightPosition[1] + 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a
	//}
	//if (GetKeyState('Z') & 0x8000 )
	//{
	//	lightPosition[2] = lightPosition[2] + 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a
	//}
	//if (GetKeyState('C') & 0x8000 )
	//{
	//	lightPosition[2] = lightPosition[2] - 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a
	//}
	//if (GetKeyState('V') & 0x8000 )
	//{
	//	lightPosition[0] = lightPosition[0] - 0.01;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//}
	//if (GetKeyState('U') & 0x8000 )
	//{
	//	lightPosition[1] = lightPosition[1] - 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a
	//}
	//if (GetKeyState('R') & 0x8000)
	//{
	//	lightPosition[0] = 0.0f;
	//	lightPosition[1] = 0.0f;
	//	lightPosition[2] = 0.0f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a
	//}


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

	MessageBox(NULL, "A/D - obr�t\nW/S - zoom", "Sterowanie", //wiadomo�� przed programem
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
		"Funkcja x^2-y^2",	// nazwa aplikacji
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
