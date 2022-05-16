#pragma once
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <string>
#include <array>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include <Neuralnet/Neuralnet.hpp>
#include "Logger.hpp"

void train(uint32_t count, Neuralnet<float>& net, std::vector<float>& lines, std::vector<float>& errors, std::vector<std::vector<float>>& input, std::vector<std::vector<float>>& rightAnswers, int&right, float& error)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	for (size_t d = 0; d < count; d++)
	{
		right = 0;
		const int countField = 4;
		//std::shuffle(input.begin(), input.end(), gen);
		input = std::vector<std::vector<float>>(4);
		rightAnswers = std::vector<std::vector<float>>(4);
		input[0] = { 0.01f, 0.01f, 0.99f };
		input[1] = { 0.01f, 0.99f, 0.01f };
		input[2] = { 0.99f, 0.01f, 0.01f };
		input[3] = { 0.99f, 0.99f, 0.99f };
		rightAnswers[0] = { 0.01f };
		rightAnswers[1] = { 0.01f };
		rightAnswers[2] = { 0.99f };
		rightAnswers[3] = { 0.99f };
		for (size_t i = 0; i < countField; i++)
		{
			net.setInput(input[i]);
			net.forwardFeed();
			

			net.findError(rightAnswers[i]);
			for (int j = 0; j < net.values[net.countLayers - 1].size; ++j)
			{
				if (rightAnswers[i][j] - net.values[net.countLayers - 1][j] > 0.1f)
				{
				}
				else
					++right;
			}
			net.backPropagation(0.8);
			lines.push_back(right);
			errors.push_back(net.error);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
	std::vector<std::vector<float>> input;
	std::vector<std::vector<float>>rightAnswers;
	Neuralnet<float> net({ 3, 1 });
	size_t sumEpoh{};
	int countEpoh{ 1 };
	int right{};
	float error{};
	std::vector<float> lines;
	std::vector<float> errors;
	int width;
	int height;
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
				std::thread(train, countEpoh, std::ref(net), std::ref(lines), std::ref(errors), std::ref(input), std::ref(rightAnswers), std::ref(right), std::ref(error)).detach();
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
			if (ImGui::BeginTable("firstLayer", net.values[0].size, ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX))
			{
				for (size_t i = 0; i < net.countLayers; ++i)
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