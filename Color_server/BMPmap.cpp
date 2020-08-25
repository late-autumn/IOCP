#include "Define.h"
#include "BMPmap.h"

BMPmap::BMPmap()
{

}

BMPmap::~BMPmap()
{

}

bool BMPmap::LoadBMP(const char* BMPfilename)
{
	FILE *fp;
	BYTE tempBYTE;

	if (fopen_s(&fp, BMPfilename, "rb") != 0)
	{
		printf("%s file open Error\n", BMPfilename);
		return false;
	}

	fread(&m_BMPheader, sizeof(m_BMPheader), sizeof(BYTE), fp);
	fread(&m_BMPinfoheader, sizeof(m_BMPinfoheader), sizeof(BYTE), fp);

	for (int i = 0; i < 32 * 32; ++i)
	{
		fread(&tempBYTE, sizeof(BYTE), sizeof(BYTE), fp);
		pixel[i].Blue = tempBYTE;
		fread(&tempBYTE, sizeof(BYTE), sizeof(BYTE), fp);
		pixel[i].Green = tempBYTE;
		fread(&tempBYTE, sizeof(BYTE), sizeof(BYTE), fp);
		pixel[i].Red = tempBYTE;
	}

	fclose(fp);

	return true;
}