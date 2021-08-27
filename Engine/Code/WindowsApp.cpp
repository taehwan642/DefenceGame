#include "WindowsApp.h"

void WindowsApp::Update()
{
}

void WindowsApp::Render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	context->ClearRenderTargetView(renderTargetView, ClearColor);

	for (auto& iter : gameObjects)
	{
		iter->Render();
	}

	static int count = 0;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Save"))
		{
			if (ImGui::MenuItem("Save New"))
			{
				//Do something
			}

			if (ImGui::MenuItem("Overwrite"))
			{
				//Do something
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Hierarchy");

	std::string sCount = "GameObject Count (" + std::to_string(count) + ")";
	ImGui::Text(sCount.c_str());
	ImGui::Text("%.3f ms / frame (%.f) FPS", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::Button("Create GameObject"))
	{
		gameObjects.push_back(std::make_unique<MyRectangle>(device, context));
		++count;
	}

	// gameObjects의 사이즈가 0이라면 0, 
	int maxSize = 0;
	if (gameObjects.size() != 0)
		maxSize = gameObjects.size() - 1;
	ImGui::SliderInt("Index", &getIndex, 0, maxSize);
	ImGui::End();


	ImGui::Begin("Inspector");
	// Inspector에 drag and drop 가능하게.
	int index = 0;
	for (auto& iter : gameObjects)
	{
		if (index++ != getIndex)
			continue;

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::DragFloat2("Position", iter->GetTransform()->GetPosition(), 0.1f, -5.0f, 5.0f);
			ImGui::DragFloat2("Scale", iter->GetTransform()->GetScale(), 0.1f, 0.0f, 5.0f);
			ImGui::DragFloat("Rotation", iter->GetTransform()->GetRotation(), 1.f, -360.0f, 360.0f);
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(0, 0);
}
