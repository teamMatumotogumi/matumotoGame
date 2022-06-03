#define _CRT_SECURE_NO_WARNINGS

#include "Precompiled.h"
#include "Save.h"



void PlayData::SetPath(const char* path)
{
    m_path = path;

}

void PlayData::Save(int score)
{
    FILE* fout = nullptr;

     fopen_s(&fout, m_path.c_str(), "w");

    fprintf(fout, "%d ", score);

    fclose(fout);


}

int PlayData::Load()
{
    int score;
    FILE* fin = nullptr;

    fopen_s(&fin,m_path.c_str(), "r");

    fscanf_s(fin,"%d",&score );

    fclose(fin);

    return 0;
}
