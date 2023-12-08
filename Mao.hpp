#ifndef MAO_H
#define MAO_H
#include <vector>
#include "Carta.hpp"

class Mao 
{
    private:
        std::vector<Carta> cartas; // Armazena as cartas da mão

    public:
        // Construtor
        Mao(std::vector<Carta> &cartas);

        // Getters
        std::vector<Carta> getCartas();

        // Adiciona uma carta à mão
        void adicionarCarta(Carta carta);

        // Verifica se a mão é uma carta menor (ace a 2)
        bool is_CartaMenor(int index); 
        
        // Verifica se a mão é um par (duas cartas iguais)
        bool is_Pair();

        // Verifica se a mão é um dobro (duas cartas iguais, em sequência)
        bool is_Dobro();
           
        // Verifica se a mão é uma sequência (três ou mais cartas em sequência)
        bool is_Sequencia();

        // Verifica se a mão é um trinca (três cartas iguais)
        bool is_Trinca();

        // Verifica se a mão é uma straight flush (sequência de cinco cartas no mesmo naipe)
        bool is_StraightFlush();

        // Verifica se a mão é um flush (cinco cartas no mesmo naipe)
        bool is_Flush();

        // Verifica se a mão é um full house (três cartas iguais e um par)
        bool is_FullHouse();

        // Verifica se a mão é um quatro (quatro cartas iguais)
        bool is_Quatro();

        // Verifica se a mão é um straigth (sequência de cinco cartas)
        bool is_Straight();

        // Verifica se a mão é um trinca flush (três cartas iguais no mesmo naipe)
        bool is_TrincaFlush();

        // Verifica se a mão é um dois pares (dois pares de cartas iguais)
        bool is_DoisPares();

        // Retorna o valor da mão, em termos de pontuação
        int valorMao();
};

#endif
