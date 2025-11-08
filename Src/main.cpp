#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "Nucleo/Config.h"
#include "Nucleo/Models.h"

using namespace std;

vector<Paciente> pacientes;

void cadastrarPaciente() {
    Paciente p;
    cin.ignore();
    cout << "Nome: ";
    getline(cin, p.nome);
    cout << "CPF (apenas numeros): ";
    getline(cin, p.cpf);
    cout << "Idade: ";
    cin >> p.idade;
    cin.ignore();
    cout << "Endereco: ";
    getline(cin, p.endereco);
    cout << "Telefone: ";
    getline(cin, p.telefone);
    cout << "Data do diagnostico (dd/mm/aaaa): ";
    getline(cin, p.data_diagnostico);
    cout << "Principais sintomas: ";
    getline(cin, p.sintomas);

    pacientes.push_back(p);
    cout << "Paciente cadastrado com sucesso!\n";
}

void listarPacientes() {
    if (pacientes.empty()) {
        cout << "Nenhum paciente cadastrado.\n";
        return;
    }

    for (size_t i = 0; i < pacientes.size(); i++) {
        cout << "\nPaciente #" << i + 1 << ":\n";
        cout << "Nome: " << pacientes[i].nome << "\n";
        cout << "CPF: " << pacientes[i].cpf << "\n";
        cout << "Idade: " << pacientes[i].idade << "\n";
        cout << "Endereco: " << pacientes[i].endereco << "\n";
        cout << "Telefone: " << pacientes[i].telefone << "\n";
        cout << "Data do diagnostico: " << pacientes[i].data_diagnostico << "\n";
        cout << "Sintomas: " << pacientes[i].sintomas << "\n";
    }
}

void buscarPorCPF() {
    string cpfBusca;
    cout << "Digite o CPF para busca: ";
    cin.ignore();
    getline(cin, cpfBusca);

    for (const auto& p : pacientes) {
        if (p.cpf == cpfBusca) {
            cout << "\nPaciente encontrado:\n";
            cout << "Nome: " << p.nome << "\n";
            cout << "Idade: " << p.idade << "\n";
            cout << "Endereco: " << p.endereco << "\n";
            cout << "Telefone: " << p.telefone << "\n";
            cout << "Data do diagnostico: " << p.data_diagnostico << "\n";
            cout << "Sintomas: " << p.sintomas << "\n";
            return;
        }
    }
    cout << "Paciente com CPF " << cpfBusca << " nao encontrado.\n";
}

void menu() {
    int opcao;
    do {
        cout << "\n======= Sistema de Cadastro COVID-19 =======\n";
        cout << "1. Cadastrar novo paciente\n";
        cout << "2. Listar todos os pacientes\n";
        cout << "3. Buscar paciente por CPF\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cadastrarPaciente();
                break;
            case 2:
                listarPacientes();
                break;
            case 3:
                buscarPorCPF();
                break;
            case 0:
                cout << "Saindo do sistema...\n";
                break;
            default:
                cout << "Opcao invalida. Tente novamente.\n";
        }
    } while (opcao != 0);
}

int main() {
    if (!Config::ensureDirectories()) {
        cerr << "Erro ao criar os diretorios necessarios.\n";
        return 1;
    }

    menu();
    return 0;
}