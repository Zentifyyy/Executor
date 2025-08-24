#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include "include/tinyfiledialogs.h"
#include "fstream"
#include "iostream"
#include "Walnut/ImGui/ImGuiTheme.h"

#include "../ExecutorApp/embed/ButtonIcons.embed"

class ExampleLayer : public Walnut::Layer
{
public:
	
	virtual void OnAttach() override {

		style.WindowRounding = 6.0f;
		style.ChildRounding = 6.0f;
		style.ScrollbarRounding = 6.0f;

		ReadPathsFromFile();
		LoadImages();
	}
	
	virtual void OnDetach() override {

		WritePathsToFile();

		m_SymbolDelete.reset();
		m_SymbolDown.reset();
		m_SymbolUp.reset();
	}
	
	virtual void OnUIRender() override { 

		UI_DrawSidebar();

		UI_DrawAboutModal();

		UI_DrawFileSelectModal(); 
	}

	void UI_DrawSidebar() {
		
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::Begin("##Sidebar", 0, ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::CollapsingHeader("All Exe's")) {
			for (int i = 0; i < m_ExePaths.size(); ++i) {
				UI_DrawHierarchy(i, m_ExePaths[i]);
			}
		}

		ImGui::End();
	}

	void UI_DrawHierarchy(int& index,std::string& exePath) {

		if (m_UseFilePath) {
			if (ImGui::Button(exePath.c_str(), ImVec2(ImGui::GetWindowSize().x - 180, 40))) {
				system(exePath.c_str());
			}
		}
		else {
			if (ImGui::Button(FilePathToName(exePath).c_str(), ImVec2(ImGui::GetWindowSize().x - 180, 40))) {
				system(exePath.c_str());
			}

		}

		ImRect rect(
			ImGui::GetItemRectMin(),
			ImVec2{ ImGui::GetItemRectMax().x + 225, ImGui::GetItemRectMax().y }
		);
		
		if (!ImGui::IsMouseHoveringRect(rect.Min, rect.Max)) { return; }

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			m_HeirarchyRightClickOpen = true;
			ImGui::SetNextWindowPos(ImGui::GetMousePos());
		}
		
		ImGui::SameLine();
		
		if (ImGui::ImageButton(m_SymbolDelete->GetDescriptorSet(), { 28, 28 })) {
			removeFilePath(index);
			return;
		}

		ImGui::SameLine();
		
		if (ImGui::ImageButton(m_SymbolDown->GetDescriptorSet(), { 28, 28 })) {

			std::string temp = "";
			
			if (index == m_ExePaths.size() - 1) { return; }

			temp = m_ExePaths[index + 1];
			m_ExePaths[index + 1] = exePath;
			m_ExePaths[index] = temp;
		}

		ImGui::SameLine();

