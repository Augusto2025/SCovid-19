#pragma once
#include <string>

using namespace std;

namespace Config
{
    const string DATA_ROOT = "Data";
    const string PACIENTES_DIR = DATA_ROOT + string("/Registros");
    const string RELATORIOS_DIR = DATA_ROOT + string("/Relatorios");
    const string SEGURANCA_DIR = DATA_ROOT + string("/Seguranca");

    const string USERS_FILE = SEGURANCA_DIR + string("/usuarios.txt");
    const string LISTA_COMORBIDADE_FILE = RELATORIOS_DIR + string("/listacomorbidade.txt");

    const string CRYPTO_KEY = "SCovid19Key2025";

    bool ensureDirectories();
} // namespace Config
