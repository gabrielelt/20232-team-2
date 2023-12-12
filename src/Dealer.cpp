#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <limits>
#include "Dealer.hpp"
#include "Jogador.hpp"
#include "Carta.hpp"

//Método construtor, iniciazlizando as variáveis necessárias para dar início ao programa
Dealer::Dealer() 
{
    rodada = 1;
    check = 0;
    primeiro_Jogador = -1;
    valor_Acumulado_mesa = 0;
    primeira_Aposta = 10;
    small_blind_apostou = 0;
    valorMesa = primeira_Aposta;
    partidaFinalizada = false;
}

Dealer::~Dealer() 
{

}

std::vector<Carta> Dealer::get_baralho()
{
    return baralho;
} 

std::vector<Carta> Dealer::get_mesa()
{
    return mesa;
} 

std::vector<Jogador> Dealer::get_jogadores()
{
    return jogadores;
}

int Dealer::get_Valor_Mesa()
{
    return valorMesa;
}

int Dealer::get_Primeiro_Jogador()
{
    return primeiro_Jogador;
}

void Dealer::set_Valor_Mesa(int valorMesa)
{
    this->valorMesa = valorMesa;
}

void Dealer::set_Valor_Acumulado_Mesa(int valorAtualMesa)
{
    valor_Acumulado_mesa += valorAtualMesa;
}

void Dealer::set_Primeiro_Jogador(int id_Primeiro_Jogador)
{
    primeiro_Jogador = id_Primeiro_Jogador;
}

void Dealer::set_Primeira_Aposta(int primeira_Aposta)
{
    this->primeira_Aposta = primeira_Aposta;
}

void Dealer::designar_Small_Blind()
{
    for (size_t i = 0; i < jogadores.size(); i++)
    {
        if(jogadores[i].is_True_Small_Blind())
        {
            jogadores[i].set_Small_Blind(false);
            if(i == jogadores.size()-1)
            {
                jogadores[0].set_Small_Blind(true);
            }
            else
            {
                jogadores[i+1].set_Small_Blind(true);
            }
        }
    }
}

void Dealer::designar_Big_Blind()
{
    for (size_t i = 0; i < jogadores.size(); i++)
    {
        if(jogadores[i].is_True_Big_Blind())
        {
            jogadores[i].set_Big_Blind(false);
            if(i == jogadores.size()-1)
            {
                jogadores[0].set_Big_Blind(true);
            }
            else
            {
                jogadores[i+1].set_Big_Blind(true);
            }
        }
    }
}

void Dealer::designar_Primeiro_Jogador()
{
    //No começo da partida o Small Blind sempre será o primeiro
    if(primeiro_Jogador == -1)
    {    
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            //encontra o Small Blind
            if(jogadores[i].is_True_Small_Blind())
            {
                jogadores[i].set_Vez(true);
                primeiro_Jogador = i;
                for (size_t  j = 0; j < jogadores.size(); j++)
                {
                    if(j != i)
                    {
                        jogadores[j].set_Vez(false);
                    }
                }
                break;
            }
        }
    }    
    //Verifica se quem era para ser o maior jogador desta rodada ainda está ativo na partida
    else if (!jogadores[primeiro_Jogador].is_True_Ativo())
    {
        while (true)
        {

            //Se o atual primeiro jogador for o último do vetor, retornamos ao primeiro 
            if((size_t)primeiro_Jogador == jogadores.size()-1)
            {
                primeiro_Jogador = 0;
            }
            //Se não, veremos o próximo 
            else
            {
                primeiro_Jogador++;
            }
            //Se o atual jogador avaliado estiver ativo na partida, então ele iniciará as apostas da rodada            
            if(jogadores[primeiro_Jogador].is_True_Ativo())
            {
                jogadores[primeiro_Jogador].set_Vez(true);
                for (size_t j = 0; j < jogadores.size(); j++)
                {
                    if(j != (size_t)primeiro_Jogador)
                    {
                        jogadores[j].set_Vez(false);
                    }
                }
                break;
            }
        }       
    }
}

