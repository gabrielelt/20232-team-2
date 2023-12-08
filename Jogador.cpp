#include "Jogador.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include "Carta.hpp"
#include "Mao.hpp"

Jogador::Jogador(std::string nick)
{
   nickname = nick;
   ativo = true;
   vez  = cobriu = ativo = small_Blind = big_Blind = false;
   //mao = new Mao(cartas);
   fichas.push_back({5, 10});// 8 de 25, 8 de 100, 4 de 500, 2 de 1000, 1 de 5000
   fichas.push_back({10, 5});
   fichas.push_back({20, 5});
   fichas.push_back({50, 2});
   fichas.push_back({100, 2});
   fichas.push_back({500, 1});
}

Jogador::~Jogador()
{

}

void Jogador::receberCarta(Carta carta)
{
   mao->adicionarCarta(carta);
}

void Jogador::check(int &valorMesa)
{
   if (valorMesa == apostado && vez)
   {
      cobriu = true;
   }

   else 
   {
      std::cout << "Você não pode selecionar essa opção até que o valor da sua aposta seja maior ou igual ao atual valor da mesa\n";
   }
}

void Jogador::apostar(int &valorMesa)
{

   //Ainda falta implementar a forma com que o usuário escolherá as fichas que serão apostadas
   try
   {
      std::cout << "Digite o valor que deseja apostar";
      int aposta;
      std::cin >> aposta;
      if (aposta < valorMesa && saldo > valorMesa) throw std::invalid_argument("Aposta insuficiente, por favor, aumente ela");   
      else if (aposta >= valorMesa && saldo <= valorMesa) 
      {
         bool confirma = false;
         std::cout << "Seu saldo é menor ou igual ao valor da mesa, deseja entrar no modo all in?\n";
         std::cin >> confirma;
         //criar a classe de exceção All_in
         //if(confirma)   throw all_in("Agora você está no modo All in");
      }

      saldo -= aposta;
   }
   catch(const std::invalid_argument& e)
   {
      std::cerr << e.what() << '\n';
   }
   /*catch(const All_inn& e)
   {
      all_in = true;
      std::cerr << e.what() << '\n';
   }*/


}

void Jogador::igualar()
{

}

void Jogador::desistir()
{
   std::cout << "Realmente deseja desistir? digite SIM para confirma\n";
   std::string confirmacao;
   std::cin >> confirmacao;
   if(confirmacao.compare("SIM") == 0) ativo = false;
}

//Acredito que não valha a pena separar os métodos apostar, aumentar e igualar
void Jogador::aumentar()
{

}

void Jogador::exibirInfo()
{
   std::cout << "\nNickname: " << nickname << "\n";
   for (int i = 0; i < 5; i++)
   {
      
      std::cout << "Fichas de " << fichas[i].first << " : " << fichas[i].second << "\n";
   }
}

/*void Jogador::comprarFichas()
{
   std::cout << "\nConversão Real-Fichas: 1 Real -> 250  fichas\n";
   std::cout << "Quantos reais (numero inteiro) deseja inserir?: ";
   unsigned int adicionar;
   std::cin >> adicionar;
   Injetar *= 250;
   while (Injetar >= 5000) Fichas[4].second ++;
   while (Injetar >= 1000) Fichas[3].second ++;
   while (Injetar >= 500) Fichas[2].second ++;
   while (Injetar >= 100) Fichas[1].second ++;
   while (Injetar >= 25) Fichas[0].second ++;
}*/

