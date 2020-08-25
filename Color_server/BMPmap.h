#pragma once
#pragma pack(1)

typedef struct tagRGB
{
	//���� : BMP�� BGR ������. RGB�� �ƴϴ�.
	BYTE Blue;
	BYTE Green;
	BYTE Red;
}RGB;

class BMPmap
{
public:
	BMPmap();
	~BMPmap();

private:
	BITMAPFILEHEADER m_BMPheader;
	BITMAPINFOHEADER m_BMPinfoheader;
	//�ӽ� : �ϴ� ������ ������� �Ѵ�.
	RGB pixel[32 * 32];

public:
	bool LoadBMP(const char* BMPfilename);

	RGB& GetPixel(int num)
	{
		if (num < 0)
			return pixel[0];

		if (num >= 32 * 32)
			return pixel[32 * 32 - 1];

		return pixel[num];
	}
};