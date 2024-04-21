#pragma once

#include <filesystem>
#include <string>
#include "imgui.h"

namespace fs = std::filesystem;

void ShowFileExplorer()
{
	static std::string currentPath = "."; // Commence à partir du répertoire actuel
	static std::string selectedPath = "";

	if (ImGui::Begin("Explorateur de fichiers"))
	{
		if (ImGui::Button("Parent"))
		{
			currentPath = fs::path(currentPath).parent_path().string(); // Monter d'un répertoire
		}

		// Affiche le chemin actuel
		ImGui::Text("Chemin : %s", currentPath.c_str());

		// Lister les répertoires et fichiers dans le chemin actuel
		for (const auto& entry : fs::directory_iterator(currentPath))
		{
			const auto& path = entry.path();
			std::string filename = path.filename().string();

			// Si l'entrée est un répertoire, ajoute un suffixe '/' pour indiquer qu'il s'agit d'un dossier
			if (entry.is_directory())
			{
				filename += "/";
			}

			if (ImGui::Selectable(filename.c_str()))
			{
				if (entry.is_directory())
				{
					currentPath = path.string(); // Change le répertoire actuel
				}
				else
				{
					selectedPath = path.string(); // Sélectionne le fichier
				}
			}
		}

		// Afficher le chemin du fichier sélectionné (si un fichier est sélectionné)
		if (!selectedPath.empty())
		{
			ImGui::Text("Fichier sélectionné : %s", selectedPath.c_str());
		}
	}
	ImGui::End();
}
