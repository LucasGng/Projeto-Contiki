#include "contiki.h"
#include <stdio.h>
#include <random.h>

// Definindo os processos
PROCESS(monitor_cardiaco, "Cardiaco");
PROCESS(monitor_saturacao, "Saturacao");
PROCESS(monitor_temperatura, "Temperatura");
PROCESS(Mostrar_alerta, "Alerta");

// Definindo a lista de processos que irão executar
AUTOSTART_PROCESSES(&monitor_cardiaco, &monitor_saturacao, &monitor_temperatura, &Mostrar_alerta);

// Controle dos recursos compartilhados (para não dar conflito de acesso)
static int controle_recurso = 0;

// Definindo o escopo do processo de monitoramento cardiaco
PROCESS_THREAD(monitor_cardiaco, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertaUsuario[80];

  // Criando Variaveis para servir de base no processo de monitoramento cardiaco (maximo e minimo)
  int maximoDeBatidas = 140;
  int minimoDeBatidas = 20;

  // Inicia o Processo
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3);

    // Define uma variavel para simular as batidas do usuario
    int batida = random_rand() % (maximoDeBatidas - minimoDeBatidas) + minimoDeBatidas;

    // Estrutura a mensagem para depois printa-la na tela
    sprintf(msg, "-------- Batimentos Cardiacos: %d Bpm`s\n", batida);
    printf("%s", msg);


    if (batida > 90)
    {
      // Avisa do batimento cardiaco alto
      sprintf(alertaUsuario, "!!!!!!!!batimento cardíaco alto!!!!!!!: %d Bpm`s ", batida);
      process_post(&Mostrar_alerta, PROCESS_EVENT_MSG, alertaUsuario);

      
    }
    else if (batida < 50)
    {
      // Avisa do batimento cardiaco baixo
      sprintf(alertaUsuario, "!!!!!!!batimento cardíaco baixo!!!!!!!: %d Bpm`s", batida);
        process_post(&Mostrar_alerta, PROCESS_EVENT_MSG, alertaUsuario);

    
    }
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  // Finaliza o processo
  PROCESS_END();
}

// Definindo o escopo do processo de monitoramento da saturação no sangue
PROCESS_THREAD(monitor_saturacao, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertaUsuario[80];

  // Criando Variaveis para servir de base no processo de monitoramento da saturação de oxigenio (maximo e minimo)
  int maximoDeSaturacao = 100;
  int minimoDeSaturacao = 80;

  // Inicia o processo
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3);

    // Define uma variavel Saturação para simular a saturação do sangue do usuario
    int saturacao = random_rand() % (maximoDeSaturacao - minimoDeSaturacao) + minimoDeSaturacao;

    // Estrutura a mensagem para depois printa-la
    sprintf(msg, "-------- Saturação do oxigenio: %d %% \n", saturacao);
    printf("%s", msg);

    if (saturacao < 90)
    {
      // Avisa da baixa Saturação
      sprintf(alertaUsuario, "!!!!!!!! Saturacao Baixa !!!!!!!: %d %% ", saturacao);
      process_post(&Mostrar_alerta, PROCESS_EVENT_MSG, alertaUsuario);
    }
    

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  // Finaliza o processo
  PROCESS_END();
}

// Definindo o escopo do processo de monitoramento da temperatura
PROCESS_THREAD(monitor_temperatura, ev, data)
{
  static struct etimer timer;
  static char msg[50];
  static char alertaUsuario[80];

  // Criando Variaveis para servir de base no processo de monitoramento da temperatura (maximo e minimo)
  int maximoDeTemperatura = 41;
  int minimoDeTemperatura = 34;

  // Inicia o processo
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timer, CLOCK_SECOND * 3);

    // Define uma variavel para simular a temperatura do usuario
    int temperatura = random_rand() % (maximoDeTemperatura - minimoDeTemperatura) + minimoDeTemperatura;

     // Estrutura a mensagem para depois printa-la
    sprintf(msg, "------- Temperatura Corporal: %d °C\n", temperatura);
    printf("%s", msg);

  if (temperatura < 35)
      {
        // Avisa que está com hipotermia 
        sprintf(alertTemp, "!!!!!!!!! Com Hipotermia !!!!!!!: %d °C", temperatura);
        process_post(&Mostrar_alerta, PROCESS_EVENT_MSG, alertaUsuario);


      }else if (temperatura > 37){
        // Avisa que está com febre
        sprintf(alertaUsuario, "!!!!!!!!! Com Febre !!!!!!!: %d °C", temperatura);
        process_post(&Mostrar_alerta, PROCESS_EVENT_MSG, alertaUsuario);

      }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }
  // Finaliza o processo
  PROCESS_END();
}

// Escopo do Processo que irá Mostrar o Alerta na tela (é utilizado como complemento para os outros alertas)
PROCESS_THREAD(Mostrar_alerta, ev, data)
{
  // Inicia o processo
  PROCESS_BEGIN();

  while (1)
  {
    PROCESS_WAIT_EVENT();
    // Se o evento for do tipo de mensagem de comunicação
    if (ev == PROCESS_EVENT_MSG)
    {
      // Se o recurso não estiver sendo usado
      if (!controle_recurso)
      {
       // Define o recurso como sendo usado
        controle_recurso = 1;
        // Realiza o print do alerta
        printf("!!!!!!!!!ALERTA!!!!!!!!! => %s \n", (char *)data);
        // Define o recurso como livre
        controle_recurso = 0;
        
      }

    }
  }
  // Finaliza o processo
  PROCESS_END();
}
