#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include "DataClientLayer.h"


Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";
	spec.CustomTitlebar = true;

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<DataClientLayer> dataclientlayer = std::make_shared<DataClientLayer>();
	app->PushLayer(dataclientlayer) ;
	app->SetMenubarCallback([app,dataclientlayer]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Brake Data"))
			{
				dataclientlayer->SetShowBrakeData(true);
			}
			if (ImGui::MenuItem("Driver Inputs"))
			{
				dataclientlayer->SetDriverInputsStatus(true);
			}
			if (ImGui::MenuItem("Reset Windows"))
			{
				dataclientlayer->SetShowBrakeData(false);
				dataclientlayer->SetDriverInputsStatus(false);
			}
			ImGui::EndMenu();
		}
	});
	return app;
}