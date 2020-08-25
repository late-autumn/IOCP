#pragma once
#pragma pack(1)

typedef struct tagRGB
{
	//참고 : BMP는 BGR 순서다. RGB가 아니다.
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
	//임시 : 일단 고정된 사이즈로 한다.
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