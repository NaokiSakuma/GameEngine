#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <SimpleMath.h>

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

class DebugFont
{
public:
	DebugFont();
	~DebugFont();

	// ê√ìIÉÅÉìÉoä÷êî
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext);
	void Debug(float num, DirectX::SimpleMath::Vector2 pos);

private:
	static std::unique_ptr<DirectX::SpriteBatch> g_spriteBatch;

	static std::unique_ptr<DirectX::SpriteFont> g_spriteFont;

	static std::unique_ptr<DirectX::CommonStates> g_state;
};


