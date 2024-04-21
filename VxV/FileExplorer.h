#pragma once

#include <filesystem>
#include <string>
#include "imgui.h"

namespace fs = std::filesystem;

void ShowFileExplorer()
{
	static std::string currentPath = "."; // Commence � partir du r�pertoire actuel
	static std::string selectedPath = "";

	if (ImGui::Begin("Explorateur de fichiers"))
	{
		if (ImGui::Button("Parent"))
		{
			currentPath = fs::path(currentPath).parent_path().string(); // Monter d'un r�pertoire
		}

		// Affiche le chemin actuel
		ImGui::Text("Chemin : %s", currentPath.c_str());

		// Lister les r�pertoires et fichiers dans le chemin actuel
		for (const auto& entry : fs::directory_iterator(currentPath))
		{
			const auto& path = entry.path();
			std::string filename = path.filename().string();

			// Si l'entr�e est un r�pertoire, ajoute un suffixe '/' pour indiquer qu'il s'agit d'un dossier
			if (entry.is_directory())
			{
				filename += "/";
			}

			if (ImGui::Selectable(filename.c_str()))
			{
				if (entry.is_directory())
				{
					currentPath = path.string(); // Change le r�pertoire actuel
				}
				else
				{
					selectedPath = path.string(); // S�lectionne le fichier
				}
			}
		}

		// Afficher le chemin du fichier s�lectionn� (si un fichier est s�lectionn�)
		if (!selectedPath.empty())
		{
			ImGui::Text("Fichier s�lectionn� : %s", selectedPath.c_str());
		}
	}
	ImGui::End();
}
