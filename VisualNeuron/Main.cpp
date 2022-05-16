#pragma once
#include <set>
#include <filesystem>
#include <random>
#include <thread>
#include <chrono>
#include <string>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include <Neuralnet/Neuralnet.hpp>
#include "Texture.hpp"
#include "Logger.hpp"
#include "stb_image.hpp"
#include <iostream>

void train(uint32_t count, Neuralnet<double>& net, std::vector<float>& lines, std::vector<float>& errors, std::vector<std::pair<std::vector<double>, uint16_t>>& input, int& right, float& error)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	for (size_t d = 0; d < count; d++)
	{
		right = 0;
		const int countField = 60000;
		std::shuffle(input.begin(), input.end(), gen);
		for (size_t i = 0; i < countField; i++)
		{
			net.setInput(input[i].first);
			net.forwardFeed();
			std::vector<double> rightAnswers(10);
			rightAnswers[input[i].second] = 1.0;
			//int index = 0;
			//float max = -110.f;
			//std::vector<float> values = net.getValues(net.countLayers - 1);
			/*for (int j = 0; j < values.size(); ++j)
			{
				if (values[j] > max)
				{
					max = values[j];
					index = j;
				}
			}*/
			//if (input[i].second == index && max > 0.8f)
			//	++right;
			
			net.findError(rightAnswers);
			net.backPropagation(0.8);
			errors.push_back(net.error);
			lines.push_back(right);
		}
	}
}

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	std::string const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
	}();

	std::string const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
	}();

	std::string const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
	}();

	Logger::log(std::string(src_str) + ", " + std::string(type_str) + ", " + std::string(severity_str) + ", " + std::to_string(id) + ": " + std::string(message));
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "VisualNeuron", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	//glfwSetWindowSizeCallback(window, resize_callback);
	glfwSwapInterval(true);
	glewExperimental = true;
	glewInit();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	glClearColor(0.f, 0.f, 0.f, 0.f);
	//Logger::init();
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(message_callback, nullptr);
	std::vector<std::pair<std::vector<double>, uint16_t>> input;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data{};
	int widthh{}, heightt{}, bpp{};
	for (size_t i = 0; i < 10; i++)
	{
		for (auto& entry : std::filesystem::directory_iterator(std::filesystem::path(std::string("mnist_png\\training\\") + std::to_string(i))))
		{
			data = stbi_load(entry.path().string().c_str(), &widthh, &heightt, &bpp, 0);
			std::vector<double> img(widthh * heightt, 0.0);
			for (size_t j = 0; j < widthh * heightt; j++)
			{
				img[j] = data[j] / 255.0;
			}
			stbi_image_free(data);
			input.push_back(std::make_pair(img, i));
		}
	}
	Neuralnet<double> net({ 784, 384, 128, 10 });
	size_t sumEpoh{};
	int countEpoh{ 1 };
	int right{};
	float error{};
	std::vector<float> lines;
	std::vector<float> errors;
	int width;
	int height;
	Texture layer1(28, 28, GL_R32F);
	Texture layer2(19, 19, GL_R32F);
	Texture layer3(11, 11, GL_R32F);
	Texture layer4(1, 10, GL_R32F);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glfwGetWindowSize(window, &width, &height);
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		if (ImGui::Begin("###MainWindow", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			if (ImGui::Button("Full reset"))
			{
				net.fullReset();
			}
			ImGui::SameLine();
			if (ImGui::Button("Generation weights"))
			{
				net.setRandomWeights();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load weights"))
			{
				net.loadWeights("weights.bin");
			}
			ImGui::SameLine();
			if (ImGui::Button("Save weights"))
			{
				net.saveWeights("weights.bin");
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(100.f);
			ImGui::InputInt("Count Epoh", &countEpoh, 0, 100, ImGuiInputTextFlags_CharsDecimal);
			ImGui::PopItemWidth();
			if (ImGui::Button("Clear Plot"))
			{
				lines.clear();
				errors.clear();
				sumEpoh = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Train"))
			{
				sumEpoh += countEpoh;
				lines.clear();
				errors.clear();
				std::thread(train, countEpoh, std::ref(net), std::ref(lines), std::ref(errors), std::ref(input), std::ref(right), std::ref(error)).detach();
			}
			ImGui::SameLine();

			ImGui::Text("Max Epoh %i", sumEpoh);
			ImGui::SameLine();
			ImGui::Text("Right answers %i", right);
			ImGui::SameLine();
			ImGui::Text("Error %.4f", net.error);
			ImGui::PlotLines("Right answers", lines.data(), lines.size(), 0, 0, 0.f, 60000.f, ImVec2(width / 2.f - ImGui::CalcTextSize("Right answers").x, 128));
			ImGui::SameLine();
			ImGui::PlotLines("Errors", errors.data(), errors.size(), 0, 0, 0.f, 1.f, ImVec2(width / 2.f - ImGui::CalcTextSize("Right answers").x, 128));
			std::vector<float> img(28*28);
			for (size_t i = 0; i < 28*28; ++i)
			{
				img[i] = net.values[0][i];
			}
			layer1.editTexture(0, 0, 28, 28, GL_RED, GL_FLOAT, (const char*)img.data());
			for (size_t i = 0; i < 19 * 19; ++i)
			{
				img[i] = net.values[1][i];
			}
			layer2.editTexture(0, 0, 19, 19, GL_RED, GL_FLOAT, (const char*)img.data());
			for (size_t i = 0; i < 11 * 11; ++i)
			{
				img[i] = net.values[2][i];
			}
			layer3.editTexture(0, 0, 11, 11, GL_RED, GL_FLOAT, (const char*)img.data());
			for (size_t i = 0; i < 10; ++i)
			{
				img[i] = net.values[3][i];
			}
			layer4.editTexture(0, 0, 1, 10, GL_RED, GL_FLOAT, (const char*)img.data());
			ImGui::Image((void*)layer1.getId(), ImVec2(112, 112));
			ImGui::SameLine();
			ImGui::Image((void*)layer2.getId(), ImVec2(76, 76));
			ImGui::SameLine();
			ImGui::Image((void*)layer3.getId(), ImVec2(44, 44));
			ImGui::SameLine();
			ImGui::Image((void*)layer4.getId(), ImVec2(4, 40));
			if (ImGui::BeginTable("firstLayer", std::min(net.values[2].size, (size_t)32), ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX))
			{
				for (size_t i = 2; i < net.countLayers; ++i)
				{
					ImGui::TableNextRow();
					for (size_t j = 0; j < std::min(net.values[i].size, (size_t)32); ++j)
					{
						ImGui::TableSetColumnIndex(j);
						ImGui::TextColored(ImVec4(0.514f, 0.921f, 0.275f, 1.f), "%lf", net.values[i][j]);
						ImGui::TableSetColumnIndex(j);
						ImGui::TextColored(ImVec4(0.921f, 0.886f, 0.275f, 1.f), "%lf", net.errors[i][j]);
						if (i != net.countLayers - 1)
						{
							for (size_t k = 0; k < std::min(net.weights[i].column, (size_t)32); ++k)
							{
								ImGui::TableSetColumnIndex(j);
								ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.8f, 1.f), "%lf", net.weights[i][j * net.weights[i].column + k]);
							}
						}
					}
				}
				ImGui::EndTable();
			}
			ImGui::End();
		}
		ImGui::PopStyleVar();
		//ImGui::ShowDemoWindow();
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	//Logger::deinit();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}