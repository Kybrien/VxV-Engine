#pragma once
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

// Liste des messages de la console
std::vector<std::string> consoleMessages;

// Classe personnalis�e pour rediriger std::cout
class ConsoleOutput : public std::streambuf
{
protected:
    virtual int_type overflow(int_type c)
    {
        if (c != EOF)
        {
            // Ajouter le caract�re � la derni�re ligne de la console
            if (!consoleMessages.empty())
            {
                consoleMessages.back() += static_cast<char>(c);
            }
            else
            {
                consoleMessages.push_back(std::string(1, static_cast<char>(c)));
            }

            // Si le caract�re est un saut de ligne, ajouter une nouvelle ligne � la console
            if (c == '\n')
            {
                consoleMessages.push_back("");
            }
        }
        return c;
    }
};

// Fonction pour rediriger std::cout vers la console personnalis�e
void RedirectCout()
{
    static ConsoleOutput consoleOutput;
    std::cout.rdbuf(&consoleOutput);
}