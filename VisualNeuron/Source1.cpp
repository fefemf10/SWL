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

using Field = std::array<std::array<bool, 3>, 3>;

void genField(Field& field, bool& horizontal, bool& vertical)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int<int> dist1(0, 18);
	int figure = dist1(gen);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			field[i][j] = false;
		}
	}
	horizontal = vertical = false;
	switch (figure)
	{
	case 0:
		break;
	case 1:
	case 2:
	case 3:
		for (size_t j = 0; j < 3; j++)
		{
			field[figure - 1][j] = true;
		}
		horizontal = true;
		break;
	case 4:
	case 5:
	case 6:
		for (size_t i = 0; i < 3; i++)
		{
			field[i][figure - 4] = true;
		}
		vertical = true;
		break;
	case 7:
		field[0][0] = true;
		field[0][1] = true;
		field[0][2] = true;
		field[1][0] = true;
		field[2][0] = true;
		horizontal = vertical = true;
		break;
	case 8:
		field[0][0] = true;
		field[0][1] = true;
		field[0][2] = true;
		field[1][2] = true;
		field[2][2] = true;
		horizontal = vertical = true;
		break;
	case 9:
		field[2][0] = true;
		field[2][1] = true;
		field[2][2] = true;
		field[0][0] = true;
		field[1][0] = true;
		horizontal = vertical = true;
		break;
	case 10:
		field[2][0] = true;
		field[2][1] = true;
		field[2][2] = true;
		field[0][2] = true;
		field[1][2] = true;
		horizontal = vertical = true;
		break;
	case 11:
		field[1][0] = true;
		field[1][1] = true;
		field[1][2] = true;
		field[0][0] = true;
		field[2][0] = true;
		horizontal = vertical = true;
		break;
	case 12:
		field[1][0] = true;
		field[1][1] = true;
		field[1][2] = true;
		field[0][2] = true;
		field[2][2] = true;
		horizontal = vertical = true;
		break;
	case 13:
		field[0][1] = true;
		field[1][1] = true;
		field[2][1] = true;
		field[0][0] = true;
		field[0][2] = true;
		horizontal = vertical = true;
		break;
	case 14:
		field[0][1] = true;
		field[1][1] = true;
		field[2][1] = true;
		field[2][0] = true;
		field[2][2] = true;
		horizontal = vertical = true;
		break;
	case 15:
		field[0][1] = true;
		field[1][1] = true;
		field[2][1] = true;
		field[1][0] = true;
		field[1][2] = true;
		horizontal = vertical = true;
		break;
	case 16:
		field[0][0] = true;
		field[2][0] = true;
		field[0][2] = true;
		field[2][2] = true;
		field[1][1] = true;
		horizontal = vertical = false;
		break;
	case 17:
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				field[i][j] = true;
			}
		}
		horizontal = vertical = true;
		break;
	case 18:
		field[0][1] = true;
		field[2][1] = true;
		field[1][0] = true;
		field[1][2] = true;
		horizontal = vertical = false;
		break;
	}
}

void train(uint32_t count, Neuralnet<float>& net, std::vector<float>& lines, std::vector<float>& errors, Field& field, bool& horizontal, bool& vertical, int& right, float& error)
{
	for (size_t d = 0; d < count; d++)
	{
		right = 0;
		const int countField = 18;
		error = 0.f;
		for (size_t i = 0; i < countField; i++)
		{
			genField(field, horizontal, vertical);
			std::vector<float> input;
			for (size_t j = 0; j < 3; j++)
			{
				for (size_t k = 0; k < 3; k++)
				{
					input.push_back(field[j][k]);
				}
			}
			net.setInput(input);
			net.forwardFeed();
			bool hor = net.values[3][0] > 0.8f;
			bool ver = net.values[3][1] > 0.8f;
			if ((horizontal == hor) && (vertical == ver))
			{
				++right;
			}
			net.findError({ (float)horizontal, (float)vertical });
			error += net.error;
			net.backPropagation(0.8f);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		error /= countField;
		errors.push_back(error);
		lines.push_back(right);
	}
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
	Field field{};
	bool horizontal{}, vertical{};
	Neuralnet<float> net({ 9, 5 , 4, 2 });
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
			if (ImGui::Button("Get Answer"))
			{
				std::vector<float> input;
				for (size_t j = 0; j < 3; j++)
				{
					for (size_t k = 0; k < 3; k++)
					{
						input.push_back(field[j][k]);
					}
				}
				net.setInput(input);
				net.forwardFeed();
				horizontal = net.values[3][0] > 0.8f;
				vertical = net.values[3][1] > 0.8f;
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
				//Matrix<float> a(3, 3);
				//a[0] = 1.f;
				//a[4] = 1.f;
				//a[6] = 1.f;
				//a[8] = 1.f;
				///*Matrix<float> b(3, 3);
				//b[0] = 0.5f;
				//b[2] = 1.f;
				//b[4] = 0.5f;
				//b[8] = 0.5f;*/
				//Vector<float> b(3);
				//b[1] = 1.f;
				//Vector<float> c(3);
				//c = a * b;
				sumEpoh += countEpoh;
				lines.clear();
				errors.clear();
				std::thread(train, countEpoh, std::ref(net), std::ref(lines), std::ref(errors), std::ref(field), std::ref(horizontal), std::ref(vertical), std::ref(right), std::ref(error)).detach();
			}
			ImGui::SameLine();

			ImGui::Text("Max Epoh %i", sumEpoh);
			ImGui::SameLine();
			ImGui::Text("Right answers %i", right);
			ImGui::SameLine();
			ImGui::Text("Error %.4f", net.error);
			ImGui::PlotLines("Right answers", lines.data(), lines.size(), 0, 0, 0.f, 10.f, ImVec2(width / 2.f - ImGui::CalcTextSize("Right answers").x, 128));
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
			if (ImGui::BeginTable("errorsTable", 3, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedSame))
			{
				for (size_t i = 0, count = 0; i < 3; ++i)
				{
					for (size_t j = 0; j < 3; ++j, ++count)
					{
						ImGui::TableNextColumn();
						if (field[i][j])
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
						else
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.f));
						ImGui::PushID(count);
						if (ImGui::Button("", ImVec2(64, 64)))
							field[i][j] ^= 1;
						ImGui::PopID();
						ImGui::PopStyleColor();
					}
				}
				ImGui::EndTable();
			}
			if (ImGui::BeginTable("rightAnw", 2, ImGuiTableFlags_::ImGuiTableFlags_SizingFixedSame))
			{
				ImGui::TableNextColumn();
				if (horizontal)
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
				else
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.f));
				ImGui::Button("##horizontal", ImVec2(64, 64));
				ImGui::PopStyleColor();
				ImGui::TableNextColumn();
				if (vertical)
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
				else
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.f));
				ImGui::Button("##vertical", ImVec2(64, 64));
				ImGui::PopStyleColor();
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