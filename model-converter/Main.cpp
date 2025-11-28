#include "GenericHairsFCPC.h"
#include "Renderer.h"

int main(int argc, char *argv[]) {
    Renderer renderer(1024, 1024);
    ConvertGenericHairsFCPC("D:\\Games\\fc26_files\\content\\character\\generichair", &renderer);
    system("pause");
    return 0;
}
