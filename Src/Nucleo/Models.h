#pragma once
#include <string>
#include <vector>

using namespace std;

enum class tipoUser { Usuario, Admin};

struct Usuario
{
    string Login;
    string Senha;
    tipoUser tipo;
};

struct Paciente
{
    string nome;
    string cpf;
    int idade;
    string endereco;
    string telefone;
    string data_diagnostico;
    string sintomas;
    
    vector<string> comorbidades;
    string comorbidadesTextoLivre;
};