//Função que de fato inicia e "roda" o jogo
void Dealer::iniciar_Partida()
{
    int partidas = 1;
    criar_Sala();
    while (true)
    {
        small_blind_apostou = 0;
        if(partidas > 1)
        {
            //Na primeira partida o small blind será sorteado
            designar_Small_Blind();
            designar_Big_Blind();
        }
        criar_Baralho();
        dar_Cartas();
        criar_Mesa();
        designar_Primeiro_Jogador();
        for (int i = 0; i < 4; i++)
        {
            if(!partidaFinalizada)
            {
                verificar_Rodadas();
            }
        }
        finalizar_Partida();

        //verificar se há apenas um jogador com dinheiro, em caso positivo o jogo será finalizado
        int jogadoresComDinheiro = 0;
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            if(jogadores[i].get_Saldo() > 0)
                jogadoresComDinheiro++;
        }
        if(jogadoresComDinheiro == 1)
        {
            std::cout << "A partida acabou\n";
            for (size_t i = 0; i < jogadores.size(); i++)
            {
                if(jogadores[i].get_Saldo() > 0)
                {
                    std::cout << "O jogador " << jogadores[i].get_Nick() << " ganhou o jogo\n";
                    std::cout << "Seu saldo é de " << jogadores[i].get_Saldo() << std::endl; 
                    return;
                }
            }
        }
    }
}

void Dealer::criar_Sala()
{
    while (true) 
    {
        try 
        {
            std::cout << "Digite a quantidade de jogadores da partida: " << std::endl;
            if (!(std::cin >> quantidadeJogadores)) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Entrada inválida, insira um número inteiro\n");
            }
            if (quantidadeJogadores < 2 || quantidadeJogadores > 8) 
            {
                throw std::invalid_argument("Deve haver entre 2 e 8 jogadores para jogar Poker\n");
            }
            break;
        }
        catch (std::invalid_argument &e) 
        {
            std::cerr << e.what() << "\n";
        }
    }
    
    //Identificar os jogadores por nick, além de verificar se há repetição deles
    for (int i = 1; i <= quantidadeJogadores; i++)
    {
        std::string nick;
        while (true) 
        {
            try 
            {
                std::cout << "Apelido do jogador " << i << ": ";
                std::getline(std::cin, nick);
                if (nick.empty()) 
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::invalid_argument("O apelido não pode estar vazio\n");
                }
                break;
            }
            catch (std::invalid_argument &e) 
            {
                std::cerr << e.what() << "\n";
            }
        }
        bool nick_repetido = false;
        for (size_t j = 0; j < jogadores.size(); j++)
        {
            if(nick.compare(jogadores[j].get_Nick()) == 0)
            {
                std::cout << "Nick já utilizado por outro jogador, troque o apelido\n";
                nick_repetido = true;
                break;
            }
        }
        if(nick_repetido)
        {
            i--;
            continue;
        }
        Jogador temporario(nick);
        jogadores.push_back(temporario);
    }
    srand(time(NULL));

    //Sortear quem será o Big Blind na primeira partida de cada jogo e dar a vez a ele
    int big = rand()%jogadores.size();
    jogadores[big].set_Big_Blind(true);
    jogadores[big].set_Vez(true);

    //Identificar o Small Blind a partir do Big Blind 
    for (size_t i = 0; i < jogadores.size(); i++)
    {
        if((size_t)big == jogadores.size()-1)
        {
            jogadores[0].set_Small_Blind(true);
        }
        else
        {
            jogadores[big+1].set_Small_Blind(true);
        }
    }
}

void Dealer::criar_Baralho()
{
    for (int i = 2; i <= 14; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            baralho.push_back(Carta(i, Carta::naipes_possiveis[j]));
        }
    }
    embaralhar_Cartas();
    embaralhar_Cartas();
    embaralhar_Cartas();
}

