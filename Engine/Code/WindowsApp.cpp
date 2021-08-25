#include "WindowsApp.h"

void WindowsApp::Update()
{
}

void WindowsApp::Render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	context->ClearRenderTargetView(renderTargetView, ClearColor);

	rect->Render();

	static int count = 0;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Hierarchy");

	std::string sCount = "GameObject Count (" + std::to_string(count) + ")";
	ImGui::Text(sCount.c_str());

	if (ImGui::Button("Create GameObject"))
	{
		// Create New Rect
		++count;
	}

	// Rect만큼 순회
	ImGui::DragFloat2("Position", rect->GetTransform()->GetPosition(), 0.1f, -5.0f, 5.0f);
	ImGui::DragFloat2("Scale", rect->GetTransform()->GetScale(), 0.1f, -5.0f, 5.0f);
	ImGui::DragFloat("Rotation", rect->GetTransform()->GetRotation(), 0.1f, -5.0f, 5.0f);

	ImGui::End();



	ImGui::Begin("Inspector");
	// Inspector에 drag and drop 가능하게.
	ImGui::End();
	
	

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(0, 0);
}
