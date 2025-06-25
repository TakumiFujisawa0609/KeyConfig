#include <DxLib.h>

static const void LogMessage(const char* message, const char* fileName = "MyLog.txt")
{
    // ��ʂ�
    printfDx("%s\n", message);

    // �f�o�b�O�E�B���h�E
    OutputDebugStringA(message);
    OutputDebugStringA("\n");

    // �t�@�C���ɂ�
    FILE* fp = nullptr;
    fopen_s(&fp, fileName, "a");

    if (fp) {
        fprintf(fp, "%s\n", message);
        fclose(fp);
    }
}
