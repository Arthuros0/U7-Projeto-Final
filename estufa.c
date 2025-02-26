#include "estufa.h"

estufa estufas[3]={{1,true,true,false,true,30,80,22},
                   {2,false,false,false,false,40,60,25},
                   {3,true,false,false,false,60,50,27}};
                                      
struct repeating_timer timer_clima;

absolute_time_t debounce_serial;

void atualiza_estado_estufa(){
  for (uint8_t i = 0; i < NUM_ESTUFAS; i++)
  {
    //Umidade do solo
    if(estufas[i].umidade_solo <= MIN_UMIDADE_SOLO && !estufas[i].status_irrigacao && !estufas[i].status_fertilizante)
    {
      estufas[i].status_irrigacao=true;
    }else if(estufas[i].umidade_solo >= MAX_UMIDADE_SOLO && (estufas[i].status_irrigacao || estufas[i].status_fertilizante))
    {
      estufas[i].status_irrigacao=false;
      estufas[i].status_fertilizante=false;
    }
    //Umidade do ar
    if (estufas[i].umidade_ar <= MIN_UMIDADE_AR && !estufas[i].status_umidade)
    {
      estufas[i].status_umidade=true;
    }else if (estufas[i].umidade_ar >= MAX_UMIDADE_AR && estufas[i].status_umidade)
    {
      estufas[i].status_umidade=false;
    }
    //Temperatura
    if (estufas[i].temperatura >= MAX_TEMPERATURA && !estufas[i].status_ventilacao)
    {
      estufas[i].status_ventilacao=true;
      if(estufas[i].umidade_ar < MAX_UMIDADE_AR)estufas[i].status_umidade=true;
    }else if(estufas[i].temperatura <= MIN_TEMPERATURA && estufas[i].status_ventilacao){
      estufas[i].status_ventilacao=false;
    }
  }
}

void imprime_informacoes(){
  if(time_reached(debounce_serial)){
    printf("\033[2J\033[1;1H");
    for (uint8_t i = 0; i < NUM_ESTUFAS; i++)
    {
      printf("Estufa |%d|\n",estufas[i].id);
      printf("Umidade solo: %.1f%%\n",estufas[i].umidade_solo);
      printf((estufas[i].status_irrigacao) ? "Irrigacao: ativada\n" : "Irrigacao: desativada\n");
      printf((estufas[i].status_fertilizante) ? "Fertirrigacao: ativada\n" : "Fertirrigacao: desativada\n");
      printf("Umidade ar: %.1f%%\n",estufas[i].umidade_ar);
      printf((estufas[i].status_umidade) ? "Aspersor: ativado\n" : "Aspersor: desativado\n");
      printf("Temperatura: %.1fC°\n",estufas[i].temperatura);
      printf((estufas[i].status_ventilacao) ? "Ventilacao: ativada\n" : "Ventilacao: desativada\n");
      
      printf("***************************************\n");
      sleep_ms(200);
    } 
    debounce_serial=delayed_by_ms(get_absolute_time(),2000);
  }
}

bool simula_clima(struct repeating_timer *t){
  for (uint8_t i = 0; i < NUM_ESTUFAS; i++)
  {
    if(estufas[i].status_umidade && estufas[i].umidade_ar < MAX_VALOR)
    {
      estufas[i].umidade_ar+=TAXA_UMIDADE_AR;
    }else if (estufas[i].umidade_ar > MIN_VALOR){
      estufas[i].umidade_ar-=TAXA_DEC_UMIDADE_AR;
    }
    
    if((estufas[i].status_irrigacao || estufas[i].status_fertilizante) && estufas[i].umidade_solo < MAX_VALOR)
    {
      estufas[i].umidade_solo+=TAXA_UMIDADE_SOLO;
    }else if (estufas[i].umidade_solo > MIN_VALOR){
      estufas[i].umidade_solo-=TAXA_DEC_UMIDADE_SOLO;
    }

    if(estufas[i].status_ventilacao && estufas[i].temperatura > MIN_VALOR)
    {
      estufas[i].temperatura-=TAXA_DEC_TEMPERATURA;
      if(estufas[i].umidade_ar > MIN_VALOR)estufas[i].umidade_ar-=(TAXA_DEC_UMIDADE_AR*1.5);
    }else if (estufas[i].temperatura < MAX_VALOR){
      estufas[i].temperatura+=TAXA_TEMPERATURA;
    }
  }
  atualiza_estado_estufa();
  return true;
}

void alterna_fertirrigacao(uint8_t i){
  if(estufas[i].umidade_solo < MAX_UMIDADE_SOLO-3){
    if(!estufas[i].status_fertilizante){
      estufas[i].status_irrigacao=false;
      estufas[i].status_fertilizante=true;
    }else{
      estufas[i].status_fertilizante=false;
    }
  }
}


