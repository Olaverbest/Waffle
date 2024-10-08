#include "wfpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Waffle/Core/Application.h"

//TEMPORARY
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "ImGuizmo.h"

namespace Waffle {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnAttach()
	{
		WF_PROFILE_FUNCTION();

		// Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);

		// Setup ImGui style
		ImGui::StyleColorsDark();

		// Makes the window look like a normal window when using viewports
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		WF_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		WF_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::End()
	{
		WF_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4{0.15f, 0.16f, 0.17f, 1.0f};

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.25f, 0.26f, 0.27f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.35f, 0.36f, 0.37f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.25f, 0.26f, 0.27f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.35f, 0.36f, 0.37f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };

		// Frame background
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.25f, 0.26f, 0.27f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.35f, 0.36f, 0.37f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.43f, 0.44f, 0.45f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.33f, 0.34f, 0.35f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.25f, 0.26f, 0.27f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.2f, 0.21f, 0.22f, 1.0f };
	}
}