//Distribuir todas as cartas da mesa, queimando uma antes das três primeiras a serem viradas, uma na quarta e uma na quinta e última
void Dealer::criar_Mesa()
{   
    baralho.pop_back();
    for (int i = 0; i < 3; i++)
    {   
        mesa.push_back(baralho.back());
        baralho.pop_back();
    }

    for (int i = 0; i < 2; i++)
    {
        baralho.pop_back();
        mesa.push_back(baralho.back());
        baralho.pop_back();
    }
}

void Dealer::embaralhar_Cartas() 
{
    srand(time(NULL));
    std::random_shuffle(baralho.begin(), baralho.end());
}

void Dealer::dar_Cartas()
{
    //Distribuir duas cartas para cada jogador
    for (int i = 0; i < quantidadeJogadores; i++)
    {
        jogadores[i].set_Ativo(true);
        jogadores[i].set_All_In(false);
        jogadores[i].set_Apostado(0);
        for (int j = 0; j < 2; j++)
        {
            jogadores[i].receber_Carta(baralho.back());
            baralho.pop_back();
        }
    }

    //Mostrar UMA única vez as cartas de cada jogador e pede que eles anotem, pois serão apagadas 
    for (size_t i = 0; i < jogadores.size(); i++)
    {
        std::cout << "\nA seguir, irei mostrar as cartas do jogador " << jogadores[i].get_Nick() << "\n\n";
        std::cout << "ATENÇÂO: essas cartas serão mostras apenas uma vez, por favor, anote-as!!!\n";
        std::cout << "TODOS OS OUTROS JOGADORES NAO DEVEM OLHAR PARA A TELA NESSE INSTANTE\n";
        std::string confirma = {};
        while(!(confirma.compare("CONFIRMA") == 0))
        {
            std::cout << "\nDigite CONFIRMA para mostrar as cartas: ";
            std::cin >> confirma;
        }
        std::cout << "Suas cartas sao: \n";
        mostrar_Cartas(2,jogadores[i].get_Mao().get_Cartas());
        confirma = {};
        while(!(confirma.compare("CONFIRMA") == 0))
        {
            std::cout << "\nDigite CONFIRMA para apagar a tela: ";
            std::cin >> confirma;
        }
        std::system("clear");

        while (true) 
        {
            std::cout << "\nDigite CONFIRMA para mostrar as cartas: ";
            try 
            {
                std::getline(std::cin, confirma);
                if (confirma != "CONFIRMA") 
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::invalid_argument("digite 'CONFIRMA'\n");
                }
                break;
            }
            catch (std::invalid_argument &e) 
            {
                std::cerr << e.what() << "\n";
            }
        }

        std::cout << "Suas cartas são:\n";
        mostrar_Cartas(2,jogadores[i].get_Mao().get_Cartas());
        confirma = {};
        while (true) 
        {
            std::cout << "\nDigite CONFIRMA para apagar a tela: ";
            try 
            {
                std::getline(std::cin, confirma);
                if (confirma != "CONFIRMA") 
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::invalid_argument("digite 'CONFIRMA'\n");
                }
                break;
            }
            catch (std::invalid_argument &e) 
            {
                std::cerr << e.what() << "\n";
            }
        }

    std::system("clear");
    }
}

void Dealer::passar_Vez()
{
    for (size_t i = 0; i < jogadores.size(); i++)
    {
        //Identifica qual jogador está jogando agora
        if(jogadores[i].is_True_Vez() && jogadores[i].is_True_Ativo())
        {
            while (true)
            {
                //Tirar ele da vez
                jogadores[i].set_Vez(false);
                if(i == jogadores.size()-1 && jogadores[0].is_True_Ativo())
                {
                    //Se o ultimo jogador estiver ativo, passa a vez pro primeiro jogador
                    jogadores[0].set_Vez(true);
                    return;
                }
                    //Se não, verificaremos o próximo jogador do vetor
                else if (jogadores[i+1].is_True_Ativo())
                {
                    jogadores[i+1].set_Vez(true);
                    return;
                }
                if (i == jogadores.size() - 1) i=0;
            }
        }
    }
}

