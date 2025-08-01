﻿#include "pch.h"
#include "TextureResource.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "Engine.h"

void TextureResource::Load(string fileName)
{
	// WinAPI 텍스처 로딩하는 방법
	{
		fs::path fullPath = fs::current_path();
		fullPath += "\\Level\\" + fileName;

		HDC hdc = ::GetDC(Engine::GetInstance()->GetHwnd());

		_textureHdc = ::CreateCompatibleDC(hdc);
		_bitmap = (HBITMAP)::LoadImageW(
			nullptr,
			fullPath.c_str(),
			IMAGE_BITMAP,
			0,
			0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION
		);
		if (_bitmap == 0)
		{
			::MessageBox(Engine::GetInstance()->GetHwnd(), fullPath.c_str(), L"Invalid Texture Load", MB_OK);
			return;
		}

		_transparent = RGB(255, 255, 255);

		HBITMAP prev = (HBITMAP)::SelectObject(_textureHdc, _bitmap);
		::DeleteObject(prev);

		BITMAP bit = {};
		::GetObject(_bitmap, sizeof(BITMAP), &bit);

		_sizeX = bit.bmWidth;
		_sizeY = bit.bmHeight;
	}
}

void TextureResource::Render(HDC hdc, Vector pos)
{
	Vector screenPos = CameraManager::GetInstance()->ConvertScreenPos(pos);

	if (_transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			screenPos.x - (_sizeX / 2),	// 텍스처를 중심좌표로 그리기위해 size의 절반만큼 빼준다.
			screenPos.y - (_sizeY / 2),
			_sizeX,
			_sizeY,
			_textureHdc,	// 텍스쳐 그리기
			0,
			0,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			screenPos.x - (_sizeX / 2),
			screenPos.y - (_sizeY / 2),
			_sizeX,
			_sizeY,
			_textureHdc,
			0,
			0,
			_sizeX,
			_sizeY,
			_transparent);	// 어떤색상을 투명하게 그릴까
	}

}

void TextureResource::Render(HDC hdc, int srcX, int srcY, int srcWidth, int srcHeight, Vector destPos, int destWidth, int destHeight)
{
	int renderPosX = (int32)destPos.x - (abs(destWidth) / 2); 
	int renderPosY = (int32)destPos.y - (destHeight / 2);

	Vector renderPos = Vector(renderPosX, renderPosY);
	Vector screenPos = CameraManager::GetInstance()->ConvertScreenPos(renderPos);

	int finalX = screenPos.x;
	int finalWidth = destWidth;

	::TransparentBlt(hdc, // 투명처리를 위함
		finalX,
		screenPos.y,
		finalWidth,
		destHeight,
		_textureHdc,
		srcX,
		srcY,
		srcWidth,
		srcHeight,
		_transparent);

	::StretchBlt(hdc, // 반전을 위함
		finalX,        
		screenPos.y,
		finalWidth,     
		destHeight,
		_textureHdc,
		srcX,
		srcY,
		srcWidth,
		srcHeight,
		SRCCOPY);
}