#pragma once

/*if (ImGui::Button("Get Answer"))
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
				input = net.getValues(3);
				horizontal = input[0] > 0.8f;
				vertical = input[1] > 0.8f;
			}*/

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

void train(uint32_t count, Neuralnet& net, std::vector<float>& lines, std::vector<float>& errors, Field& field, bool& horizontal, bool& vertical, int& right, float& error)
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
			input = net.getValues(3);
			bool hor = input[0] > 0.8f;
			bool ver = input[1] > 0.8f;
			if ((horizontal == hor) && (vertical == ver))
			{
				++right;

			}
			net.findError({ (float)horizontal, (float)vertical });
			error += net.error;
			net.backPropagation(0.2f);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		error /= countField;
		errors.push_back(error);
		lines.push_back(right);
	}
}

Field field{};
bool horizontal{}, vertical{};

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
	input = net.getValues(3);
	horizontal = input[0] > 0.8f;
	vertical = input[1] > 0.8f;
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