bool Dealer::verificar_Check()
{
    //verificar se o valor da aposta de todos os jogadores são a mesma que a  atual da mão
    for (int i = 0;; i++)
    {
        if(jogadores[i].is_True_Vez() && !jogadores[i].is_True_All_In())
        {
            int JogadoresAtivos = 0;
            for (size_t i = 0; i < jogadores.size(); i++)
            {
                if(jogadores[i].is_True_Ativo())
                    JogadoresAtivos++;
            }
            if(JogadoresAtivos <= 1)
            {
                return true;
            }
            if (rodada == 2)
            {
                std::cout << "As cartas da mesa são: \n";
                mostrar_Cartas(3, mesa);
            }
            if (rodada == 3)
            {
                std::cout << "As cartas da mesa são: \n"; 
                mostrar_Cartas(4, mesa);
            }
            if (rodada == 4)
            {
                std::cout << "As cartas da mesa são: \n";
                mostrar_Cartas(5, mesa);
            }

            again:
            std::cout << "Está na vez do jogador " << jogadores[i].get_Nick() << "\n\n";
            int escolha;
            while (true) 
            {
                try 
                {
                    std::cout << "Digite 1 para ver suas informações\nDigite 2 para apostar algum valor\nDigite 3 para desistir da partida\n";
                    std::cout << "Digite o numero escolhido: ";
                    if (!(std::cin >> escolha)) 
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::invalid_argument("Entrada inválida, entre com um número inteiro\n");
                    }
                    if (escolha < 1 || escolha > 3) 
                    {
                        throw std::invalid_argument("Escolha um numero entre 1 e 3\n\n");
                    }
                    break;
                }
                catch (std::invalid_argument &e) 
                {
                    std::cerr << e.what() << "\n";
                }
            }
            
            int valorMesaAntigo = valorMesa;   
            if(escolha == 1)
            { 
                jogadores[i].exibir_Info(valorMesa);
                goto again;
            }
            else if(escolha == 2)
            {
                if (jogadores[i].apostar(valorMesa))
                {
                    small_blind_apostou++;
                    if(small_blind_apostou == 1)
                        valorMesa *= 2;
                    if(valorMesa == valorMesaAntigo) check++;
                    else check = 1;
                    passar_Vez();
                }
                else
                {
                    goto again;
                }
                
            }
            else if(escolha == 3)
            {
                passar_Vez();
                jogadores[i].desistir();
            } 

            int jogadoresAtivos = 0;
            for (size_t j = 0; j < jogadores.size(); j++)
            {
                if(jogadores[j].is_True_Ativo())
                    jogadoresAtivos++;
            }
            if(jogadoresAtivos == check)
                return true;
            
            std::cout << "check = " << check << "\n\n\n\n"; 
        }
        if(jogadores[i].is_True_All_In())
        {
            check++;
            int jogadoresAtivos = 0;
            for (size_t j = 0; j < jogadores.size(); j++)
            {
                if(jogadores[j].is_True_Ativo())
                    jogadoresAtivos++;
            }
            if(jogadoresAtivos == check)
                return true;
            passar_Vez();
        }
        if( (size_t)i == jogadores.size() - 1 )
            i = -1;
    } 
}

void Dealer::verificar_Rodadas()
{   
    if(verificar_Check())
    {
        //resetar o valor das apostas da mesa
        valorMesa = 0;
        check = 0;
        //resetar o valor que cada jogador tem apostado
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            valor_Acumulado_mesa += jogadores[i].get_Apostado();
        }
        
        for (size_t i = 0; i < jogadores.size(); i++) {jogadores[i].set_Apostado(0);}
        std::cout << "Todos os jogadores cobriram a aposta mais alta da mesa\n";
        std::cout << "Agora iremos para a próxima rodada\n";
        rodada++;

        if(rodada == 4)
        {
            partidaFinalizada = true;
        }
    }
}

