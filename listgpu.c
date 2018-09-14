#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <stdio.h>
#include <stdbool.h>

#include <initguid.h>
DEFINE_GUID(SetGPU_IID_IDXGIFactory2, 0x50c83a1c, 0xe072, 0x4c48, 0x87,0xb0, 0x36,0x30,0xfa,0x36,0xa6,0xd0);
DEFINE_GUID(SetGPU_IID_IDXGIFactory, 0x7b7166ec, 0x21c7, 0x44ae, 0xb2,0x1a, 0xc9,0xae,0x32,0x1a,0xe3,0x69);

static const char* GLOBAL_whitelisted_gpu_name = "NVIDIA";

bool is_device_name_ok(char* name) {
    bool ok = strstr(name, GLOBAL_whitelisted_gpu_name) != NULL;
    printf("%s", name);
    if (ok) printf(" (MATCH)");
    printf("\n");
    return ok;
    // return true;
}

int main() {
    // SETGPU_global_info* info = SETGPU_get_global_info();
    bool use_dxgi10 = false;

    IDXGIFactory* factory;
    if (use_dxgi10) {
        // 1.0
        printf("Using DXGI 1.0\n");
        CreateDXGIFactory(&SetGPU_IID_IDXGIFactory, (void**)&factory);
    } else {
        printf("Assuming DXGI 1.1\n");
        CreateDXGIFactory1(&SetGPU_IID_IDXGIFactory2, (void**)&factory);
    }

    UINT i = 0;
    IDXGIAdapter* temp_adapter = NULL;
    while(1) {
        bool done = (factory->lpVtbl->EnumAdapters(factory, i, &temp_adapter)) == DXGI_ERROR_NOT_FOUND;
        if (done) {
            break;
        } else {
            DXGI_ADAPTER_DESC desc;
            temp_adapter->lpVtbl->GetDesc(temp_adapter, &desc);
            char gpu_name[1024];
            wcstombs(gpu_name, desc.Description, 1024);
            // printf("%s\n", gpu_name);

            bool is_ok = is_device_name_ok(gpu_name);
            if (is_ok) {
                // printf("Using GPU: %s\n", gpu_name);
                // break;
            } else {
                // continue
            }
            i++;
        }
    }
    system("PAUSE");
}