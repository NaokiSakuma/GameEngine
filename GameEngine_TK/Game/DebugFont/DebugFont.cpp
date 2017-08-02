#include "DebugFont.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::unique_ptr<DirectX::SpriteBatch> DebugFont::g_spriteBatch;

std::unique_ptr<DirectX::SpriteFont> DebugFont::g_spriteFont;

std::unique_ptr<DirectX::CommonStates> DebugFont::g_state;

DebugFont::DebugFont()
{

}

DebugFont::~DebugFont()
{

}



void DebugFont::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	g_state.reset(new CommonStates(d3dDevice.Get()));

	g_spriteBatch.reset(new SpriteBatch(d3dContext.Get()));

	g_spriteFont.reset(new SpriteFont(d3dDevice.Get(), L"myfile.spritefont"));
}

void DebugFont::Debug(float num, Vector2 pos)
{
	wchar_t wstr[128];

	swprintf(wstr, 128, L"%f", num);

	g_spriteBatch->Begin();
	g_spriteFont->DrawString(g_spriteBatch.get(), wstr, pos);
	g_spriteBatch->End();
}