void Dealer::mostrar_Cartas(int quantidade_Cartas_Mostradas, std::vector<Carta> vetor)
{
    for (int i = 0; i < quantidade_Cartas_Mostradas; i++)
    {
        if(vetor[i].get_Valor_Carta() <= 10)
        {
            std::cout << vetor[i].get_Valor_Carta() << "   " << vetor[i].get_Naipe() << std::endl;
        }
        else
        {
            if (vetor[i].get_Valor_Carta() == 11)
            {
                std::cout << 'J' << "   " << vetor[i].get_Naipe() << std::endl;
            }
                
            if (vetor[i].get_Valor_Carta() == 12)
            {
                std::cout << 'Q' << "   " << vetor[i].get_Naipe() << std::endl;
            }

            if (vetor[i].get_Valor_Carta() == 13)
            {
                std::cout << 'K' << "   " << vetor[i].get_Naipe() << std::endl;
            }
                
            if (vetor[i].get_Valor_Carta() == 14)
            {
                std::cout << 'A' << "   " << vetor[i].get_Naipe() << std::endl;
            }
        }
    }
}

void Dealer::finalizar_Partida()
{
    int jogadoresAtivos = 0;

    for (size_t i = 0; i < jogadores.size(); i++)
    {
        if(jogadores[i].is_True_Ativo())
        jogadoresAtivos++;
    }
    std::cout << "A partida acabou\n";
    // Quem ganhou se tiver apenas um jogador
    if(jogadoresAtivos == 1)
    {
        std::cout << "A partida acabou\n";
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            if(jogadores[i].is_True_Ativo())
            {
                std::cout << "O jogador " << jogadores[i].get_Nick() << " ganhou a partida\n";
                std::cout << "Obrigado por jogar\n";
                jogadores[i].aumenta_Saldo(valor_Acumulado_mesa);
                return;
            }
        }
    }

    // Quem ganhou se tiver mais de um jogador
    if(partidaFinalizada)
    {
        std::pair<int,int> maior_Valor;
        maior_Valor.first = 0;  //maior valor da mão (second é a posição do jogador)
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            for (size_t j = 0; j < mesa.size(); j++)
            {
                jogadores[i].receber_Carta(mesa[j]);
            }
        }
        
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            std::cout << "Cartas do jogador " << jogadores[i].get_Nick() << "\n\n";
            for (size_t j = 0; j < jogadores[i].get_Mao().get_Cartas().size(); j++)
            {
                mostrar_Cartas(7, jogadores[i].get_Mao().get_Cartas());
            } 
        }
        
        
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            if(jogadores[i].get_Valor_Mao() > maior_Valor.first)
            {
                maior_Valor.first = jogadores[i].get_Valor_Mao();
                maior_Valor.second = i;
            }
        }
        std::cout << "O jogador " << jogadores[maior_Valor.second].get_Nick() 
        <<" ganhou a partida com uma pontuação de: " << maior_Valor.first << std::endl;
        std::cout << "A partida acabou\n";
        std::cout << "Obrigado por jogar\n";
        std::cout << "valor acumulado da mesa = " << valor_Acumulado_mesa << "\n";
        jogadores[maior_Valor.second].aumenta_Saldo(valor_Acumulado_mesa);
        valor_Acumulado_mesa = 0;
        rodada = 1;
        check = 0;
        primeiro_Jogador = -1;
        primeira_Aposta += 10;
        valorMesa = primeira_Aposta;
        for (size_t i = 0; i < jogadores.size(); i++)
        {
            jogadores[i].set_Vez(false);
            jogadores[i].set_Ativo(true);
            jogadores[i].set_All_In(false);
            jogadores[i].set_Apostado(0);
            jogadores[i].reset_Mao();
           for (size_t i = 0; i < baralho.size(); i++)
           {
                baralho.pop_back();
           }
           
        }
        return;
    }


}