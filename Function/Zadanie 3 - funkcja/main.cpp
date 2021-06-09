#define WIN32_LEAN_AND_MEAN				// "odchudza" aplikacjê Windows

// Pliki nag³ówkowe
#include <windows.h>					// standardowy plik nag³ówkowy Windows
#include <gl/gl.h>						// standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>						// plik nag³ówkowy biblioteki GLU
#include <math.h>
//#include <gl/glaux.h>					// funkcje pomocnicze OpenGL 

#pragma comment(lib, "legacy_stdio_definitions.lib")

// Zmienne globalne
HDC g_HDC;								// globalny kontekst urz¹dzenia
bool fullScreen = false;				// true = tryb pe³noekranowy; false = tryb okienkowy

// Parametry oœwietlenia
float ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };	    // œwiat³o otoczenia
float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };	    // œwiat³o rozproszone
float lightPosition[] = { 0.0f, 0.0f, 1.0f, 1.0f };	    // po³o¿enie Ÿród³a œwiat³a
int m = 1;
// Parametry materia³u


GLenum PolygonMode = GL_FILL;  // wybór trybu rysowania wielok¹tów (GL_FILL, GL_LINE, GL_POINT)

void Initialize()       // Inicjacja OpenGL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// czarny kolor t³a

	glShadeModel(GL_SMOOTH);					// cieniowanie g³adkie
	glEnable(GL_DEPTH_TEST);					// w³¹czenie bufora g³êbi
	glEnable(GL_CULL_FACE);						// ukrywanie tylnych stron wielok¹tów
	glFrontFace(GL_CCW);						// porz¹dek wierzcho³ków przeciwny do kierunku ruchu wskazówek zegara
	glEnable(GL_LIGHTING);						// w³¹czenie oœwietlenia
	glPolygonMode(GL_FRONT, PolygonMode);		// tryb rysowania wielok¹tów

	// Ustawienie w³aœciwoœci materia³u dla pierwszego Ÿród³a œwiat³a LIGHT0


	// Ustawienie Ÿród³a œwiat³a GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		// sk³adowa otoczenia
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		// sk³adowa rozproszona
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a

	// W³¹czenie Ÿród³a œwiat³a
	glEnable(GL_LIGHT0);

	glPointSize(8.0f);					// rozmiar punktów (dla GL_POINTS)
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Opró¿nienie bufora ekranu i bufora g³êbi
	glLoadIdentity();										// Zresetowanie macierzy modelowania

	static float rotationAngle = 0;
	static float cameraDistance = -5;
	glTranslatef(0.0f, 0.0f, cameraDistance);		// Wykonanie przekszta³ceñ geometrycznych
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);

	DrawVertexes(m);						// Rozmieszczenie wierzcho³ków

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
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a

	//}
	//if (GetKeyState('Y') & 0x8000 )
	//{
	//	lightPosition[1] = lightPosition[1] + 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a
	//}
	//if (GetKeyState('Z') & 0x8000 )
	//{
	//	lightPosition[2] = lightPosition[2] + 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a
	//}
	//if (GetKeyState('C') & 0x8000 )
	//{
	//	lightPosition[2] = lightPosition[2] - 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a
	//}
	//if (GetKeyState('V') & 0x8000 )
	//{
	//	lightPosition[0] = lightPosition[0] - 0.01;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//}
	//if (GetKeyState('U') & 0x8000 )
	//{
	//	lightPosition[1] = lightPosition[1] - 0.01f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a
	//}
	//if (GetKeyState('R') & 0x8000)
	//{
	//	lightPosition[0] = 0.0f;
	//	lightPosition[1] = 0.0f;
	//	lightPosition[2] = 0.0f;
	//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a
	//}


	glFlush();								// Zrzucenie bufora graficznego na ekran

	SwapBuffers(g_HDC);						// Prze³¹czenie buforów
}