		if (ImGui::ImageButton(m_SymbolUp->GetDescriptorSet(), { 28, 28 })) {
			std::string temp = "";

			if (index == 0) { return; }

			temp = m_ExePaths[index - 1];
			m_ExePaths[index - 1] = exePath;
			m_ExePaths[index] = temp;
		}
	}
	
	void UI_DrawFileSelectModal() {
		if (!m_FileSelectModalOpen)
			return;

		ImGui::OpenPopup("Select File");
		m_FileSelectModalOpen = ImGui::BeginPopupModal("Select File", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		if (Walnut::UI::ButtonCentered("Select File")) {
			if (!addFilePath(tinyfd_openFileDialog("Open File", "*.exe", NULL, NULL, NULL, 0))) {
				m_FileSelectModalOpen = true;
			}
			else {
				ImGui::CloseCurrentPopup();
				m_FileSelectModalOpen = false;
			}
		}

		if (Walnut::UI::ButtonCentered("Close")) {
			ImGui::CloseCurrentPopup();
			m_FileSelectModalOpen = false;
		}

		ImGui::EndPopup();
	}

	void UI_DrawAboutModal() {
		if (!m_AboutModalOpen)
			return;

		ImGui::OpenPopup("About");
		m_AboutModalOpen = ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_AboutModalOpen)
		{
			auto image = Walnut::Application::Get().GetApplicationIcon();

			Walnut::UI::ShiftCursorX(7.5f);
			ImGui::Image(image->GetDescriptorSet(), { 56, 56 });

			ImGui::SameLine();
			Walnut::UI::ShiftCursorX(20.0f);

			ImGui::BeginGroup();
			ImGui::Text("Executor");
			ImGui::Text("by Zentifyyy.");
			ImGui::EndGroup();

			ImGui::SetWindowFontScale(.75);
			Walnut::UI::ShiftCursorX(7.5);
			ImGui::Text("Using the Walnut application framework");
			Walnut::UI::ShiftCursorX(7.5);
			ImGui::Text("by Studio Cherno.");
			ImGui::SetWindowFontScale(1);

			if (Walnut::UI::ButtonCentered("Close")) {
				m_AboutModalOpen = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	std::string FilePathToName(std::string& filePath) {
		std::string result = "";
		
		for (int i = filePath.length(); i >= 0; i--) {
			if (filePath[i] != '\\') {
				result.insert(result.begin(), filePath[i]);
			}
			else{
				return result;
			}
		}
	}
	
	void LoadImages(){
		
		{
			uint32_t w, h;
			void* data = Walnut::Image::Decode(g_ButtonDeleteSymbol, sizeof(g_ButtonDeleteSymbol), w, h);
			m_SymbolDelete = std::make_shared<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA, data);
			free(data);
		}

		{
			uint32_t w, h;
			void* data = Walnut::Image::Decode(g_ButtonUpIcon, sizeof(g_ButtonUpIcon), w, h);
			m_SymbolUp = std::make_shared<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA, data);
			free(data);
		}

		{
			uint32_t w, h;
			void* data = Walnut::Image::Decode(g_ButtonDownIcon, sizeof(g_ButtonDownIcon), w, h);
			m_SymbolDown = std::make_shared<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA, data);
			free(data);
		}
	}

	void ShowAboutModal() {
		m_AboutModalOpen = true;
	}

	void ShowFileSelectModal() {
		m_FileSelectModalOpen = true;
	}
	
	void ReadPathsFromFile() {
		pathsFileIn.open("exePaths.txt");
		std::string line;

		if (!pathsFileIn.is_open()) { return; }

		while (std::getline(pathsFileIn, line)) {
			m_ExePaths.push_back(line);
		}
	}
	
	void WritePathsToFile() {

		std::string path = "";

		for (int i = 0; i < m_ExePaths.size(); i++) {
			path += m_ExePaths[i] + "\n";
		}

		pathsFileOut.open("exePaths.txt");
		if (pathsFileOut.is_open()) {
			pathsFileOut << path;
			pathsFileOut.close();
		}
	}

	bool addFilePath(const char* path) {
		if (path == nullptr) { return false; }

		m_ExePaths.push_back(path);

		return true;
	}

	void removeFilePath(int &index) {
		m_ExePaths.erase(m_ExePaths.begin() + index);
	}

	void ToggleUseFilePath() {
		m_UseFilePath = !m_UseFilePath;
	}

private:

	ImGuiStyle& style = ImGui::GetStyle();

	bool m_AboutModalOpen = false;
	bool m_FileSelectModalOpen = false;
	bool m_HeirarchyRightClickOpen = false;
	bool m_UseFilePath = false;

	std::vector<std::string> m_ExePaths{};

	std::ofstream pathsFileOut{};
	std::ifstream pathsFileIn{};

	std::shared_ptr<Walnut::Image> m_SymbolDelete;
	std::shared_ptr<Walnut::Image> m_SymbolUp;
	std::shared_ptr<Walnut::Image> m_SymbolDown;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
	Walnut::ApplicationSpecification spec;
	spec.Name = "Executor";
	spec.IconPath = "img/AppIcon.png";

	spec.CustomTitlebar = true;
	spec.CenterWindow = true;

	spec.Width = 1200;
	spec.Height = 900;

	spec.TitlebarButtonColour = UI::Colors::Theme::text;

	spec.TitlebarButtonHoveredColour = ImGui::ColorConvertFloat4ToU32(UI::Colors::ConvertFromSRGB({48, 0, 113, 0.5f}));

	spec.TitlebarButtonPressedColour = ImGui::ColorConvertFloat4ToU32(UI::Colors::ConvertFromSRGB({ 48, 0, 113, 0.25f }));

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ExampleLayer> exampleLayer = std::make_shared<ExampleLayer>();

	app->SetApplicationIcon("img/AppIcon.png");

	app->PushLayer(exampleLayer);
	app->SetMenubarCallback([app, exampleLayer]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Add EXE")){
				exampleLayer->ShowFileSelectModal();
			}

			if (ImGui::MenuItem("Toggle File Names")) {
				exampleLayer->ToggleUseFilePath();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) {
				exampleLayer->ShowAboutModal();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}