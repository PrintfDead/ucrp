#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <random>

static std::string ToLowerCase(std::string input);
static std::string ToLowerCase(std::string input)
{
	std::string output;
	// Transform commandInput to lowercase in command
	transform(input.cbegin(), input.cend(), back_inserter(output), tolower);
	return output;
}

static bool ParseInt(const std::string& str, int& result) {
    std::istringstream iss(str);
    return !(iss >> result).fail() && iss.eof();
}

static std::string VectorToString(std::vector<std::string> tokens) {
	std::string text = "";

    for (const std::string& i : tokens) {
        text += i;
    }

	return text;
}

static std::vector<std::string> splice(std::string main, char separator) {
    // Declarar un stringstream para manipular la cadena.
    std::stringstream ss(main);

    // Declarar un vector de strings para almacenar las partes separadas.
    std::vector<std::string> tokens;

    // Separar la cadena utilizando el carácter '_' como delimitador.
    std::string token;
    while (std::getline(ss, token, separator)) {
        tokens.push_back(token);
    }

	return tokens;
}

static std::vector<std::string> split(std::string s)
{
	std::vector<std::string> tokens;
	if (s.size() == 0) return tokens;

	std::stringstream stringStream(s);
	std::copy(std::istream_iterator<std::string>(stringStream), std::istream_iterator<std::string>(), std::back_inserter(tokens));

	return tokens;
}

static std::string generateRandomSalt() {
    // Caracteres que se utilizarán para el salt
    const std::string caracteresPermitidos = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789./";

    // Longitud máxima del salt (46 caracteres)
    const int longitudMaxima = 46;

    // Generador de números aleatorios
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, caracteresPermitidos.size() - 1);

    std::string salt;
    salt.reserve(longitudMaxima);

    for (int i = 0; i < longitudMaxima; ++i) {
        int indice = distribution(generator);
        salt += caracteresPermitidos[indice];
    }

    return salt;
}