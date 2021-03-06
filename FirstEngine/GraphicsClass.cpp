#include "GraphicsClass.h"

#include "memoryUtility.h"

GraphicsClass::GraphicsClass(const EngineOptions& options) {
	m_Direct3D		= nullptr;
	m_TextureHolder	= nullptr;
}

GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Initialize(const EngineOptions& options, HWND hwnd) {
	bool result;

	mClientWidth = options.m_screenWidth;
	mClientHeight = options.m_screenHeight;

	// Create the Direct3D object.
	m_Direct3D = std::shared_ptr<D3DClass>(new D3DClass);
	if (!m_Direct3D) {
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(options, hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_TextureHolder = std::shared_ptr<TextureHolder>(new TextureHolder);
	if (!m_TextureHolder) {
		return false;
	}

	// Initialize the texture holder object.
	std::vector<std::string> textures{ "D.bmp" };
	result = m_TextureHolder->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), textures);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the textures.", L"Error", MB_OK);
		return false;
	}

	m_ShaderHolder = std::shared_ptr<ShaderHolder>(new ShaderHolder);
	if (!m_ShaderHolder) {
		return false;
	}

	// Initialize the texture holder object.
	std::vector<ShaderHolder::ShaderProperty> shaders{ 
		{L"BasicNormalMapPixelShader.fx", ShaderClass::ShaderType::PixelShader},
		{L"BasicNormalMapVertexShader.fx", ShaderClass::ShaderType::VertexShader},
		{L"SkyPixelShader.fx", ShaderClass::ShaderType::PixelShader},
		{L"SkyVertexShader.fx", ShaderClass::ShaderType::VertexShader}
	};
	result = m_ShaderHolder->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), shaders);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the shaders.", L"Error", MB_OK);
		return false;
	}

	//m_Scene1 = std::make_unique<Scene1>(GetD3D()->GetDevice());
	m_Scene3 = std::make_unique<Scene3>(GetD3D()->GetDevice());

	//m_sceneNumber = 1;

	return true;
}

void GraphicsClass::Shutdown() {

	// Release the Direct3D object.
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
	}

	return;
}

void GraphicsClass::OnResize(const EngineOptions& options) {

	mClientWidth = options.m_screenWidth;
	mClientHeight = options.m_screenHeight;

	m_Direct3D->OnResize(options);
}

void GraphicsClass::Update(InputClass& kbd, Mouse& mouse, const GameTimer& gt) {

	//m_Scene1->Update(GetD3D()->GetDeviceContext(), kbd, mouse, gt.DeltaTime(), gt.TotalTime());
	m_Scene3->Update(GetD3D()->GetDeviceContext(), kbd, mouse, gt.DeltaTime(), gt.TotalTime());
}

bool GraphicsClass::Frame() {
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result) {
		return false;
	}

	return true;
}

void GraphicsClass::DrawIndexed(int count) {

	m_Direct3D->GetDeviceContext()->DrawIndexed(count, 0u, 0u);
}

float GraphicsClass::AspectRatio() const {
	return static_cast<float>(mClientWidth) / mClientHeight;
}

std::shared_ptr<D3DClass> GraphicsClass::GetD3D() {
	return m_Direct3D;
}

std::shared_ptr<TextureHolder> GraphicsClass::GetTexture() {
	return m_TextureHolder;
}

int GraphicsClass::GetWidth() {
	return mClientWidth;
}

int GraphicsClass::GetHeight() {
	return mClientHeight;
}

bool GraphicsClass::Render() {
	
	// Clear the buffers to begin the scene.
	//m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	bool result;

	//m_Scene1->Draw(GetD3D()->GetDeviceContext());
	m_Scene3->Draw(GetD3D()->GetDeviceContext());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
