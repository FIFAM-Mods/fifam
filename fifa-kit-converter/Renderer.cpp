#include "Renderer.h"
#include "Error.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return DefWindowProcW(hWnd, msg, wParam, lParam); };

Renderer::Renderer(int w, int h) {
    ZeroMemory(&mWindowClass, sizeof(mWindowClass));
    mWindowClass.cbSize = sizeof(WNDCLASSEXW);
    mWindowClass.style = CS_CLASSDC;
    mWindowClass.hInstance = 0;
    mWindowClass.lpszClassName = L"RendererWindow";
    mWindowClass.lpfnWndProc = WndProc;
    auto classResult = RegisterClassExW(&mWindowClass);
    if (classResult == 0) {
        ::Error("Renderer::Renderer(): Failed to register Window Class (%X / %d)", GetLastError(), GetLastError());
        return;
    }
    mWindowHandle = CreateWindowExW(0, L"RendererWindow", L"Renderer", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, 0, 0, 0, 0);
    if (mWindowHandle == NULL) {
        ::Error("Renderer::Renderer(): Failed to create Window (%X / %d)", GetLastError(), GetLastError());
        return;
    }
    mCreatedWindow = true;
    Create(w, h, mWindowHandle);
}

Renderer::Renderer(int w, int h, HWND hwnd) {
    Create(w, h, hwnd);
}

void Renderer::Create(int w, int h, HWND hwnd) {
	mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (mDirect3D) {
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
        mDevice = nullptr;
        auto result = mDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &mDevice);
        if (FAILED(result)) {
            mDirect3D->Release();
            mDirect3D = nullptr;
            ::Error("Renderer::Create(): Failed to create Device (%X / %u / %d)", result, result, result & 0xFFFF);
        }
        else {
            auto sampling = D3DMULTISAMPLE_NONE;
            if (FAILED(mDevice->CreateRenderTarget(w, h, D3DFMT_A8R8G8B8, sampling, sampling, FALSE, &mRT, NULL))) {
                mDevice->Release();
                mDevice = nullptr;
                mDirect3D->Release();
                mDirect3D = nullptr;
                ::Error("Renderer::Create(): Failed to create Render Target");
            }
            else {
                if (FAILED(mDevice->SetRenderTarget(0, mRT))) {
                    mRT->Release();
                    mRT = nullptr;
                    mDevice->Release();
                    mDevice = nullptr;
                    mDirect3D->Release();
                    mDirect3D = nullptr;
                    ::Error("Renderer::Create(): Failed to set Render Target");
                }
                else {
                    if (FAILED(mDevice->CreateVertexBuffer(sizeof(Vertex) * 3, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_DEFAULT, &mVB, NULL))) {
                        mRT->Release();
                        mRT = nullptr;
                        mDevice->Release();
                        mDevice = nullptr;
                        mDirect3D->Release();
                        mDirect3D = nullptr;
                        ::Error("Renderer::Create(): Failed to create Vertex Buffer");
                    }
                    else
                        mAvailable = true;
                }
            }
        }
    }
    else
        ::Error("Renderer::Create(): Failed to create Direct3D");
}

bool Renderer::Available() {
    return mAvailable;
}

bool Renderer::Begin() {
    if (FAILED(mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 255)))
        return false;
    if (FAILED(mDevice->BeginScene()))
        return false;
    return true;
}

bool Renderer::End() {
    if (FAILED(mDevice->EndScene()))
        return false;
    return true;
}

IDirect3DTexture9 *Renderer::CreateTexture(wchar_t const *path) {
    IDirect3DTexture9 *tex = NULL;
    if (mDevice) {
        if (FAILED(D3DXCreateTextureFromFileW(mDevice, path, &tex)))
            return NULL;
    }
    return tex;
}

void Renderer::DestroyTexture(IDirect3DTexture9 *tex) {
    tex->Release();
}

bool Renderer::SaveRT(wchar_t const *path) {
    if (mRT) {
        if (FAILED(D3DXSaveSurfaceToFileW(path, D3DXIFF_PNG, mRT, NULL, NULL)))
            return false;
        return true;
    }
    return false;
}

bool Renderer::RenderTriangle(float *positions, float *uvs) {
    if (!mVB)
        return false;
    void *lockedData = nullptr;
    if (FAILED(mVB->Lock(0, sizeof(Vertex) * 3, &lockedData, 0)))
        return false;
    for (unsigned int i = 0; i < 3; i++) {
        mVertices[i].x = positions[i * 2];
        mVertices[i].y = positions[i * 2 + 1];
        mVertices[i].z = 0.0f;
        mVertices[i].rwh = 1.0f;
        mVertices[i].u = uvs[i * 2];
        mVertices[i].v = uvs[i * 2 + 1];
    }
    memcpy(lockedData, mVertices, sizeof(Vertex) * 3);
    if (FAILED(mVB->Unlock()))
        return false;
    if (FAILED(mDevice->SetStreamSource(0, mVB, 0, sizeof(Vertex)))) // TODO: move to initialization
        return false;
    if (FAILED(mDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1))) // TODO: move to initialization
        return false;
    if (FAILED(mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1)))
        return false;
    return true;
}

Renderer::~Renderer() {
    if (mVB)
        mVB->Release();
    if (mRT)
        mRT->Release();
	if (mDevice)
		mDevice->Release();
	if (mDirect3D)
		mDirect3D->Release();
    if (mCreatedWindow) {
        DestroyWindow(mWindowHandle);
        UnregisterClassW(L"RendererWindow", mWindowClass.hInstance);
    }
}

IDirect3DDevice9 *Renderer::Interface() {
    return mDevice;
}
