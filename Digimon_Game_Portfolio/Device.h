
void InitWindow();
void InitWindow(HINSTANCE hInstance, int ShowWnd);
void InitDirect3D();
void Destroy();

WPARAM Running();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////

void InitScene();
void DestroyScene();
void Update();
void Render();
void ImGui_Event();

void ViewProjection_Init();

void CreateBackBuffer();
void DeleteBackBuffer();