void SetupPixelFormat(HDC hDC)  // Funkcja okreœlaj¹ca format pikseli
{
	int nPixelFormat;					// indeks formatu pikseli

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
		1,								// domyœlna wersja
		PFD_DRAW_TO_WINDOW |			// grafika w oknie
		PFD_SUPPORT_OPENGL |			// grafika OpenGL 
		PFD_DOUBLEBUFFER,				// podwójne buforowanie
		PFD_TYPE_RGBA,					// tryb kolorów RGBA 
		32,								// 32-bitowy opis kolorów
		0, 0, 0, 0, 0, 0,				// nie specyfikuje bitów kolorów
		0,								// bez bufora alfa
		0,								// nie specyfikuje bitu przesuniêcia
		0,								// bez bufora akumulacji
		0, 0, 0, 0,						// ignoruje bity akumulacji
		16,								// 16-bitowy bufor Z
		0,								// bez bufora powielania
		0,								// bez buforów pomocniczych
		PFD_MAIN_PLANE,					// g³ówna p³aszczyzna rysowania
		0,								// zarezerwowane
		0, 0, 0 };						// ignoruje maski warstw

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// wybiera najbardziej zgodny format pikseli 
	SetPixelFormat(hDC, nPixelFormat, &pfd);		// okreœla format pikseli dla danego kontekstu urz¹dzenia
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  // Procedura okienkowa
{
	static HGLRC hRC;					// kontekst tworzenia grafiki
	static HDC hDC;						// kontekst urz¹dzenia
	int width, height;					// szerokoœæ i wysokoœæ okna

	switch (message)						// obs³uga komunikatów
	{
	case WM_CREATE:					// Utworzenie okna
		hDC = GetDC(hwnd);			// pobiera kontekst urz¹dzenia dla okna
		g_HDC = hDC;
		SetupPixelFormat(hDC);		// wywo³uje funkcjê okreœlaj¹c¹ format pikseli
		// tworzy kontekst grafiki i czyni go bie¿¹cym
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		return 0;
		break;

	case WM_CLOSE:					// Zamkniêcie okna
		wglMakeCurrent(hDC, NULL);  // usuwa kontekst renderowania okna
		wglDeleteContext(hRC);
		PostQuitMessage(0);			// wysy³a WM_QUIT do kolejki komunikatów
		return 0;
		break;

	case WM_SIZE:					// Zmiana wymiarów okna
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		if (height == 0)				// zabezpieczenie przed dzieleniem przez 0
			height = 1;
		glViewport(0, 0, width, height);		// nadanie nowych wymiarów okna OpenGL
		glMatrixMode(GL_PROJECTION);			// prze³¹czenie macierzy rzutowania
		glLoadIdentity();						// zresetowanie macierzy rzutowania
		gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);  // wyznaczenie proporcji obrazu i ustawienie rzutowania perspektywicznego
		glMatrixMode(GL_MODELVIEW);				// prze³¹czenie macierzy modelowania
		glLoadIdentity();						// zresetowanie macierzy modelowania
		return 0;
		break;
	case WM_CHAR:					// rozpoznanie przyciœniêtego klawisza
		m = wParam - 48;
		if (m < 0 || m>9) m = 0;
		break;

	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

// Funkcja g³ówna, od której rozpoczyna siê wykonywanie aplikacji
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX windowClass;		// klasa okna
	HWND	   hwnd;			// uchwyt okna
	MSG		   msg;				// komunikat
	bool	   done;			// znacznik zakoñczenia aplikacji
	DWORD	   dwExStyle;		// rozszerzony styl okna
	DWORD	   dwStyle;			// styl okna
	RECT	   windowRect;		// rozmiar okna

	// parametry okna
	int width = 800;
	int height = 600;
	int bits = 32;

	// fullScreen = TRUE/FALSE;		// Prze³¹cznik aplikacja okienkowa/aplikacja pe³noekranowa

	windowRect.left = (long)0;						// struktura okreœlaj¹ca rozmiary okna
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
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// domyœlna ikona
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// domyœlny kursor
	windowClass.hbrBackground = NULL;								// bez t³a
	windowClass.lpszMenuName = NULL;								// bez menu
	windowClass.lpszClassName = "MojaKlasa";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		// logo Windows

	// zarejestrowanie klasy okna
	if (!RegisterClassEx(&windowClass))
		return 0;

	MessageBox(NULL, "A/D - obrót\nW/S - zoom", "Sterowanie", //wiadomoœæ przed programem
		MB_OK | MB_ICONEXCLAMATION);

	if (fullScreen)								// W³¹czenie trybu pe³noekranowego
	{
		DEVMODE dmScreenSettings;						// tryb urz¹dzenia
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;			// szerokoœæ ekranu
		dmScreenSettings.dmPelsHeight = height;			// wysokoœæ ekranu
		dmScreenSettings.dmBitsPerPel = bits;			// iloœæ bitów na piksel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// je¿eli prze³¹czenie trybu na pe³noekranowy nie powiod³o siê, prze³¹czenie z powrotem na tryb okienkowy
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "Prze³¹czenie trybu wyswietlania nie powiod³o siê", NULL, MB_OK);
			fullScreen = FALSE;
		}
	}

	if (fullScreen)								// Tryb pe³noekranowy?
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
		0, 0,								// wspó³rzêdne x,y
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,	// szerokoœæ, wysokoœæ
		NULL,									// uchwyt okna nadrzêdnego
		NULL,									// uchwyt menu
		hInstance,							// instancja aplikacji
		NULL);

	if (!hwnd)	// sprawdzenie, czy utworzenie okna nie powiod³o siê (wtedy wartoœæ hwnd równa NULL)
		return 0;

	ShowWindow(hwnd, SW_SHOW);			// wyœwietlenie okna
	UpdateWindow(hwnd);					// aktualizacja okna

	done = false;						// inicjacja zmiennej warunku pêtli przetwarzania komunikatów
	Initialize();						// inicjacja OpenGL

	while (!done)	// pêtla przetwarzania komunikatów
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)		// aplikacja otrzyma³a komunikat WM_QUIT?
			done = true;				// jeœli tak, to koñczy dzia³anie
		else
		{
			Render();					// jeœli nie to renderuje scenê,
			TranslateMessage(&msg);		// t³umaczy komunikat i wysy³a do systemu
			DispatchMessage(&msg);
		}
	}

	if (fullScreen)						// je¿eli by³ tryb pe³noekranowy
	{
		ChangeDisplaySettings(NULL, 0);	// to przywrócenie pulpitu
		ShowCursor(TRUE);				// i wskaŸnika myszy
	}

	return msg.wParam;
}
