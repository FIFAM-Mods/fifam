#pragma once
#include "d3d9.h"
#include <d3dx9.h>

class Renderer {
	IDirect3D9 *mDirect3D = nullptr;
	IDirect3DDevice9 *mDevice = nullptr;
    IDirect3DSurface9 *mRT = nullptr;
    IDirect3DVertexBuffer9 *mVB = nullptr;
    bool mAvailable = false;
    struct Vertex {
        float x, y, z, rwh, u, v;
    };
    Vertex mVertices[3];
public:
    Renderer();
	~Renderer();
	IDirect3DDevice9 *Interface();
    bool Available();
    bool Begin();
    bool End();
    IDirect3DTexture9 *CreateTexture(wchar_t const *path);
    void DestroyTexture(IDirect3DTexture9 *tex);
    bool SaveRT(wchar_t const *path);
    bool RenderTriangle(float *positions, float *uvs);
    bool SetSourceTexture(IDirect3DTexture9 *tex) {
        if (!mDevice)
            return false;
        if (FAILED(mDevice->SetTexture(0, tex)))
            return false;
        return true;
    }
};
