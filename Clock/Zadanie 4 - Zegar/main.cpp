#define WIN32_LEAN_AND_MEAN				// "odchudza" aplikacj� Windows

// Pliki nag��wkowe
#include <ctime>
#include <cmath>
#include <windows.h>					// standardowy plik nag��wkowy Windows
#include <gl/gl.h>						// standardowy plik nag��wkowy OpenGL
#include <gl/glu.h>						// plik nag��wkowy biblioteki GLU
//#include <gl/glaux.h>					// funkcje pomocnicze OpenGL 
int m = 0;
#pragma comment(lib, "legacy_stdio_definitions.lib")

// Zmienne globalne
HDC g_HDC;								// globalny kontekst urz�dzenia
bool fullScreen = false;				// true = tryb pe�noekranowy; false = tryb okienkowy
double secondsAngle = 0;
double hoursAngle = 0;
double minutesAngle = 0;

// Parametry o�wietlenia
float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };	    // �wiat�o otoczenia
float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };	    // �wiat�o rozproszone
float lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };	    // po�o�enie �r�d�a �wiat�a

// Parametry materia�u
float ambientMaterial[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float diffuseMaterial[] = { 0.3f, 0.5f, 0.7f, 1.0f };

void calcAngle(double h, double m, double s)
{

	if (h == 12) h = 0;
	if (m == 60)
	{
		m = 0;
		h += 1;
		if (h > 12)
			h = h - 12;
	}

	if (s == 60)
	{
		s = 0;
		m += 1;
		if (m > 60)
			m = m - 60;
	}

	// Calculate the angles moved 
	// by hour and minute hands 
	// with reference to 12:00 
	float hour_angle = 0.5 * (h * 60 + m);
	float minute_angle = 6 * m;
	float second_angle = 6 * s;
	hoursAngle = -hour_angle;
	minutesAngle = -minute_angle;
	secondsAngle = -second_angle;

}

void Initialize()       // Inicjacja OpenGL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// czarny kolor t�a

	glShadeModel(GL_SMOOTH);					// cieniowanie g�adkie
	glEnable(GL_DEPTH_TEST);					// w��czenie bufora g��bi
	glEnable(GL_CULL_FACE);						// ukrywanie tylnych stron wielok�t�w
	glFrontFace(GL_CCW);						// porz�dek wierzcho�k�w przeciwny do kierunku ruchu wskaz�wek zegara
	glEnable(GL_LIGHTING);	// w��czenie o�wietlenia
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_POINT_SIZE);
	glPointSize(7);

	glEnable(GL_LINE_WIDTH);

	// Ustawienie �r�d�a �wiat�a GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		// sk�adowa otoczenia
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		// sk�adowa rozproszona
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po�o�enie �r�d�a �wiat�a

	// W��czenie �r�d�a �wiat�a
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

	time_t now;
	struct tm* now_tm;
	int hour;
	int minutes;
	int seconds;

	now = time(NULL);
	now_tm = localtime(&now);
	hour = now_tm->tm_hour;
	minutes = now_tm->tm_min;
	seconds = now_tm->tm_sec;

	calcAngle(hour, minutes, seconds);

}


void Render()   // Renderowanie sceny
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Opr�nienie bufora ekranu i bufora g��bi
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();// Zresetowanie macierzy modelowania

	static float rotAng = 0;
	static float rotAng1 = 0;
	static int a = GetTickCount64();
	const int sides = 60;  // The amount of segment to create the circle
	const double radius = 1.0f; // The radius of the circle

	glTranslatef(0, 0, -3);
	glRotatef(rotAng, 0.0f, 1.0f, 0.0f);
	glRotatef(rotAng1, 1.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	for (int b = 0; b < 360; b += 360 / sides)
	{
		double heading = b * 2 * acos(0.0) / 180;
		glVertex2d(cos(heading) * radius, sin(heading) * radius);
	}
	glEnd();
	glPopMatrix();
	
	glLineWidth(1.5);
	glPushMatrix();
	glTranslatef(0, 0, 0.01f);
	glRotatef(secondsAngle, 0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2d(0, 0);
	glVertex2d(0, 0.9);
	glEnd();
	glPopMatrix();

	glLineWidth(2.4);
	glPushMatrix();
	glTranslatef(0, 0, 0.01f);
	glRotatef(minutesAngle, 0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2d(0, 0);
	glVertex2d(0, 0.8);
	glEnd();
	glPopMatrix();

	glLineWidth(4);
	glPushMatrix();
	glTranslatef(0, 0, 0.01f);
	glRotatef(hoursAngle, 0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2d(0, 0);
	glVertex2d(0, 0.4);
	glEnd();
	glPopMatrix();


	if (GetTickCount64() - a > 1000) {

		secondsAngle -= 6;
		if (secondsAngle == -360)
		{
			secondsAngle = 0;
			minutesAngle -= 6;
			hoursAngle -= 0.5;

			if (minutesAngle == -360)
			{
				minutesAngle = 0;


				if (hoursAngle == -360)
				{
					hoursAngle = 0;
				}
			}
		}
		a = GetTickCount64();
	}

	if (GetKeyState('A') & 0x8000 && rotAng < 60)
	{
		rotAng += 0.05;
	}

	if (GetKeyState('D') & 0x8000 && rotAng > -60)
	{
		rotAng -= 0.05;
	}

	if (GetKeyState('W') & 0x8000 && rotAng1 < 60)
	{
		rotAng1 += 0.05;
	}

	if (GetKeyState('S') & 0x8000 && rotAng1 > 0)
	{
		rotAng1 -= 0.05;
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
	MessageBox(NULL, "W/A/S/D - obr�t kamery", "Sterowanie",
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
		"Zegar",		// nazwa aplikacji
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
